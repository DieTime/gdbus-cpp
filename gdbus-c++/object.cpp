#include "object.hpp"
#include "interface.hpp"
#include "service.hpp"

namespace gdbus {

object::object(std::string path, gdbus::service *service) noexcept
    : m_path(std::move(path))
    , m_service(service)
{}

const std::string &object::path() const noexcept
{
    return m_path;
}

object &object::with_interfaces(std::vector<std::shared_ptr<gdbus::interface>> &&interfaces) noexcept
{
    m_interfaces = std::move(interfaces);

    for (auto &interface: m_interfaces) {
        interface->attach_to_object(this);
    }

    return *this;
}

void object::attach_to_service(gdbus::service *service) noexcept
{
    m_service = service;
}

const gdbus::service *object::service() const noexcept
{
    return m_service;
}

const std::vector<std::shared_ptr<gdbus::interface>> &object::interfaces() const noexcept
{
    return m_interfaces;
}

} /* namespace gdbus */
