#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include "easy_bind.h"
#include "VariadicInvoker.h"

using namespace std;

class TestClass
{
public:
    double my_method( double a, int b, double c)
    {
        cout << "my_method called" << endl;
        cout << "a+b+c+1=" << a + b + c + 1 << endl;
        return a + b + c + 1;
    }
};

void my_fn( double a, int b, double c )
{
	cout << "my_fn called" << endl;
    cout << "a+b+c=" << a + b + c << endl;
}

double my_fn2( double const& a, int const b, double & c )
{
	cout << "my_fn2 called" << endl;
    cout << "a+b+c=" << a + b + c << endl;
    return a + b + c;
}

int main(int, char**)
{
    cout << "begin" << endl;

    TestClass tc;

    auto invoker  = Invoker<decltype(my_fn)>(my_fn);
	auto invoker2 = Invoker<decltype(my_fn2)>(my_fn2);
    auto invoker3 = Invoker<decltype(&TestClass::my_method)>(&TestClass::my_method, tc);
    auto args = {string("1"), string("2"), string("3")};

	cout << "return:" << invoker.Invoke( args ) << endl;
	cout << "return:" << invoker2.Invoke( args ) << endl;
    cout << "return:" << invoker3.Invoke( args ) << endl;

	return 0;
}
