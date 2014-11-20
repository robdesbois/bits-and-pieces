#include <boost/function_output_iterator.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/range/iterator_range.hpp>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <random>
#include <string>
#include <vector>

struct EdgeProperties
{};


struct NodeProperties
{
    std::string label;
};

// fwd-decl; requires graph type definition
struct NodePathInfo;

using graph_t = boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, NodeProperties >;
using node_t  = boost::graph_traits< graph_t >::vertex_descriptor;
using edge_t  = boost::graph_traits< graph_t >::edge_descriptor;
using path_t  = std::vector< node_t >;

using node_seq      = std::vector< node_t >;
using node_iterator = node_seq::const_iterator;
using node_info_map = std::map< node_t, NodePathInfo >;


/** Pathfinding-related information about a node. */
struct NodePathInfo
{
    boost::optional<node_t>  prevNode;
    unsigned int             pathLength = std::numeric_limits<unsigned int>::max(); // initialise to max length
};


/** Functor that outputs the labels of a node sequence as a comma-separated list. */
struct NodeSeqLabelPrinter
{
    NodeSeqLabelPrinter( const graph_t& graph_ )
        : first( true )
        , out( std::cout )
        , graph( graph_ )
    {}

    void operator()( const node_t& node )
    {
        if (first)
            first = false;
        else
            out << ", ";
        
        out << graph[node].label;
    }

private:
    bool           first;
    std::ostream&  out;
    const graph_t& graph;
};


/** hack to get around std library not yet having vector::erase() that takes a const iterator :-( */
node_seq::iterator to_mutable_iterator( node_seq& nodes, node_iterator it )
{
    // take advantage of vector's random-access iterator
    return std::next(nodes.begin(), std::distance(nodes.cbegin(), it));
}


/** Chooses a node at random. */
auto choose_random_node(const node_seq& nodes, const node_info_map&) -> node_iterator
{
    // generate a random index into the vector
    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> uniform_dist(0, nodes.size() - 1 );

    return std::next( nodes.begin(), uniform_dist(e1) );
}

/** Chooses one of the nodes with the shortest path; also called 'Uniform Cost Search'.
 * Behaviour is undefined if nodes is empty.
 * If the path info is missing for a node then it will only be considered if all nodes' path infos are missing.
 */
auto choose_node_with_shortest_path(const node_seq& nodes, const node_info_map& nodeInfo) -> node_iterator
{
    // can't use map::operator[] as it's const, so we'll hide yuckiness in here
    auto path_length = [&]( const node_t& node ) {
        auto info = nodeInfo.find(node);
        if (info == nodeInfo.end())
            return std::numeric_limits<unsigned int>::max();

        return info->second.pathLength;
    };

    // cache best length to avoid repeatedly searching for current best node's info
    auto bestLength = path_length( nodes.front() );
    
    return std::min_element( nodes.begin(), nodes.end(), [&]( const node_t& node, const node_t& /*currentBest*/ ) {
        return path_length(node) < bestLength;
    } );
}


/** Chooses one of the nodes that appears to have the shortest distance to the goal node.
 * This heuristic requires a functor that can estimate the distance, and makes the below
 * search algorithm A*.
 * Note that this functor caches graph information for performance, so if the graph structure
 * changes or a new graph is being used then a new instance must be constructed.
 */
struct choose_node_closest_to_goal
{
    using calc_distance_func = std::function< unsigned int( const node_t& node ) >;

    choose_node_closest_to_goal( calc_distance_func estimate_distance_ )
        : estimate_distance(estimate_distance_)
    {}


    node_iterator operator()(const node_seq& nodes, const node_info_map& /*nodeInfo*/)
    {
        // hides away use of cached distances
        auto get_distance = [&]( const node_t& node ) -> unsigned int {
            auto it = distances.find(node);
            
            // calculate & save if not there
            if ( it == distances.end() )
                it = distances.insert( std::make_pair(node, estimate_distance(node)) ).first;
            
            return it->second;
        };

        auto bestDistance = get_distance( nodes.front() );
        
        return std::min_element( nodes.begin(), nodes.end(), [&]( const node_t& node, const node_t& /*currentBest*/ ) {
            return get_distance(node) < bestDistance;
        } );
    }


private:
    calc_distance_func               estimate_distance;
    std::map< node_t, unsigned int > distances; //< cache estimated distances instead of repeatedly recalculating
};



