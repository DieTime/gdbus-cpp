/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#include "error.hpp"

namespace gdbus {

error::error(std::string name, std::string message) noexcept
    : m_name(std::move(name))
    , m_message(std::move(message))
{}

const std::string &error::name() const noexcept
{
    return m_name;
}

const std::string &error::message() const noexcept
{
    return m_message;
}

const char *error::what() const noexcept
{
    return m_message.c_str();
}

} /* namespace gdbus */
