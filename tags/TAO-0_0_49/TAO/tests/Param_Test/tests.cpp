// $Id:

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    tests.cpp
//
// = DESCRIPTION
//    All the test objects defined here
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "tests.h"


// ************************************************************************
//               Test_Short
// ************************************************************************

Test_Short::Test_Short (void)
  : opname_ (CORBA::string_dup ("test_short"))
{
}

Test_Short::~Test_Short (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Short::opname (void) const
{
  return this->opname_;
}

void
Test_Short::init_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator

  this->in_ = gen->gen_short ();
  this->inout_ =  0;
}

int
Test_Short::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  this->ret_ = objref->test_short (this->in_, this->inout_, this->out_, env);
  return (env.exception () ? -1:0);
}

int
Test_Short::add_args (CORBA::NVList_ptr &param_list,
                      CORBA::NVList_ptr &retval,
                      CORBA::Environment &env)
{
  CORBA::Any in_arg (CORBA::_tc_short, &this->in_, 0);
  CORBA::Any inout_arg (CORBA::_tc_short, &this->inout_, 0);
  CORBA::Any out_arg (CORBA::_tc_short, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (CORBA::_tc_short,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

int
Test_Short::check_validity (void)
{
  if (this->inout_ == this->in_*2 &&
      this->out_ == this->in_*3 &&
      this->ret_ == this->in_*4)
    return 0; // success
  else
    return -1;
}

int
Test_Short::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = *(CORBA::Short *) req->arguments ()->item (1, env)->value ()->value ();
  this->out_ = *(CORBA::Short *) req->arguments ()->item (2, env)->value ()->value ();
  this->ret_ = *(CORBA::Short *)req->result ()->value ()->value ();
  return this->check_validity ();
}

void
Test_Short::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*"
              "in = %d, "
              "inout = %d, "
              "out = %d, "
              "ret = %d*=*=*=*=*=\n",
              this->in_,
              this->inout_,
              this->out_,
              this->ret_));
}


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
Test_Unbounded_String::init_parameters (void)
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
}

int
Test_Unbounded_String::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  CORBA::String_out str_out (this->out_);
  this->ret_ = objref->test_unbounded_string (this->in_, this->inout_, str_out,
                                              env);
  return (env.exception () ? -1:0);
}

int
Test_Unbounded_String::add_args (CORBA::NVList_ptr &param_list,
                      CORBA::NVList_ptr &retval,
                      CORBA::Environment &env)
{
  CORBA::Any in_arg (CORBA::_tc_string, &this->in_, 0);
  CORBA::Any inout_arg (CORBA::_tc_string, &this->inout_, 0);
  CORBA::Any out_arg (CORBA::_tc_string, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (CORBA::_tc_string,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

int
Test_Unbounded_String::check_validity (void)
{
  CORBA::ULong len = ACE_OS::strlen (this->in_);

  if (!ACE_OS::strcmp (this->in_, this->out_) &&
      !ACE_OS::strcmp (this->in_, this->ret_) &&
      ACE_OS::strlen (this->inout_) == 2*len &&
      !ACE_OS::strncmp (this->in_, this->inout_, len) &&
      !ACE_OS::strncmp (this->in_, &this->inout_[len], len))
    return 0;

  return -1; // otherwise
}

int
Test_Unbounded_String::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = *(char **) req->arguments ()->item (1, env)->value ()->value ();
  this->out_ = *(char **) req->arguments ()->item (2, env)->value ()->value ();
  this->ret_ = *(char **)req->result ()->value ()->value ();
  return this->check_validity ();
}

void
Test_Unbounded_String::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "in with len (%d) = %s\n"
              "inout with len (%d) = %s\n"
              "out with len (%d) = %s\n"
              "ret with len (%d) = %s\n",
              (this->in_ ? ACE_OS::strlen (this->in_):0),
              (this->in_ ? this->in_:"<nul string>"),
              (this->inout_ ? ACE_OS::strlen (this->inout_):0),
              (this->inout_ ? this->inout_:"<nul string>"),
              (this->out_ ? ACE_OS::strlen (this->out_):0),
              (this->out_ ? this->out_:"<nul string>"),
              (this->ret_ ? ACE_OS::strlen (this->ret_):0),
              (this->ret_ ? this->ret_:"<nul string>")));
}
