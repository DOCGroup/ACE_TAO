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
Test_TypeCode::dii_req_invoke (CORBA::Request *req
                               TAO_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (CORBA::_tc_TypeCode);

  req->invoke (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_ptr tmp;
  req->return_value () >>= tmp;
  this->ret_ = CORBA::TypeCode::_duplicate (tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= tmp;
  this->inout_ = CORBA::TypeCode::_duplicate (tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= tmp;
  this->out_ = CORBA::TypeCode::_duplicate (tmp);
}

int
Test_TypeCode::init_parameters (Param_Test_ptr
                                TAO_ENV_ARG_DECL_NOT_USED)
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

  static CORBA::ULong index = 0;

  this->tc_holder_ = CORBA::TypeCode::_duplicate (tc_table [index]);
  this->in_ = this->tc_holder_;
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  // Must initialize these for DII
  this->out_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  this->ret_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  index++;
  if (index >= sizeof(tc_table)/sizeof(tc_table[0]))
    index = 0;

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
Test_TypeCode::run_sii_test (Param_Test_ptr objref
                             TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->init_parameters (objref TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::TypeCode_out out (this->out_);

      this->ret_ = objref->test_typecode (this->in_.in (),
                                          this->inout_.inout (),
                                          out
                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_TypeCode::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_TypeCode::check_validity (void)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      CORBA::Boolean one, two, three;

      one = this->in_.in ()->equal (this->inout_.in ()
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      two = this->in_.in ()->equal (this->out_.in ()
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      three = this->in_.in ()->equal (this->ret_.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (one && two && three)
        return 1;
      else
        return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_TypeCode::check_validity\n");
    }
  ACE_ENDTRY;
  return 0;
}

CORBA::Boolean
Test_TypeCode::check_validity (CORBA::Request_ptr)
{
  return this->check_validity ();
}

void
Test_TypeCode::print_values (void)
{
}
