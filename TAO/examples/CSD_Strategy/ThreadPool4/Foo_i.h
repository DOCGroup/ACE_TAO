#ifndef FOO_I_H
#define FOO_I_H

#include <atomic>
#include <mutex>
#include "FooS.h"
#include "CallbackC.h"

class Foo_i : public virtual POA_Foo
{
public:
  Foo_i () = default;
  ~Foo_i () override = default;

  Foo_i (Foo_i const &) = delete;
  Foo_i (Foo_i &&) = delete;
  Foo_i & operator= (Foo_i const &) = delete;
  Foo_i & operator= (Foo_i &&) = delete;

  void op1 () override;

  void op2 (CORBA::Long value) override;

  CORBA::Long op3 () override;

  void op4 (CORBA::Long value) override;

  void op5 () override;

  CORBA::Boolean op6 (char const *user_name, char *&message) override;

  void test_unbounded_string_arg (char const *message) override;

  void test_bounded_string_arg (char const *message) override;

  void test_fixed_array_arg (::Fixed_Array const message) override;

  void test_var_array_arg (::Var_Array const messages) override;

  void test_bounded_var_size_arg (::Bounded_Var_Size const &message) override;

  void test_unbounded_var_size_arg (::Unbounded_Var_Size const &message) override;

  void test_fixed_size_arg (::TimeOfDay const &t) override;

  void test_fixed_size_arg_two_way (::TimeOfDay const &t) override;

  void test_special_basic_arg (::CORBA::Boolean value) override;

  void test_objref_arg (::Callback_ptr cb) override;

  void callback_object (::Callback_ptr cb) override;

  void test_callback () override;

  void done () override;

private:
  std::atomic<CORBA::Long> value_ { 0 };
  std::atomic<unsigned> count_op4_ { 0 };
  std::atomic<unsigned> count_op5_ { 0 };
  std::mutex callback_mutex_;
  Callback_var callback_ {};

  void set_callback (Callback_ptr callback);
};

#endif
