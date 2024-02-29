# SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: Apache-2.0

project('gdbus-c++', 'cpp',
         version: '0.1.0',
         license: 'Apache-2.0',
         default_options: ['cpp_std=c++17', 'warning_level=3'])

subdir('options')
subdir('gdbus-c++')

if GDBUS_CPP_BUILD_EXAMPLE
    subdir('example')
endif
