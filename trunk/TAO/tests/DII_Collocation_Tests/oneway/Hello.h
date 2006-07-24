//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#include "ace/OS.h"

class  Test_Simple_Test_i 
  : public virtual POA_Test::Simple_Test
{
public:
  //Constructor 
  Test_Simple_Test_i (void);
  
  //Destructor 
  virtual ~Test_Simple_Test_i (void);
  
  virtual
  void test_method (
      ::CORBA::ULong & error_count
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
};

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb,
         ACE_thread_t thr_id);

  // = The skeleton methods
  virtual
  void test_basic_arg (
      ::CORBA::Long basic
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual
  void test_unbounded_string_arg (
      const char * message
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_bounded_string_arg (
      const char * message
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_fixed_array_arg (
      const ::Test::Fixed_Array message
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_var_array_arg (
      const ::Test::Var_Array messages
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_bounded_var_size_arg (
      const ::Test::Bounded_Var_Size_Arg & message
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_unbounded_var_size_arg (
      const ::Test::Unbounded_Var_Size_Arg & message
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_fixed_size_arg (
      const ::Test::TimeOfDay & t
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_special_basic_arg (
      ::CORBA::Char value
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_objref_arg (
      ::Test::Simple_Test_ptr test
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_object_arg (
      ::CORBA::Object_ptr o
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_args_1 (
      ::CORBA::Object_ptr arg1,
      ::CORBA::Char arg2,
      ::Test::Simple_Test_ptr arg3
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_args_2 (
      const char * arg1,
      const ::Test::Fixed_Array arg2,
      const ::Test::TimeOfDay & arg3
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void test_args_3 (
      const char * arg1,
      const ::Test::Bounded_Var_Size_Arg & arg2,
      const ::Test::Unbounded_Var_Size_Arg & arg3,
      const ::Test::Var_Array arg4
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual
  char * test_unbounded_string (
      const char * s1,
      char *& s2,
      CORBA::String_out s3
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong error_count () const;

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  ACE_thread_t thr_id_;

  CORBA::ULong error_count_;
};


#include /**/ "ace/post.h"
#endif /* HELLO_H */
