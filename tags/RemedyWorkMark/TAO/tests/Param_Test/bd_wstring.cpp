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
#include "ace/OS_NS_wchar.h"

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
Test_Bounded_WString::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= CORBA::Any::from_wstring (this->in_, 128);
  req->add_inout_arg ("s2") <<= CORBA::Any::from_wstring (this->inout_, 128);
  req->add_out_arg ("s3") <<= CORBA::Any::from_wstring (this->out_, 128);

  // The Any arg manages its memory but this class member does not.
  CORBA::wstring_free (this->inout_);

  req->set_return_type (Param_Test::_tc_short_wstring);

  req->invoke ();

  CORBA::WChar *tmp;
  req->return_value () >>= CORBA::Any::to_wstring (tmp, 128);
  this->ret_ = CORBA::wstring_dup (tmp);

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= CORBA::Any::to_wstring (tmp, 128);
  this->inout_ = CORBA::wstring_dup (tmp);

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  *arg3->value () >>= CORBA::Any::to_wstring (tmp, 128);
  this->out_ = CORBA::wstring_dup (tmp);
}

int
Test_Bounded_WString::init_parameters (Param_Test_ptr)
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
Test_Bounded_WString::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::WString_out str_out (this->out_);

      this->ret_ = objref->test_bounded_wstring (this->in_,
                                                 this->inout_,
                                                 str_out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Bounded_WString::run_sii_test\n");

    }
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
