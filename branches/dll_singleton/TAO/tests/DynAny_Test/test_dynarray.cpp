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
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "test_dynarray.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/DynamicAny/DynamicAny.h"

Test_DynArray::Test_DynArray (CORBA::ORB_var orb)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynarray")),
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

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[4]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/seek/rewind/current_component\n"));

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

      CORBA::Any in_any1;
      in_any1 <<= ta;
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynamicAny::DynArray_var fa1 = DynamicAny::DynArray::_narrow (dp1.in ()
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->seek (1
                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->insert_long (data.m_long1
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->rewind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Long l_out1 = data.m_long2;
      fa1->seek (1
                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      l_out1 = fa1->get_long (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (l_out1 == data.m_long1)
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

      DynamicAny::DynAny_var ftc1_base =
        dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_array
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynArray_var ftc1 =
        DynamicAny::DynArray::_narrow (ftc1_base.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ftc1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynArray::_narrow() returned nil\n"),
                            -1);
        }

      ta[1] = data.m_long1;
      CORBA::Any in_any2;
      in_any2 <<= ta;
      ftc1->from_any (in_any2
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Any_var out_any1 = ftc1->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynAnyTests::test_array_forany ta_out;
      out_any1.in () >>= ta_out;

      if (ta_out[(CORBA::ULong) 1] == data.m_long1)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: set_elements/get_elements\n"));

      DynamicAny::AnySeq as_in (2);
      as_in.length (2);
      CORBA::Any in_any3;
      in_any3 <<= data.m_long1;
      as_in[0] = in_any3;
      as_in[1] = in_any3;
      ftc1->set_elements (as_in
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynamicAny::AnySeq_var as_out = ftc1->get_elements (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA_Any out_any2 = as_out[1U];
      CORBA::Long l_out2;
      out_any2 >>= l_out2;

      if (l_out2 == data.m_long1)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      fa1->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ftc1->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "test_dynarray::run_test");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}
