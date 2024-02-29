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
]

args = []

if GDBUS_CPP_BUILD_SHARED_LIBRARY
    args += '-DGDBUS_CPP_BUILD_SHARED_LIBRARY'
endif

gdbuscpp = library('gdbus-c++', src,
                    version: meson.project_version(),
                    soversion: 0,
                    dependencies: deps,
                    cpp_args: args,
                    gnu_symbol_visibility: 'hidden')

gdbuscpp_dep = declare_dependency(link_with: gdbuscpp,
                                  include_directories: ['..'],
                                  dependencies: deps)
