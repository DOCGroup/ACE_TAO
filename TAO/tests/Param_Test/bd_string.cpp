// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_string.cpp
//
// = DESCRIPTION
//    tests bounded strings
//
// = AUTHORS
//      Carlos O'Ryan
//
// ============================================================================

#include "helper.h"
#include "bd_string.h"

ACE_RCSID(Param_Test, bd_string, "$Id$")

// ************************************************************************
//               Test_Bounded_String
// ************************************************************************

Test_Bounded_String::Test_Bounded_String (void)
  : opname_ (CORBA::string_dup ("test_bounded_string")),
    in_ (0),
    inout_ (0),
    out_ (0),
    ret_ (0)
{
}

Test_Bounded_String::~Test_Bounded_String (void)
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
Test_Bounded_String::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_String::dii_req_invoke (CORBA::Request *req,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Bounded_String::init_parameters (Param_Test_ptr objref,
				      CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator
  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // release any previously occupied values
  CORBA::string_free (this->in_);
  CORBA::string_free (this->inout_);
  CORBA::string_free (this->out_);
  CORBA::string_free (this->ret_);
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->in_ = gen->gen_string (32);
  this->inout_ = CORBA::string_dup (this->in_);
  return 0;
}

int
Test_Bounded_String::reset_parameters (void)
{
  // release any previously occupied values
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->inout_ = CORBA::string_dup (this->in_);
  return 0;
}

int
Test_Bounded_String::run_sii_test (Param_Test_ptr objref,
				   CORBA::Environment &env)
{
  CORBA::String_out str_out (this->out_);
  this->ret_ = objref->test_unbounded_string (this->in_,
                                              this->inout_,
                                              str_out,
                                              env);
  return (env.exception () ? -1:0);
}

int
Test_Bounded_String::add_args (CORBA::NVList_ptr param_list,
			       CORBA::NVList_ptr retval,
			       CORBA::Environment &env)
{
  // create the parameters
  CORBA::Any in_arg (CORBA::_tc_string,
                     &this->in_,
                     0);

  CORBA::Any inout_arg (CORBA::_tc_string,
                        &this->inout_,
                        0);

  CORBA::Any out_arg (CORBA::_tc_string,
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

  // add return value
  retval->item (0, env)->value ()->replace (CORBA::_tc_string,
                                            &this->ret_,
                                            0, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_Bounded_String::check_validity (void)
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
Test_Bounded_String::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Bounded_String::print_values (void)
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
