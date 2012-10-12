
//=============================================================================
/**
 *  @file    fixed_array.cpp
 *
 *  $Id$
 *
 *  tests fixed size arrays
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "fixed_array.h"

// ************************************************************************
//               Test_Fixed_Array
// ************************************************************************

Test_Fixed_Array::Test_Fixed_Array (void)
  : opname_ (CORBA::string_dup ("test_fixed_array")),
    ret_ (new Param_Test::Fixed_Array)
{
}

Test_Fixed_Array::~Test_Fixed_Array (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Fixed_Array::opname (void) const
{
  return this->opname_;
}

void
Test_Fixed_Array::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= Param_Test::Fixed_Array_forany (this->in_);
  req->add_inout_arg ("s2") <<= Param_Test::Fixed_Array_forany (this->inout_);
  req->add_out_arg ("s3") <<= Param_Test::Fixed_Array_forany (this->out_);
  req->set_return_type (Param_Test::_tc_Fixed_Array);

  req->invoke ();

  Param_Test::Fixed_Array_forany forany;
  req->return_value () >>= forany;
  Param_Test::Fixed_Array_copy (this->ret_, forany.in ());

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= forany;
  Param_Test::Fixed_Array_copy (this->inout_, forany.in ());

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  Param_Test::Fixed_Array_forany out_any (this->out_);
  *arg3->value () >>= forany;
  Param_Test::Fixed_Array_copy (this->out_, forany.in ());
}

int
Test_Fixed_Array::init_parameters (Param_Test_ptr /*objref*/
/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  //ACE_UNUSED_ARG (objref);

  for (CORBA::ULong i=0; i < Param_Test::DIM1; i++)
    {
      this->in_ [i] = gen->gen_long ();
    }

  return 0;
}

int
Test_Fixed_Array::reset_parameters (void)
{

  for (CORBA::ULong i=0; i < Param_Test::DIM1; i++)
    {
      this->inout_ [i] = 0;
      this->out_ [i] = 0;
    }
  // free the return value array
  Param_Test::Fixed_Array_free (this->ret_._retn ());
  // needed for repeated DII calls
  this->ret_ = new Param_Test::Fixed_Array;
  return 0;
}

int
Test_Fixed_Array::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_fixed_array (this->in_,
                                             this->inout_,
                                             this->out_);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Fixed_Array::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Fixed_Array::check_validity (void)
{
  if (this->compare (this->in_, this->inout_) &&
      this->compare (this->in_, this->out_) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Fixed_Array::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

CORBA::Boolean
Test_Fixed_Array::compare (const Param_Test::Fixed_Array_slice *a1,
                           const Param_Test::Fixed_Array_slice *a2)
{
  for (CORBA::ULong i=0; i < Param_Test::DIM1; i++)
    {
      if (a1[i] != a2[i])
        return 0;
    }
  return 1; // success
}

void
Test_Fixed_Array::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG, "IN array\n"));
  this->print (this->in_);
  ACE_DEBUG ((LM_DEBUG, "INOUT array\n"));
  this->print (this->inout_);
  ACE_DEBUG ((LM_DEBUG, "OUT array\n"));
  this->print (this->out_);
  ACE_DEBUG ((LM_DEBUG, "RET array\n"));
  this->print (this->ret_.in ());
}

void
Test_Fixed_Array::print (const Param_Test::Fixed_Array_slice *a)
{
  for (CORBA::ULong i = 0; i < Param_Test::DIM1; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "\t\tElement #%d = %d\n",i, a[i]));
    }
}
