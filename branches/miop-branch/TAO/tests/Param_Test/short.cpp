// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    short.cpp
//
// = DESCRIPTION
//    tests shorts
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "short.h"

ACE_RCSID(Param_Test, short, "$Id$")

// ************************************************************************
//               Test_Short
// ************************************************************************

Test_Short::Test_Short (void)
  : opname_ (CORBA::string_dup ("test_short"))
{
}

Test_Short::~Test_Short (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Short::opname (void) const
{
  return this->opname_;
}

void
Test_Short::dii_req_invoke (CORBA::Request *req
                            TAO_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_;

  req->set_return_type (CORBA::_tc_short);

  req->invoke (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  req->return_value () >>= this->ret_;

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= this->inout_;

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= this->out_;
}

int
Test_Short::init_parameters (Param_Test_ptr /*objref*/
                             TAO_ENV_ARG_DECL_NOT_USED)
{
  Generator *gen = GENERATOR::instance (); // value generator

  this->in_ = gen->gen_short ();
  this->inout_ =  0;
  return 0;
}

int
Test_Short::reset_parameters (void)
{
  this->inout_ =  0;
  this->out_ =  0;
  this->ret_ =  0;
  return 0;
}

int
Test_Short::run_sii_test (Param_Test_ptr objref
                          TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->ret_ = objref->test_short (this->in_,
                                       this->inout_,
                                       this->out_
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Short::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_Short::check_validity (void)
{
  if (this->inout_ == this->in_ * 2 &&
      this->out_ == this->in_ * 3 &&
      this->ret_ == this->in_ * 4)
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_Short::check_validity (CORBA::Request_ptr /*req*/)
{
  return this->check_validity ();
}

void
Test_Short::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in = %d, "
              "inout = %d, "
              "out = %d, "
              "ret = %d\n"
              "\n=*=*=*=*=*=*\n",
              this->in_,
              this->inout_,
              this->out_,
              this->ret_));
}
