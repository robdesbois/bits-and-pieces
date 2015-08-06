#include <boost/mpl/identity.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <string>
#include <iostream>
#include <memory>

struct Base
{
    virtual ~Base() {}
    static constexpr const char* const name() { return "base"; }

    virtual std::string whoami() { return name(); }
};

struct Derived1: public Base
{
    static constexpr const char* const name() { return "one"; }

    virtual std::string whoami() { return name(); }
};

struct Derived2: public Base
{
    static constexpr const char* const name() { return "two"; }

    virtual std::string whoami() { return name(); }
};

struct Factory
{
    using Strategies = boost::mpl::vector<
        boost::mpl::identity< Base >,
        boost::mpl::identity< Derived1 >,
        boost::mpl::identity< Derived2 >
    >;

    struct CreateNamedStrategy
    {
        CreateNamedStrategy(std::string strategyName_, std::unique_ptr<Base>& strategyPtr_):
                strategyName(std::move(strategyName_)),
                strategyPtr(&strategyPtr_)
        {}

        template<class T>
        void operator()(const T&)
        {
            if (T::type::name() == strategyName)
                strategyPtr->reset( new typename T::type{} );
        }

        std::unique_ptr<Base>* strategyPtr;

    private:
        std::string strategyName;
    };

    static std::unique_ptr<Base> create(std::string name)
    {
        std::unique_ptr<Base> strategy;
        CreateNamedStrategy creator{name, strategy};
        boost::mpl::for_each<Strategies>(creator);
        return std::move(strategy);
    }
};

int main(int argc, char**argv)
{
    if (argc < 2)
        return -1;

    auto p = Factory::create(argv[1]);
    if (p)
        std::cout << "Created " << p->whoami() << std::endl;
    else
        std::cout << "No match!" << std::endl;
}
