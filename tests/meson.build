deps = [
    dependency('gtest'),
    gdbuscpp_dep
]

args = []

if meson.get_compiler('cpp').get_id() == 'clang'
    # Use default value of GCC -fconstexpr-ops-limit option to compile
    # variant test with complex tuple type.
    #
    # https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/C_002b_002b-Dialect-Options.html
    args += [ '-fconstexpr-steps=33554432' ]
endif

test('variant', executable('variant', 'variant.cpp', dependencies: deps, cpp_args: args))
