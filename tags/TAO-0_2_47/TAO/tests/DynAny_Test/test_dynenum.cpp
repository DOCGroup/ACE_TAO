// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynenum.cpp
//
// = DESCRIPTION
//    Implementation of the simple DynEnum test
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "test_dynenum.h"
#include "da_testsC.h"
#include "data.h"

Test_DynEnum::Test_DynEnum (CORBA::ORB_var orb)
  : test_name_ (CORBA::string_dup ("test_dynenum")),
    orb_ (orb),
    error_count_ (0)
{
}

Test_DynEnum::~Test_DynEnum (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynEnum::test_name (void) const
{ 
  return this->test_name_;
}

int
Test_DynEnum::run_test (void)
{
  DynAnyTests::test_enum te = DynAnyTests::ZEROTH;

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/value_as_ulong\n"));

      CORBA_Any in_any1;
      in_any1 <<= te;
      CORBA_DynAny_ptr dp1 = this->orb_->create_dyn_any (in_any1,
                                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_DynEnum_ptr de1 = CORBA_DynEnum::_narrow (dp1,
                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      de1->value_as_ulong (2,
                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::ULong ul_out1 = de1->value_as_ulong (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (ul_out1 == 2)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: value_as_string\n"));

      de1->value_as_string ("FIRST",
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::String_var s = de1->value_as_string (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (!ACE_OS::strcmp (s.in (), "FIRST"))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;


      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynEnum_ptr de2 = 
        this->orb_->create_dyn_enum (DynAnyTests::_tc_test_enum,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_Any in_any2;
      in_any2 <<= DynAnyTests::THIRD;
      de2->from_any (in_any2,
                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_Any* out_any1 = de2->to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      *out_any1 >>= te;
      if (te == DynAnyTests::THIRD)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete out_any1;

      de1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (de1);
      de2->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (de2);
      CORBA::release (dp1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("test_dynenum::run_test");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