struct PathGenerator
{
    using choose_node_func = std::function< node_iterator( const node_seq&, const node_info_map& ) >;
 
    PathGenerator( choose_node_func choose_node_ = choose_random_node )
        : choose_node( choose_node_ )
        , logger( std::cout )
    {}


    /** @param out   Output iterator to which nodes in the path will be written. */
    template< class It >
    void operator()( const graph_t& graph, const node_t& srcNode, const node_t& dstNode, It out )
    {
        node_seq      reachable{ srcNode };
        node_seq      explored;
        node_info_map nodeInfo;

        // setup info for src; prevNode is uninitialised
        nodeInfo[srcNode].pathLength = 0;

        // keep trying until we can't reach any more nodes
        while ( !reachable.empty() )
        {
            node_iterator nodeIt = choose_node(reachable, nodeInfo);
            node_t        node   = *nodeIt;
            logger << "A* exploring node " << graph[*nodeIt].label << std::endl;

            // if reached goal then build path & bug out
            if ( node == dstNode )
            {
                logger << "A* reached goal node with path length " << nodeInfo[dstNode].pathLength << std::endl;

                // build path (in reverse)
                node_seq path;
                boost::optional<node_t> nextNode{ dstNode };

                while ( nextNode )
                {
                    path.push_back( *nextNode );
                    nextNode = nodeInfo[*nextNode].prevNode;
                }// while ( node != srcNode );
                
                NodeSeqLabelPrinter printNodeLabels(graph);

                logger << "Explored: ";
                std::copy( explored.begin(), explored.end(), boost::make_function_output_iterator(printNodeLabels) );
                logger << std::endl << "Path: ";

                // then walk it copying nodes to output iterator
                std::copy( path.rbegin(), path.rend(), out );
                logger << std::endl;

                return;
            }

            // ensure we don't repeat work
            reachable.erase( to_mutable_iterator(reachable, nodeIt) );
            explored.push_back(node);

            // now where can we go?
            for ( const node_t& adjNode : boost::make_iterator_range(boost::adjacent_vertices(node, graph)) )
            {
                // ignore if already explored
                if ( std::find(explored.begin(),  explored.end(),  adjNode) != explored.end() )
                    continue;

                // add newly-reachable
                if ( std::find(reachable.begin(), reachable.end(), adjNode) == reachable.end() ) {
                    logger << "   found new path to node " << graph[adjNode].label << std::endl;
                    reachable.push_back(adjNode);
                }
                else
                    logger << "   found alternate path to node " << graph[adjNode].label << std::endl;
                    
                if ( nodeInfo[node].pathLength + 1 < nodeInfo[adjNode].pathLength)
                {
                    nodeInfo[adjNode].prevNode   = node;
                    nodeInfo[adjNode].pathLength = nodeInfo[node].pathLength + 1;
                    logger << "      new path length " << nodeInfo[adjNode].pathLength << std::endl;
                }
            }
        }

        logger << "A* ran out of reachable nodes" << std::endl;
    }

private:
    choose_node_func choose_node;
    std::ostream&    logger;
};


using label_node_map = std::map< std::string, node_t >;

