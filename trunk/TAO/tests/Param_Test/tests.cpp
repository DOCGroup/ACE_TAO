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

  CORBA::Any ret_arg (CORBA::_tc_short, &this->ret_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0)->value ()->replace (CORBA::_tc_short,
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
  this->inout_ = *(CORBA::Short *) req->arguments ()->item (1)->value ()->value ();
  this->out_ = *(CORBA::Short *) req->arguments ()->item (2)->value ()->value ();
  this->ret_ = *(CORBA::Short *)req->result ()->value ()->value ();
  return this->check_validity ();
}

void
Test_Short::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "in = %d, "
              "inout = %d, "
              "out = %d, "
              "ret = %d\n",
              this->in_,
              this->inout_,
              this->out_,
              this->ret_));
}


// ************************************************************************
//               Test_UnboundedString
// ************************************************************************
