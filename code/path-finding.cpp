#include <boost/function_output_iterator.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/range/iterator_range.hpp>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <random>
#include <vector>

struct EdgeProperties
{};


struct NodeProperties
{
    char label;
};


using graph_t = boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, NodeProperties >;
using node_t  = boost::graph_traits< graph_t >::vertex_descriptor;
using edge_t  = boost::graph_traits< graph_t >::edge_descriptor;
using path_t  = std::vector< node_t >;

using node_seq         = std::vector< node_t >;
using node_prev_map    = std::map< node_t, node_t >; // map from node to its previous node in current path finding
using node_iterator    = node_seq::const_iterator;

/** hack to get around std library not yet having vector::erase() that takes a const iterator :-( */
node_seq::iterator to_mutable_iterator( node_seq& nodes, node_iterator it )
{
    // take advantage of vector's random-access iterator
    return std::next(nodes.begin(), std::distance(nodes.cbegin(), it));
}


/** Chooses a node at random. */
auto choose_random_node(const node_seq& nodes) -> node_iterator
{
    // generate a random index into the vector
    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> uniform_dist(0, nodes.size() - 1 );

    return std::next( nodes.begin(), uniform_dist(e1) );
}


struct PathGenerator
{
    using choose_node_func = std::function< node_iterator( const node_seq& ) >;

 
    PathGenerator()
        : choose_node( &choose_random_node )
    {}


    /** @param out   Output iterator to which nodes in the path will be written. */
    template< class It >
    void operator()( const graph_t& graph, const node_t& srcNode, const node_t& dstNode, It out )
    {
        node_seq      reachable{ srcNode };
        node_seq      explored;
        node_prev_map previous;

        while ( !reachable.empty() )
        {
            node_iterator nodeIt = choose_node(reachable);
            node_t        node   = *nodeIt;

            if ( node == dstNode )
            {
                // build reverse path
                node_seq path{ dstNode };

                do
                {
                    node = previous[node];
                    path.push_back( node );
                } while ( node != srcNode );
                
                // then walk it copying nodes to output iterator
                std::copy( path.rbegin(), path.rend(), out );

                return;
            }

            // don't repeat work [node came from choose_node so damn well should be in reachable!]
            reachable.erase( to_mutable_iterator(reachable, nodeIt) );
            explored.push_back(node);

            // now where can we go?
            for ( const node_t& adjNode : boost::make_iterator_range(boost::adjacent_vertices(node, graph)) )
            {
                // if is newly-reachable
                if ( std::find(explored.begin(),  explored.end(),  adjNode) == explored.end() and
                     std::find(reachable.begin(), reachable.end(), adjNode) == reachable.end() )
                {
                    previous[adjNode] = node;
                    reachable.push_back(adjNode);
                }
            }
        }
    }

    choose_node_func choose_node;
};


using label_node_map = std::map< char, node_t >;

label_node_map populate_graph( graph_t& graph )
{
    label_node_map labelToNode;

    for ( char c = 'a'; c != 'z'; ++c ) {
        auto vtx = boost::add_vertex(graph);
        graph[vtx].label = c;
        labelToNode[c] = vtx;
    }

    auto add_edges = [&](char from, std::initializer_list<char> to) {
        for ( char c : to )
            boost::add_edge(labelToNode[from], labelToNode[c], graph);
    };

    add_edges('a', {'b', 'f'});
    add_edges('b', {'a', 'c'});
    add_edges('c', {'b'});
    add_edges('e', {'j'});
    add_edges('f', {'a', 'k'});
    add_edges('j', {'e', 'o'});
    add_edges('k', {'f', 'l'});
    add_edges('l', {'k', 'm', 'q'});
    add_edges('m', {'l', 'n'});
    add_edges('n', {'m', 'o'});
    add_edges('o', {'j', 'n', 't'});
    add_edges('q', {'l', 'v'});
    add_edges('t', {'o', 'y'});
    add_edges('v', {'q', 'w'});
    add_edges('w', {'v', 'x'});
    add_edges('x', {'w', 'y'});
    add_edges('y', {'t', 'x'});

    return labelToNode;
}


int main()
{
    graph_t graph;
    auto labelToNode = populate_graph(graph);
    node_t src       = labelToNode['a'];
    node_t dst       = labelToNode['t'];

    PathGenerator generate;

    bool first = true;
    auto out = boost::make_function_output_iterator( [&](const node_t& node) {
        if (first)
            first = false;
        else
            std::cout << ", ";
        
        std::cout << graph[node].label;
    } );
    // std::ostream_iterator<node_t> out(std::cout, ", ");

    generate( graph, src, dst, out );
    std::cout << std::endl;
}