label_node_map populate_graph_1( graph_t& graph )
{
    label_node_map labelToNode;

    for ( char c = 'a'; c != 'z'; ++c ) {
        auto vtx = boost::add_vertex(graph);
        graph[vtx].label = c;
        labelToNode[{c}] = vtx;
    }

    auto add_edges = [&](std::string from, std::initializer_list<std::string> toAll) {
        for ( const auto& to: toAll )
            boost::add_edge(labelToNode[from], labelToNode[to], graph);
    };

    add_edges("a", {"b", "f"});
    add_edges("b", {"a", "c"});
    add_edges("c", {"b"});
    add_edges("e", {"j"});
    add_edges("f", {"a", "k"});
    add_edges("j", {"e", "o"});
    add_edges("k", {"f", "l"});
    add_edges("l", {"k", "m", "q"});
    add_edges("m", {"l", "n"});
    add_edges("n", {"m", "o"});
    add_edges("o", {"j", "n", "t"});
    add_edges("q", {"l", "v"});
    add_edges("t", {"o", "y"});
    add_edges("v", {"q", "w"});
    add_edges("w", {"v", "x"});
    add_edges("x", {"w", "y"});
    add_edges("y", {"t", "x"});

    return labelToNode;
}


/** Generates a 10x10 grid with rows,cols labeled a-j.
 * It then removes all edges to/from some of those vertices to provide a 'barrier' that the
 * algorithm must find a way around.
 */
void populate_graph_2( graph_t& graph )
{
    label_node_map labelToNode;

    const char MIN_LABEL = 'a';
    const char MAX_LABEL = 'j';

    auto add_edges_if_valid = [&]( const node_t& node, char row, char col )
    {
        if ( row >= MIN_LABEL and col >= MIN_LABEL )
        {
            // add in both directions
            std::string label{row, col};
            boost::add_edge(node, labelToNode[label], graph);
            boost::add_edge(labelToNode[label], node, graph);
        }
    };

    // add all the nodes and connect each to its N,S,E,W neighbours
    for ( char row = MIN_LABEL; row < MAX_LABEL + 1; ++row )
        for ( char col = MIN_LABEL; col < MAX_LABEL + 1; ++col )
        {
            auto vtx = boost::add_vertex(graph);
            
            std::string label{row, col};
            graph[vtx].label   = label;
            labelToNode[label] = vtx;

            add_edges_if_valid(vtx, row - 1, col);
            add_edges_if_valid(vtx, row, col - 1);
        }

    // now remove the edges we don't want
    auto clear_vertex = [&]( const std::string& label ) {
        auto nodeIt = labelToNode.find(label);
        if ( nodeIt == labelToNode.end() )
        {
            std::cout << "Cannot remove node " << label << ": unknown" << std::endl;
            return;
        }

        boost::clear_vertex ( nodeIt->second, graph );
    };

    clear_vertex("gi");
    clear_vertex("hh");
    clear_vertex("hi");
    clear_vertex("ig");
    clear_vertex("ih");
}


node_t find_labeled_node( const graph_t& graph, const std::string& label )
{
    const auto& nodes = boost::vertices(graph);
    auto it = std::find_if( nodes.first, nodes.second, [&]( const node_t& node ) {
        return graph[node].label == label;
    } );
    
    if ( it == nodes.second )
        throw (label + " is an unknown node label");
    return *it;
}

struct options
{
    std::string src;
    std::string dst;
};

options parse_args( int argc, char* argv[] )
{
    options opts;
    
    if (argc < 3)
    {
        opts.src = "aa";
        opts.dst = "ii";
    }
    else
    {
        opts.src = argv[1];
        opts.dst = argv[2];
    }

    return opts;
}

int main( int argc, char* argv[] )
{
    options opts = parse_args(argc, argv);
    graph_t graph;

    // auto labelToNode = populate_graph_1(graph);
    // node_t src       = labelToNode["a"];
    // node_t dst       = labelToNode["t"];
    
    populate_graph_2(graph);
    node_t src = find_labeled_node(graph, opts.src);
    node_t dst = find_labeled_node(graph, opts.dst);

    std::cout << "Will route-find from " << graph[src].label << " to " << graph[dst].label << std::endl;
    PathGenerator generate( choose_node_with_shortest_path );

    NodeSeqLabelPrinter printNodeLabels(graph);
    auto out = boost::make_function_output_iterator( printNodeLabels );
    // std::ostream_iterator<node_t> out(std::cout, ", ");

    generate( graph, src, dst, out );
    std::cout << std::endl;
    
}
