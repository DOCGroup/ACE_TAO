#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/ORB.h"

#include <cstring>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const ifr_object =
        orb->resolve_initial_references ("InterfaceRepository");
      CORBA::Repository_var const repository =
        CORBA::Repository::_narrow (ifr_object.in ());

      if (CORBA::is_nil (repository.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Nil IFR reference\n"), 1);

      CORBA::InterfaceDefSeq base_interfaces;
      CORBA::InterfaceDef_var const interface =
        repository->create_interface ("IDL:interface865:1.0",
                                      "interface865",
                                      "1.0",
                                      base_interfaces);

      CORBA::PrimitiveDef_var const void_primitive =
        repository->get_primitive (CORBA::pk_void);
      CORBA::PrimitiveDef_var const char_primitive =
        repository->get_primitive (CORBA::pk_char);
      CORBA::PrimitiveDef_var const long_primitive =
        repository->get_primitive (CORBA::pk_long);
      CORBA::PrimitiveDef_var const short_primitive =
        repository->get_primitive (CORBA::pk_short);

      constexpr CORBA::ULong parameter_count {3};
      CORBA::ParDescriptionSeq parameters (parameter_count);
      parameters.length (parameter_count);

      parameters[0].name = CORBA::string_dup ("p1");
      parameters[0].type_def =
        CORBA::PrimitiveDef::_duplicate (char_primitive.in ());
      parameters[0].type = char_primitive->type ();
      parameters[0].mode = CORBA::PARAM_IN;

      parameters[1].name = CORBA::string_dup ("p2");
      parameters[1].type_def =
        CORBA::PrimitiveDef::_duplicate (long_primitive.in ());
      parameters[1].type = long_primitive->type ();
      parameters[1].mode = CORBA::PARAM_OUT;

      parameters[2].name = CORBA::string_dup ("p3");
      parameters[2].type_def =
        CORBA::PrimitiveDef::_duplicate (short_primitive.in ());
      parameters[2].type = short_primitive->type ();
      parameters[2].mode = CORBA::PARAM_INOUT;

      CORBA::ExceptionDefSeq exceptions;
      CORBA::ContextIdSeq context_ids;
      CORBA::OperationDef_var const operation =
        interface->create_operation ("IDL:interface865/f:1.0",
                                     "f",
                                     "1.0",
                                     void_primitive.in (),
                                     CORBA::OP_NORMAL,
                                     parameters,
                                     exceptions,
                                     context_ids);

      CORBA::NVList_var operation_list {};
      orb->create_operation_list (operation.in (), operation_list.out ());

      bool failed {};
      CORBA::ULong const actual_count = operation_list->count ();
      if (actual_count != parameter_count)
        {
          ACE_DEBUG ((LM_DEBUG, "Test failed: incorrect number of parameters\n"));
          failed = true;
        }

      CORBA::NamedValue_ptr parameter = operation_list->item (0);
      if (std::strcmp (parameter->name (), "p1") != 0 ||
          !parameter->value ()->type ()->equal (CORBA::_tc_char) ||
          parameter->flags () != CORBA::ARG_IN)
        {
          ACE_DEBUG ((LM_DEBUG, "Test failed: parameter 1 is incorrect\n"));
          failed = true;
        }

      parameter = operation_list->item (1);
      if (std::strcmp (parameter->name (), "p2") != 0 ||
          !parameter->value ()->type ()->equal (CORBA::_tc_long) ||
          parameter->flags () != CORBA::ARG_OUT)
        {
          ACE_DEBUG ((LM_DEBUG, "Test failed: parameter 2 is incorrect\n"));
          failed = true;
        }

      parameter = operation_list->item (2);
      if (std::strcmp (parameter->name (), "p3") != 0 ||
          !parameter->value ()->type ()->equal (CORBA::_tc_short) ||
          parameter->flags () != CORBA::ARG_INOUT)
        {
          ACE_DEBUG ((LM_DEBUG, "Test failed: parameter 3 is incorrect\n"));
          failed = true;
        }

      interface->destroy ();

      if (failed)
        return 1;

      ACE_DEBUG ((LM_DEBUG, ". seems OK\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception - test failed:");
      return 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "An unknown exception occurred - test failed\n"));
      return 1;
    }

  return 0;
}
