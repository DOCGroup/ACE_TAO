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

ACE_RCSID(Param_Test, except, "$Id$")

// ************************************************************************
//               Test_Exception
// ************************************************************************

Test_Exception::Test_Exception (void)
  : opname_ (CORBA::string_dup ("test_exception"))
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
Test_Exception::dii_req_invoke (CORBA::Request *req,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      req->invoke (ACE_TRY_ENV);
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
                      " (%s,%d)\n", reason, ex.input));
        }
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      
      // Why should we use clear ?????
      //TAO_TRY_ENV.clear ();

      return;
    }
  ACE_CATCH (CORBA::UNKNOWN, ex)
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ex,"Test_Exception::run_sii_test - "
                               "expected system exception\n");
        }
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      
      // TAO_TRY_ENV.clear ();

      return;
    }
  ACE_CATCH (Param_Test::BadBoy, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Test_Exception::run_sii_test - "
                           " unexpected exception\n");
      ACE_TRY_THROW (Param_Test::BadBoy);
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
Test_Exception::init_parameters (Param_Test_ptr,
                                 CORBA::Environment &)
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
Test_Exception::run_sii_test (Param_Test_ptr objref,
                              CORBA::Environment &TAO_IN_ENV)
{
  TAO_TRY
    {
      this->ret_ = objref->test_exception (this->in_,
                                           this->inout_,
                                           this->out_,
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      return 0;
    }
  TAO_CATCH (Param_Test::Ooops, ex)
    {
      if (TAO_debug_level > 0)
        {
          const char *reason = ex.reason.in ();
          if (reason == 0)
            reason = "nil";
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Exception::run_sii_test - "
                      "expected user exception"
                      " (%s,%d)\n", reason, ex.input));
        }
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      TAO_TRY_ENV.clear ();

      return 0;
    }
  TAO_CATCH (CORBA::UNKNOWN, ex)
    {
      if (TAO_debug_level > 0)
        {
          TAO_TRY_ENV.print_exception ("Test_Exception::run_sii_test - "
                                       "expected system exception\n");
        }
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      TAO_TRY_ENV.clear ();

      return 0;
    }
  TAO_CATCH (Param_Test::BadBoy, ex)
    {
      TAO_TRY_ENV.print_exception ("Test_Exception::run_sii_test - "
                                   " unexpected exception\n");
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY_RETURN (-1);
}

int
Test_Exception::add_args (CORBA::NVList_ptr param_list,
                          CORBA::NVList_ptr retval,
                          CORBA::Environment &env)
{
  // we provide top level memory to the ORB to retrieve the data
  CORBA::Any in_arg (CORBA::_tc_ulong,
                     &this->in_,
                     0);

  CORBA::Any inout_arg (CORBA::_tc_ulong,
                        &this->inout_,
                        0);

  CORBA::Any out_arg (CORBA::_tc_ulong,
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

  // add return value. Let the ORB allocate storage. We simply tell the ORB
  // what type we are expecting.
  retval->item (0, env)->value ()->replace (CORBA::_tc_ulong,
                                            &this->ret_,
                                            0, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_Exception::check_validity (void)
{
  if (this->inout_ == this->in_ * 2 &&
      this->out_ == this->in_ * 3 &&
      this->ret_ == this->in_ * 4)
    return 1;
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
