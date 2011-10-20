
//=============================================================================
/**
 *  @file    ub_string.cpp
 *
 *  $Id$
 *
 *  tests unbounded strings
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "ub_string.h"

// ************************************************************************
//               Test_Unbounded_String
// ************************************************************************

Test_Unbounded_String::Test_Unbounded_String (void)
  : opname_ (CORBA::string_dup ("test_unbounded_string")),
    in_ (0),
    inout_ (0),
    out_ (0),
    ret_ (0)
{
}

Test_Unbounded_String::~Test_Unbounded_String (void)
{
  CORBA::string_free (this->opname_);
  CORBA::string_free (this->in_);
  CORBA::string_free (this->inout_);
  CORBA::string_free (this->out_);
  CORBA::string_free (this->ret_);
  this->opname_ = 0;
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;
}

const char *
Test_Unbounded_String::opname (void) const
{
  return this->opname_;
}

void
Test_Unbounded_String::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_;

  // The Any arg manages its memory but this class member does not.
  CORBA::string_free (this->inout_);

  req->set_return_type (CORBA::_tc_string);

  req->invoke ();

  const char *tmp;
  req->return_value () >>= tmp;
  this->ret_ = CORBA::string_dup (tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = CORBA::string_dup (tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = CORBA::string_dup (tmp);
}

int
Test_Unbounded_String::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator


  // release any previously occupied values
  CORBA::string_free (this->in_);
  CORBA::string_free (this->inout_);
  CORBA::string_free (this->out_);
  CORBA::string_free (this->ret_);
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->in_ = gen->gen_string ();
  this->inout_ = CORBA::string_dup (this->in_);
  return 0;
}

int
Test_Unbounded_String::reset_parameters (void)
{
  // release any previously occupied values
  CORBA::string_free (this->inout_);
  CORBA::string_free (this->out_);
  CORBA::string_free (this->ret_);
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->inout_ = CORBA::string_dup (this->in_);
  return 0;
}

int
Test_Unbounded_String::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::String_out str_out (this->out_);

      this->ret_ = objref->test_unbounded_string (this->in_,
                                                  this->inout_,
                                                  str_out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Unbounded_String::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Unbounded_String::check_validity (void)
{
  CORBA::ULong len = ACE_OS::strlen (this->in_);

  if (!ACE_OS::strcmp (this->in_, this->out_) &&
      !ACE_OS::strcmp (this->in_, this->ret_) &&
      ACE_OS::strlen (this->inout_) == 2*len &&
      !ACE_OS::strncmp (this->in_, this->inout_, len) &&
      !ACE_OS::strncmp (this->in_, &this->inout_[len], len))
    return 1;

  return 0; // otherwise
}

CORBA::Boolean
Test_Unbounded_String::check_validity (CORBA::Request_ptr )
{
  // No need to retrieve anything because, for all the args and
  // the return, we provided the memory and we own it.
  return this->check_validity ();
}

void
Test_Unbounded_String::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in with len (%d) = %s\n"
              "inout with len (%d) = %s\n"
              "out with len (%d) = %s\n"
              "ret with len (%d) = %s\n"
              "\n=*=*=*=*=*=*\n",
              (this->in_ ? ACE_OS::strlen (this->in_):0),
              (this->in_ ? this->in_:"<nul string>"),
              (this->inout_ ? ACE_OS::strlen (this->inout_):0),
              (this->inout_ ? this->inout_:"<nul string>"),
              (this->out_ ? ACE_OS::strlen (this->out_):0),
              (this->out_ ? this->out_:"<nul string>"),
              (this->ret_ ? ACE_OS::strlen (this->ret_):0),
              (this->ret_ ? this->ret_:"<nul string>")));
}
