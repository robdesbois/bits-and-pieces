#include <aria-common/fusion_find_if.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>

using namespace testing;
using namespace aria::utils::fusion;

namespace
{

TEST(ut_fusion_find_if, does_nothing_if_sequence_empty)
{
    bool predCalled{false};
    bool funcCalled{false};

    // note the lambdas passed will never be called so don't need to actually have a param
    find_if(std::make_tuple(),
            [&] {predCalled = true;},
            [&] {funcCalled = true;});

    EXPECT_FALSE(predCalled);
    EXPECT_FALSE(funcCalled);
}


TEST(ut_fusion_find_if, calls_predicate_for_every_element_in_sequence_in_order)
{
    std::vector<int> args;

    find_if(std::make_tuple(42, 27, 169),
            [&](int i)
            {
                args.push_back(i);
                return false;
            },
            [](int) {});

    EXPECT_THAT(args, ElementsAre(42, 27, 169));
}


TEST(ut_fusion_find_if, does_not_call_func_if_predicate_returns_false_for_all_elements)
{
    bool funcCalled{false};

    find_if(std::make_tuple("hello", "world"),
            [ ](const char*) { return false; },
            [&](const char*) { funcCalled = true; });

    EXPECT_FALSE(funcCalled);
}


TEST(ut_fusion_find_if, calls_func_when_predicate_returns_true_and_stops_iteration)
{
    unsigned int nPredCalls{0};
    std::string  result;

    find_if(std::make_tuple("hello", "world"),
            [&](const char*)
            {
                ++nPredCalls;
                return true;
            },
            [&](const char* str) { result = str; });

    EXPECT_EQ(1,       nPredCalls);
    EXPECT_EQ("hello", result);
}


//------------------------------------------------------------------------------
// Now for a complex test with a truly heterogeneous sequence.

struct Is14
{
    bool operator()(int i) const
    {
        return i == 14;
    }

    bool operator()(const std::string& s) const
    {
        return s == "14";
    }
};

struct SaveString
{
    void operator()(const char* s)
    {
        result = s;
    }

    // templated function call operator to match & ignore non-string args
    template<class T>
    void operator()(const T&)
    {}

    std::string result;
};

TEST(ut_fusion_find_if, works_over_heterogeneous_sequences)
{
    SaveString stringResult;

    // Search this heterogeneous sequence for 14. The first 14 - a string - should
    // be the matching element here, should be passed to the SaveString functor
    // which saves it.
    find_if(std::make_tuple(1, 5, "5", "14", 14),
            Is14{},
            stringResult);

    EXPECT_EQ("14", stringResult.result);
}

}
