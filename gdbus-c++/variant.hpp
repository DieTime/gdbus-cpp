/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_MESSAGE_HPP
#define GDBUS_CPP_MESSAGE_HPP

#include "common.hpp"
#include "error.hpp"

#include "details/dbus-type-traits.hpp"
#include "details/pointer.hpp"
#include "details/variant-deserializer.hpp"
#include "details/variant-serializer.hpp"

namespace gdbus {

class variant
{
public:
    template<typename T>
    variant(const T &value) /* NOLINT(google-explicit-constructor) */
        : variant(details::variant_serializer<T>::serialize(value))
    {
        static_assert(details::is_dbus_type_v<T>,
                      "Attempt to construct gdbus::message from value of type T using "
                      "gdbus::variant::variant<T>(const T &), but T is not a dbus type");
    }

    explicit variant(const char *string) noexcept
        : variant(std::string(string))
    {}

    /* NOLINTNEXTLINE(google-explicit-constructor) */
    variant(GVariant *message) noexcept
        : m_variant(g_variant_ref_sink(message))
    {}

    template<typename T>
    bool is() const
    {
        static_assert(details::is_dbus_type_v<T>,
                      "Attempt to check whether gdbus::message stores a value of type T using "
                      "gdbus::variant::is<T>(), but T is not a dbus type");

        return g_variant_is_of_type(const_cast<GVariant *>(static_cast<const GVariant *>(m_variant)),
                                    details::dbus_variant_type_v<T>);
    }

    template<typename T>
    T get() const
    {
        static_assert(details::is_dbus_type_v<T>,
                      "Attempt to read a value of type T using gdbus::variant::get<T>(), "
                      "but T is not a dbus type");

        if (!is<T>()) {
            static constexpr auto error_message_base
                = details::compile_time_string("Attempt to read a value of type ")
                  + details::dbus_type<T>::class_name + details::compile_time_string(" aka ")
                  + details::dbus_type<T>::name + details::compile_time_string(" ")
                  + details::compile_time_string("using gdbus::variant::get<T>(), ")
                  + details::compile_time_string("but gdbus::variant contains value of type ");

            throw gdbus::error(GDBUS_CPP_ERROR_NAME,
                               std::string(error_message_base.data())
                                   + g_variant_get_type_string(const_cast<GVariant *>(
                                       static_cast<const GVariant *>(m_variant))));
        }

        return details::variant_deserializer<T>::deserialize(
            const_cast<GVariant *>(static_cast<const GVariant *>(m_variant)));
    }

private:
    details::pointer<GVariant> m_variant;
};

} /* namespace gdbus */

#endif /* GDBUS_CPP_MESSAGE_HPP */
