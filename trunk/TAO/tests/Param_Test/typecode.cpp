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

static const CORBA::TypeCode_ptr tc_table [] = {
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

int
Test_TypeCode::init_parameters (Param_Test_ptr objref,
                                CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);

  this->in_ = CORBA::TypeCode::_duplicate (tc_table [index]);
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  return 0;
}

int
Test_TypeCode::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  this->in_ = CORBA::TypeCode::_duplicate (tc_table [index]);
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  return 0;
}

int
Test_TypeCode::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  CORBA::TypeCode_out out (this->out_.out ());
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
  return 0;
}

CORBA::Boolean
Test_TypeCode::check_validity (void)
{
  CORBA::Environment env;
  if (this->in_->equal (this->inout_.in (), env) &&
      this->in_->equal (this->out_.in (), env) &&
      this->in_->equal (this->ret_.in (), env))
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

