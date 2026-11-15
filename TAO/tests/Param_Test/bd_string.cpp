/**
 *  @file    bd_string.cpp
 *
 *  tests bounded strings
 *
 *  @author   Carlos O'Ryan
 */

#include "helper.h"

#include <cstring>
#include "bd_string.h"

// ************************************************************************
//               Test_Bounded_String
// ************************************************************************

Test_Bounded_String::Test_Bounded_String ()
  : opname_ ("test_bounded_string")
{
}

const char *
Test_Bounded_String::opname () const
{
  return this->opname_;
}

void
Test_Bounded_String::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= CORBA::Any::from_string (this->in_.in (), 128);
  req->add_inout_arg ("s2") <<= CORBA::Any::from_string (this->inout_.in (), 128);
  req->add_out_arg ("s3") <<= CORBA::Any::from_string (this->out_.in (), 128);

  // The Any arg has copied the value; release the member before reuse.
  this->inout_.out ();

  req->set_return_type (Param_Test::_tc_short_string);

  req->invoke ();

  char *tmp;
  req->return_value () >>= CORBA::Any::to_string (tmp, 128);
  this->ret_ = CORBA::string_dup (tmp);

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= CORBA::Any::to_string (tmp, 128);
  this->inout_ = CORBA::string_dup (tmp);

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  *arg3->value () >>= CORBA::Any::to_string (tmp, 128);
  this->out_ = CORBA::string_dup (tmp);
}

int
Test_Bounded_String::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // release any previously occupied values
  this->in_.out ();
  this->inout_.out ();
  this->out_.out ();
  this->ret_.out ();

  this->in_ = gen->gen_string (32);
  this->inout_ = CORBA::string_dup (this->in_.in ());
  return 0;
}

int
Test_Bounded_String::reset_parameters ()
{
  // release any previously occupied values
  this->inout_.out ();
  this->out_.out ();
  this->ret_.out ();

  this->inout_ = CORBA::string_dup (this->in_.in ());
  return 0;
}

int
Test_Bounded_String::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::String_out const str_out (this->out_);

      this->ret_ = objref->test_bounded_string (this->in_.in (),
                                                this->inout_.inout (),
                                                str_out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Bounded_String::run_sii_test\n");
    }
  return -1;
}

CORBA::Boolean
Test_Bounded_String::check_validity ()
{
  CORBA::ULong const len = std::strlen (this->in_.in ());

  if (!std::strcmp (this->in_.in (), this->out_.in ()) &&
      !std::strcmp (this->in_.in (), this->ret_.in ()) &&
      std::strlen (this->inout_.in ()) == 2*len &&
      !std::strncmp (this->in_.in (), this->inout_.in (), len) &&
      !std::strncmp (this->in_.in (), &this->inout_[len], len))
  {
    return 1;
  }

  return 0; // otherwise
}

CORBA::Boolean
Test_Bounded_String::check_validity (CORBA::Request_ptr)
{
  // No need to retrieve anything because, for all the args and
  // the return, we provided the memory and we own it.
  return this->check_validity ();
}

void
Test_Bounded_String::print_values ()
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in with len (%d) = %C\n"
              "inout with len (%d) = %C\n"
              "out with len (%d) = %C\n"
              "ret with len (%d) = %C\n"
              "\n=*=*=*=*=*=*\n",
              (this->in_.in () ? std::strlen (this->in_.in ()):0),
              (this->in_.in () ? this->in_.in ():"<nul string>"),
              (this->inout_.in () ? std::strlen (this->inout_.in ()):0),
              (this->inout_.in () ? this->inout_.in ():"<nul string>"),
              (this->out_.in () ? std::strlen (this->out_.in ()):0),
              (this->out_.in () ? this->out_.in ():"<nul string>"),
              (this->ret_.in () ? std::strlen (this->ret_.in ()):0),
              (this->ret_.in () ? this->ret_.in ():"<nul string>")));
}
