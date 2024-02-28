/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_CONNECTION_HPP
#define GDBUS_CPP_CONNECTION_HPP

#include "pointer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace gdbus {

class object;
class interface;

class connection
{
public:
    static connection for_bus_with_type(GBusType type);
    ~connection();

    void register_name(const std::string &name);
    void register_objects(const std::vector<gdbus::object> &objects);
    void start();

private:
    connection(GBusType type,
               gdbus::pointer<GDBusConnection> connection,
               gdbus::pointer<GMainContext> context,
               gdbus::pointer<GMainLoop> mainloop) noexcept;

    void register_object(const gdbus::object &object);
    void register_object_interface(const std::shared_ptr<gdbus::interface> &interface);

    static void on_dbus_name_lost(GDBusConnection *, const gchar *name, gpointer userdata);

private:
    GBusType m_type;
    gdbus::pointer<GDBusConnection> m_connection;
    gdbus::pointer<GMainContext> m_context;
    gdbus::pointer<GMainLoop> m_mainloop;
    guint m_name_registration;
    std::vector<guint> m_object_registrations;
    std::vector<gdbus::pointer<GDBusNodeInfo>> m_nodes;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_CONNECTION_HPP */
