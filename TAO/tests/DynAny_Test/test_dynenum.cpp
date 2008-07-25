//=============================================================================
/**
 *  @file    test_dynenum.cpp
 *
 *  $Id$
 *
 *  Implementation of the simple DynEnum test
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "test_dynenum.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "analyzer.h"
#include "ace/OS_NS_string.h"

Test_DynEnum::Test_DynEnum (CORBA::ORB_var orb, int debug)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynenum")),
    error_count_ (0),
    debug_ (debug)
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
  Data data (this->orb_);

  try
    {
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

      DynAnyAnalyzer analyzer (this->orb_.in(),
                               dynany_factory.in (),
                               debug_);
      {
        ACE_DEBUG ((LM_DEBUG,
                  "\t*=*=*=*= %s =*=*=*=*\n",
                  data.labels[18]));
        ACE_DEBUG ((LM_DEBUG,
                  "testing: constructor(Any)/set_as_string/get_as_ulong\n"));


        DynAnyTests::test_enum te = DynAnyTests::TE_ZEROTH;
        CORBA::Any in_any1;
        in_any1 <<= te;
        DynamicAny::DynAny_var dp1 =
          dynany_factory->create_dyn_any (in_any1);

        DynamicAny::DynEnum_var de1 =
          DynamicAny::DynEnum::_narrow (dp1.in ());

        analyzer.analyze(dp1.in());

        de1->set_as_string ("TE_FIRST");

        CORBA::ULong ul_out1 =
          de1->get_as_ulong ();

        analyzer.analyze(de1.in());

        CORBA::Any_var out_any2 =
          de1->to_any ();

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

        de1->set_as_ulong (3);
        CORBA::String_var s =
          de1->get_as_string ();

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
          dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_enum);

        DynamicAny::DynEnum_var de2 =
          DynamicAny::DynEnum::_narrow (de2_base.in ());

        if (CORBA::is_nil (de2.in ()))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                              "DynEnum::_narrow() returned nil\n"),
                              -1);
          }

        CORBA::Any in_any2;
        in_any2 <<= DynAnyTests::TE_THIRD;
        de2->from_any (in_any2);

        analyzer.analyze(de2.in());

        CORBA::Any_var out_any1 =
          de2->to_any ();

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

        ACE_DEBUG ((LM_DEBUG,
                  "testing: equal\n"));

        CORBA::Boolean equal =
          de1->equal (de2.in ());

        if (equal)
          {
            ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
          }
        else
          {
            ++this->error_count_;
          }

        de1->destroy ();

        de2->destroy ();
      }
      {
        ACE_DEBUG ((LM_DEBUG,
                  "\t*=*=*=*= %s =*=*=*=*\n",
                  data.labels[19]));
        ACE_DEBUG ((LM_DEBUG,
                  "testing: set_as_string/get_as_ulong\n"));

        // initialise instances
        DynAnyTests::StructTypedefEnum l_struct_typedef_enum;
        l_struct_typedef_enum.field_1 = DynAnyTests::TE_ZEROTH;
        l_struct_typedef_enum.field_2 = DynAnyTests::TE_FIRST;
        l_struct_typedef_enum.field_3 = DynAnyTests::TE_SECOND;
        CORBA::Any l_any_struct_typedef_enum;
        l_any_struct_typedef_enum <<= l_struct_typedef_enum;

        DynamicAny::DynAny_var da
          = dynany_factory->create_dyn_any (l_any_struct_typedef_enum);
        DynamicAny::DynStruct_var ds =
          DynamicAny::DynStruct::_narrow (da.in ());
        DynamicAny::DynAny_var cc
          = ds->current_component();

        DynamicAny::FieldName_var fn
          = ds->current_member_name();

        DynamicAny::DynEnum_var member1
           = DynamicAny::DynEnum::_narrow(cc.in ());

        member1->set_as_string ("TE_FOURTH");

        CORBA::ULong ul_out1 = member1->get_as_ulong ();

        if (ul_out1 == 4)
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

        member1->set_as_ulong (3);

        CORBA::String_var s = member1->get_as_string ();

        if (ACE_OS::strcmp (s.in (), "TE_THIRD") == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
          }
        else
          {
            ++this->error_count_;
          }
       }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_dynenum::run_test");
      ++this->error_count_;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return !!error_count_;
}
