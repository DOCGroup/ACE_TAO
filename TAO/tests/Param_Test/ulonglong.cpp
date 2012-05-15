
//=============================================================================
/**
 *  @file    ulonglong.cpp
 *
 *  $Id$
 *
 *  tests 64 bit longs
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "ulonglong.h"

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
Test_ULongLong::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_;

  req->set_return_type (CORBA::_tc_ulonglong);

  req->invoke ();

  req->return_value () >>= this->ret_;

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= this->inout_;

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= this->out_;
}

int
Test_ULongLong::init_parameters (Param_Test_ptr)
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
Test_ULongLong::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_ulonglong (this->in_,
                                           this->inout_,
                                           this->out_);


      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_ULongLong::run_sii_test\n");

    }
  return -1;
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
              "in = %Q, "
              "inout = %Q, "
              "out = %Q, "
              "ret = %Q\n"
              "\n=*=*=*=*=*=*\n",
              this->in_,
              this->inout_,
              this->out_,
              this->ret_));
}
