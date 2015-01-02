#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include "VariadicInvoker.h"

using namespace std;

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

    auto invoker = Invoker<decltype(my_fn)>(my_fn);
	auto invoker2 = Invoker<decltype(my_fn2)>(my_fn2);
    auto args = {string("1"), string("2"), string("3")};

	cout << "return:" << invoker.Invoke( args ) << endl;
	cout << "return:" << invoker2.Invoke( args ) << endl;

	return 0;
}
