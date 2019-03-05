libssh2 library build for Staticlibs
====================================

[![travis](https://travis-ci.org/staticlibs/external_libssh2.svg?branch=master)](https://travis-ci.org/staticlibs/external_libssh2)
[![appveyor](https://ci.appveyor.com/api/projects/status/github/staticlibs/external_libssh2?svg=true)](https://ci.appveyor.com/project/staticlibs/external-libssh2)

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [libssh2 library](https://www.libssh2.org/) that
can be used to build sources imported from [CentOS lookaside](https://github.com/staticlibs/lookaside_libssh2).

Link to the [documentation](https://www.libssh2.org/docs.html).

How to build
------------

[CMake](http://cmake.org/) is required for building.

[pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/) utility is used for dependency management.
For Windows users ready-to-use binary version of `pkg-config` can be obtained from [tools_windows_pkgconfig](https://github.com/staticlibs/tools_windows_pkgconfig) repository.
See [StaticlibsPkgConfig](https://github.com/staticlibs/wiki/wiki/StaticlibsPkgConfig) for Staticlibs-specific details about `pkg-config` usage.

[Perl](https://www.perl.org/) is also required for building, Windows users can obtain ready-to-use
Perl distribution from [tools_windows_perl](https://github.com/staticlibs/tools_windows_perl) repository.

[NASM](http://nasm.us/) is also required for building on Windows.
You can obtain ready-to-use NASM distribution from 
[tools_windows_nasm](https://github.com/staticlibs/tools_windows_nasm) repository.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone https://github.com/staticlibs/external_zlib.git
    git clone https://github.com/staticlibs/lookaside_zlib.git
    git clone https://github.com/staticlibs/external_openssl.git
    git clone https://github.com/staticlibs/lookaside_openssl.git
    git clone https://github.com/staticlibs/external_libssh2.git
    git clone https://github.com/staticlibs/lookaside_libssh2.git
    cd external_libssh2
    mkdir build
    cd build
    cmake ..
    msbuild external_libssh2.sln

Cloning of [external_zlib](https://github.com/staticlibs/external_zlib) and 
[external_openssl](https://github.com/staticlibs/external_openssl.git) is not required on Linux - 
system libraries will be used instead.

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2019-03-04**

 * version 1.4.3-1
 * initial public version
