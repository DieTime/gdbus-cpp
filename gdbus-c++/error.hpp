/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_ERROR_HPP
#define GDBUS_CPP_ERROR_HPP

#include "common.hpp"

#include <stdexcept>

namespace gdbus {

class GDBUS_CPP_EXPORT_CLASS(error)
    : public std::runtime_error
{
public:
    error(std::string name, std::string message);

    const std::string &name() const noexcept;
    const std::string &message() const noexcept;

private:
    std::string m_name;
    std::string m_message;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_ERROR_HPP */
