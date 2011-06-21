
//=============================================================================
/**
 *  @file    var_array.cpp
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
#include "var_array.h"

// ************************************************************************
//               Test_Var_Array
// ************************************************************************

Test_Var_Array::Test_Var_Array (void)
  : opname_ (CORBA::string_dup ("test_var_array")),
    out_ (new Param_Test::Var_Array),
    ret_ (new Param_Test::Var_Array)
{
}

Test_Var_Array::~Test_Var_Array (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Var_Array::opname (void) const
{
  return this->opname_;
}

void
Test_Var_Array::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= Param_Test::Var_Array_forany (this->in_);
  req->add_inout_arg ("s2") <<= Param_Test::Var_Array_forany (this->inout_);
  req->add_out_arg ("s3") <<= Param_Test::Var_Array_forany (this->out_.inout ());

  req->set_return_type (Param_Test::_tc_Var_Array);

  req->invoke ();

  Param_Test::Var_Array_forany forany;

  req->return_value () >>= forany;
  this->ret_ = Param_Test::Var_Array_dup (forany.in ());

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= forany;
  Param_Test::Var_Array_copy (this->inout_, forany.in ());

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= forany;
  this->out_ = Param_Test::Var_Array_dup (forany.in ());
}

int
Test_Var_Array::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator


  // fill the in_ array
  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      this->in_[i] = gen->gen_string ();
    }

  return 0;
}

int
Test_Var_Array::reset_parameters (void)
{
  // free the out, and return value arrays
  Param_Test::Var_Array_free (this->out_._retn ());
  Param_Test::Var_Array_free (this->ret_._retn ());
  this->out_ = new Param_Test::Var_Array;
  this->ret_ = new Param_Test::Var_Array;
  return 0;
}

int
Test_Var_Array::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      Param_Test::Var_Array_out out_arr (this->out_.out ());
      this->ret_ = objref->test_var_array (this->in_,
                                           this->inout_,
                                           out_arr);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Var_Array::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Var_Array::check_validity (void)
{
  if (this->compare (this->in_, this->inout_) &&
      this->compare (this->in_, this->out_.in ()) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Var_Array::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

CORBA::Boolean
Test_Var_Array::compare (const Param_Test::Var_Array_slice *a1,
                         const Param_Test::Var_Array_slice *a2)
{
  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      if (ACE_OS::strcmp (a1[i].in (), a2[i].in ()))
        return 0;
    }
  return 1; // success
}

void
Test_Var_Array::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG, "IN array\n"));
  this->print (this->in_);
  ACE_DEBUG ((LM_DEBUG, "INOUT array\n"));
  this->print (this->inout_);
  ACE_DEBUG ((LM_DEBUG, "OUT array\n"));
  this->print (this->out_.in ());
  ACE_DEBUG ((LM_DEBUG, "RET array\n"));
  this->print (this->ret_.in ());
}

void
Test_Var_Array::print (const Param_Test::Var_Array_slice *a)
{
  for (CORBA::ULong i = 0; i < Param_Test::DIM2; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "\t\tElement #%d = %s\n",i, a[i].in ()));
    }
}
