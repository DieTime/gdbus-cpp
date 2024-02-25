/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_CONNECTION_HPP
#define GDBUS_CPP_CONNECTION_HPP

#include <gio/gio.h>
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
               GDBusConnection *connection,
               GMainContext *context,
               GMainLoop *mainloop) noexcept;

    void register_object(const gdbus::object &object);
    void register_object_interface(const std::shared_ptr<gdbus::interface> &interface);

    static void on_dbus_name_lost(GDBusConnection *, const gchar *name, gpointer userdata);

private:
    struct object_registration
    {
        guint id;
        GDBusNodeInfo *introspection;
    };

private:
    GBusType m_type;
    GDBusConnection *m_connection;
    GMainContext *m_context;
    GMainLoop *m_mainloop;
    guint m_name_registration;
    std::vector<object_registration> m_object_registrations;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_CONNECTION_HPP */
