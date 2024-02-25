#include "service.hpp"
#include "connection.hpp"

namespace gdbus {

service::service(std::string name) noexcept
    : m_name(std::move(name))
    , m_bus_type(G_BUS_TYPE_NONE)
{}

const std::string &service::name() const noexcept
{
    return m_name;
}

const std::vector<gdbus::object> &service::objects() const noexcept
{
    return m_objects;
}

service &service::on_session_bus() noexcept
{
    m_bus_type = G_BUS_TYPE_SESSION;
    return *this;
}

service &service::on_system_bus() noexcept
{
    m_bus_type = G_BUS_TYPE_SYSTEM;
    return *this;
}

service &service::with_objects(std::vector<gdbus::object> &&objects) noexcept
{
    m_objects = std::move(objects);

    for (auto &object: m_objects) {
        object.attach_to_service(this);
    }

    return *this;
}

void service::start()
{
    gdbus::connection connection = gdbus::connection::for_bus_with_type(m_bus_type);

    connection.register_name(m_name);
    connection.register_objects(m_objects);
    connection.start();
}

} /* namespace gdbus */
