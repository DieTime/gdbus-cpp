/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#include "connection.hpp"
#include "common.hpp"
#include "error.hpp"
#include "interface.hpp"
#include "object.hpp"

namespace {

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

std::string append_g_error(const std::string &message, GError *error) noexcept
{
    if (error) {
        return message + " " + error->message;
    }

    return message;
}

void on_dbus_name_lost(GDBusConnection *, const char *name, gpointer userdata)
{
    gdbus::connection *connection = static_cast<gdbus::connection *>(userdata);

    throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                       "Lost '" + std::string(name) + "' name on "
                           + bus_type_to_string(connection->type()) + " bus connection");
}

void process_method_call(GDBusConnection *,
                         const char *,
                         const char *,
                         const char *interface_name,
                         const char *,
                         GVariant *,
                         GDBusMethodInvocation *invocation,
                         gpointer userdata)
{
    gdbus::interface *interface = static_cast<gdbus::interface *>(userdata);

    if (interface->name() != interface_name) {
        g_dbus_method_invocation_return_dbus_error(invocation, GDBUS_CPP_ERROR_NAME, "Unsupported");
        return;
    }

    g_dbus_method_invocation_return_dbus_error(invocation, GDBUS_CPP_ERROR_NAME, "Unimplemented");
}

GVariant *process_get_property(GDBusConnection *,
                               const char *,
                               const char *,
                               const char *,
                               const char *,
                               GError **error,
                               gpointer)
{
    g_dbus_error_set_dbus_error(error, GDBUS_CPP_ERROR_NAME, "Unimplemented", nullptr);
    return nullptr;
}

gboolean process_set_property(GDBusConnection *,
                              const char *,
                              const char *,
                              const char *,
                              const char *,
                              GVariant *,
                              GError **error,
                              gpointer)
{
    g_dbus_error_set_dbus_error(error, GDBUS_CPP_ERROR_NAME, "Unimplemented", nullptr);
    return FALSE;
}

const GDBusInterfaceVTable vtable = {
    process_method_call,
    process_get_property,
    process_set_property,
    {},
};

} /* namespace */

namespace gdbus {

connection connection::for_bus_with_type(GBusType type)
{
    gdbus::pointer<GError> error;
    gdbus::pointer<GDBusConnection> connection = g_bus_get_sync(type, nullptr, &error);

    if (!connection) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           append_g_error("Couldn't create " + bus_type_to_string(type)
                                              + " bus connection",
                                          error));
    }

    gdbus::pointer<GMainContext> context = g_main_context_new();

    if (!context) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           "Couldn't create main context for " + bus_type_to_string(type)
                               + " bus connection");
    }

    gdbus::pointer<GMainLoop> mainloop = g_main_loop_new(context, false);

    if (!mainloop) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           "Couldn't create main loop for " + bus_type_to_string(type)
                               + " bus connection");
    }

    return {type, std::move(connection), std::move(context), std::move(mainloop)};
}

connection::connection(GBusType type,
                       gdbus::pointer<GDBusConnection> connection,
                       gdbus::pointer<GMainContext> context,
                       gdbus::pointer<GMainLoop> mainloop) noexcept
    : m_type(type)
    , m_connection(std::move(connection))
    , m_context(std::move(context))
    , m_mainloop(std::move(mainloop))
    , m_name_registration(0)
{
    g_main_context_push_thread_default(m_context);
}

connection::~connection()
{
    stop();

    for (const auto &object_registration: m_object_registrations) {
        g_dbus_connection_unregister_object(m_connection, object_registration);
    }

    g_bus_unown_name(m_name_registration);
    g_main_context_pop_thread_default(m_context);
}

GBusType connection::type() const noexcept
{
    return m_type;
}

void connection::register_name(const std::string &name)
{
    guint name_registration = g_bus_own_name_on_connection(m_connection,
                                                           name.c_str(),
                                                           G_BUS_NAME_OWNER_FLAGS_NONE,
                                                           nullptr,
                                                           on_dbus_name_lost,
                                                           this,
                                                           nullptr);
    if (!name_registration) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           "Couldn't register name on " + bus_type_to_string(m_type)
                               + " bus connection");
    }

    m_name_registration = name_registration;
}

void connection::register_objects(const std::vector<gdbus::object> &objects)
{
    for (const auto &object: objects) {
        register_object(object);
    }
}

void connection::start()
{
    g_main_loop_run(m_mainloop);
}

void connection::stop()
{
    if (g_main_loop_is_running(m_mainloop)) {
        g_main_loop_quit(m_mainloop);
    }
}

void connection::register_object(const gdbus::object &object)
{
    for (const auto &interface: object.interfaces()) {
        register_object_interface(interface);
    }
}

void connection::register_object_interface(const std::shared_ptr<gdbus::interface> &interface)
{
    const std::string &introspection = interface->introspection();

    gdbus::pointer<GError> error;
    gdbus::pointer<GDBusNodeInfo> node = g_dbus_node_info_new_for_xml(introspection.c_str(), &error);

    if (!node) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           append_g_error("Couldn't parse " + interface->name()
                                              + " interface introspection",
                                          error));
    }

    guint id = g_dbus_connection_register_object(m_connection,
                                                 interface->object()->path().c_str(),
                                                 node->interfaces[0],
                                                 &vtable,
                                                 interface.get(),
                                                 nullptr,
                                                 &error);
    if (!id) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           append_g_error("Couldn't register object with path "
                                              + interface->object()->path() + " on "
                                              + bus_type_to_string(m_type) + " bus connection",
                                          error));
    }

    m_object_registrations.push_back(id);
    m_nodes.push_back(std::move(node));
}

} /* namespace gdbus */
