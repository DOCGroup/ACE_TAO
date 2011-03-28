
//=============================================================================
/**
 *  @file    multidim_array.cpp
 *
 *  $Id$
 *
 *  tests multidimensional fixed size arrays
 *
 *
 *  @author   Bala
 */
//=============================================================================


#include "helper.h"
#include "multdim_array.h"


// ************************************************************************
//               Test_Multdim_Array
// ************************************************************************

Test_Multdim_Array::Test_Multdim_Array (void)
  : opname_ (CORBA::string_dup ("test_multdim_array")),
    in_ (new Param_Test::Multdim_Array),
    inout_ (new Param_Test::Multdim_Array),
    out_ (new Param_Test::Multdim_Array),
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
Test_Multdim_Array::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= Param_Test::Multdim_Array_forany (this->in_.inout ());
  req->add_inout_arg ("s2") <<= Param_Test::Multdim_Array_forany (this->inout_.inout ());
  req->add_out_arg ("s3") <<= Param_Test::Multdim_Array_forany (this->out_.inout ());

  req->set_return_type (Param_Test::_tc_Multdim_Array);

  req->invoke ();


  Param_Test::Multdim_Array_forany forany;
  req->return_value () >>= forany;
  Param_Test::Multdim_Array_copy (this->ret_, forany.in ());

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= forany;
  Param_Test::Multdim_Array_copy (this->inout_, forany.in ());

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= forany;
  Param_Test::Multdim_Array_copy (this->out_, forany.in ());
}

int
Test_Multdim_Array::init_parameters (Param_Test_ptr /*objref*/
/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      for (CORBA::ULong j=0; j < Param_Test::DIM3; j++)
        {
          for (CORBA::ULong k=0; k < Param_Test::DIM1; k++)
            {
              this->in_ [i][j][k] = gen->gen_long ();
              this->inout_ [i][j][k] = 0;
            }
        }
    }
  return 0;
}

int
Test_Multdim_Array::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // free the in value array
  Param_Test::Multdim_Array_free (this->in_._retn ());
  // needed for repeated DII calls
  this->in_ = new Param_Test::Multdim_Array;
  // free the inout value array
  Param_Test::Multdim_Array_free (this->inout_._retn ());
  // needed for repeated DII calls
  this->inout_ = new Param_Test::Multdim_Array;

  for (CORBA::ULong i=0; i < Param_Test::DIM2; i++)
    {
      for (CORBA::ULong j=0; j < Param_Test::DIM3; j++)
        {
          for (CORBA::ULong k=0; k < Param_Test::DIM1; k++)
            {
              this->inout_ [i][j][k] = 0;
              this->in_ [i][j][k] = gen->gen_long ();
            }
        }
    }
  // free the out value array
  Param_Test::Multdim_Array_free (this->out_._retn ());
  // needed for repeated DII calls
  this->out_ = new Param_Test::Multdim_Array;
  // free the return value array
  Param_Test::Multdim_Array_free (this->ret_._retn ());
  // needed for repeated DII calls
  this->ret_ = new Param_Test::Multdim_Array;
  return 0;
}

int
Test_Multdim_Array::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_multdim_array (this->in_.in (),
                                               this->inout_.inout (),
                                               this->out_.inout ());
      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Multdim_Array::run_sii_test\n");
    }
  return -1;
}

CORBA::Boolean
Test_Multdim_Array::check_validity (void)
{
  if (this->compare (this->in_.in (),
                     this->inout_.in ()) &&
      this->compare (this->in_.in (),
                     this->out_.in ()) &&
      this->compare (this->in_.in (),
                     this->ret_.in ()))
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
  this->print (this->in_.in ());
  ACE_DEBUG ((LM_DEBUG, "INOUT array\n"));
  this->print (this->inout_.in ());
  ACE_DEBUG ((LM_DEBUG, "OUT array\n"));
  this->print (this->out_.in ());
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
              ACE_DEBUG ((LM_DEBUG, "\t\tElement #%d #%d #%d = %d\n",i,j,k, a[i][j][k]));
            }
        }
    }
}
