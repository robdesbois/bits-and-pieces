#include <gtest/gtest.h>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/ref.hpp>

using namespace boost::interprocess;

template< class T > struct default_delete;
typedef unique_ptr< int, default_delete<int> > int_ptr;


struct foo
{
    foo()
        : m_destroyed(0)
    {}

    foo(bool& destroyed)
        : m_destroyed(&destroyed)
    {
        *m_destroyed=false;
    }
    ~foo()
    {
        if (m_destroyed)
            *m_destroyed=true;
    }

    bool* m_destroyed;
};

template<class T>
struct default_delete
{
    default_delete():called(false)
    {}

    void operator()(T* p)
    {
        delete p;
        called = true;
    }

    bool called;
};

template<class T>
struct default_delete<T[]>
{
    default_delete():called(false)
    {}

    void operator()(T* p)
    {
        delete[] p;
        called = true;
    }

    bool called;
};

TEST(deleter, deletes)
{
    default_delete<foo> d;
    
    bool destroyed;
    foo* f = new foo(destroyed);
    EXPECT_FALSE(destroyed);

    d(f);
    EXPECT_TRUE(destroyed);
}

TEST(deleter, deletes_array)
{
    default_delete<foo[]> d;
    
    bool destroyed;
    foo* f = new foo[1]();
    f[0] = foo(destroyed);
    destroyed = false;

    d(f);
    EXPECT_TRUE(destroyed);
}



TEST(up, construct_empty)
{
    int_ptr p;
    EXPECT_FALSE( p );
}

TEST(up, construct)
{
    default_delete<int> d;
    EXPECT_FALSE(d.called);
    {
        unique_ptr< int, default_delete<int>& > p( new int(42), d );
        ASSERT_TRUE( p );
        EXPECT_EQ(42, *p);
        EXPECT_FALSE(d.called);
    }
    EXPECT_TRUE(d.called);
}

TEST(x, y)
{
    int* p = new int(42);
    delete p;
    int* a = new int[2]();
    delete[] a;
}

TEST(up, construct_array)
{
    default_delete<int[]> d;
    EXPECT_FALSE(d.called);
    {
        unique_ptr< int, default_delete<int[]>& > p( new int[1](), d );
        ASSERT_TRUE( p );
        EXPECT_FALSE(d.called);
    }
    EXPECT_TRUE(d.called);
}

int_ptr foo(int_ptr p)
{
    return boost::move(p);
}

TEST(up, pass_by_move)
{
    int_ptr p( new int(42) );
    int_ptr p2 = foo( boost::move(p) );
    EXPECT_FALSE(p);
    EXPECT_TRUE(p2);
    EXPECT_EQ(42, *p2);
}

TEST(up, pass_temporary)
{
    int_ptr p = foo( int_ptr( new int(42) ) );
    EXPECT_TRUE(p);
    EXPECT_EQ(42, *p);
}

// TEST(up, pass_by_copy)
// {
//     int_ptr p;
//     foo(p);
// }



int main(int argc, char* argv[])
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
