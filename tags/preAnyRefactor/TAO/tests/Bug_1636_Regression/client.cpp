//
// $Id$
//
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicAny/DynAnyFactory.h"
#include "testC.h"

int main (int argc, char* argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY_NEW_ENV
  {
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv,
                       "" /* the ORB name, it can be anything! */
                       ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG,"Get reference to the DynAny Factory\n"));
    CORBA::Object_var obj =
      orb->resolve_initial_references ("DynAnyFactory"
                                       ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    DynamicAny::DynAnyFactory_var daf =
      DynamicAny::DynAnyFactory::_narrow (obj.in()
                                          ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    UnionIecs::S_i_num s_num;
    s_num.type_num = UnionIecs::E_type_num_called_party_number;
    s_num.nature = 5;

    UnionIecs::S_num num1;
    num1.num(s_num);
    CORBA::Any MyAny1;
    MyAny1 <<= num1;

    ACE_DEBUG ((LM_DEBUG,
                "calling from_any () and to_any () with non-empty union...\n"));
    DynamicAny::DynAny_var dynany1 =
      daf->create_dyn_any_from_type_code (UnionIecs::_tc_S_num
                                          ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    dynany1->from_any(MyAny1
                      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    CORBA::Any_var arg1 =
      dynany1->to_any (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG, "...done\n"));

    UnionIecs::S_num num2;
    num2._default();
    CORBA::Any MyAny2;
    MyAny2 <<= num2;

    ACE_DEBUG ((LM_DEBUG,
                "calling from_any () and to_any () with empty union...\n" ));
    DynamicAny::DynAny_var dynany2 =
      daf-> create_dyn_any_from_type_code (UnionIecs::_tc_S_num
                                           ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    dynany2->from_any (MyAny2
                       ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::Any_var arg2 =
      dynany2->to_any(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG,"...done.\nTest passed.\n"));

    // Finally destroy the ORB
    orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception - test failed:\n");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN(1);

  return 0;
}
