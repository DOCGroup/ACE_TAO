// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ulonglong.cpp
//
// = DESCRIPTION
//    tests 64 bit longs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "ulonglong.h"

ACE_RCSID(Param_Test, ulonglong, "$Id$")

Test_ULongLong::Test_ULongLong (void)
  : opname_ (CORBA::string_dup ("test_ulonglong"))
{
}

Test_ULongLong::~Test_ULongLong (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_ULongLong::opname (void) const
{
  return this->opname_;
}

void
Test_ULongLong::dii_req_invoke (CORBA::Request *req,
                                CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_ULongLong::init_parameters (Param_Test_ptr ,
                                 CORBA::Environment &)
{
  Generator *gen = GENERATOR::instance (); // value generator

  this->in_ = gen->gen_long ();
  this->inout_ =  0;
  return 0;
}

int
Test_ULongLong::reset_parameters (void)
{
  this->inout_ =  0;
  this->out_ =  0;
  this->ret_ =  0;
  return 0;
}

int
Test_ULongLong::run_sii_test (Param_Test_ptr objref,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->ret_ = objref->test_ulonglong (this->in_,
                                           this->inout_,
                                           this->out_,
                                           ACE_TRY_ENV);

      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ULongLong::run_sii_test\n");

      return -1;
    }
  ACE_ENDTRY;
  ACE_NOTREACHED (return 0);
}

int
Test_ULongLong::add_args (CORBA::NVList_ptr param_list,
                          CORBA::NVList_ptr retval,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // we provide top level memory to the ORB to retrieve the data
      CORBA::Any in_arg (CORBA::_tc_ulonglong,
                         &this->in_,
                         0);

      CORBA::Any inout_arg (CORBA::_tc_ulonglong,
                            &this->inout_,
                            0);

      CORBA::Any out_arg (CORBA::_tc_ulonglong,
                          &this->out_,
                          0);

      // add parameters
      param_list->add_value ("s1",
                             in_arg,
                             CORBA::ARG_IN,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("s2",
                             inout_arg,
                             CORBA::ARG_INOUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("s3",
                             out_arg,
                             CORBA::ARG_OUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // add return value. Let the ORB allocate storage. We simply tell the ORB
      // what type we are expecting.
      CORBA::NamedValue *item = retval->item (0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      item->value ()->replace (CORBA::_tc_ulonglong,
                               &this->ret_,
                               0, // does not own
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ULongLong::add_args\n");

      return -1;
    }
  ACE_ENDTRY;
  ACE_NOTREACHED (return 0);
}

CORBA::Boolean
Test_ULongLong::check_validity (void)
{
  if (this->inout_ == this->in_ * 2 &&
      this->out_ == this->in_ * 3 &&
      this->ret_ == this->in_ * 4)
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_ULongLong::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

void
Test_ULongLong::print_values (void)
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
