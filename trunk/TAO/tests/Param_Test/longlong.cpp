// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    longlong.cpp
//
// = DESCRIPTION
//    tests 64 bit longs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "longlong.h"

ACE_RCSID(Param_Test, longlong, "$Id$")

Test_LongLong::Test_LongLong (void)
  : opname_ (CORBA::string_dup ("test_longlong"))
{
}

Test_LongLong::~Test_LongLong (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_LongLong::opname (void) const
{
  return this->opname_;
}

int
Test_LongLong::init_parameters (Param_Test_ptr objref,
				CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  this->in_ = gen->gen_long ();
  this->inout_ =  0;
  return 0;
}

int
Test_LongLong::reset_parameters (void)
{
  this->inout_ =  0;
  this->out_ =  0;
  this->ret_ =  0;
  return 0;
}

int
Test_LongLong::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  this->ret_ = objref->test_longlong (this->in_,
				      this->inout_,
				      this->out_,
				      env);

  return (env.exception () ? -1:0);
}

int
Test_LongLong::add_args (CORBA::NVList_ptr param_list,
                      CORBA::NVList_ptr retval,
                      CORBA::Environment &env)
{
  // we provide top level memory to the ORB to retrieve the data
  CORBA::Any in_arg (CORBA::_tc_longlong,
                     &this->in_,
                     CORBA::B_FALSE);

  CORBA::Any inout_arg (CORBA::_tc_longlong,
                        &this->inout_,
                        CORBA::B_FALSE);

  CORBA::Any out_arg (CORBA::_tc_longlong,
                      &this->out_,
                      CORBA::B_FALSE);

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

  // add return value. Let the ORB allocate storage. We simply tell the ORB
  // what type we are expecting.
  retval->item (0, env)->value ()->replace (CORBA::_tc_longlong,
                                            &this->ret_,
                                            CORBA::B_FALSE, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_LongLong::check_validity (void)
{
  if (this->inout_ == this->in_ * 2 &&
      this->out_ == this->in_ * 3 &&
      this->ret_ == this->in_ * 4)
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_LongLong::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_LongLong::print_values (void)
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


