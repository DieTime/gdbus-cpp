/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_UTILS_HPP
#define GDBUS_CPP_UTILS_HPP

#include <string>
#include <gio/gio.h>

namespace gdbus {

std::string bus_type_to_string(GBusType bus_type) noexcept;
std::string append_g_error(const std::string &string, GError *error) noexcept;

} /* namespace gdbus */

#endif /* GDBUS_CPP_UTILS_HPP */
