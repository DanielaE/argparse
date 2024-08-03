#include <doctest.hpp>

// this must work according to [basic.def.odr]/15
// the same definitions come from *two different* TUs:
//   - this TU by means of #include <argparse.hpp>
//   - another TU that's creating module 'argparse'
//
// the definitions must *both* be attached to the global module!

#if defined(__clang__)
// the test fails to compile on Clang 18
constexpr bool included = false;
#else
constexpr bool included = true;
#include <argparse/argparse.hpp>
#endif

import argparse;

using doctest::skip;
using doctest::test_suite;

TEST_CASE("Module is usable in mixed mode" * test_suite("module") *
          skip(not included)) {
  REQUIRE(requires {
    typename argparse::nargs_pattern;
    typename argparse::default_arguments;
    {argparse::default_arguments::none & argparse::default_arguments::all};
    typename argparse::ArgumentParser;
    typename argparse::Argument;
  });
}

#if defined(__GNUC__) && !defined(__clang__)
// the following tests confuse gcc, causing an internal compiler error
#else

TEST_CASE("Module can build and execute a test program in mixed mode" *
          test_suite("module") * skip(not included)) {
  int argc = 2;
  const char *argv[] = {"test", "--foo"};
  using namespace argparse;

  ArgumentParser program(
      "test", "1.0", default_arguments::all & default_arguments::help, false);
  program.add_argument("--foo")
      .implicit_value(true)
      .default_value(false)
      .action([&](const auto &) { ++argc; });

  const auto unknown_args = program.parse_known_args(argc, argv);
  bool ok = unknown_args.empty();

  if (program.is_used("--foo"))
    ok &= program.get<bool>("--foo");
  else
    ok &= not program.usage().empty();
  REQUIRE(ok);
}

#endif
