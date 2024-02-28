/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_ERROR_HPP
#define GDBUS_CPP_ERROR_HPP

#include <stdexcept>
#include <string>

namespace gdbus {

class error: public std::exception
{
public:
    error(std::string name, std::string message) noexcept;

    const std::string &name() const noexcept;
    const std::string &message() const noexcept;

    const char *what() const noexcept override;

private:
    std::string m_name;
    std::string m_message;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_ERROR_HPP */
