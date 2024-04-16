/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_DETAILS_POINTER_HPP
#define GDBUS_CPP_DETAILS_POINTER_HPP

#include <gio/gio.h>
#include <utility>

namespace gdbus::details {

template<typename T>
struct pointer_cleanuper
{};

template<>
struct pointer_cleanuper<GError>
{
    static void cleanup(GError *error) noexcept
    {
        g_error_free(error);
    }
};

template<>
struct pointer_cleanuper<GDBusConnection>
{
    static void cleanup(GDBusConnection *connection) noexcept
    {
        g_object_unref(connection);
    }
};

template<>
struct pointer_cleanuper<GMainContext>
{
    static void cleanup(GMainContext *context) noexcept
    {
        g_main_context_unref(context);
    }
};

template<>
struct pointer_cleanuper<GMainLoop>
{
    static void cleanup(GMainLoop *main_loop) noexcept
    {
        g_main_loop_unref(main_loop);
    }
};

template<>
struct pointer_cleanuper<GDBusNodeInfo>
{
    static void cleanup(GDBusNodeInfo *node) noexcept
    {
        g_dbus_node_info_unref(node);
    }
};

template<>
struct pointer_cleanuper<GVariant>
{
    static void cleanup(GVariant *variant) noexcept
    {
        g_variant_unref(variant);
    }
};

template<>
struct pointer_cleanuper<GVariantIter>
{
    static void cleanup(GVariantIter *iterator) noexcept
    {
        g_variant_iter_free(iterator);
    }
};

template<>
struct pointer_cleanuper<GVariantBuilder>
{
    static void cleanup(GVariantBuilder *builder) noexcept
    {
        g_variant_builder_unref(builder);
    }
};

template<typename T>
struct pointer
{
    pointer()
        : m_pointer(nullptr)
    {}

    /* NOLINTNEXTLINE(google-explicit-constructor) */
    pointer(T *pointer) noexcept
        : m_pointer(pointer)
    {}

    pointer(pointer &&other) noexcept
        : m_pointer(std::exchange(other.m_pointer, nullptr))
    {}

    pointer &operator=(pointer &&other) noexcept
    {
        if (this != std::addressof(other)) {
            cleanup();
            m_pointer = std::exchange(other.m_pointer, nullptr);
        }

        return *this;
    }

    pointer(const pointer &) = delete;
    pointer &operator=(const pointer &) = delete;

    ~pointer() noexcept
    {
        cleanup();
    }

    /* NOLINTNEXTLINE(google-explicit-constructor) */
    operator T *() noexcept
    {
        return m_pointer;
    }

    /* NOLINTNEXTLINE(google-explicit-constructor) */
    operator const T *() const noexcept
    {
        return m_pointer;
    }

    T *operator->() noexcept
    {
        return m_pointer;
    }

    const T *operator->() const noexcept
    {
        return m_pointer;
    }

    T **operator&() noexcept
    {
        return &m_pointer;
    }

    const T **operator&() const noexcept
    {
        return &m_pointer;
    }

    explicit operator bool() const noexcept
    {
        return m_pointer != nullptr;
    }

private:
    void cleanup() noexcept
    {
        if (m_pointer) {
            pointer_cleanuper<T>::cleanup(m_pointer);
            m_pointer = nullptr;
        }
    }

    T *m_pointer;
};

} /* namespace gdbus::details */

#endif /* GDBUS_CPP_DETAILS_POINTER_HPP */
