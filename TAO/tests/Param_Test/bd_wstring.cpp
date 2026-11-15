/**
 *  @file    bd_wstring.cpp
 *
 *  tests bounded wide strings
 *
 *  @author   Jeff Parsons
 */

#include "helper.h"
#include "bd_wstring.h"
#include "ace/OS_NS_wchar.h"

// ************************************************************************
//               Test_Bounded_WString
// ************************************************************************

Test_Bounded_WString::Test_Bounded_WString ()
  : opname_ ("test_bounded_wstring")
{
}

const char *
Test_Bounded_WString::opname () const
{
  return this->opname_;
}

void
Test_Bounded_WString::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= CORBA::Any::from_wstring (this->in_.in (), 128);
  req->add_inout_arg ("s2") <<= CORBA::Any::from_wstring (this->inout_.in (), 128);
  req->add_out_arg ("s3") <<= CORBA::Any::from_wstring (this->out_.in (), 128);

  // The Any arg has copied the value; release the member before reuse.
  this->inout_.out ();

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
  this->in_.out ();
  this->inout_.out ();
  this->out_.out ();
  this->ret_.out ();

  this->in_ = gen->gen_wstring (32);
  this->inout_ = CORBA::wstring_dup (this->in_.in ());
  return 0;
}

int
Test_Bounded_WString::reset_parameters ()
{
  // release any previously occupied values
  this->inout_.out ();
  this->out_.out ();
  this->ret_.out ();

  this->inout_ = CORBA::wstring_dup (this->in_.in ());
  return 0;
}

int
Test_Bounded_WString::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::WString_out str_out (this->out_);

      this->ret_ = objref->test_bounded_wstring (this->in_.in (),
                                                 this->inout_.inout (),
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
Test_Bounded_WString::check_validity ()
{
  CORBA::ULong len = ACE_OS::wslen (this->in_.in ());

  if (!ACE_OS::wscmp (this->in_.in (), this->out_.in ()) &&
      !ACE_OS::wscmp (this->in_.in (), this->ret_.in ()) &&
      ACE_OS::wslen (this->inout_.in ()) == 2*len &&
      !ACE_OS::wsncmp (this->in_.in (), this->inout_.in (), len) &&
      !ACE_OS::wsncmp (this->in_.in (), &this->inout_[len], len))
  {
    return 1;
  }

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
Test_Bounded_WString::print_values ()
{
}
