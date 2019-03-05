/*
 * Copyright 2019, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   libssh2_test.cpp
 * Author: alex
 *
 * Created on February 10, 2015, 11:25 AM
 */

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <iostream>
#include <string>

#include "libssh2_config.h"
#include "libssh2.h"

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>

#include "staticlib/config/assert.hpp"
#include "staticlib/io.hpp"
#include "staticlib/support.hpp"

int main() {

#ifdef WIN32
    WSADATA wsadata;
    std::memset(std::addressof(a), '\0', sizeof(WSADATA))
    auto err_wsa = WSAStartup(MAKEWORD(2, 0), std::addressof(wsadata));
    slassert(0 == err_wsa);
#endif

    auto err_init = libssh2_init(0);
    slassert(0 == err_init);

    /* Ultra basic "connect to port 22 on localhost"
     * Your code is responsible for creating the socket establishing the
     * connection
     */
    auto sock = socket(AF_INET, SOCK_STREAM, 0);

    unsigned long hostaddr = htonl(0x7f000001);
    struct sockaddr_in sin;
    std::memset(std::addressof(sin), '\0', sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    auto err_conn = connect(sock, reinterpret_cast<struct sockaddr*>(std::addressof(sin)), sizeof(struct sockaddr_in));
    slassert(0 == err_conn);

    auto deferred_sock = sl::support::defer([&sock]() STATICLIB_NOEXCEPT {
#ifdef WIN32
        closesocket(sock);
#else
        close(sock);
#endif
    });

    /* Create a session instance
     */
    LIBSSH2_SESSION* session = libssh2_session_init();
    slassert(nullptr != session);

    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */
    auto err_session = libssh2_session_handshake(session, sock);
    slassert(0 == err_session);

    auto deferred_session = sl::support::defer([&session]() STATICLIB_NOEXCEPT {
        libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
        libssh2_session_free(session);
    });

    const char* fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
    auto fp_binary = std::string(fingerprint, 20);
    auto fp_str = sl::io::string_to_hex(fp_binary);
    std::cout << "Fingerprint: " << fp_str << std::endl;

    libssh2_exit();

    return 0;
}
