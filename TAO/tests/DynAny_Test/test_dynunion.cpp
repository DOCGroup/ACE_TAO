//=============================================================================
/**
 *  @file    test_dynunion.cpp
 *
 *  $Id$
 *
 *  Implementation of the simple DynUnion test
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#include "da_testsC.h"
#include "test_dynunion.h"
#include "data.h"
#include "analyzer.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "ace/OS_NS_string.h"

Test_DynUnion::Test_DynUnion (CORBA::ORB_var orb, int debug)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynunion")),
    error_count_ (0),
    debug_ (debug)
{
}

Test_DynUnion::~Test_DynUnion (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynUnion::test_name (void) const
{
  return this->test_name_;
}

int
Test_DynUnion::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_union tu;

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[12]));

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

      DynAnyAnalyzer analyzer(this->orb_.in(), dynany_factory.in(), debug_);

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/from_any/to_any with string\n"));

      DynAnyTests::test_union tstring;
      tstring._d (DynAnyTests::TE_THIRD);
      tstring.str (CORBA::string_dup (data.m_string1));
      CORBA::Any in_any4;
      in_any4 <<= tstring;
      DynamicAny::DynAny_var dp4 =
        dynany_factory->create_dyn_any (in_any4);
      DynamicAny::DynUnion_var fa4 =
        DynamicAny::DynUnion::_narrow (dp4.in ());

      analyzer.analyze(fa4.in());

      CORBA::Any_var out_any5 = fa4->to_any ();

      DynamicAny::DynAny_var dp5 =
        dynany_factory->create_dyn_any (out_any5.in());

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      tu._d (DynAnyTests::TE_SECOND);
      tu.tc (data.m_typecode2);
      CORBA::Any in_any1;
      in_any1 <<= tu;
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1);
      DynamicAny::DynUnion_var fa1 =
        DynamicAny::DynUnion::_narrow (dp1.in ());
      fa1->seek (1);
      fa1->insert_typecode (data.m_typecode1);

      CORBA::TypeCode_var s_out1 = fa1->get_typecode ();

      CORBA::Boolean const equal_tc1 =
        s_out1->equal (data.m_typecode1);

      if (equal_tc1)
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

      analyzer.analyze (fa1.in());

      CORBA::Any_var out_any1 = fa1->to_any ();
      DynamicAny::DynAny_var ftc1_base =
        dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_union);

      DynamicAny::DynUnion_var ftc1 =
        DynamicAny::DynUnion::_narrow (ftc1_base.in ());

      if (CORBA::is_nil (ftc1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynUnion::_narrow() returned nil\n"),
                            -1);
        }

      ftc1->from_any (out_any1.in ());

      ftc1->seek (1);

      CORBA::TypeCode_var s_out2 =
        ftc1->get_typecode ();

      CORBA::Boolean const equal_tc2 =
        s_out2->equal (data.m_typecode1);

      if (equal_tc2)
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

      analyzer.analyze( fa1.in());

      CORBA::Any_var out_any2 = fa1->to_any ();

      CORBA::TypeCode_var s_out3;

      try
        {
          DynamicAny::DynAny_var ftc2_base =
            dynany_factory->create_dyn_any_from_type_code
            (DynAnyTests::_tc_test_union_alias);

          DynamicAny::DynUnion_var ftc2 =
            DynamicAny::DynUnion::_narrow (ftc2_base.in ());

          if (CORBA::is_nil (ftc2.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "DynUnion::_narrow() returned nil\n"),
                                -1);
            }

          ftc2->from_any (out_any2.in ());

          ftc2->seek (1);

          s_out3 = ftc2->get_typecode ();
        }
      catch (const CORBA::TypeCode::BadKind& )
        {
          // Failed to create
        }

      if (!CORBA::is_nil (s_out3.in ()))
        {
          CORBA::Boolean const equal_tc =
            s_out3->equal (data.m_typecode1);

          if (equal_tc)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "++ OK ++\n"));
            }
          else
            {
              ++this->error_count_;
            }
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing:discriminator/discriminator_kind\n"));

      DynamicAny::DynAny_var dp2 =
        ftc1->get_discriminator ();

      CORBA::TypeCode_var tc2 =
        dp2->type ();

      CORBA::TCKind tc1kind =
        ftc1->discriminator_kind ();

      CORBA::TCKind tc2kind = tc2->kind ();

      if (tc2kind == tc1kind)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing:member/member_kind/member_name\n"));

      DynamicAny::DynAny_var dp3 =
        ftc1->member ();

      CORBA::String_var m_nm =
        ftc1->member_name ();

      CORBA::TCKind tk =
        ftc1->member_kind ();

      CORBA::TypeCode_var tc3 =
        dp3->get_typecode ();

      CORBA::Boolean const equal_tc3 =
        data.m_typecode1->equal (tc3.in ());

      if (!ACE_OS::strcmp (m_nm.in (), "tc")
          && tk == CORBA::tk_TypeCode
          && equal_tc3)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: set_to_default_member\n"));

      ftc1->set_to_default_member ();

      ftc1->seek (1);

      ftc1->insert_short (data.m_short1);

      DynamicAny::DynAny_var mem =
        ftc1->member ();

      CORBA::Short out_s =
        mem->get_short ();

      if (out_s == data.m_short1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      fa1->destroy ();

      ftc1->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                 "testing: create_dyn_any with _default()\n"));
      DynAnyTests::test_implicit_def test_implicit_def_union;
      CORBA::Any any_union;

      test_implicit_def_union._default();

      any_union <<= test_implicit_def_union;

      DynamicAny::DynAny_var da_union =
        dynany_factory->create_dyn_any (any_union);
      ACE_DEBUG ((LM_DEBUG,
                  "++ OK ++\n"));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: union_no_active_member destroy\n"));
          
      // Test fix for bug on destroy when there is no active member in union.
      DynAnyTests::test_union_no_active_member l_union_value_no_active_member;

      CORBA::Any corba_union;
      corba_union <<= l_union_value_no_active_member;
      
      DynamicAny::DynAny_var l_dynany = dynany_factory->create_dyn_any (corba_union);
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
      

      // if we get here the create_dyn_any worked.
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_dynunion::run_test");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}
