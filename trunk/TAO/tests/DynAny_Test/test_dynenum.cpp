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

#include "tao/corba.h"
#include "tao/DynEnum_i.h"
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
  DynAnyTests::test_enum te;

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/value_as_ulong\n"));

      CORBA_Any in_any1;
      in_any1 <<= DynAnyTests::SECOND;
      TAO_DynEnum_i de1 (in_any1);
      CORBA::ULong ul_out1 = de1.value_as_ulong (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (ul_out1 == 2)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: value_as_string\n"));

      CORBA::String s = de1.value_as_string (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (!ACE_OS::strcmp (s, "SECOND"))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;


      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      TAO_DynEnum_i de2 (DynAnyTests::_tc_test_enum);
      de2.from_any (in_any1,
                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_Any* out_any1 = de2.to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ul_out1 = 0;
      *out_any1 >>= te;
      if (te == DynAnyTests::SECOND)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete out_any1;

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

