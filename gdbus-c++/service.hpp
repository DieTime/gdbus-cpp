/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_SERVICE_HPP
#define GDBUS_CPP_SERVICE_HPP

#include "object.hpp"

#include <string>
#include <vector>
#include <gio/gio.h>

namespace gdbus {

class service
{
public:
    explicit service(std::string name) noexcept;

    const std::string &name() const noexcept;

    service &on_system_bus() noexcept;
    service &on_session_bus() noexcept;
    service &with_objects(std::vector<gdbus::object> &&objects) noexcept;

    void start();

private:
    const std::vector<gdbus::object> &objects() const noexcept;

private:
    std::string m_name;
    std::vector<gdbus::object> m_objects;
    GBusType m_bus_type;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_SERVICE_HPP */
