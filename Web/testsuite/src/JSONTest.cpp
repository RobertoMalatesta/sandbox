//
// JSONTest.cpp
//
// $Id: //poco/Main/Util/testsuite/src/JSONTest.cpp#6 $
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


#include "JSONTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Web/JSONEntity.h"
#include "Poco/Web/JSONParser.h"
#include "Poco/Web/JSONPrinter.h"
#include "Poco/Web/JSONCondenser.h"
#include "Poco/Web/ExtJS/DirectHandler.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/SharedPtr.h"
#include <sstream>


using Poco::Web::JSONEntity;
using Poco::Web::JSONParser;
using Poco::Web::JSONPrinter;
using Poco::Web::JSONCondenser;
using Poco::Web::ExtJS::DirectHandler;
using Poco::Dynamic::Var;
using Poco::SharedPtr;


class TestDirectHandler: public DirectHandler
{
public:
	TestDirectHandler(std::ostream& out): DirectHandler(out)
	{
	}

	~TestDirectHandler()
	{
	}

	void handleData(const JSONEntity& val)
	{
		if (isArray())
		{
			switch(val.type())
			{
			case JSONEntity::JSON_T_STRING:
				_data.push_back(val.toString());
				break;
			case JSONEntity::JSON_T_INTEGER:
				_data.push_back(val.toInteger());
				break;
			case JSONEntity::JSON_T_FLOAT:
				_data.push_back(val.toFloat());
				break;
			case JSONEntity::JSON_T_TRUE:
				_data.push_back(true);
				break;
			case JSONEntity::JSON_T_FALSE:
				_data.push_back(false);
				break;
			case JSONEntity::JSON_T_NULL:
				_data.push_back(Var());
				break;
			default:
				throw Poco::InvalidArgumentException("Unknown type.");
			}
		}
	}

	Var& get(int index)
	{
		poco_assert (index < _data.size());
		return _data[index];
	}

private:
	std::vector<Var> _data;
};


JSONTest::JSONTest(const std::string& name): CppUnit::TestCase(name)
{
}


JSONTest::~JSONTest()
{
}


void JSONTest::testPrinter()
{
	const std::string str("{"
		"\"firstName\": \"John\","
		"\"lastName\": \"Smith\","
		"\"address\": {"
			"\"streetAddress\": \"21 2nd Street\","
			"\"city\": \"New York\","
			"\"state\": \"NY\","
			"\"postalCode\": 10021"
		"},"
	"\"phoneNumbers\": ["
		"\"212 555-1234\","
		"\"646 555-4567\""
	"],"
	"\"weight\": {"
		"\"value\": 123.456, \"units\": \"lbs\""
		"}"
	"}");

	std::ostringstream os;
	JSONParser jp(new JSONPrinter(os));
	jp.parse(str);

	std::string s1 = os.str();

	std::ostringstream ros;
	ros << "{" << std::endl;
	ros << "key = 'firstName', value = string: 'John'" << std::endl;
	ros << "key = 'lastName', value = string: 'Smith'" << std::endl;
	ros << "key = 'address', value = {" << std::endl;
	ros << "key = 'streetAddress', value = string: '21 2nd Street'" << std::endl;
	ros << "key = 'city', value = string: 'New York'" << std::endl;
	ros << "key = 'state', value = string: 'NY'" << std::endl;
	ros << "key = 'postalCode', value = integer: 10021" << std::endl;
	ros << "\t}" << std::endl;
	ros << "key = 'phoneNumbers', value = [" << std::endl;
	ros << "\t\tstring: '212 555-1234'" << std::endl;
	ros << "\t\tstring: '646 555-4567'" << std::endl;
	ros << "\t]" << std::endl;
	ros << "key = 'weight', value = {" << std::endl;
	ros << "key = 'value', value = float: 123.456" << std::endl;
	ros << "key = 'units', value = string: 'lbs'" << std::endl;
	ros << "\t}" << std::endl;
	ros << "}" << std::endl;

	assert (ros.str() == os.str());
}


void JSONTest::testCondenser()
{
	const std::string str("{"
		"\"firstName\": \"John\","
		"\"lastName\":  \"Smith\","
		"\"address\": {"
			"\"streetAddress\": \"21 2nd Street\","
			"\"city\":          \"New York\","
			"\"state\":         \"NY\","
			"\"postalCode\":     10021"
		"},"
	"\"phoneNumbers\": ["
		"\"212 555-1234\","
		"\"646 555-4567\""
	"],"
	"\"weight\": {"
		"\"value\": 123.456, \"units\": \"lbs\""
		"}"
	"}");

	std::ostringstream os;
	JSONParser jp(new JSONCondenser(os));
	jp.parse(str);

	std::string s1 = os.str();

	std::ostringstream ros;
	ros << "{\"firstName\":\"John\",\"lastName\":\"Smith\","
		"\"address\":{"
			"\"streetAddress\":\"21 2nd Street\","
			"\"city\":\"New York\","
			"\"state\":\"NY\","
			"\"postalCode\":10021},"
	"\"phoneNumbers\":[\"212 555-1234\",\"646 555-4567\"],"
	"\"weight\":{\"value\":123.456,\"units\":\"lbs\"}}";

	assert (ros.str() == os.str());
}


void JSONTest::testExtJSDirect()
{
	std::string str = "{\"action\":\"AlbumList\",\"method\":\"getAll\",\"data\":[\"abc\",456,1.5,null,true,false],\"type\":\"rpc\",\"tid\":123}";
	std::ostringstream os;
	SharedPtr<TestDirectHandler> pTDH = new TestDirectHandler(os);
	JSONParser jp(pTDH);
	jp.parse(str);

	assert (pTDH->action() == "AlbumList");
	assert (pTDH->method() == "getAll");
	assert (pTDH->type() == DirectHandler::DIRECT_TYPE_RPC);
	assert (pTDH->tid() == 123);
	assert (pTDH->get(0) == "abc");
	assert (pTDH->get(1) == 456);
	assert (pTDH->get(2) == 1.5);
	assert (pTDH->get(3).isEmpty());
	assert (pTDH->get(4));
	assert (!pTDH->get(5));
}


void JSONTest::setUp()
{
}


void JSONTest::tearDown()
{
}


CppUnit::Test* JSONTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONTest");

	CppUnit_addTest(pSuite, JSONTest, testPrinter);
	CppUnit_addTest(pSuite, JSONTest, testCondenser);
	CppUnit_addTest(pSuite, JSONTest, testExtJSDirect);

	return pSuite;
}
