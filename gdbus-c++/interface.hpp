/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_INTERFACE_HPP
#define GDBUS_CPP_INTERFACE_HPP

#include <memory>
#include <string>

namespace gdbus {

class object;
class connection;

class interface
{
public:
    explicit interface(gdbus::object *object = nullptr);
    virtual ~interface() = default;

    virtual const std::string &name() const noexcept = 0;
    virtual const std::string &introspection() const noexcept = 0;

private:
    friend class gdbus::connection;
    const gdbus::object *object() const noexcept;

    friend class gdbus::object;
    void attach_to_object(gdbus::object *object) noexcept;

private:
    gdbus::object *m_object;
};

template<typename Interface>
std::shared_ptr<gdbus::interface> make_interface()
{
    return std::shared_ptr<gdbus::interface>(new Interface());
}

} /* namespace gdbus */

#endif /* GDBUS_CPP_INTERFACE_HPP */
