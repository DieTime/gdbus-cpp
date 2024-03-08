/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_DEBUGGER_HPP
#define GDBUS_CPP_DEBUGGER_HPP

#include <iostream>
#include <sstream>

namespace gdbus {

class debugger
{
public:
#ifdef GDBUS_CPP_ENABLE_DEBUG_LOGGING
    ~debugger()
    {
        if (buffer_size() > 0) {
            std::cout << "[gdbuscpp] " << m_buffer.str() << "\n";
        }
    }
#endif

    template<typename T>
    debugger &operator<<([[maybe_unused]] T &&message)
    {
#ifdef GDBUS_CPP_ENABLE_DEBUG_LOGGING
        m_buffer << std::forward<T>(message);
#endif
        return *this;
    }

private:
#ifdef GDBUS_CPP_ENABLE_DEBUG_LOGGING
    std::streampos buffer_size()
    {
        m_buffer.seekg(0, std::ios::end);
        std::streampos size = m_buffer.tellg();

        m_buffer.seekg(0, std::ios::beg);
        return size;
    }

    std::stringstream m_buffer;
#endif
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_DEBUGGER_HPP */
