// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    param_test_i.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "tao/corba.h"
#include "param_test_i.h"

// ********* class Coffee_i ****************
// Constructor

Coffee_i::Coffee_i (const char *name,
                    const char *obj_name)
  : name_ (name),
    POA_Coffee (obj_name)
{
}

// Destructor

Coffee_i::~Coffee_i (void)
{
}

// get attribute
Coffee::Desc *
Coffee_i::description (CORBA::Environment & /*env*/)
{
  Coffee::Desc *desc = new Coffee::Desc;
  desc->name = CORBA::string_dup (this->name_);
  return desc;
}

// set attribute
void
Coffee_i::description (const Coffee::Desc &description,
                       CORBA::Environment & /*env*/)
{
  this->name_ = CORBA::string_dup (description.name);
}


// ********* class Param_Test_i ****************

// Constructor

Param_Test_i::Param_Test_i (const char *coffee_name,
                            const char *obj_name)
  : obj_ (new Coffee_i (coffee_name)),
    POA_Param_Test (obj_name)
{
}

// Destructor

Param_Test_i::~Param_Test_i (void)
{
}

// test shorts
CORBA::Short
Param_Test_i::test_short (CORBA::Short s1,
                          CORBA::Short &s2,
                          CORBA::Short_out s3,
                          CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  s2 = s1 * 2;
  s3 = s1 * 3;
  return s1 * 4;
}

// test unbounded strings. For return and out types, we return duplicates of
// the in string. For the inout, we append the same string to itself and send
// it back
char *
Param_Test_i::test_unbounded_string (const char *s1,
                                     char *&s2,
                                     CORBA::String_out s3,
                                     CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  char *retstr = CORBA::string_dup (s1);
  s3 = CORBA::string_dup (s1);
  char *tmp = CORBA::string_alloc (2*ACE_OS::strlen (s2));
  ACE_OS::sprintf (tmp, "%s%s", s2, s2);
  CORBA::string_free (s2);
  s2 = tmp;
  return retstr;
}

// test for fixed structures. Just copy the in parameter into all the others
Param_Test::Fixed_Struct
Param_Test_i::test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                                 Param_Test::Fixed_Struct &s2,
                                 Param_Test::Fixed_Struct_out s3,
                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  s2 = s1;
  s3 = s1;
  return s1;
}

// test for string sequence
Param_Test::StrSeq *
Param_Test_i::test_strseq (const Param_Test::StrSeq &s1,
                           Param_Test::StrSeq &s2,
                           Param_Test::StrSeq_out s3,
                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::StrSeq
    *ret = new Param_Test::StrSeq,
    *out = new Param_Test::StrSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for variable structs
Param_Test::Var_Struct *
Param_Test_i::test_var_struct (const Param_Test::Var_Struct &s1,
                             Param_Test::Var_Struct &s2,
                             Param_Test::Var_Struct_out s3,
                             CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Var_Struct
    *ret = new Param_Test::Var_Struct,
    *out = new Param_Test::Var_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for nested structs
Param_Test::Nested_Struct *
Param_Test_i::test_nested_struct (const Param_Test::Nested_Struct &s1,
                                  Param_Test::Nested_Struct &s2,
                                  Param_Test::Nested_Struct_out s3,
                                  CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::Nested_Struct
    *ret = new Param_Test::Nested_Struct,
    *out = new Param_Test::Nested_Struct;

  // now copy all elements of s1 into the others
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// test for struct sequences
Param_Test::StructSeq *
Param_Test_i::test_struct_sequence (const Param_Test::StructSeq &s1,
                                    Param_Test::StructSeq &s2,
                                    Param_Test::StructSeq_out s3,
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  // we copy the "in" sequences into all the inout, out and return sequences.

  Param_Test::StructSeq
    *ret = new Param_Test::StructSeq,

    *out = new Param_Test::StructSeq;

  // now copy all elements of s1 into the others using the assignment operator
  s2 = s1;
  *out = s1;
  *ret = s1;
  s3 = out;
  return ret;
}

// make a Coffee object
Coffee_ptr
Param_Test_i::make_coffee (CORBA::Environment & /*env*/)
{
  return Coffee::_duplicate (this->obj_.in ());
}

// test for object references
Coffee_ptr
Param_Test_i::test_objref (Coffee_ptr o1,
                           Coffee_ptr &o2,
                           Coffee_out o3,
                           CORBA::Environment &env)
{
  // o1's attribute should be same as the one we have
  if (this->obj_->_is_equivalent (o1, env))
    {
      o2 = Coffee::_duplicate (this->obj_.in ());
      o3 = Coffee::_duplicate (this->obj_.in ());
      return Coffee::_duplicate (this->obj_.in ());
    }
  else
    {
      o2 = Coffee::_nil ();
      o3 = Coffee::_nil ();
      return Coffee::_nil ();
    }
}
