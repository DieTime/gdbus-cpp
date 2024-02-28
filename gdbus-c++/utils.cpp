/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#include "utils.hpp"

namespace gdbus {

std::string bus_type_to_string(GBusType bus_type) noexcept
{
    switch (bus_type) {
    case G_BUS_TYPE_SESSION:
        return "session";
    case G_BUS_TYPE_SYSTEM:
        return "system";
    default:
        return "(unknown)";
    }
}

std::string append_g_error(const std::string &string, GError *error) noexcept
{
    if (error) {
        return string + " (" + error->message + ")";
    }

    return string;
}

} /* namespace gdbus */
