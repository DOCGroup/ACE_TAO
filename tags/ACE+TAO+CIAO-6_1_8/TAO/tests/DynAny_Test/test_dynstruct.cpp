//=============================================================================
/**
 *  @file    test_dynstruct.cpp
 *
 *  $Id$
 *
 *  Implementation of the DynStruct basic test
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "test_dynstruct.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "analyzer.h"
#include "ace/OS_NS_string.h"

Test_DynStruct::Test_DynStruct (CORBA::ORB_var orb, int debug)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynstruct")),
    error_count_ (0),
    debug_ (debug)
{
}

Test_DynStruct::~Test_DynStruct (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynStruct::test_name (void) const
{
  return this->test_name_;
}

int
Test_DynStruct::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_struct ts;
  ts.c = data.m_char2;
  ts.ss.length (0UL);
  ts.es.f = data.m_float2;
  ts.es.s = data.m_short2;

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "testing: constructor(Any)/insert/get/next/seek/"
                  "rewind/current_component\n"));

      CORBA::Object_var factory_obj =
        this->orb_->resolve_initial_references ("DynAnyFactory");

      DynamicAny::DynAnyFactory_var dynany_factory =
        DynamicAny::DynAnyFactory::_narrow (factory_obj.in ());

      if (CORBA::is_nil (dynany_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil dynamic any factory after narrow\n"),
                            -1);
        }

      DynAnyAnalyzer analyzer (this->orb_.in (),
                               dynany_factory.in (),
                               debug_);

      CORBA::Any in_any1;
      in_any1 <<= ts;
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1);

      DynamicAny::DynStruct_var fa1 =
        DynamicAny::DynStruct::_narrow (dp1.in ());

      fa1->insert_char (data.m_char1);

      fa1->next ();

      fa1->insert_short_seq (data.m_shortseq1);

      fa1->next ();

      DynamicAny::DynAny_var cc =
        fa1->current_component ();

      cc->insert_float (data.m_float1);

      cc->next ();

      cc->insert_short (data.m_short1);
      cc->rewind ();

      // To show that calling destroy() on a component does
      // nothing, as required by the spec.
      cc->destroy ();

      fa1->rewind ();

      CORBA::Char c = fa1->get_char ();

      if (c != data.m_char1)
        {
          ++this->error_count_;
        }

      fa1->next ();

      data.m_shortseq2 =
        fa1->get_short_seq ();

      bool good =
        data.m_shortseq2->length () == data.m_shortseq1.length ();

      if (good)
        {
          for (CORBA::ULong i = 0; i < data.m_shortseq1.length (); ++i)
            {
              if (data.m_shortseq2[i] != data.m_shortseq1[i])
                {
                  ++this->error_count_;
                  break;
                }
            }
        }
      else
        {
          ++this->error_count_;
        }

      fa1->next ();

      cc = fa1->current_component ();

      cc->seek (1);

      CORBA::Short s = cc->get_short ();

      if (s != data.m_short1)
        {
          ++this->error_count_;
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
                            DynAnyTests::_tc_test_struct
                          );

      DynamicAny::DynStruct_var ftc1 =
        DynamicAny::DynStruct::_narrow (ftc1_base.in ());

      if (CORBA::is_nil (ftc1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynStruct::_narrow() returned nil\n"),
                            -1);
        }

      ts.c = data.m_char1;
      ts.ss.length (0UL);
      ts.es.f = data.m_float1;
      ts.es.s = data.m_short1;
      CORBA::Any in_any2;
      in_any2 <<= ts;
      ftc1->from_any (in_any2);

      analyzer.analyze (ftc1.in ());

      CORBA::Any_var out_any1 =
        ftc1->to_any ();

      DynAnyTests::test_struct* ts_out;
      out_any1.in () >>= ts_out;

      if (ts_out->es.s == data.m_short1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "testing: constructor(TypeCode alias)/from_any/to_any\n"));


      const DynAnyTests::test_struct *ts_out2 = 0;
      CORBA::Any_var out_any2 ;
      DynamicAny::DynStruct_var ftc2;

      try
        {
          DynamicAny::DynAny_var ftc2_base =
            dynany_factory->create_dyn_any_from_type_code (
                                DynAnyTests::_tc_test_struct_alias
                              );

          ftc2 = DynamicAny::DynStruct::_narrow (ftc2_base.in ());

          if (CORBA::is_nil (ftc2.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "DynStruct::_narrow() returned nil\n"),
                                -1);
            }

          ts.c = data.m_char1;
          ts.ss.length (0UL);
          ts.es.f = data.m_float1;
          ts.es.s = data.m_short1;
          CORBA::Any in_any3;
          in_any3 <<= ts;
          ftc2->from_any (in_any3);

//          char c = ftc2->get_char ();
//          CORBA::Boolean status = ftc2->next ();
//          CORBA::Long lo = ftc2->get_long ();
//          analyzer.analyze (ftc2.in ());

          out_any2 = ftc2->to_any ();

          if (!(out_any2.in () >>= ts_out2)) // problem
            {
              ts_out2 = 0;
            }
        }
      catch (const CORBA::TypeCode::BadKind& )
        {
        }

      if (ts_out2 != 0 && ts_out2->es.s == data.m_short1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }


      ACE_DEBUG ((LM_DEBUG,
                  "testing: current_member_name/current_member_kind\n"));

      ftc1->seek (2);

      DynamicAny::FieldName_var fn =
        ftc1->current_member_name ();

      if (ACE_OS::strcmp (fn.in (), "es"))
        {
          ++this->error_count_;
        }

      CORBA::TCKind tk =
        ftc1->current_member_kind ();

      if (tk != CORBA::tk_struct)
        {
          ++this->error_count_;
        }

      if (this->error_count_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "testing: current_member_name/current_member_kind "
                  "with alias\n"));
      ftc2->seek (2);

      DynamicAny::FieldName_var fn2 =
        ftc2->current_member_name ();

      if (ACE_OS::strcmp (fn2.in (), "es"))
        {
          ++this->error_count_;
        }

      CORBA::TCKind tk2 =
        ftc2->current_member_kind ();

      if (tk2 != CORBA::tk_struct)
        {
          ++this->error_count_;
        }

      if (this->error_count_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "testing: get_members/set_members\n"));

      DynamicAny::NameValuePairSeq_var nvps =
        fa1->get_members ();

      DynamicAny::DynAny_var sm_base =
        dynany_factory->create_dyn_any_from_type_code (
            DynAnyTests::_tc_test_struct
          );

      DynamicAny::DynStruct_var sm =
        DynamicAny::DynStruct::_narrow (sm_base.in ());

      if (CORBA::is_nil (sm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynStruct::_narrow()[2] returned nil\n"),
                            -1);
        }

      sm->set_members (nvps.in ());

      DynamicAny::NameValuePairSeq_var gm =
        sm->get_members ();

      CORBA::ULong index = 2;

      if (ACE_OS::strcmp (gm[index].id, "es"))
        {
          ++this->error_count_;
        }

      fa1->destroy ();

      ftc1->destroy ();

      sm->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                "++ OK ++\n"));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: struct_with_long_double destroy\n"));

      // Test fix for bug on destroy when struct contains long_double value.
      DynAnyTests::test_struct_with_long_double l_long_double_in_struct;

      CORBA::Any corba_long_double;
      corba_long_double <<= l_long_double_in_struct;

      DynamicAny::DynAny_var l_dynany = dynany_factory->create_dyn_any (corba_long_double);
      try
      {
        l_dynany->destroy ();
        ACE_DEBUG ((LM_DEBUG,
          "++ OK ++\n"));
      }
      catch (...)
      {
        ++this->error_count_;
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_dynstruct::run_test");
      ++this->error_count_;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return !!error_count_;
}
