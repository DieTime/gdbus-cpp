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
struct freer
{};

template<>
struct freer<GError>
{
    void operator()(GError *error)
    {
        g_error_free(error);
    }
};

template<>
struct freer<GDBusConnection>
{
    void operator()(GDBusConnection *connection)
    {
        g_object_unref(connection);
    }
};

template<>
struct freer<GMainContext>
{
    void operator()(GMainContext *context)
    {
        g_main_context_unref(context);
    }
};

template<>
struct freer<GMainLoop>
{
    void operator()(GMainLoop *mainloop)
    {
        g_main_loop_unref(mainloop);
    }
};

template<>
struct freer<GDBusNodeInfo>
{
    void operator()(GDBusNodeInfo *node)
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
    pointer(T *pointer)
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
            gdbus::freer<T>()(m_pointer);
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
