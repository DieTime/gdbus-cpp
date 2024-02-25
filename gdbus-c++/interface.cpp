/**
* SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
* SPDX-License-Identifier: Apache-2.0
*/

#include "interface.hpp"

namespace gdbus {

interface::interface(gdbus::object *object)
    : m_object(object)
{}

void interface::attach_to_object(gdbus::object *object) noexcept
{
    m_object = object;
}

const gdbus::object *interface::object() const noexcept
{
    return m_object;
}

} /* namespace gdbus */
