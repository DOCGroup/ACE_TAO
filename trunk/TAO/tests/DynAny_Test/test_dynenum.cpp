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
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "test_dynenum.h"
#include "da_testsC.h"
#include "data.h"

Test_DynEnum::Test_DynEnum (CORBA::ORB_var orb)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynenum")),
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
  DynAnyTests::test_enum te = DynAnyTests::TE_ZEROTH;

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/value_as_ulong\n"));

      CORBA_Any in_any1;
      in_any1 <<= te;
      CORBA_DynAny_ptr dp1 = this->orb_->create_dyn_any (in_any1,
                                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_DynEnum_ptr de1 = CORBA_DynEnum::_narrow (dp1,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      de1->value_as_ulong (2,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::ULong ul_out1 = de1->value_as_ulong (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (ul_out1 == 2)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: value_as_string\n"));

      de1->value_as_string ("TE_FIRST",
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::String_var s = de1->value_as_string (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (!ACE_OS::strcmp (s.in (), "TE_FIRST"))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;


      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynEnum_ptr de2 = 
        this->orb_->create_dyn_enum (DynAnyTests::_tc_test_enum,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_Any in_any2;
      in_any2 <<= DynAnyTests::TE_THIRD;
      de2->from_any (in_any2,
                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_Any* out_any1 = de2->to_any (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      *out_any1 >>= te;
      if (te == DynAnyTests::TE_THIRD)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete out_any1;

      de1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::release (de1);
      de2->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::release (de2);
      CORBA::release (dp1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "test_dynenum::run_test");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

