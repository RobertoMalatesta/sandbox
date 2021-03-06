//
// DynamicAnyTest.h
//
// $Id: //poco/svn/Foundation/testsuite/src/DynamicAnyTest.h#2 $
//
// Tests for Any types
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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

#ifndef DynamicAnyTest_INCLUDED
#define DynamicAnyTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DynamicAny.h"
#include "Poco/Exception.h"
#include "CppUnit/TestCase.h"


class DynamicAnyTest: public CppUnit::TestCase
{
public:
	DynamicAnyTest(const std::string& name);
	~DynamicAnyTest();

	void testInt8();
	void testInt16();
	void testInt32();
	void testInt64();
	void testUInt8();
	void testUInt16();
	void testUInt32();
	void testUInt64();
	void testBool();
	void testChar();
	void testFloat();
	void testDouble();
	void testLong();
	void testULong();
	void testString();
	void testUDT();
	void testConversionOperator();
	void testComparisonOperators();
	void testArithmeticOperators();
	void testLimitsInt();
	void testLimitsFloat();
	void testCtor();
	void testIsStruct();
	void testIsArray();
	void testArrayIdxOperator();
	void testDynamicStructBasics();
	void testDynamicStruct();
	void testArrayToString();
	void testStructToString();
	void testArrayOfStructsToString();
	void testStructWithArraysToString();
	void testJSONDeserializeString();
	void testJSONDeserializePrimitives();
	void testJSONDeserializeArray();
	void testJSONDeserializeStruct();
	void testJSONDeserializeComplex();
	void testDate();
	void testEmpty();


	void setUp();
	void tearDown();
	static CppUnit::Test* suite();

private:
	void testGetIdxMustThrow(Poco::DynamicAny& a1, std::vector<Poco::DynamicAny>::size_type n);
	template<typename T>
	void testGetIdx(Poco::DynamicAny& a1, std::vector<Poco::DynamicAny>::size_type n, const T& expectedResult)
	{
		Poco::DynamicAny& val1 = a1[n];
		assert (val1 == expectedResult);

		const Poco::DynamicAny& c1 = a1;
		assert (a1 == c1); // silence the compiler
		const Poco::DynamicAny& cval1 = a1[n];
		assert (cval1 == expectedResult);
	}


	template<typename TL, typename TS>
	void testLimitsSigned()
	{
		TL iMin = std::numeric_limits<TS>::min();
		Poco::DynamicAny da = iMin - 1;
		try { TS i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}

		TL iMax = std::numeric_limits<TS>::max();
		da = iMax + 1;
		try { TS i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template<typename TL, typename TS>
	void testLimitsFloatToInt()
	{
		Poco::DynamicAny da;

		if (std::numeric_limits<TS>::is_signed)
		{
			TL iMin = static_cast<TL>(std::numeric_limits<TS>::min());
			da = iMin * 10;
			try { TS i; i = da.convert<TS>(); fail("must fail"); }
			catch (Poco::RangeException&) {}
		}

		TL iMax = static_cast<TL>(std::numeric_limits<TS>::max());
		da = iMax * 10;
		try { TS i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template<typename TS, typename TU>
	void testLimitsSignedUnsigned()
	{
		assert (std::numeric_limits<TS>::is_signed);
		assert (!std::numeric_limits<TU>::is_signed);

		TS iMin = std::numeric_limits<TS>::min();
		Poco::DynamicAny da = iMin;
		try { TU i; i = da.convert<TU>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template<typename TL, typename TS>
	void testLimitsUnsigned()
	{
		TL iMax = std::numeric_limits<TS>::max();
		Poco::DynamicAny da = iMax + 1;
		try { TS i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template <typename T>
	void testEmptyComparisons()
	{
		Poco::DynamicAny da;
		T val = 0;

		assert (da != val);
		assert (val != da);
		assert (!(val == da));
		assert (!(da == val));
		assert (!(da < val));
		assert (!(val < da));
		assert (!(da > val));
		assert (!(val > da));
		assert (!(da <= val));
		assert (!(val <= da));
		assert (!(da >= val));
		assert (!(val >= da));
	}
};


#endif // DynamicAnyTest_INCLUDED
