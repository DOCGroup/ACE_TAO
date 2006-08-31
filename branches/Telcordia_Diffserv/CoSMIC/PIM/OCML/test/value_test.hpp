#ifndef _VALUE_TEST_HPP_
#define _VALUE_TEST_HPP_

#include <engine/option_value.hpp>

template <typename OPTION_TRAIT>
void write_info(OCML::Basic_Option_Value<OPTION_TRAIT>* value);

template <typename OPTION_TRAIT>
bool try_write_info(OCML::Option_Value* value);

void write_info(OCML::Option_Value* value);

void write_bool_str(bool value);

template <typename VALUE_TYPE>
struct create_some_value {
  VALUE_TYPE operator()();
};

template <>
struct create_some_value<bool> {
  bool operator()();
};

template <>
struct create_some_value<int> {
  int operator()();
};

template <>
struct create_some_value<std::string> {
  std::string operator()();
};

#include "value_test.ipp"
#include "value_test.tpp"

#endif // _VALUE_TEST_HPP_
