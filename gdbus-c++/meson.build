# SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: Apache-2.0

deps = [
    dependency('gio-unix-2.0')
]

src = [
    'connection.cpp',
    'error.cpp',
    'interface.cpp',
    'object.cpp',
    'service.cpp',
    'utils.cpp',
]

gdbus_cpp = shared_library('gdbus-c++', src, dependencies: deps)
gdbus_cpp_dep = declare_dependency(link_with: gdbus_cpp, dependencies: deps, include_directories: ['..'])
