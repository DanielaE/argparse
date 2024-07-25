#ifdef ARGPARSE_MODULE_USE_STD_MODULE
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#if defined(_MSC_VER)
#define _ANY_
#define _IOSTREAM_
#define _ISTREAM_
#define _LIST_
#define _SET_
#define _SSTREAM_
#define _STREAMBUF_
#define _STRING_
#define _OPTIONAL_
#define _OSTREAM_
#define _VECTOR_

#define _CISO646_
#define _CSTDDEF_
#define _CSTDINT_
#define _TYPE_TRAITS_

#elif defined(__clang__)
#define _LIBCPP_ANY
#define _LIBCPP_IOSTREAM
#define _LIBCPP_ISTREAM
#define _LIBCPP_LIST
#define _LIBCPP_SET
#define _LIBCPP_SSTREAM
#define _LIBCPP_STREAMBUF
#define _LIBCPP_STRING
#define _LIBCPP_OPTIONAL
#define _LIBCPP_OSTREAM
#define _LIBCPP_VECTOR

#define _LIBCPP_CISO646
#define _LIBCPP_CSTDDEF
#define _LIBCPP_TYPE_TRAITS

#endif
#endif
#endif
