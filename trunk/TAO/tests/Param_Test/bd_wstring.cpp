// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_wstring.cpp
//
// = DESCRIPTION
//    tests bounded wide strings
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#include "helper.h"
#include "bd_wstring.h"

// ************************************************************************
//               Test_Bounded_WString
// ************************************************************************

Test_Bounded_WString::Test_Bounded_WString (void)
  : opname_ (CORBA::string_dup ("test_bounded_wstring")),
    in_ (0),
    inout_ (0),
    out_ (0),
    ret_ (0)
{
}

Test_Bounded_WString::~Test_Bounded_WString (void)
{
  CORBA::string_free (this->opname_);
  CORBA::wstring_free (this->in_);
  CORBA::wstring_free (this->inout_);
  CORBA::wstring_free (this->out_);
  CORBA::wstring_free (this->ret_);
  this->opname_ = 0;
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;
}

const char *
Test_Bounded_WString::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_WString::dii_req_invoke (CORBA::Request *req,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Bounded_WString::init_parameters (Param_Test_ptr,
				                               CORBA::Environment &)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // release any previously occupied values
  CORBA::wstring_free (this->in_);
  CORBA::wstring_free (this->inout_);
  CORBA::wstring_free (this->out_);
  CORBA::wstring_free (this->ret_);
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->in_ = gen->gen_wstring (32);
  this->inout_ = CORBA::wstring_dup (this->in_);
  return 0;
}

int
Test_Bounded_WString::reset_parameters (void)
{
  // release any previously occupied values
  CORBA::wstring_free (this->inout_);
  CORBA::wstring_free (this->out_);
  CORBA::wstring_free (this->ret_);
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->inout_ = CORBA::wstring_dup (this->in_);
  return 0;
}

int
Test_Bounded_WString::run_sii_test (Param_Test_ptr objref,
				                            CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::WString_out str_out (this->out_);

      this->ret_ = objref->test_bounded_wstring (this->in_,
                                                 this->inout_,
                                                 str_out,
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Bounded_WString::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

int
Test_Bounded_WString::add_args (CORBA::NVList_ptr param_list,
			                          CORBA::NVList_ptr retval,
			                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // create the parameters
      CORBA::Any in_arg (CORBA::_tc_wstring,
                         &this->in_,
                         0);

      CORBA::Any inout_arg (CORBA::_tc_wstring,
                            &this->inout_,
                            0);

      CORBA::Any out_arg (CORBA::_tc_wstring,
                          &this->out_,
                          0);

      // add parameters
      param_list->add_value ("ws1",
                             in_arg,
                             CORBA::ARG_IN,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("ws2",
                             inout_arg,
                             CORBA::ARG_INOUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("ws3",
                             out_arg,
                             CORBA::ARG_OUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // add return value
      CORBA::NamedValue *item = retval->item (0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      item->value ()->replace (CORBA::_tc_wstring,
                               &this->ret_,
                               0, // does not own
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Bounded_WString::add_args\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_Bounded_WString::check_validity (void)
{
  CORBA::ULong len = ACE_OS::wslen (this->in_);

  if (!ACE_OS::wscmp (this->in_, this->out_) &&
      !ACE_OS::wscmp (this->in_, this->ret_) &&
      ACE_OS::wslen (this->inout_) == 2*len &&
      !ACE_OS::wsncmp (this->in_, this->inout_, len) &&
      !ACE_OS::wsncmp (this->in_, &this->inout_[len], len))
    return 1;

  return 0; // otherwise
}

CORBA::Boolean
Test_Bounded_WString::check_validity (CORBA::Request_ptr)
{
  // No need to retrieve anything because, for all the args and
  // the return, we provided the memory and we own it.
  return this->check_validity ();
}

void
Test_Bounded_WString::print_values (void)
{
}
