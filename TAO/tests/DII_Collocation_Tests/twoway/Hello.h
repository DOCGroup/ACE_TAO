
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

class  Test_Simple_Test_i
  : public virtual POA_Test::Simple_Test
{
public:
  Test_Simple_Test_i () = default;
  Test_Simple_Test_i (Test_Simple_Test_i const &) = delete;
  Test_Simple_Test_i (Test_Simple_Test_i &&) = delete;
  Test_Simple_Test_i &operator= (Test_Simple_Test_i const &) = delete;
  Test_Simple_Test_i &operator= (Test_Simple_Test_i &&) = delete;
  ~Test_Simple_Test_i () override = default;

  void test_method (::CORBA::ULong &error_count) override;
};

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb,
         ACE_thread_t thr_id);
  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello &operator= (Hello &&) = delete;

  // = The skeleton methods
  void test_basic_arg (
      ::CORBA::Long basic,
      ::CORBA::Long_out x) override;

  void test_unbounded_string_arg (char const *message) override;

  void test_unbounded_string_arg_out (
      char const *message,
      ::CORBA::String_out out_message) override;

  void test_bounded_string_arg (
      char const *message) override;

  void test_fixed_array_arg (
      ::Test::Fixed_Array const message) override;

  void test_var_array_arg (
      ::Test::Var_Array const messages) override;

  void test_bounded_var_size_arg (
      ::Test::Bounded_Var_Size_Arg const &message) override;

  void test_unbounded_var_size_arg (
      ::Test::Unbounded_Var_Size_Arg const &message) override;

  void test_fixed_size_arg (::Test::TimeOfDay const &t) override;

  void test_special_basic_arg (::CORBA::Char value) override;

  void test_objref_arg (::Test::Simple_Test_ptr test) override;

  void test_object_arg (::CORBA::Object_ptr o) override;

  void test_args_1 (
      ::CORBA::Object_ptr arg1,
      ::CORBA::Char arg2,
      ::Test::Simple_Test_ptr arg3) override;

  void test_args_2 (
      char const *arg1,
      ::Test::Fixed_Array const arg2,
      ::Test::TimeOfDay const &arg3) override;

  void test_args_3 (
      char const *arg1,
      ::Test::Bounded_Var_Size_Arg const &arg2,
      ::Test::Unbounded_Var_Size_Arg const &arg3,
      ::Test::Var_Array const arg4) override;

  char *get_string () override;

  char * test_unbounded_string (
      char const *s1,
      char *&s2,
      CORBA::String_out s3);

  void shutdown () override;

  CORBA::ULong error_count () const;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var const orb_;

  ACE_thread_t thr_id_;

  CORBA::ULong error_count_ {};
};


#include /**/ "ace/post.h"
#endif /* HELLO_H */
