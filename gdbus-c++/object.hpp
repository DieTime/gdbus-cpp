#ifndef GDBUS_CPP_OBJECT_HPP
#define GDBUS_CPP_OBJECT_HPP

#include <functional>
#include <memory>
#include <string>

namespace gdbus {

class service;
class interface;

class object
{
public:
    explicit object(std::string path, gdbus::service *service = nullptr) noexcept;

    const std::string &path() const noexcept;
    const std::vector<std::shared_ptr<gdbus::interface>> &interfaces() const noexcept;

    object &with_interfaces(std::vector<std::shared_ptr<gdbus::interface>> &&interfaces) noexcept;

private:
    const gdbus::service *service() const noexcept;

    friend class gdbus::service;
    void attach_to_service(gdbus::service *service) noexcept;

private:
    std::string m_path;
    gdbus::service *m_service;
    std::vector<std::shared_ptr<gdbus::interface>> m_interfaces;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_OBJECT_HPP */
