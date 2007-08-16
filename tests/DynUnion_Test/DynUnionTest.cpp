// $Id$

#include "dynanyC.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"

// Creates DynUnion using typecodes generated through IDL compiler.
int runStatic(CORBA::ORB_ptr orb)
{
  try
    {
        CORBA::Object_var obj =
          orb->resolve_initial_references("DynAnyFactory");

        DynamicAny::DynAnyFactory_var dfact =
          DynamicAny::DynAnyFactory::_narrow(obj.in ());

        if (CORBA::is_nil (dfact.in ()))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Nil dynamic any factory after narrow\n"),
                              -1);
          }

        DynamicAny::DynAny_var dany =
           dfact->create_dyn_any_from_type_code(DynAnyTest::_tc_EventData);

        DynamicAny::DynUnion_var dunion =
          DynamicAny::DynUnion::_narrow(dany.in ());

        DynamicAny::DynAny_var disc = dunion->get_discriminator();

        DynamicAny::DynEnum_var denum =
          DynamicAny::DynEnum::_narrow(disc.in ());

        if (CORBA::is_nil (denum.in ()))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Nil dynamic enum after narrow\n"),
                              -1);
          }

        ACE_DEBUG ((LM_DEBUG, "Static Test Passed\n"));
    }
  catch (const CORBA::Exception & ex)
    {
      ex._tao_print_exception ("runStatic");
      return -1;
    }

  return 0;
}

// Creates DynUnion from typecode created programmatically

int runDynamic(CORBA::ORB_ptr orb)
{
  try
    {
        // Create typecode for EventKind.

        CORBA::EnumMemberSeq eseq;

        eseq.length(2);

        eseq[0] = CORBA::string_dup("ALARM_EVENT");
        eseq[1] = CORBA::string_dup("LOG_EVENT");

        CORBA::TypeCode_var eventKindTc;

        eventKindTc = orb->create_enum_tc("IDL:DynAnyTest/EventKind:1.0",
                                          "EventKind",
                                          eseq);

        // Create typecode for EventData. EventKind tc is used.

        CORBA::Any label;
        CORBA::UnionMemberSeq useq;

        useq.length(2);

        label <<= DynAnyTest::ALARM_EVENT;
        useq[0].name = CORBA::string_dup("ev_data");
        useq[0].label = label;
        useq[0].type = CORBA::TypeCode::_duplicate(CORBA::_tc_any);
        useq[0].type_def = CORBA::IDLType::_nil ();

        label <<= DynAnyTest::LOG_EVENT;
        useq[1].name = CORBA::string_dup("ev_desc");
        useq[1].label = label;
        useq[1].type = CORBA::TypeCode::_duplicate(CORBA::_tc_string);
        useq[1].type_def = CORBA::IDLType::_nil ();

        CORBA::TypeCode_var eventDataTc;

        eventDataTc = orb->create_union_tc("IDL:DynAnyTest/EventData:1.0",
                                           "EventData",
                                           eventKindTc.in (),
                                           useq);

        // Now create DynUnion

        CORBA::Object_var obj = orb->resolve_initial_references("DynAnyFactory");

        DynamicAny::DynAnyFactory_var dfact =
                    DynamicAny::DynAnyFactory::_narrow(obj.in ());

        if (CORBA::is_nil (dfact.in ()))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Nil dynamic any factory after narrow\n"),
                              -1);
          }

        DynamicAny::DynAny_var dany =
                    dfact->create_dyn_any_from_type_code(eventDataTc.in ());

        DynamicAny::DynUnion_var dunion =
          DynamicAny::DynUnion::_narrow(dany.in ());

        DynamicAny::DynAny_var disc = dunion->get_discriminator();

        DynamicAny::DynEnum_var denum =
          DynamicAny::DynEnum::_narrow(disc.in ());

        // This fails. The type of discriminator is ulong
        if (CORBA::is_nil (denum.in ()))
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Nil dynamic enum after narrow\n"),
                              -1);
          }

        ACE_DEBUG ((LM_DEBUG, "Dynamic Test Passed\n"));

        denum->destroy();
        dunion->destroy();
    }
  catch (const CORBA::Exception & ex)
    {
      ex._tao_print_exception ("Dynamic Test Failed with exception");
    }
  return 0;
}

int main(int argc, char * argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      if (runStatic(orb.in()) != 0)
        return -1;

      if (runDynamic(orb.in()) != 0)
        return -1;
  }
  catch (const CORBA::Exception & ex)
    {
      ex._tao_print_exception ("Tests failed with exception");
      return 1;
    }

  return 0;
}

