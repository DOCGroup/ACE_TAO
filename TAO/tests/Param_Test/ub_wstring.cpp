/**
 *  @file    ub_wstring.cpp
 *
 *  tests unbounded wide strings
 *
 *  @author   Jeff Parsons <parsons@cs.wustl.edu>
 */

#include "helper.h"
#include "ub_wstring.h"
#include "ace/OS_NS_wchar.h"

// ************************************************************************
//               Test_Unbounded_WString
// ************************************************************************

Test_Unbounded_WString::Test_Unbounded_WString ()
  : opname_ ("test_unbounded_wstring")
{
}

const char *
Test_Unbounded_WString::opname () const
{
  return this->opname_;
}

void
Test_Unbounded_WString::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  // The Any arg has copied the value; release the member before reuse.
  this->inout_.out ();

  req->set_return_type (CORBA::_tc_wstring);

  req->invoke ();

  const CORBA::WChar *tmp;
  req->return_value () >>= tmp;
  this->ret_ = CORBA::wstring_dup (tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = CORBA::wstring_dup (tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = CORBA::wstring_dup (tmp);
}

int
Test_Unbounded_WString::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // release any previously occupied values
  this->in_.out ();
  this->inout_.out ();
  this->out_.out ();
  this->ret_.out ();

  this->in_ = gen->gen_wstring ();
  this->inout_ = CORBA::wstring_dup (this->in_.in ());
  return 0;
}

int
Test_Unbounded_WString::reset_parameters ()
{
  // release any previously occupied values
  this->inout_.out ();
  this->out_.out ();
  this->ret_.out ();

  this->inout_ = CORBA::wstring_dup (this->in_.in ());
  return 0;
}

int
Test_Unbounded_WString::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::WString_out const str_out (this->out_);

      this->ret_ = objref->test_unbounded_wstring (this->in_.in (),
                                                   this->inout_.inout (),
                                                   str_out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Unbounded_WString::run_sii_test\n");
    }
  return -1;
}

CORBA::Boolean
Test_Unbounded_WString::check_validity ()
{
  CORBA::ULong const len = ACE_OS::wslen (this->in_.in ());

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
Test_Unbounded_WString::check_validity (CORBA::Request_ptr )
{
  // No need to retrieve anything because, for all the args and
  // the return, we provided the memory and we own it.
  return this->check_validity ();
}

void
Test_Unbounded_WString::print_values ()
{
}
