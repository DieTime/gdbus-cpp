/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef GDBUS_CPP_POINTER_HPP
#define GDBUS_CPP_POINTER_HPP

#include <gio/gio.h>
#include <utility>

namespace gdbus {

template<typename T>
struct deleter
{};

template<>
struct deleter<GError>
{
    void operator()(GError *error) noexcept
    {
        g_error_free(error);
    }
};

template<>
struct deleter<GDBusConnection>
{
    void operator()(GDBusConnection *connection) noexcept
    {
        g_object_unref(connection);
    }
};

template<>
struct deleter<GMainContext>
{
    void operator()(GMainContext *context) noexcept
    {
        g_main_context_unref(context);
    }
};

template<>
struct deleter<GMainLoop>
{
    void operator()(GMainLoop *mainloop) noexcept
    {
        g_main_loop_unref(mainloop);
    }
};

template<>
struct deleter<GDBusNodeInfo>
{
    void operator()(GDBusNodeInfo *node) noexcept
    {
        g_dbus_node_info_unref(node);
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
        m_pointer = std::exchange(other.m_pointer, nullptr);
        return *this;
    }

    pointer(const pointer &) = delete;
    pointer &operator=(const pointer &) = delete;

    ~pointer()
    {
        if (m_pointer) {
            gdbus::deleter<T>()(m_pointer);
        }
    }

    /* NOLINTNEXTLINE(google-explicit-constructor) */
    constexpr operator T *() noexcept
    {
        return m_pointer;
    }

    /* NOLINTNEXTLINE(google-explicit-constructor) */
    constexpr operator const T *() const noexcept
    {
        return m_pointer;
    }

    constexpr T *operator->() noexcept
    {
        return m_pointer;
    }

    constexpr const T *operator->() const noexcept
    {
        return m_pointer;
    }

    constexpr T **operator&() noexcept
    {
        return &m_pointer;
    }

    constexpr const T **operator&() const noexcept
    {
        return &m_pointer;
    }

    explicit operator bool() const noexcept
    {
        return m_pointer != nullptr;
    }

private:
    T *m_pointer;
};

} /* namespace gdbus */

#endif //GDBUS_CPP_POINTER_HPP
