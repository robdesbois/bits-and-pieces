#pragma  once
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/sequence/intrinsic.hpp>

namespace aria { namespace utils { namespace fusion
{
    namespace detail
    {
        /// This terminates the recursive find_if when we've reached the sequence
        /// end.
        ///
        /// @param atEnd Tag-dispatch parameter; indicates we have reached the
        ///              sequence end.
        template<class It, class Pred, class F>
        void find_if(It /*it*/, It /*end*/, Pred, F, boost::mpl::true_ /*atEnd*/)
        {}

        /// This implements the recursive find_if where we haven't reached the sequence
        /// end yet.
        ///
        /// @param it    Current sequence iterator.
        /// @param end   End of sequence iterator.
        /// @param pred  Predicate to call on sequence elements.
        /// @param f     Functor to call on matching sequence element.
        /// @param atEnd Tag-dispatch parameter; indicates we have not reached the
        ///              sequence end.
        template<class It, class End, class Pred, class F>
        void find_if(It it, End end, Pred&& pred, F&& f, boost::mpl::false_ /*atEnd*/)
        {
            if (pred(*it))
            {
                f(*it);
                return;
            }

            using It1  = typename boost::fusion::result_of::next<It>::type;
            auto atEnd = boost::fusion::result_of::equal_to<It1,
                                                            End>{};
            find_if(boost::fusion::next(it), end, pred, f, atEnd);
        }
    }

    /// Finds the first element in a sequence for which a predicate returns true,
    /// then passes it to a second function object. Sequence traversal will halt
    /// either when a matching element has been found or when the end of sequence
    /// is reached.
    /// If the sequence is empty or no elements match the predicate then the result
    /// functor will not be called.
    ///
    /// @param seq  A ForwardSequence as defined by Boost.Fusion. May be empty.
    /// @param pred The predicate to test elements of the sequence. Must be callable
    ///             with any element type in the sequence, and return something
    ///             convertible to bool.
    /// @param f    The result functor. Must be callable with any element type in
    ///             the sequence.
    template<class Sequence, class Pred, class F>
    void find_if(Sequence&& seq, Pred&& pred, F&& f)
    {
        // Reference qualifiers confuse Fusion's metafunctions
        using DecayedSequence = typename std::remove_reference<Sequence>::type;

        // Since this is a compile-time sequence where different iterator values
        // have different *types*, the first thing to do is obtain the begin/end
        // iterator types using the appropriate Boost.Fusion metafunctions.
        using It   = typename boost::fusion::result_of::begin<DecayedSequence>::type;
        using End  = typename boost::fusion::result_of::end  <DecayedSequence>::type;

        // Now we can compare the types for equality using another metafunction.
        // The result is either boost::mpl::true_/false_ type, which we immediately
        // construct an instance of.
        auto atEnd = boost::fusion::result_of::equal_to<It, End>{};

        // Now we call through to the find_if implementation. The appropriate overload
        // of find_if will be called through tag-dispatch dependent on the type of
        // atEnd.
        detail::find_if(boost::fusion::begin(seq),
                        boost::fusion::end(seq),
                        std::forward<Pred>(pred),
                        std::forward<F>(f),
                        atEnd);
    }
}}}
