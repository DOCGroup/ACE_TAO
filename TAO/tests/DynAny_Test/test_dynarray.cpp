// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynarray.cpp
//
// = DESCRIPTION
//    Implementation of the DynArray test
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "test_dynarray.h"
#include "da_testsC.h"
#include "data.h"

Test_DynArray::Test_DynArray (CORBA::ORB_var orb)
  : test_name_ (CORBA::string_dup ("test_dynarray")),
    orb_ (orb),
    error_count_ (0)
{
}

Test_DynArray::~Test_DynArray (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynArray::test_name (void) const
{ 
  return this->test_name_;
}

int
Test_DynArray::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_array ta = {0};

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[4]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/seek/rewind/current_component\n"));

      CORBA::Any in_any1;
      in_any1 <<= ta;
      CORBA_DynAny_ptr dp1 = this->orb_->create_dyn_any (in_any1,
                                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_DynArray_ptr fa1 = CORBA_DynArray::_narrow (dp1,
                                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->seek (1,
                 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->insert_long (data.m_long1,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->rewind (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Long l_out1 = data.m_long2;
      fa1->seek (1,
                 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      l_out1 = fa1->get_long (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (l_out1 == data.m_long1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynArray_ptr ftc1 = 
        this->orb_->create_dyn_array (DynAnyTests::_tc_test_array,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ta[1] = data.m_long1;
      CORBA::Any in_any2;
      in_any2 <<= ta;
      ftc1->from_any (in_any2,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Any* out_any1 = ftc1->to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      DynAnyTests::test_array_forany ta_out;
      *out_any1 >>= ta_out; 
      
      if (ta_out[(CORBA::ULong) 1] == data.m_long1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete out_any1;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: set_elements/get_elements\n"));

      AnySeq as_in (2);
      as_in.length (2);
      CORBA::Any in_any3;
      in_any3 <<= data.m_long1;
      as_in[0] = in_any3;
      as_in[1] = in_any3;
      ftc1->set_elements (as_in,
                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      AnySeq* as_out = ftc1->get_elements (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_Any out_any2 = (*as_out)[1];
      CORBA::Long l_out2;
      out_any2 >>= l_out2;
      if (l_out2 == data.m_long1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete as_out;

      fa1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (fa1);
      ftc1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (ftc1);
      CORBA::release (dp1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("test_dynarray::run_test");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

