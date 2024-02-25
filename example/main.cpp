/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#include <gdbus-c++/gdbus-c++.hpp>
#include <iostream>

namespace org::example {

struct Greeter: public gdbus::interface
{
    Greeter()
        : m_name("org.example.Greeter")
        , m_introspection(R"gdbuscpp(
<node>
    <interface name="org.example.Greeter">
        <method name="Greeting">
            <annotation name="org.freedesktop.DBus.Method.Async" value="server"/>
            <arg name="greeting" type="s" direction="out"/>
        </method>
    </interface>
</node>
)gdbuscpp")
    {}

    const std::string &name() const noexcept override
    {
        return m_name;
    }

    const std::string &introspection() const noexcept override
    {
        return m_introspection;
    }

private:
    std::string m_name;
    std::string m_introspection;
};

}

int main()
{
    try {
        gdbus::service("org.example.Greeter")
            .on_session_bus()
            .with_objects({
                gdbus::object("/").with_interfaces({
                    gdbus::make_interface<org::example::Greeter>(),
                }),
            })
            .start();
    } catch (const gdbus::error &error) {
        std::cout << error.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
