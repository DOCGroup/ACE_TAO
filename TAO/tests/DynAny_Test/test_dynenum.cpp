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
#include "tao/DynamicAny/DynamicAny.h"

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
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/set_as_string/get_as_ulong\n"));

      CORBA::Object_var factory_obj =
        this->orb_->resolve_initial_references ("DynAnyFactory"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynAnyFactory_var dynany_factory =
        DynamicAny::DynAnyFactory::_narrow (factory_obj.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (dynany_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil dynamic any factory after narrow\n"),
                            -1);
        }

      DynAnyTests::test_enum te = DynAnyTests::TE_ZEROTH;
      CORBA_Any in_any1;
      in_any1 <<= te;
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynamicAny::DynEnum_var de1 =
        DynamicAny::DynEnum::_narrow (dp1.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      de1->set_as_string ("TE_FIRST"
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::ULong ul_out1 = de1->get_as_ulong (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ul_out1 == 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: set_as_ulong/get_as_string\n"));

      de1->set_as_ulong (3
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::String_var s = de1->get_as_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (s.in (), "TE_THIRD") == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }


      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      DynamicAny::DynAny_var de2_base =
        dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_enum
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynEnum_var de2 =
        DynamicAny::DynEnum::_narrow (de2_base.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (de2.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynEnum::_narrow() returned nil\n"),
                            -1);
        }

      CORBA_Any in_any2;
      in_any2 <<= DynAnyTests::TE_THIRD;
      de2->from_any (in_any2
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA_Any_var out_any1 = de2->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      out_any1.in () >>= te;

      if (te == DynAnyTests::TE_THIRD)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      de1->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      de2->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
