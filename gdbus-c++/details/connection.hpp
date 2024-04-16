/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_CONNECTION_CONNECTION_HPP
#define GDBUS_CPP_CONNECTION_CONNECTION_HPP

#include "pointer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace gdbus {

class object;
class interface;

} /* namespace gdbus */

namespace gdbus::details {

class connection
{
public:
    static connection for_bus_with_type(GBusType type);
    ~connection();

    GBusType type() const noexcept;

    void register_name(const std::string &name);
    void register_objects(const std::vector<gdbus::object> &objects);

    void start();
    void stop();

private:
    connection(GBusType type,
               pointer<GDBusConnection> connection,
               pointer<GMainContext> context,
               pointer<GMainLoop> mainloop) noexcept;

    void register_object(const gdbus::object &object);
    void register_object_interface(const std::shared_ptr<gdbus::interface> &interface);

private:
    GBusType m_type;
    pointer<GDBusConnection> m_connection;
    pointer<GMainContext> m_context;
    pointer<GMainLoop> m_mainloop;
    guint m_name_registration;
    std::vector<guint> m_object_registrations;
    std::vector<pointer<GDBusNodeInfo>> m_nodes;
};

} /* namespace gdbus::details */

#endif /* GDBUS_CPP_CONNECTION_CONNECTION_HPP */
