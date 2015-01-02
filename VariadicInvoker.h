#include <string>
#include <list>
#include <functional>
#include "easy_bind.h"
#include "StringUtilities.hpp"

using namespace std;

template <typename SIG>
class Invoker;

template < typename RET >
class Invoker< RET () >
{
	typedef function< RET() > fn_t;
public:

	Invoker( fn_t fn_in )
		: fn(fn_in)
	{
	} 

    function< RET() > GetInvoker( list<string> )
	{
		return fn;
	}

	template < typename U = RET >
	typename enable_if<!is_void<U>::value,string>::type 
	Invoke( list<string> args )
	{
		return toString<RET>(GetInvoker(args)());
	}	

	template < typename U = RET >
	typename enable_if<is_void<U>::value,string>::type
	Invoke( list<string> args )
	{
		GetInvoker(args)();
		return "";
	}

private:
		fn_t fn;
};
	
template < typename RET, typename T, typename... ARGS >
class Invoker< RET ( T, ARGS... ) >
{
	typedef function< RET( T, ARGS... ) > fn_t;

public:
	Invoker( fn_t fn_in )
		: Rest(nullptr), fn( fn_in )
	{
	}

	function< RET() > GetInvoker( list<string> args )
	{
        auto t = fromString<T>(args.front());
		args.pop_front();
		auto bound = easy_bind( fn, t );
		Rest = new Invoker< RET(ARGS...) >(bound);
		return Rest->GetInvoker( args );
	}

	template < typename U = RET >
	typename enable_if<!is_void<U>::value,string>::type 
	Invoke( list<string> args )
	{
		return toString<RET>(GetInvoker(args)());
	}	

	template < typename U = RET >
	typename enable_if<is_void<U>::value,string>::type
	Invoke( list<string> args )
	{
		GetInvoker(args)();
		return "";
	}

private:
    Invoker< RET( ARGS... ) > *Rest;
    fn_t fn;
};
