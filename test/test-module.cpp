#include <doctest.hpp>
#include <ostream>

#if !defined(__clang__) && defined(__GNUC__)
#include <any> // gcc requires it
#endif

import argparse;
using doctest::test_suite;

#define Check(...) REQUIRE(requires __VA_ARGS__)

TEST_CASE("Module exports enum 'nargs_pattern'" * test_suite("module")) {
  Check({ typename argparse::nargs_pattern; });
}
TEST_CASE("Module exports enumerators of 'nargs_pattern'" *
          test_suite("module")) {
  Check({
    {argparse::nargs_pattern::optional};
    {argparse::nargs_pattern::any};
    {argparse::nargs_pattern::at_least_one};
  });
}

TEST_CASE("Module exports enum 'default_arguments'" * test_suite("module")) {
  Check({ typename argparse::default_arguments; });
}
TEST_CASE("Module exports enumerators of 'default_arguments'" *
          test_suite("module")) {
  Check({
    {argparse::default_arguments::none};
    {argparse::default_arguments::all};
    {argparse::default_arguments::help};
    {argparse::default_arguments::version};
  });
}
TEST_CASE("Module exports operator& for enum 'default_arguments'" *
          test_suite("module")) {
  Check({
    {argparse::default_arguments::none & argparse::default_arguments::all};
  });
}

TEST_CASE("Module exports class 'ArgumentParser'" * test_suite("module")) {
  Check({ typename argparse::ArgumentParser; });
}
TEST_CASE("Module exports operator<< for class 'ArgumentParser'" *
          test_suite("module")) {
  Check((argparse::ArgumentParser A, std::ostream O) { O << A; });
}

TEST_CASE("Module exports class 'Argument'" * test_suite("module")) {
  Check({ typename argparse::Argument; });
}
TEST_CASE("Module exports operator<< for class 'Argument'" *
          test_suite("module")) {
  Check((argparse::Argument A, std::ostream O) { O << A; });
}
TEST_CASE("Module can instantiate template<T> Argument::operator==(T)" *
          test_suite("module")) {
  Check((argparse::Argument A) { A == 0; });
}
TEST_CASE("Module can instantiate template<T> Argument::default_value(T)" *
          test_suite("module")) {
  Check((argparse::Argument A) { A.default_value<double>(0); });
}
TEST_CASE("Module can instantiate template<T> Argument::action(T)" *
          test_suite("module")) {
  constexpr auto Callback = [](const auto &) {};
  Check((argparse::Argument A) { A.action(Callback); });
}

TEST_CASE("Module can build and execute a test program" *
          test_suite("module")) {
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
