/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_DEBUGGER_HPP
#define GDBUS_CPP_DEBUGGER_HPP

#ifdef GDBUS_CPP_BUILD_WITH_DEBUG_LOGGING
#include <iostream>
#include <sstream>
#include <unistd.h>
#endif

namespace gdbus {

#ifdef GDBUS_CPP_BUILD_WITH_DEBUG_LOGGING
class debugger
{
public:
    debugger()
        : m_enabled(getenv("GDBUS_CPP_DEBUG") != nullptr)
    {}

    ~debugger()
    {
        if (m_enabled && m_buffer.tellp() != std::streampos(0)) {
            bool use_color = isatty(fileno(stdout)) && !getenv("GDBUS_CPP_DEBUG_NO_COLORS");

            if (use_color) {
                std::cout << "\033[0;90m";
            }

            std::cout << '\n' << "[gdbuscpp] " << m_buffer.str() << '\n';

            if (use_color) {
                std::cout << "\033[0m";
            }
        }
    }

    template<typename T>
    debugger &operator<<(T &&message)
    {
        if (m_enabled) {
            m_buffer << std::forward<T>(message);
        }

        return *this;
    }

private:
    bool m_enabled;
    std::stringstream m_buffer;
};
#else
class debugger
{
public:
    template<typename T>
    debugger &operator<<(T &&)
    {
        return *this;
    }
};
#endif

} /* namespace gdbus */

#endif /* GDBUS_CPP_DEBUGGER_HPP */
