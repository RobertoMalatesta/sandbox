//
// RunnableAdapter.h
//
// $Id: //poco/svn/Foundation/include/Poco/RunnableAdapter.h#2 $
//
// Library: Foundation
// Package: Threading
// Module:  RunnableAdapter
//
// Definition of the RunnableAdapter template class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_RunnableAdapter_INCLUDED
#define Foundation_RunnableAdapter_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Runnable.h"


namespace Poco {


template <class C>
class RunnableAdapter: public Runnable
	/// This adapter simplifies using ordinary methods as
	/// targets for threads.
	/// Usage:
	///    RunnableAdapter<MyClass> ra(myObject, &MyObject::doSomething));
	///    Thread thr;
	///    thr.start(ra);
{
public:
	typedef void (C::*Callback)();
	
	RunnableAdapter(C& object, Callback method):
		_pObject(&object),
		_method(method)
	{
	}
	
	RunnableAdapter(const RunnableAdapter& ra):
		_pObject(ra._pObject),
		_method(ra._method)
	{
	}

	~RunnableAdapter()
	{
	}

	RunnableAdapter& operator = (const RunnableAdapter& ra)
	{
		_pObject = ra._pObject;
		_method  = ra._method;
		return *this;
	}

	void run()
	{
		(_pObject->*_method)();
	}
	
private:
	RunnableAdapter();

	C*       _pObject;
	Callback _method;
};


} // namespace Poco


#endif // Foundation_RunnableAdapter_INCLUDED
