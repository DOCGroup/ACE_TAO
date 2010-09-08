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
Test_Exception::dii_req_invoke (CORBA::Request_ptr req)
{
  try
    {
      this->in_++;
      req->add_in_arg ("s1") <<= this->in_;
      req->add_inout_arg ("s2") <<= this->inout_;
      req->add_out_arg ("s3") <<= this->out_;

      req->set_return_type (CORBA::_tc_ulong);

      req->exceptions ()->add (CORBA::TypeCode::_duplicate (
                                   Param_Test::_tc_Ooops
                                 ));

      req->invoke ();

      req->return_value () >>= this->ret_;

      CORBA::NamedValue_ptr o2 =
        req->arguments ()->item (1);

      *o2->value () >>= this->inout_;

      CORBA::NamedValue_ptr o3 =
        req->arguments ()->item (2);

      *o3->value () >>= this->out_;
    }
  catch (CORBA::UnknownUserException& user_ex)
    {
      Param_Test::Ooops* oops;
      Param_Test::BadBoy* bad_boy;

      if (user_ex.exception () >>= oops)
        {
          const char *reason = oops->reason.in ();
          CORBA::ULong mod_value = oops->input;

          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::dii_req_invoke - "
                          "expected user exception"
                          " (%s,%d)\n",
                          reason,
                          mod_value));
            }

          if (reason != 0 && mod_value == 1)
            {
              this->inout_ = this->in_ * 2;
              this->out_ = this->in_ * 3;
              this->ret_ = this->in_ * 4;
            }
        }
      else if (user_ex.exception () >>= bad_boy)
        {
          (*bad_boy)._tao_print_exception (
            "Test_Exception::dii_req_invoke - "
            "unexpected (but known) user exception\n");

          // Since 'BadBoy' is not in the exception list the DII request,
          // it should not be caught explicitly. See comment below.
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
  // Catch the SystemException type CORBA::NO_MEMORY thrown by the
  // server to test the system exception.
  catch (const CORBA::NO_MEMORY&)
    {
      // 'NO_MEMORY' system exception should be caught here. This
      // happens when the IN arg == 2.
      // Otherwise we don't set the other arg values so the validity
      // check will flag the error.
      if (this->in_ % 4 == 2)
        {
          this->inout_ = this->in_ * 2;
          this->out_ = this->in_ * 3;
          this->ret_ = this->in_ * 4;

          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::dii_req_invoke - "
                          "expected CORBA::NO_MEMORY system exception\n"));
            }
        }
      else if (this->in_ % 4 == 1)
        {
          // We caught NO_MEMORY system exception when we should have caught Ooops.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught NO_MEMORY system exception - "
                          "expected known user exception\n"));
            }
        }
      else if (this->in_ % 4 == 3)
        {
          // We caught NO_MEMORY system exception when we should have caught UNKNOWN.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught NO_MEMORY system exception - "
                          "expected UNKNOWN exception\n"));
            }
        }
      else
        {
          // We caught NO_MEMORY system exception when we should have caught nothing.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught unexpected uknown exception\n"));
            }
        }
    }
  catch (const CORBA::UNKNOWN&)
    {
      // 'BadBoy' should be caught here. This happens when the IN arg == 3.
      // Otherwise we don't set the other arg values so the validity
      // check will flag the error.
      if (this->in_ % 4 == 3)
        {
          this->inout_ = this->in_ * 2;
          this->out_ = this->in_ * 3;
          this->ret_ = this->in_ * 4;

          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::dii_req_invoke - "
                          "expected CORBA::UNKNOWN\n"));
            }
        }
      else if (this->in_ % 4 == 1)
        {
          // We caught UNKNOWN when we should have caught Ooops.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught unknown exception - "
                          "expected known user exception\n"));
            }
        }
      else if (this->in_ % 4 == 2)
        {
          // We caught UNKNOWN exception when we should have caught NO_MEMORY.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught unknown exception - "
                          "expected NO_MEMORY system exception\n"));
            }
        }
      else
        {
          // We caught UNKNOWN when we should have caught nothing.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught unexpected uknown exception\n"));
            }
        }
    }
}

int
Test_Exception::init_parameters (Param_Test_ptr)
{
  this->in_ = 0;
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
Test_Exception::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->in_++;
      this->ret_ = objref->test_exception (this->in_,
                                           this->inout_,
                                           this->out_);
    }
  catch (const Param_Test::Ooops& ex)
    {
      const char *reason = ex.reason.in ();

      if (reason == 0)
        {
          return -1;
        }

      CORBA::ULong mod_value = ex.input;

      // We should be catching Ooops only when this is true.
      if (mod_value != 1)
        {
          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Exception::run_sii_test - "
                      "expected user exception"
                      " (%s,%d)\n",
                      reason,
                      mod_value));
        }

      // These weren't passed back because of the exception. We
      // set them here to the 'correct' values so the validity
      // check won't return an error.
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      return 0;
    }
  catch (const CORBA::NO_MEMORY& ex)
    {
      // 'SystemException' should be caught here, 'CORBA::NO_MEMORY'
      // system exception is thrown by the servant when the
          // IN argument == 2.
      int d = this->in_ % 4;

      if (d != 2)
        {
          if (d == 1 && TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught system exception - "
                          "expected known user exception\n"));
            }
          else if (d == 3 && TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught system exception - "
                          "expected unknown exception\n"));
            }

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ex._tao_print_exception (
            "Test_Exception::run_sii_test - ""expected system exception\n");
        }

      // These weren't passed back because of the exception. We
      // set them here to the 'correct' values so the validity
      // check won't return an error.
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      return 0;
    }
  catch (const CORBA::UNKNOWN& ex)
    {
      // 'BadBoy' should be caught here, since generated code for
      // Param_Test::test_exception() knows nothing about it.
      // 'Ooops' however, should not be caught here. 'BadBoy'
      // is thrown by the servant when the IN argument == 3.
      int d = this->in_ % 4;

      if (d != 3)
        {
          if (d == 1 && TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught unknown exception - "
                          "expected known user exception\n"));
            }
          else if (d == 2 && TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Test_Exception::run_sii_test - "
                          "caught unknown exception - "
                          "expected known system exception\n"));
            }

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ex._tao_print_exception (
            "Test_Exception::run_sii_test - ""expected unknown exception\n");
        }

      // These weren't passed back because of the exception. We
      // set them here to the 'correct' values so the validity
      // check won't return an error.
      this->inout_ = this->in_ * 2;
      this->out_ = this->in_ * 3;
      this->ret_ = this->in_ * 4;
      return 0;
    }
  catch (const Param_Test::BadBoy& ex)
    {
      // We shouldn't end up here. See comment above.
      ex._tao_print_exception (
        "Test_Exception::run_sii_test - ""unexpected user exception\n");

      return -1;
    }

  // Normal reply - no exception thrown.
  return 0;
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
