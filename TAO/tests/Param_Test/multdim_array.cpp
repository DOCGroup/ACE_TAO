// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    fixed_array.cpp
//
// = DESCRIPTION
//    tests multidimensional fixed size arrays
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "multdim_array.h"

ACE_RCSID(Param_Test, fixed_array, "$Id$")

// ************************************************************************
//               Test_Multdim_Array
// ************************************************************************

Test_Multdim_Array::Test_Multdim_Array (void)
  : opname_ (CORBA::string_dup ("test_multdim_array")),
    ret_ (new Param_Test::Multdim_Array)
{
}

Test_Multdim_Array::~Test_Multdim_Array (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Multdim_Array::opname (void) const
{
  return this->opname_;
}

void
Test_Multdim_Array::dii_req_invoke (CORBA::Request *req,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Multdim_Array::init_parameters (Param_Test_ptr /*objref*/,
                                     CORBA::Environment &/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      for (CORBA::ULong j=0; j < Param_Test::DIM3; j++)
        {
          for (CORBA::ULong k=0; k < Param_Test::DIM1; k++)
            {
              this->in_ [i][j][k] = gen->gen_long ();
            }
        }
    }
  return 0;
}

int
Test_Multdim_Array::reset_parameters (void)
{

  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      for (CORBA::ULong j=0; j < Param_Test::DIM3; j++)
        {
          for (CORBA::ULong k=0; k < Param_Test::DIM1; k++)
            {
              this->inout_ [i][j][k] = 0;
              this->out_ [i][j][k] = 0;
            }
        }
    }
  // free the return value array
  Param_Test::Multdim_Array_free (this->ret_._retn ());
  // needed for repeated DII calls
  this->ret_ = new Param_Test::Multdim_Array;
  return 0;
}

int
Test_Multdim_Array::run_sii_test (Param_Test_ptr objref,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  this->ret_ = objref->test_multdim_array (this->in_,
                                           this->inout_,
                                           this->out_,
                                           ACE_TRY_ENV);
  return (ACE_TRY_ENV.exception () ? -1:0);
}

int
Test_Multdim_Array::add_args (CORBA::NVList_ptr param_list,
                              CORBA::NVList_ptr retval,
                              CORBA::Environment &ACE_TRY_ENV)
{
  // We provide the top level memory
  // the Any does not own any of these
  CORBA::Any in_arg (Param_Test::_tc_Multdim_Array,
                     this->in_,
                     0);

  CORBA::Any inout_arg (Param_Test::_tc_Multdim_Array,
                        this->inout_,
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Multdim_Array,
                      this->out_,
                      0);

  // add parameters
  param_list->add_value ("l1",
                         in_arg,
                         CORBA::ARG_IN,
                         ACE_TRY_ENV);

  param_list->add_value ("l2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         ACE_TRY_ENV);

  param_list->add_value ("l3",
                         out_arg,
                         CORBA::ARG_OUT,
                         ACE_TRY_ENV);

  // add return value type
  retval->item (0, ACE_TRY_ENV)->value ()->replace (Param_Test::_tc_Multdim_Array,
                                                    this->ret_.inout (),
                                                    0, // does not own
                                                    ACE_TRY_ENV);
  return 0;
}

CORBA::Boolean
Test_Multdim_Array::check_validity (void)
{
  if (this->compare (this->in_, this->inout_) &&
      this->compare (this->in_, this->out_) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Multdim_Array::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

CORBA::Boolean
Test_Multdim_Array::compare (const Param_Test::Multdim_Array_slice *a1,
                             const Param_Test::Multdim_Array_slice *a2)
{
  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      for (CORBA::ULong j=0; j < Param_Test::DIM3; j++)
        {
          for (CORBA::ULong k=0; k < Param_Test::DIM1; k++)
            {
              if (a1[i][j][k] != a2[i][j][k])
                return 0;
            }
        }
    }
  return 1; // success
}

void
Test_Multdim_Array::print_values (void)
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
Test_Multdim_Array::print (const Param_Test::Multdim_Array_slice *a)
{
  for (CORBA::ULong i = 0; i < Param_Test::DIM2; i++)
    {
      for (CORBA::ULong j=0; j < Param_Test::DIM3; j++)
        {
          for (CORBA::ULong k=0; k < Param_Test::DIM1; k++)
            {
              ACE_DEBUG ((LM_DEBUG, "\t\tElement #%d #%d #%d = %d \n",i,j,k, a[i][j][k]));
            }
        }
    }
}
