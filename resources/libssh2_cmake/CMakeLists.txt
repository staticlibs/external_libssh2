# Copyright 2019, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )
project ( ssh2 C )
include ( ${CMAKE_CURRENT_LIST_DIR}/../macros.cmake )

if ( NOT DEFINED ${PROJECT_NAME}_LOOKASIDE_DIR )
    set ( ${PROJECT_NAME}_LOOKASIDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../lookaside_libssh2 CACHE INTERNAL "" )
endif ( )

# standalone build
if ( NOT DEFINED STATICLIB_TOOLCHAIN )
    if ( NOT DEFINED ${PROJECT_NAME}_TOOLCHAIN )
        if ( WIN32 )
            if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_amd64_msvc" )
            else ( )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_i386_msvc" )
            endif ( )
        elseif ( APPLE )
            set ( ${PROJECT_NAME}_TOOLCHAIN "macosx_amd64_clang" )
        elseif ( UNIX )
            set ( ${PROJECT_NAME}_TOOLCHAIN "linux_amd64_gcc" )
        else ( )
            message ( FATAL_ERROR "Cannot determine toolchain to use, please specify [${PROJECT_NAME}_TOOLCHAIN]" )
        endif ( )
    endif ( )
else ( )
    set ( ${PROJECT_NAME}_TOOLCHAIN ${STATICLIB_TOOLCHAIN} )
endif ( )

# dependencies
set ( ${PROJECT_NAME}_DEPS zlib openssl )
external_libssh2_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# check deplibs cache
if ( STATICLIB_USE_DEPLIBS_CACHE )
    set ( ${PROJECT_NAME}_CACHED_LIB_PATH ${STATICLIB_DEPLIBS_CACHE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} )
    if ( EXISTS ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
        set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        message ( STATUS "Using cached library: [${${PROJECT_NAME}_CACHED_LIB_PATH}]" )
        return ( )
    endif ( )
endif ( )

# sources
set ( ${PROJECT_NAME}_SRC 
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/channel.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/comp.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/crypt.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/hostkey.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/kex.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/mac.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/misc.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/packet.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/publickey.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/scp.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/session.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/sftp.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/userauth.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/transport.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/version.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/knownhost.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/agent.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/openssl.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/libgcrypt.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/pem.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/keepalive.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/global.c )
source_group ( "src" FILES ${${PROJECT_NAME}_SRC} )

# library
add_library ( ${PROJECT_NAME} STATIC 
        ${${PROJECT_NAME}_SRC} )
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include_${${PROJECT_NAME}_TOOLCHAIN}
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/include
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} ) 
target_compile_definitions ( ${PROJECT_NAME} PRIVATE -DHAVE_CONFIG_H )
