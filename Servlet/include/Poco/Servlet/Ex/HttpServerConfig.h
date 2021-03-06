//
// HttpServerConfig.h
//
//
// Library: ServletEx
// Package: Http
// Module:  HttpServerConfig
//
// Definition of the HttpServerConfig class.
//
// Copyright (c) 2006, Aleksandar Fabijanic and Contributors.
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


#ifndef ServletEx_Http_HttpServerConfig_INCLUDED
#define ServletEx_Http_HttpServerConfig_INCLUDED


#include "Poco/Servlet/ServletBase.h"
#include "Poco/Servlet/Ex/GenericServer.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Exception.h"
#include <vector>


namespace Poco {
namespace Servlet {
namespace Ex {


class Servlet_API HttpServerConfig : public GenericServerConfig
{
public:
	typedef std::map<std::string, std::string> AliasMap;

	HttpServerConfig();

	HttpServerConfig(const std::string& name,
		const std::vector<std::string>& interfaces,
		const AliasMap* pAliases=0,
		const std::string& globalDir="");

	HttpServerConfig(const HttpServerConfig& config);

	HttpServerConfig& operator=(const HttpServerConfig& other);

	const std::vector<std::string>& interfaces() const;

	const AliasMap& aliases() const;

	const std::string& getGlobalDir() const;

	void setGlobalDir(const std::string& globalDir);

protected:
	std::vector<std::string> _interfaces;
	AliasMap _aliases;
	std::string _globalDir;
	 /// This is the directory where the server stores global files,
	 /// such as Javascript, html etc. Requests for these files are
	 /// intercepted by the server and are served directly, without 
	 /// being passed to the servlet container.
};


///
/// inlines
///


inline const std::vector<std::string>& HttpServerConfig::interfaces() const
{
	return _interfaces;
}


inline const HttpServerConfig::AliasMap& HttpServerConfig::aliases() const
{
	return _aliases;
}


} } } // namespace Poco::Servlet::Ex


#endif //ServletEx_Http_HttpServerConfig_INCLUDED
