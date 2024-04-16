/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_DETAILS_VARIANT_DESERIALIZER_HPP
#define GDBUS_CPP_DETAILS_VARIANT_DESERIALIZER_HPP

#include "pointer.hpp"

#include <cstdint>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <gio/gio.h>

namespace gdbus::details {

template<typename T>
struct variant_deserializer
{};

template<>
struct variant_deserializer<bool>
{
    static bool deserialize(GVariant *message) noexcept
    {
        return g_variant_get_boolean(message);
    }
};

template<>
struct variant_deserializer<uint8_t>
{
    static uint8_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_byte(message);
    }
};

template<>
struct variant_deserializer<int16_t>
{
    static int16_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_int16(message);
    }
};

template<>
struct variant_deserializer<uint16_t>
{
    static uint16_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_uint16(message);
    }
};

template<>
struct variant_deserializer<int32_t>
{
    static int32_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_int32(message);
    }
};

template<>
struct variant_deserializer<uint32_t>
{
    static uint32_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_uint32(message);
    }
};

template<>
struct variant_deserializer<int64_t>
{
    static int64_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_int64(message);
    }
};

template<>
struct variant_deserializer<uint64_t>
{
    static uint64_t deserialize(GVariant *message) noexcept
    {
        return g_variant_get_uint64(message);
    }
};

template<>
struct variant_deserializer<double>
{
    static double deserialize(GVariant *message) noexcept
    {
        return g_variant_get_double(message);
    }
};

template<>
struct variant_deserializer<std::string>
{
    static std::string deserialize(GVariant *message)
    {
        return g_variant_get_string(message, nullptr);
    }
};

template<typename T, typename Allocator>
struct variant_deserializer<std::vector<T, Allocator>>
{
    using Vector = std::vector<T, Allocator>;

    static Vector deserialize(GVariant *message)
    {
        Vector vector;
        vector.reserve(g_variant_n_children(message));

        pointer<GVariantIter> iterator = g_variant_iter_new(message);
        pointer<GVariant> entry;

        while ((entry = g_variant_iter_next_value(iterator))) {
            vector.emplace_back(variant_deserializer<T>::deserialize(entry));
        }

        return vector;
    }
};

template<typename K, typename V, typename Hash, typename Pred, typename Allocator>
struct variant_deserializer<std::unordered_map<K, V, Hash, Pred, Allocator>>
{
    using Map = std::unordered_map<K, V, Hash, Pred, Allocator>;

    static Map deserialize(GVariant *message)
    {
        Map map;
        map.reserve(g_variant_n_children(message));

        pointer<GVariantIter> iterator = g_variant_iter_new(message);
        pointer<GVariant> entry;

        while ((entry = g_variant_iter_next_value(iterator))) {
            pointer<GVariant> key = g_variant_get_child_value(entry, 0);
            pointer<GVariant> value = g_variant_get_child_value(entry, 1);

            map.insert({
                variant_deserializer<K>::deserialize(key),
                variant_deserializer<V>::deserialize(value),
            });
        }

        return map;
    }
};

template<typename... T>
struct variant_deserializer<std::tuple<T...>>
{
    using Tuple = std::tuple<T...>;

    static Tuple deserialize(GVariant *message)
    {
        return implementation(message, std::index_sequence_for<T...>());
    }

private:
    template<size_t... I>
    static Tuple implementation(GVariant *message, std::index_sequence<I...>)
    {
        return {variant_deserializer<T>::deserialize(
            pointer<GVariant>(g_variant_get_child_value(message, I)))...};
    }
};

} /* namespace gdbus::details */

#endif /* GDBUS_CPP_DETAILS_VARIANT_DESERIALIZER_HPP */
