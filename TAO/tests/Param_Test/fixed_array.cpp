// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    fixed_array.cpp
//
// = DESCRIPTION
//    tests fixed size arrays
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "fixed_array.h"

ACE_RCSID(Param_Test, fixed_array, "$Id$")

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
  req->invoke ();
}

int
Test_Fixed_Array::init_parameters (Param_Test_ptr objref,
                                   CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

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
Test_Fixed_Array::run_sii_test (Param_Test_ptr objref,
                                CORBA::Environment &env)
{
  this->ret_ = objref->test_fixed_array (this->in_,
                                         this->inout_,
                                         this->out_,
                                         env);
  return (env.exception () ? -1:0);
}

int
Test_Fixed_Array::add_args (CORBA::NVList_ptr param_list,
                            CORBA::NVList_ptr retval,
                            CORBA::Environment &env)
{
  // We provide the top level memory
  // the Any does not own any of these
  CORBA::Any in_arg (Param_Test::_tc_Fixed_Array,
                     this->in_,
                     0);

  CORBA::Any inout_arg (Param_Test::_tc_Fixed_Array,
                        this->inout_,
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Fixed_Array,
                      this->out_,
                      0);

  // add parameters
  param_list->add_value ("l1",
                         in_arg,
                         CORBA::ARG_IN,
                         env);

  param_list->add_value ("l2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         env);

  param_list->add_value ("l3",
                         out_arg,
                         CORBA::ARG_OUT,
                         env);

  // add return value type
  retval->item (0, env)->value ()->replace (Param_Test::_tc_Fixed_Array,
                                            this->ret_.inout (),
                                            0, // does not own
                                            env);
  return 0;
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
Test_Fixed_Array::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
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
