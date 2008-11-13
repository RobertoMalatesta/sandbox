--
-- testsuite.lua
--
-- Premake build script for test
--
-- Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
-- and Contributors.
--
-- Permission is hereby granted, free of charge, to any person or organization
-- obtaining a copy of the software and accompanying documentation covered by
-- this license (the "Software") to use, reproduce, display, distribute,
-- execute, and transmit the Software, and to prepare derivative works of the
-- Software, and to permit third-parties to whom the Software is furnished to
-- do so, all subject to the following:
--
-- The copyright notices in the Software and this entire statement, including
-- the above license grant, this restriction and the following disclaimer,
-- must be included in all copies of the Software, in whole or in part, and
-- all derivative works of the Software, unless such copies or derivative
-- works are solely in the form of machine-executable object code generated by
-- a source language processor.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
-- SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
-- FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
-- ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
-- DEALINGS IN THE SOFTWARE.
--

package.name = "testsuite"
package.kind = "exe"
package.language = "c++"
package.path = "custom"
package.objdir = "obj/tests"
package.bindir = "../../bin"

package.config["debug_shared"].target= package.name .."Sharedd"
package.config["release_shared"].target= package.name .."Shared"
package.config["debug_static"].target= package.name .. "d"
package.config["release_static"].target= package.name


if (not windows) then

    table.insert(package.config["debug_shared"].buildoptions, "-shared")
    table.insert(package.config["release_shared"].buildoptions, "-shared")

    -- for cygwin
    if(not linux) then
        table.insert(package.config["release_shared"].buildoptions, "-mwindows")
        table.insert(package.config["debug_shared"].buildoptions, "-mwindows")
    end

end

package.includepaths =
{
    "../../Foundation/include"
}

package.defines =
{
    "_CRT_SECURE_NO_DEPRECATE"
}

package.files =
{
    matchrecursive("../../Foundation/testsuite/*.h"),
    matchrecursive("../../Foundation/testsuite/*.cpp")
}

package.links = { "Foundation"}

if (linux) then
    table.insert(package.links, "pthread")
end

if (windows) then
    table.insert(package.defines, "WIN32")
end

-- Output is placed in a directory named for the target toolset.
package.path = options["target"]

table.insert(package.config["debug_shared"].defines, "_USRDLL")
table.insert(package.config["debug_shared"].defines, "Foundation_EXPORTS")
table.insert(package.config["release_shared"].defines, "_USRDLL")
table.insert(package.config["release_shared"].defines, "Foundation_EXPORTS")

table.insert(package.config["debug_static"].defines, "POCO_STATIC")
table.insert(package.config["release_static"].defines, "POCO_STATIC")
