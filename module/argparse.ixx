/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2024 Daniela Engert
and other contributors.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

module;
#include <cerrno> // due to macro definition of 'errno'
#ifndef ARGPARSE_MODULE_USE_STD_MODULE
#include <algorithm>
#include <any>
#include <array>
#include <charconv>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#endif

export module argparse;

#if defined(__clang__)
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#elif defined(_MSC_VER)
#pragma warning(disable : 5244) // #include in module purview appears erroneous
#pragma warning(disable : 5050) // possibly incompatible module `std`: _UTF8

#ifdef ARGPARSE_AUTOLINK
#pragma comment(lib, "argparse.lib") // msvc understands autolinking
#endif
#endif

#define ARGPARSE_EXPORT export
#define ARGPARSE_BEGIN_EXPORT export {
#define ARGPARSE_END_EXPORT }

// detaching *all* names from the module allows for using the module in
// mixed scenarios where at least one translation unit #includes argparse
#ifdef ARGPARSE_ATTACH_TO_GLOBAL_MODULE
#if defined(__clang__) && defined(ARGPARSE_MODULE_USE_STD_MODULE)
// Clang 18 violates [dcl.link]/4
import std; // must be imported here, before the linkage specification
#define import namespace dummy_ns = // make 'import std;' a no-op
#endif
extern "C++" {
#endif

#if __has_include("argparse.hpp")
#include "argparse.hpp"
#elif __has_include(<argparse/argparse.hpp>)
#include <argparse/argparse.hpp>
#else
#error "could not find argparse.hpp"
#endif

#ifdef ARGPARSE_ATTACH_TO_GLOBAL_MODULE
}
#endif

// diagnostic pops are not required here
