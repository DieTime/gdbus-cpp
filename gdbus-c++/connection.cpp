/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#include "connection.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "interface.hpp"
#include "object.hpp"
#include "utils.hpp"

namespace {

void process_method_call(GDBusConnection *,
                         const char *sender,
                         const char *object_path,
                         const char *interface_name,
                         const char *method_name,
                         GVariant *parameters,
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

GVariant *process_get_property(GDBusConnection *connection,
                               const gchar *sender,
                               const gchar *object_path,
                               const gchar *interface_name,
                               const gchar *property_name,
                               GError **error,
                               gpointer userdata)
{
    g_dbus_error_set_dbus_error(error, GDBUS_CPP_ERROR_NAME, "Unimplemented", nullptr);
    return nullptr;
}

gboolean process_set_property(GDBusConnection *connection,
                              const gchar *sender,
                              const gchar *object_path,
                              const gchar *interface_name,
                              const gchar *property_name,
                              GVariant *value,
                              GError **error,
                              gpointer user_data)
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

gdbus::connection connection::for_bus_with_type(GBusType type)
{
    g_autoptr(GError) error = nullptr;
    g_autoptr(GDBusConnection) connection = g_bus_get_sync(type, nullptr, &error);

    g_autoptr(GMainContext) context = g_main_context_new();
    g_autoptr(GMainLoop) mainloop = g_main_loop_new(context, false);

    if (!connection) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           append_g_error("Couldn't create " + bus_type_to_string(type)
                                              + " bus connection",
                                          error));
    }

    return {type,
            g_steal_pointer(&connection),
            g_steal_pointer(&context),
            g_steal_pointer(&mainloop)};
}

connection::connection(GBusType type,
                       GDBusConnection *connection,
                       GMainContext *context,
                       GMainLoop *mainloop) noexcept
    : m_type(type)
    , m_connection(connection)
    , m_context(context)
    , m_mainloop(mainloop)
    , m_name_registration(0)
{
    g_main_context_push_thread_default(m_context);
}

connection::~connection()
{
    if (g_main_loop_is_running(m_mainloop)) {
        g_main_loop_quit(m_mainloop);
    }

    for (const auto &registration: m_object_registrations) {
        g_dbus_connection_unregister_object(m_connection, registration.id);
        g_dbus_node_info_unref(registration.introspection);
    }

    if (m_name_registration) {
        g_bus_unown_name(m_name_registration);
    }

    g_object_unref(m_connection);
    g_main_loop_unref(m_mainloop);

    g_main_context_pop_thread_default(m_context);
    g_main_context_unref(m_context);
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

void connection::register_object(const gdbus::object &object)
{
    for (const auto &interface: object.interfaces()) {
        register_object_interface(interface);
    }
}

void connection::register_object_interface(const std::shared_ptr<gdbus::interface> &interface)
{
    g_autoptr(GError) error = nullptr;
    g_autoptr(GDBusNodeInfo) introspection = nullptr;

    introspection = g_dbus_node_info_new_for_xml(interface->introspection().c_str(), &error);

    if (!introspection) {
        throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                           append_g_error("Couldn't parse " + interface->name()
                                              + " interface introspection",
                                          error));
    }

    guint id = g_dbus_connection_register_object(m_connection,
                                                 interface->object()->path().c_str(),
                                                 introspection->interfaces[0],
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

    m_object_registrations.push_back({id, g_steal_pointer(&introspection)});
}

void connection::on_dbus_name_lost(GDBusConnection *, const gchar *name, gpointer userdata)
{
    gdbus::connection *self = static_cast<gdbus::connection *>(userdata);

    throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                       "The name " + std::string(name) + " has been lost on "
                           + bus_type_to_string(self->m_type) + " bus connection");
}

} /* namespace gdbus */
