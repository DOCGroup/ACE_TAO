// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ulonglong.cpp
//
// = DESCRIPTION
//    tests 64 bit longs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "ulonglong.h"

ACE_RCSID(Param_Test, ulonglong, "$Id$")

Test_ULongLong::Test_ULongLong (void)
  : opname_ (CORBA::string_dup ("test_ulonglong"))
{
}

Test_ULongLong::~Test_ULongLong (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_ULongLong::opname (void) const
{
  return this->opname_;
}

void
Test_ULongLong::dii_req_invoke (CORBA::Request *req
                                ACE_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_;

  req->set_return_type (CORBA::_tc_ulonglong);

  req->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  req->return_value () >>= this->ret_;

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= this->inout_;

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= this->out_;
}

int
Test_ULongLong::init_parameters (Param_Test_ptr
                                 ACE_ENV_ARG_DECL_NOT_USED)
{
  Generator *gen = GENERATOR::instance (); // value generator

  this->in_ = gen->gen_long ();
  this->inout_ =  0;
  return 0;
}

int
Test_ULongLong::reset_parameters (void)
{
  this->inout_ =  0;
  this->out_ =  0;
  this->ret_ =  0;
  return 0;
}

int
Test_ULongLong::run_sii_test (Param_Test_ptr objref
                              ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->ret_ = objref->test_ulonglong (this->in_,
                                           this->inout_,
                                           this->out_
                                           ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ULongLong::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_ULongLong::check_validity (void)
{
  if (this->inout_ == this->in_ * 2 &&
      this->out_ == this->in_ * 3 &&
      this->ret_ == this->in_ * 4)
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_ULongLong::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

void
Test_ULongLong::print_values (void)
{
#if defined(ACE_LACKS_LONGLONG_T)
  char bufferin[32];
  char bufferinout[32];
  char bufferout[32];
  char bufferret[32];
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in = %s, "
              "inout = %s, "
              "out = %s, "
              "ret = %s\n"
              "\n=*=*=*=*=*=*\n",
              this->in_.as_string (bufferin),
              this->inout_.as_string (bufferinout),
              this->out_.as_string (bufferout),
              this->ret_.as_string (bufferret)));
#else
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in = %Q, "
              "inout = %Q, "
              "out = %Q, "
              "ret = %Q\n"
              "\n=*=*=*=*=*=*\n",
              this->in_,
              this->inout_,
              this->out_,
              this->ret_));
#endif
}
