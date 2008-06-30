//
// SharedArray.h
//
// $Id: //poco/svn/Foundation/include/Poco/SharedArray.h#2 $
//
// Library: Foundation
// Package: Core
// Module: SharedArray
//
// Definition of the SharedArray template class.
//
// Copyright (c) 2005-2008, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_SharedArray_INCLUDED
#define Foundation_SharedArray_INCLUDED


#include "Poco/SharedPtr.h"

namespace Poco {

template <class C, class RC = ReferenceCounter>
class SharedArray
/// SharedArray is a "smart" pointer for classes implementing reference 
/// counting based garbage collection. SharedArray is thus similar to 
/// SharedPtr. Unlike the SharedPtr template, which can only be used with
/// single object pointer management, SharedArray can be used with 
/// dynamically allocated array (Dynamically allocated array are allocated
/// with the C++ new[] expression).
///
/// SharedArray works as SharedPtr but using delete[] expression to
/// destroy the array object.
//
/// For temporal array buffer class, check Poco::Buffer.
{
public:

    typedef void (*pDeleter)(C* p);

    SharedArray(): _pCounter(0), _ptr(0)
    {
    }

    explicit SharedArray(C* ptr): _ptr(ptr)
        /// Constructs a SharedArray object from user provided raw pointer.
        ///
        /// ptr must be a pointer to an dynamically allocated array or null pointer 0.
        /// 
        /// Exception: Thown std::bad_alloc if allocation for reference counting object
        /// fails. If this exception is thrown, the array object will also be deallocated.
    {
        try{
            _pCounter = new RC;
        }
        catch(std::bad_alloc& e){
            delete [] _ptr;
            throw e;
        }
    }

    /// TODO
    /// This copy constructor will be removed. 
    /// 1 Since we can not make sure the class Other have the same layout as type C. 
    ///   We need to check this because for array the layout is always an important issue.
    /// 2 Different from SharePtr, this kind of conversion it is seldom useful for 
    ///   array objects.

    //    template <class Other>
    //    SharedArray(const SharedArray<Other, RC>& ptr): _pCounter(ptr._pCounter), _ptr(const_cast<Other*>(ptr.get()))
    //        _pCounter->duplicate();
    //    }

    SharedArray(const SharedArray& ptr) : _pCounter(0), _ptr(0)
        /// Copy constructs a SharedArray object from another SharedArray object.
        ///
        /// Reference count will be added by 1.
        /// 
        /// Exception: No exception will be thrown.
    {
        assign(ptr);
    }

    /// TODO:
    /// This assignment will be removed.
    //
    //    SharedArray& operator = (C* ptr)
    //    {
    //        return assign(ptr);
    //    }

    SharedArray& operator = (const SharedArray& ptr)
    {
        return assign(ptr);
    }

    ~SharedArray()
        /// The reference count will be decremented by 1. 
        ///
        /// If the count becomes 0, both the array and the reference counting object will be
        /// destroyed. 
        ///
        /// Exception: No exception will be thrown, if and only if the underlying array element's
        /// destructor does not throw.
    {
        release();
    }

    void swap(SharedArray& ptr)
        /// Swap the two pointer
        ///
        /// Exception: No exception will be thrown.
    {
        std::swap(_ptr, ptr._ptr);
        std::swap(_pCounter, ptr._pCounter);
    }

    C& operator [] (std::ptrdiff_t index)
        /// Element access, returns a reference to the desired element according to the given index.
        ///
        /// The user provided index must be less than the object's size, otherwise the behavior is 
        /// undefined. 
        ///
        /// If the SharedArray does not have a valid pointer, which pointer to a dynamically 
        /// allocated array object, NullPointerException will be thrown.
        ///
        /// Exception: Throws NullPointerException if the SharedArray does not have a valid 
        /// pointer to an array.
    {
        return deref(index);
    }

    /// TODO
    /// Do we really need this one?
    const C& operator [] (std::ptrdiff_t index) const
        /// Element access, returns a const reference to the desired element according to the given index.
        ///
        /// The user provided index must be less than the object's size, otherwise the behavior is 
        /// undefined. 
        ///
        /// If the SharedArray does not have a valid pointer, which pointer to a dynamically 
        /// allocated array object, NullPointerException will be thrown.
        ///
        /// Exception: Throws NullPointerException if the SharedArray does not have a valid 
        /// pointer to an array.
    {
        return deref(index);
    }

    C* get()
        /// Returns the underlying raw pointer to the array.
        ///
        /// Exception: No exception will be thrown.
    {
        return _ptr;
    }

    const C* get() const
        /// Returns the const raw pointer.
        ///
        /// Exception: No exception will be thrown.
    {
        return _ptr;
    }

    /// TODO
    /// This implicit pointer cast will be removed. Since it is danger.
    //
    //    operator C* ()
    //    {
    //        return _ptr;
    //    }

    /// TODO
    /// This implicit cast will be removed. Since it is danger.
    //    operator const C* () const
    //    {
    //        return _ptr;
    //    }

    operator bool () const
        /// Returns true if the underlying pionter is not null pointer. 
        ///
        /// Exception: No exception will be thrown.
    {
        return (! isNull() );
    }

    bool operator ! () const
        /// Returns true if the underlying pionter is null pointer. 
        ///
        /// Exception: No exception will be thrown.
    {
        return isNull();
    }

    bool operator == (const SharedArray& ptr) const
    {
        return get() == ptr.get();
    }

    bool operator == (const C* ptr) const
    {
        return get() == ptr;
    }

    bool operator == (C* ptr) const
    {
        return get() == ptr;
    }

    bool operator != (const SharedArray& ptr) const
    {
        return get() != ptr.get();
    }

    bool operator != (const C* ptr) const
    {
        return get() != ptr;
    }

    bool operator != (C* ptr) const
    {
        return get() != ptr;
    }

    bool operator < (const SharedArray& ptr) const
    {
        return get() < ptr.get();
    }

    bool operator < (const C* ptr) const
    {
        return get() < ptr;
    }

    bool operator < (C* ptr) const
    {
        return get() < ptr;
    }

    bool operator <= (const SharedArray& ptr) const
    {
        return get() <= ptr.get();
    }

    bool operator <= (const C* ptr) const
    {
        return get() <= ptr;
    }

    bool operator <= (C* ptr) const
    {
        return get() <= ptr;
    }

    bool operator > (const SharedArray& ptr) const
    {
        return get() > ptr.get();
    }

    bool operator > (const C* ptr) const
    {
        return get() > ptr;
    }

    bool operator > (C* ptr) const
    {
        return get() > ptr;
    }

    bool operator >= (const SharedArray& ptr) const
    {
        return get() >= ptr.get();
    }

    bool operator >= (const C* ptr) const
    {
        return get() >= ptr;
    }

    bool operator >= (C* ptr) const
    {
        return get() >= ptr;
    }

private:

    bool isNull() const
    {
        return _ptr == 0;
    }

    /// TODO:
    /// This assignment will be removed.
    // SharedArray& assign(C* ptr)
    // {
    //     if (get() != ptr)
    //     {
    //         RC* pTmp = 0;
    //         // if allocation failed, delete the given object and do not touch the
    //         // SharedArray object.
    //         try{
    //             pTmp = new RC;
    //         }
    //         catch(std::bad_alloc e){
    //             delete [] ptr;
    //             return *this;
    //         }
    // 
    //         // destroy the SharedArray and build the new one.
    //         release();
    // 
    //         // 
    //         _pCounter = pTmp;
    //         _ptr = ptr;
    //     }
    //     return *this;
    // }

    SharedArray& assign(const SharedArray& ptr)
    {
        if (&ptr != this)
        {
            release();
            _pCounter = ptr._pCounter;
            _ptr = ptr._ptr;
            if(_pCounter){
                _pCounter->duplicate();
            }
        }
        return *this;
    }

    /// TODO
    /// This assigment helper will be removed. 
    /// See copy constructor for details.
    //
    //    template <class Other>
    //    SharedArray& assign(const SharedArray<Other, RC>& ptr)
    //    {
    //        if (ptr.get() != _ptr)
    //        {
    //            SharedArray tmp(ptr);
    //            swap(tmp);
    //        }
    //        return *this;
    //    }

    C& deref(std::ptrdiff_t index)
    {
        if (!_ptr)
            throw NullPointerException();

        return _ptr[index];
    }

    const C& deref(std::ptrdiff_t index) const
    {
        if (!_ptr)
            throw NullPointerException();

        return _ptr[index];
    }

    void release()
    {
        if(_pCounter){
           int i = _pCounter->release();
           if (i == 0)
           {
               if (_ptr)
                   delete [] _ptr;
               _ptr = 0;
               delete _pCounter;
               _pCounter = 0;
           }
        }        
    }

private:

    RC* _pCounter;
    C*  _ptr;
};

} // namespace Poco


#endif // Foundation_SharedArray_INCLUDED

