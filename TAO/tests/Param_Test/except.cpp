// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Param_Test
//
// = FILENAME
//   except.cpp
//
// = DESCRIPTION
//   tests exception
//
// = AUTHORS
//   Carlos O'Ryan
//
// ============================================================================

#include "helper.h"
#include "except.h"
#include "tao/debug.h"

#include "tao/DynamicInterface/Unknown_User_Exception.h"

ACE_RCSID(Param_Test, except, "$Id$")

// ************************************************************************
//               Test_Exception
// ************************************************************************

Test_Exception::Test_Exception (void)
  : opname_ (CORBA::string_dup ("test_exception")),
    iterations_ (0)
{
}

Test_Exception::~Test_Exception (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Exception::opname (void) const
{
  return this->opname_;
}

void
Test_Exception::dii_req_invoke (CORBA::Request_ptr req
                                ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->in_++;
      req->add_in_arg ("s1") <<= this->in_;
      req->add_inout_arg ("s2") <<= this->inout_;
      req->add_out_arg ("s3") <<= this->out_;

      req->set_return_type (CORBA::_tc_ulong);

      req->exceptions ()->add (CORBA::TypeCode::_duplicate (
                                   Param_Test::_tc_Ooops
                                 ));

      req->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
  ACE_CATCH (CORBA::UnknownUserException, user_ex)
    {
      Param_Test::Ooops* oops;
      Param_Test::BadBoy* bad_boy;

      if (user_ex.exception () >>= oops)
        {
          if (TAO_debug_level > 0)
            {
              const char *reason = oops->reason.in ();

              if (reason == 0)
                {
                  reason = "nil";
                }

              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::dii_req_invoke - "
                          "expected user exception"
                          " (%s,%d)\n", 
                          reason, 
                          oops->input));
            }

          this->inout_ = this->in_ * 2;
          this->out_ = this->in_ * 3;
          this->ret_ = this->in_ * 4;
        }
      else if (user_ex.exception () >>= bad_boy)
        {
          ACE_PRINT_EXCEPTION ((*bad_boy),
                               "Test_Exception::dii_req_invoke - "
                               "unexpected (but known) user exception\n");
          this->inout_ = this->in_ * 5;
          this->out_ = this->in_ * 5;
          this->ret_ = this->in_ * 5;
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR Test_Exception::dii_req_invoke - "
                      "unexpected (and unknown) user exception\n"));
        }

      return;
    }
  ACE_CATCH (CORBA::UNKNOWN, ex)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Exception::dii_req_invoke - "
                      "expected CORBA::UNKNOWN\n"));
        }

      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
Test_Exception::init_parameters (Param_Test_ptr
                                 ACE_ENV_ARG_DECL_NOT_USED)
{
  Generator *gen = GENERATOR::instance (); // value generator

  this->in_ = gen->gen_short ();
  this->inout_ =  0;
  return 0;
}

int
Test_Exception::reset_parameters (void)
{
  this->inout_ =  0;
  this->out_ =  0;
  this->ret_ =  0;
  return 0;
}

int
Test_Exception::run_sii_test (Param_Test_ptr objref
                              ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->in_++;
      this->ret_ = objref->test_exception (this->in_,
                                           this->inout_,
                                           this->out_
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (Param_Test::Ooops, ex)
    {
      if (TAO_debug_level > 0)
        {
          const char *reason = ex.reason.in ();
          if (reason == 0)
            reason = "nil";
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Exception::run_sii_test - "
                      "expected user exception"
                      " (%s,%d)\n", 
                      reason, 
                      ex.input));
        }

      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      return -1;
    }
  ACE_CATCH (CORBA::UNKNOWN, ex)
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ex,
                               "Test_Exception::run_sii_test - "
                               "expected system exception\n");
        }

      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      return 0;
    }
  ACE_CATCH (Param_Test::BadBoy, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Test_Exception::run_sii_test - "
                           "unexpected user exception\n");
    }
  ACE_ENDTRY;

  return -1;
}

CORBA::Boolean
Test_Exception::check_validity (void)
{
  if (this->inout_ == this->in_ * 2 &&
      this->out_ == this->in_ * 3 &&
      this->ret_ == this->in_ * 4)
    {
      return 1;
    }

  return 0;
}

CORBA::Boolean
Test_Exception::check_validity (CORBA::Request_ptr)
{
  return this->check_validity ();
}

void
Test_Exception::print_values (void)
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
