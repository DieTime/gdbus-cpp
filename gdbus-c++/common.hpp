#ifndef GDBUS_CPP_COMMON_HPP
#define GDBUS_CPP_COMMON_HPP

#ifdef GDBUS_CPP_BUILD_SHARED_LIBRARY
#define GDBUS_CPP_EXPORT_CLASS(X) __attribute__((__visibility__("default"))) X
#else
#define GDBUS_CPP_EXPORT_CLASS(X) X
#endif

#ifndef GDBUS_CPP_ERROR_NAME
#define GDBUS_CPP_ERROR_NAME "org.gdbuscpp.Error"
#endif

#endif /* GDBUS_CPP_COMMON_HPP */
