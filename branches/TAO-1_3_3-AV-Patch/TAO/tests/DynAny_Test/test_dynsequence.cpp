// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynsequence.cpp
//
// = DESCRIPTION
//    Implementation of the simple DynSequence test
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "test_dynsequence.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/DynamicAny/DynamicAny.h"

Test_DynSequence::Test_DynSequence (CORBA::ORB_var orb)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynsequence")),
    error_count_ (0)
{
}

Test_DynSequence::~Test_DynSequence (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynSequence::test_name (void) const
{
  return this->test_name_;
}

int
Test_DynSequence::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_seq ts (2);
  ts.length (2);

    ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[11]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/seek/"
                 "rewind/current_component\n"));

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

      ts[0] = data.m_string2;
      ts[1] = data.m_string2;
      CORBA::Any in_any1;
      in_any1 <<= ts;
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynamicAny::DynSequence_var fa1 =
        DynamicAny::DynSequence::_narrow (dp1.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->seek (1
                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->insert_string (data.m_string1
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->rewind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      fa1->seek (1
                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::String_var out_str1 = fa1->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!ACE_OS::strcmp (out_str1.in (), data.m_string1))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((
          LM_DEBUG,
          "testing: set_elements_as_dyn_any/get_elements_as_dyn_any/equal\n"
        ));

      static const char *values[] =
      {
        "zero",
        "one",
        "two"
      };

      CORBA::ULong length = 3;
      DynamicAny::DynAnySeq *elem_ptr = 0;
      ACE_NEW_RETURN (elem_ptr,
                      DynamicAny::DynAnySeq (length),
                      -1);
      elem_ptr->length (length);
      DynamicAny::DynAnySeq_var elements (elem_ptr);
      CORBA::Any elem_any;
      CORBA::ULong i;

      for (i = 0; i < length; ++i)
        {
          elem_any <<= CORBA::Any::from_string (values[i],
                                                8);
          elements[i] = dynany_factory->create_dyn_any (elem_any
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      fa1->set_elements_as_dyn_any (elements.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      DynamicAny::DynAnySeq_var out_elems =
        fa1->get_elements_as_dyn_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (out_elems->length () != 3)
        {
          ++this->error_count_;
          return -1;
        }

      CORBA::String_var out_str;

      for (i = 0; i < length; ++i)
        {
          out_str = out_elems[i]->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Boolean equal =
            out_elems[i]->equal (elements[i].in ()
                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (!equal)
            {
              ++this->error_count_;
            }

          // To show that calling destroy() on a component does
          // nothing, as required by the spec.
          out_elems[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (this->error_count_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      DynamicAny::DynAny_var ftc1_base =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_test_seq
                            ACE_ENV_ARG_PARAMETER
                          );
      ACE_TRY_CHECK;

      DynamicAny::DynSequence_var ftc1 =
        DynamicAny::DynSequence::_narrow (ftc1_base.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ftc1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynSequence::_narrow() returned nil\n"),
                            -1);
        }

      ts[0] = CORBA::string_dup (data.m_string1);
      CORBA::Any in_any2;
      in_any2 <<= ts;
      ftc1->from_any (in_any2
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Any_var out_any1 = ftc1->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynAnyTests::test_seq *ts_out;
      out_any1.in () >>= ts_out;

      if (!ACE_OS::strcmp ((*ts_out)[0U], data.m_string1))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: length/set_elements/get_elements\n"));

      length = ftc1->get_length (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (length != 2)
        {
          ++this->error_count_;
        }

      ftc1->set_length (3
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynamicAny::AnySeq as_in (3);
      as_in.length (3);
      CORBA::Any in_any3;
      in_any3 <<= CORBA::Any::from_string (data.m_string2, 8);
      as_in[0] = in_any3;
      as_in[1] = in_any3;
      in_any3 <<= CORBA::Any::from_string (data.m_string1, 8);
      as_in[2] = in_any3;
      ftc1->set_elements (as_in
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      DynamicAny::AnySeq_var as_out = ftc1->get_elements (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::ULong index = 2;
      CORBA::Any out_any2 = as_out[index];
      char* out_str2;
      out_any2 >>= CORBA::Any::to_string (out_str2, 8);

      if (ACE_OS::strcmp (out_str2, data.m_string1))
        {
          ++this->error_count_;
        }

      if (this->error_count_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }

      fa1->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ftc1->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "test_dynsequence::run_test");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));


  return 0;
}
