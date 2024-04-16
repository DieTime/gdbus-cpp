/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_DETAILS_COMPILE_TIME_STRING_HPP
#define GDBUS_CPP_DETAILS_COMPILE_TIME_STRING_HPP

#include <algorithm>
#include <cstddef>

namespace gdbus::details {

template<size_t Size>
class compile_time_string
{
public:
    constexpr explicit compile_time_string(const char (&data)[Size]) noexcept
    {
        std::copy(data, data + Size, m_data);
    }

    constexpr const char *data() const noexcept
    {
        return m_data;
    }

    template<size_t OtherSize>
    constexpr auto operator+(compile_time_string<OtherSize> other) const noexcept
    {
        char data[Size + OtherSize - 1] = {0};

        std::copy(m_data, m_data + Size - 1, data);
        std::copy(other.data(), other.data() + OtherSize, data + Size - 1);

        return compile_time_string<Size + OtherSize - 1>(data);
    }

private:
    char m_data[Size] = {0};
};

} /* namespace gdbus::details */

#endif /* GDBUS_CPP_DETAILS_COMPILE_TIME_STRING_HPP */
