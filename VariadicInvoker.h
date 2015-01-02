#pragma once

#include <string>
#include <list>
#include <functional>
#include "easy_bind.h"
#include "StringUtilities.hpp"

using namespace std;

class EmptyClass{};

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
        Rest = Invoker< RET(ARGS...) >(bound);
        return Rest.GetInvoker( args );
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
    Invoker< RET( ARGS... ) > Rest;
    fn_t fn;
};

// This template allows you to invoke a method.  It takes as
// its template parameter the type of a method pointer
//    ie. decltype(&MyClass::myMethod)
template < typename K, typename RET, typename... ARGS >
class Invoker< RET(K::*)( ARGS... ) >
{
    typedef RET(K::*method_ptr_t)( ARGS... );
    typedef function< RET( ARGS... ) > fn_t;

public:
    Invoker( method_ptr_t const& method_ptr, K * thisp )
        : Rest(nullptr)
    {
        fn = easy_bind( std::function< RET( K*, ARGS... )>(method_ptr), thisp );
    }

    Invoker( method_ptr_t const& method_ptr, K & thisr )
        : Rest(nullptr)
    {
        fn = easy_bind( std::function< RET( K*, ARGS... )>(method_ptr), &thisr );
    }

    function< RET() > GetInvoker( list<string> args )
    {
        Rest = Invoker< RET(ARGS...) >(fn);
        return Rest.GetInvoker( args );
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
    Invoker< RET( ARGS... ) > Rest;
    fn_t fn;
};
