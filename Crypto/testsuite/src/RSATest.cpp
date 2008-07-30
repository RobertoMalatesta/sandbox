//
// RSATest.cpp
//
// $Id: //poco/Main/Crypto/testsuite/src/RSATest.cpp#2 $
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#include "RSATest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include <sstream>

using namespace Poco::Crypto;


RSATest::RSATest(const std::string& name): CppUnit::TestCase(name)
{
}


RSATest::~RSATest()
{
}


void RSATest::testNewKeys()
{
	RSAKey key(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	std::ostringstream strPub;
	std::ostringstream strPriv;
	key.save(&strPub, &strPriv, "testpwd");
	std::string pubKey = strPub.str();
	std::string privKey = strPriv.str();

	//now do the round trip
	std::istringstream iPub(pubKey);
	std::istringstream iPriv(privKey);
	RSAKey key2(&iPub, &iPriv, "testpwd");

	std::istringstream iPriv2(privKey);
	RSAKey key3(0, &iPriv2,  "testpwd");
	std::ostringstream strPub3;
	key3.save(&strPub3);
	std::string pubFromPrivate = strPub3.str();
	assert (pubFromPrivate == pubKey);
}


void RSATest::testSign()
{
	std::string msg("Test this sign message");
	RSAKey key(RSAKey::KL_2048, RSAKey::EXP_LARGE);
	RSADigestEngine eng(key);
	eng.update(msg.c_str(), msg.length());
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	//verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	RSAKey keyPub(&iPub);
	RSADigestEngine eng2(key);
	eng2.update(msg.c_str(), msg.length());
	eng2.verify(sig);
}


void RSATest::testSignManipulated()
{
	std::string msg("Test this sign message");
	std::string msgManip("Test that sign message");
	RSAKey key(RSAKey::KL_2048, RSAKey::EXP_LARGE);
	RSADigestEngine eng(key);
	eng.update(msg.c_str(), msg.length());
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	//verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	RSAKey keyPub(&iPub);
	RSADigestEngine eng2(key);
	eng2.update(msgManip.c_str(), msgManip.length());
	try
	{
		eng2.verify(sig);
		fail("must fail");
	}
	catch(Poco::DataFormatException&)
	{
	}
}


void RSATest::createRSACipher()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(RSAKey(RSAKey::KL_1024,RSAKey::EXP_SMALL));
	std::string val("lets do some encryption");
	std::string enc = pCipher->encryptString(val);
	std::string dec = pCipher->decryptString(enc);
	assert (dec == val);
}



void RSATest::createRSACipherLarge()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(RSAKey(RSAKey::KL_1024,RSAKey::EXP_SMALL));
	std::string val(16385, 'x');
	std::string enc = pCipher->encryptString(val);
	std::string dec = pCipher->decryptString(enc);
	assert (dec == val);
}


void RSATest::setUp()
{
}


void RSATest::tearDown()
{
}


CppUnit::Test* RSATest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RSATest");

	CppUnit_addTest(pSuite, RSATest, testNewKeys);
	CppUnit_addTest(pSuite, RSATest, testSign);
	CppUnit_addTest(pSuite, RSATest, testSignManipulated);
	CppUnit_addTest(pSuite, RSATest, createRSACipher);
	CppUnit_addTest(pSuite, RSATest, createRSACipherLarge);

	return pSuite;
}
