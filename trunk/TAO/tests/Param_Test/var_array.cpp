// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    var_array.cpp
//
// = DESCRIPTION
//    tests fixed size arrays
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "var_array.h"

ACE_RCSID(Param_Test, var_array, "$Id$")

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
Test_Var_Array::dii_req_invoke (CORBA::Request *req,
                                CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Var_Array::init_parameters (Param_Test_ptr ,
                                 CORBA::Environment &)
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
Test_Var_Array::run_sii_test (Param_Test_ptr objref,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      Param_Test::Var_Array_out out_arr (this->out_.out ());
      this->ret_ = objref->test_var_array (this->in_,
                                           this->inout_,
                                           out_arr,
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Var_Array::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

int
Test_Var_Array::add_args (CORBA::NVList_ptr param_list,
			  CORBA::NVList_ptr retval,
			  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any in_arg (Param_Test::_tc_Var_Array,
                         this->in_,
                         0);

      CORBA::Any inout_arg (Param_Test::_tc_Var_Array,
                            this->inout_,
                            0);

      CORBA::Any out_arg (Param_Test::_tc_Var_Array,
                          this->out_.inout (), // .out () causes crash
                          0);

      // add parameters
      param_list->add_value ("v1",
                             in_arg,
                             CORBA::ARG_IN,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("v2",
                             inout_arg,
                             CORBA::ARG_INOUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("v3",
                             out_arg,
                             CORBA::ARG_OUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::NamedValue *item = retval->item (0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      item->value ()->replace (Param_Test::_tc_Var_Array,
                               this->ret_.in (),
                               0, // does not own
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Var_Array::add_args\n");

    }
  ACE_ENDTRY;
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
      if (strcmp (a1[i].in (), a2[i].in ()))
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
