//
// IFRBrowser.cpp
//
// A rudimentary interface repository browser.
//

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include "ace/Log_Msg.h"

#include <iostream>

ACE_TCHAR const * const program_label = ACE_TEXT ("IFR Browser");

void list_contents (CORBA::ContainedSeq const &repo_contents);
void list_interface (CORBA::InterfaceDef_ptr interface_def);
void list_operation (CORBA::OperationDescription const &operation_description);
void list_parameter (CORBA::ParameterDescription const &parameter_description);

char const *decode_type_code (CORBA::TypeCode_ptr type_code);
char const *decode_parameter_mode (CORBA::ParameterMode mode);
char const *decode_operation_mode (CORBA::OperationMode mode);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
    CORBA::Object_var const obj = orb->resolve_initial_references ("InterfaceRepository");
    CORBA::Repository_var const repository = CORBA::Repository::_narrow (obj.in ());

    if (CORBA::is_nil (repository.in ()))
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%N) failed to narrow interface repository reference.\n")));
        return -1;
      }

    CORBA::ContainedSeq_var const repo_contents = repository->contents (CORBA::dk_all, true);
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("%s: the interface repository contains %u elements.\n"),
                program_label,
                repo_contents->length ()));
    list_contents (repo_contents.in ());
    return 0;
  }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "main() Caught CORBA::Exception : " << ex << std::endl;
    }
  return 1;
}

void
list_contents (CORBA::ContainedSeq const &repo_contents)
{
  for (CORBA::ULong i = 0; i < repo_contents.length (); ++i)
    {
      CORBA::Contained::Description_var const description = repo_contents[i]->describe ();
      switch (description->kind)
        {
        case CORBA::dk_Constant:
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("%s: element[%u] is a constant definition.\n"),
                      program_label, i + 1));
          break;
        case CORBA::dk_Typedef:
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("%s: element[%u] is a typedef definition.\n"),
                      program_label, i + 1));
          break;
        case CORBA::dk_Exception:
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("%s: element[%u] is an exception definition.\n"),
                      program_label, i + 1));
          break;
        case CORBA::dk_Interface:
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("%s: element[%u] is an interface definition.\n")
                        ACE_TEXT ("%s: listing element[%u]...\n"),
                        program_label, i + 1, program_label, i + 1));
            CORBA::InterfaceDef_var const interface_def =
              CORBA::InterfaceDef::_narrow (repo_contents[i]);
            list_interface (interface_def.in ());
            break;
          }
        case CORBA::dk_Module:
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("%s: element[%u] is a module definition.\n"),
                        program_label, i + 1));
            CORBA::ModuleDef_var const module_def = CORBA::ModuleDef::_narrow (repo_contents[i]);
            CORBA::ContainedSeq_var const module_contents =
              module_def->contents (CORBA::dk_all, true);
            CORBA::String_var const module_id = module_def->id ();
            CORBA::String_var const module_name = module_def->name ();

            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("%s:\n// %C\nmodule %C\n{\n")
                        ACE_TEXT ("%s: the module contains %u elements.\n"),
                        program_label, module_id.in (), module_name.in (), program_label,
                        module_contents->length ()));
            list_contents (module_contents.in ());
            ACE_DEBUG ((LM_INFO, ACE_TEXT ("}\n")));
            break;
          }
        default:
          break;
        }
    }
}

void
list_interface (CORBA::InterfaceDef_ptr interface_def)
{
  CORBA::InterfaceDef::FullInterfaceDescription_var const description =
    interface_def->describe_interface ();
  char const * const interface_name = description->name;
  char const * const interface_repo_id = description->id;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%s:\n\t// %C\n\tinterface %C\n\t{"),
              program_label, interface_repo_id, interface_name));

  CORBA::ULong const operations_count = description->operations.length ();
  for (CORBA::ULong i = 0; i < operations_count; ++i)
    {
      list_operation (description->operations[i]);
    }

  CORBA::ULong const attributes_count = description->attributes.length ();
  if (attributes_count > 0)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("%s: %C has %u attribute(s).\n"),
                  program_label, interface_name, attributes_count));
    }

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n\t}\n")));
}

void
list_operation (CORBA::OperationDescription const &operation_description)
{
  char const * const operation_repo_id = operation_description.id;
  char const * const operation_result = decode_type_code (operation_description.result.in ());
  char const * const operation_name = operation_description.name;
  char const * const operation_mode = decode_operation_mode (operation_description.mode);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("\n\t\t// %C \n\t\t%C %C %C"),
              operation_repo_id, operation_result, operation_name, operation_mode));

  CORBA::ParDescriptionSeq const &parameters = operation_description.parameters;
  CORBA::ULong const parameters_count = parameters.length ();
  if (parameters_count > 0)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n\t\t(\n\t\t")));
      for (CORBA::ULong i = 0; i < parameters_count; ++i)
        {
          list_parameter (parameters[i]);
          if (i + 1 < parameters_count)
            {
              ACE_DEBUG ((LM_INFO, ACE_TEXT (",\n\t\t")));
            }
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n\t\t);\n")));
    }
  else
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("();\n")));
    }
}

void
list_parameter (CORBA::ParameterDescription const &parameter_description)
{
  char const * const type_code = decode_type_code (parameter_description.type.in ());
  char const * const parameter_mode = decode_parameter_mode (parameter_description.mode);
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("%C %C %C"),
              parameter_mode, type_code, parameter_description.name.in ()));
}

char const *
decode_type_code (CORBA::TypeCode_ptr type_code)
{
  if (type_code->equivalent (CORBA::_tc_void))
    {
      return "void";
    }
  if (type_code->equivalent (CORBA::_tc_boolean))
    {
      return "boolean";
    }
  if (type_code->equivalent (CORBA::_tc_string))
    {
      return "string";
    }
  return "";
}

char const *
decode_parameter_mode (CORBA::ParameterMode mode)
{
  switch (mode)
    {
    case CORBA::PARAM_IN:
      return "in";
    case CORBA::PARAM_OUT:
      return "out";
    case CORBA::PARAM_INOUT:
      return "inout";
    default:
      return "";
    }
}

char const *
decode_operation_mode (CORBA::OperationMode mode)
{
  return mode == CORBA::OP_NORMAL ? "synchronous" : "asynchronous";
}
