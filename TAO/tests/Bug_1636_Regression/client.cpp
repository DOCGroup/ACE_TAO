#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "testC.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_DEBUG ((LM_DEBUG, "Get reference to the DynAny Factory\n"));
      CORBA::Object_var object =
        orb->resolve_initial_references ("DynAnyFactory");

      DynamicAny::DynAnyFactory_var factory =
        DynamicAny::DynAnyFactory::_narrow (object.in ());

      UnionIecs::S_i_num s_num;
      s_num.type_num = UnionIecs::E_type_num_called_party_number;
      s_num.nature = 5;

      UnionIecs::S_num num1;
      num1.num (s_num);
      CORBA::Any my_any1;
      my_any1 <<= num1;

      ACE_DEBUG ((LM_DEBUG,
                  "calling from_any () and to_any () with non-empty union...\n"));
      DynamicAny::DynAny_var dynany1 =
        factory->create_dyn_any_from_type_code (UnionIecs::_tc_S_num);

      dynany1->from_any (my_any1);
      CORBA::Any_var arg1 =
        dynany1->to_any ();

      ACE_DEBUG ((LM_DEBUG, "...done\n"));

      UnionIecs::S_num num2;
      num2._default ();
      CORBA::Any my_any2;
      my_any2 <<= num2;

      ACE_DEBUG ((LM_DEBUG,
                  "calling from_any () and to_any () with empty union...\n"));
      DynamicAny::DynAny_var dynany2 =
        factory->create_dyn_any_from_type_code (UnionIecs::_tc_S_num);

      dynany2->from_any (my_any2);

      CORBA::Any_var arg2 =
        dynany2->to_any ();

      ACE_DEBUG ((LM_DEBUG, "...done.\nTest passed.\n"));

      // Finally destroy the ORB
      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception - test failed:\n");
      return 1;
    }

  return 0;
}
