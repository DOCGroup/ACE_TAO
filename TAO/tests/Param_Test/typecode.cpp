// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    typecode.cpp
//
// = DESCRIPTION
//    tests typecodes
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "typecode.h"

ACE_RCSID(Param_Test, typecode, "$Id$")

// ************************************************************************
//               Test_TypeCode
// ************************************************************************

Test_TypeCode::Test_TypeCode (void)
  : opname_ (CORBA::string_dup ("test_typecode"))
{
}

Test_TypeCode::~Test_TypeCode (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_TypeCode::opname (void) const
{
  return this->opname_;
}

void
Test_TypeCode::dii_req_invoke (CORBA::Request *req,
                               CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_TypeCode::init_parameters (Param_Test_ptr objref,
                                CORBA::Environment &env)
{
  static CORBA::TypeCode_ptr tc_table [] =
    {
      // primitive parameterless typecodes
      CORBA::_tc_short,
      // typecode with a simple parameter
      CORBA::_tc_string,
      // complex typecodes
      CORBA::_tc_Object,
      _tc_Param_Test,
      Param_Test::_tc_StructSeq,
      Param_Test::_tc_Nested_Struct
    };

  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index =
    (CORBA::ULong) (gen->gen_long () % sizeof(tc_table)/sizeof(CORBA::TypeCode_ptr));

  this->tc_holder_ = CORBA::TypeCode::_duplicate (tc_table [index]);
  this->in_ = this->tc_holder_;
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  // Must initialize these for DII
  this->out_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  this->ret_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  return 0;
}

int
Test_TypeCode::reset_parameters (void)
{
  this->in_ = this->tc_holder_;
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  return 0;
}

int
Test_TypeCode::run_sii_test (Param_Test_ptr objref,
                             CORBA::Environment &env)
{
  CORBA::TypeCode_out out (this->out_);
  this->ret_ = objref->test_typecode (this->in_.in (),
                                      this->inout_.inout (),
                                      out,
                                      env);
  return (env.exception () ? -1:0);
}

int
Test_TypeCode::add_args (CORBA::NVList_ptr param_list,
			 CORBA::NVList_ptr retval,
			 CORBA::Environment &env)
{
  CORBA::Any in_arg (CORBA::_tc_TypeCode,
                     &this->in_,
                     0);

  CORBA::Any inout_arg (CORBA::_tc_TypeCode,
                        &this->inout_,
                        0);

  CORBA::Any out_arg (CORBA::_tc_TypeCode,
                      &this->out_,
                      0);

  // add parameters
  param_list->add_value ("s1",
                         in_arg,
                         CORBA::ARG_IN,
                         env);

  param_list->add_value ("s2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         env);

  param_list->add_value ("s3",
                         out_arg,
                         CORBA::ARG_OUT,
                         env);

  // add return value
  retval->item (0, env)->value ()->replace (CORBA::_tc_TypeCode,
                                            &this->ret_,
                                            0, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_TypeCode::check_validity (void)
{
  CORBA::Environment env;
  if (this->in_.in ()->equal (this->inout_.in (), env) &&
      this->in_.in ()->equal (this->out_.in (), env) &&
      this->in_.in ()->equal (this->ret_.in (), env))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_TypeCode::check_validity (CORBA::Request_ptr req)
{
  return this->check_validity ();
}

void
Test_TypeCode::print_values (void)
{
}
