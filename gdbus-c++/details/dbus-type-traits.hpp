/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_DETAILS_DBUS_TYPE_TRAITS_HPP
#define GDBUS_CPP_DETAILS_DBUS_TYPE_TRAITS_HPP

#include "compile-time-string.hpp"

#include <cstdint>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <gio/gio.h>

namespace gdbus {

class variant;

} /* namespace gdbus */

namespace gdbus::details {

template<typename T>
struct dbus_type: public std::false_type
{};

template<>
struct dbus_type<bool>: public std::true_type
{
    static constexpr auto name = compile_time_string("b");
    static constexpr auto class_name = compile_time_string("bool");
};

template<>
struct dbus_type<uint8_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("y");
    static constexpr auto class_name = compile_time_string("uint8_t");
};

template<>
struct dbus_type<int16_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("n");
    static constexpr auto class_name = compile_time_string("int16_t");
};

template<>
struct dbus_type<uint16_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("q");
    static constexpr auto class_name = compile_time_string("uint16_t");
};

template<>
struct dbus_type<int32_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("i");
    static constexpr auto class_name = compile_time_string("int32_t");
};

template<>
struct dbus_type<uint32_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("u");
    static constexpr auto class_name = compile_time_string("uint32_t");
};

template<>
struct dbus_type<int64_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("x");
    static constexpr auto class_name = compile_time_string("int64_t");
};

template<>
struct dbus_type<uint64_t>: public std::true_type
{
    static constexpr auto name = compile_time_string("t");
    static constexpr auto class_name = compile_time_string("uint64_t");
};

template<>
struct dbus_type<double>: public std::true_type
{
    static constexpr auto name = compile_time_string("d");
    static constexpr auto class_name = compile_time_string("double");
};

template<>
struct dbus_type<std::string>: public std::true_type
{
    static constexpr auto name = compile_time_string("s");
    static constexpr auto class_name = compile_time_string("std:string");
};

template<typename T, typename Allocator>
struct dbus_type<std::vector<T, Allocator>>: public std::true_type
{
    static constexpr auto name = compile_time_string("a") + dbus_type<T>::name;
    static constexpr auto class_name = compile_time_string("std::vector<")
                                       + dbus_type<T>::class_name + compile_time_string(">");
};

template<typename K, typename V, typename Hash, typename Pred, typename Allocator>
struct dbus_type<std::unordered_map<K, V, Hash, Pred, Allocator>>: public std::true_type
{
    static constexpr auto name = compile_time_string("a{") + dbus_type<K>::name + dbus_type<V>::name
                                 + compile_time_string("}");
    static constexpr auto class_name = compile_time_string("std::unordered_map<")
                                       + dbus_type<K>::class_name + compile_time_string(", ")
                                       + dbus_type<V>::class_name + compile_time_string(">");
};

template<typename T, typename... R>
struct dbus_type<std::tuple<T, R...>>: public std::true_type
{
    static constexpr auto name = compile_time_string("(")
                                 + (dbus_type<T>::name + ... + dbus_type<R>::name)
                                 + compile_time_string(")");
    static constexpr auto class_name = compile_time_string("std::tuple<")
                                       + (dbus_type<T>::class_name + ...
                                          + (compile_time_string(", ") + dbus_type<R>::class_name))
                                       + compile_time_string(">");
};

template<>
struct dbus_type<gdbus::variant>: public std::true_type
{
    static constexpr auto name = compile_time_string("v");
    static constexpr auto class_name = compile_time_string("gdbus::variant");
};

template<typename T>
inline constexpr bool is_dbus_type_v = dbus_type<T>::value;

template<typename T>
inline const GVariantType *dbus_variant_type_v = reinterpret_cast<const GVariantType *>(
    dbus_type<T>::name.data());

} /* namespace gdbus::details */

#endif /* GDBUS_CPP_DETAILS_DBUS_TYPE_TRAITS_HPP */
