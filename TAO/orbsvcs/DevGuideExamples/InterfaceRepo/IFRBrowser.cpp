//
// IFRBrowser.cpp
//
// A rudimentary interface repository
// browser, very rudimentary
//

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include "ace/Log_Msg.h"

#include <iostream>

constexpr char const * programLabel = "IFR Browser";

void listContents (CORBA::ContainedSeq const & repoContents);
void listInterface (CORBA::InterfaceDef_ptr interfaceDef);
void listOperation (CORBA::OperationDescription const * operationDescr);
void listParameter (CORBA::ParameterDescription const * parameterDescr);

char const * decodeTypeCode (CORBA::TypeCode_ptr typeCode);
char const * decodeParameterMode (CORBA::ParameterMode mode);
char const * decodeOperationMode (CORBA::OperationMode mode);

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj =
      orb->resolve_initial_references("InterfaceRepository");

    CORBA::Repository_var ifrRepo = CORBA::Repository::_narrow(obj.in());

    if (CORBA::is_nil(ifrRepo.in()))
    {
      ACE_DEBUG((LM_ERROR,
        ACE_TEXT("(%N) failed to narrow interface repository referece.\n")
        ));
      return -1;
    }

    CORBA::ContainedSeq_var repoContents = ifrRepo->contents(CORBA::dk_all, 1);

    ACE_DEBUG((LM_INFO,
      ACE_TEXT("%s: the interface repository contains %d elements.\n"),
      programLabel,
      repoContents->length()
      ));
    listContents(repoContents.in());

    return 0;
  }
  catch (CORBA::Exception const & ex)
  {
    std::cerr << "main() Caught CORBA::Exception : " << ex << std::endl;
  }
  return 1;
}

void listContents (CORBA::ContainedSeq const & repoContents)
{
  //
  // List the contents of each element.
  //
  for(unsigned int i = 0; i < repoContents.length(); ++i)
  {
    CORBA::Contained::Description_var desc = repoContents[i]->describe();
    switch(desc->kind)
    {
    case CORBA::dk_Constant:
      ACE_DEBUG((LM_INFO,
        ACE_TEXT("%s: element[%d] is a constant definition.\n"),
        programLabel,
        i + 1
        ));
      break;
    case CORBA::dk_Typedef:
      ACE_DEBUG((LM_INFO,
        ACE_TEXT("%s: element[%d] is a typedef definition.\n"),
        programLabel,
        i + 1
        ));
      break;
    case CORBA::dk_Exception:
      ACE_DEBUG((LM_INFO,
        ACE_TEXT("%s: element[%d] is an exception definition.\n"),
        programLabel,
        i + 1
        ));
      break;
    case CORBA::dk_Interface:
      {
        ACE_DEBUG((LM_INFO,
          ACE_TEXT("%s: element[%d] is an interface definition.\n")
          ACE_TEXT("%s: listing element[%d]...\n"),
          programLabel,
          i + 1,
          programLabel,
          i + 1
          ));
        CORBA::InterfaceDef_var interfaceDef =
          CORBA::InterfaceDef::_narrow(repoContents[i]);
        listInterface(interfaceDef.in());
        break;
      }
    case CORBA::dk_Module: {
      ACE_DEBUG((LM_INFO,
        ACE_TEXT("%s: element[%d] is a module definition.\n"),
        programLabel,
        i + 1
        ));
      CORBA::ModuleDef_var moduleDef =
        CORBA::ModuleDef::_narrow(repoContents[i]);
      CORBA::ContainedSeq_var moduleContents =
        moduleDef->contents(CORBA::dk_all,1);
      CORBA::String_var moduleId = moduleDef->id();
      CORBA::String_var moduleName = moduleDef->name();

      ACE_DEBUG((LM_INFO,
        ACE_TEXT("%s:\n// %s\nmodule %s\n{\n")
        ACE_TEXT("%s: the module contains %d elements.\n"),
        programLabel,
        moduleId.in(),
        moduleName.in(),
        programLabel,
        moduleContents->length()
        ));
      listContents(moduleContents.in());
      ACE_DEBUG((LM_INFO, ACE_TEXT("}\n")));
      break;
                           }
    default:
      break;
    }
  }
}

void listInterface (CORBA::InterfaceDef_ptr interfaceDef)
{
  CORBA::InterfaceDef::FullInterfaceDescription_var fullDescr =
    interfaceDef->describe_interface();

  char const * interfaceName =
    fullDescr->name;
  char const * interfaceRepoId =
    fullDescr->id;

  ACE_DEBUG((LM_INFO,
    ACE_TEXT("%s:\n\t// %s\n\tinterface %s\n\t{"),
    programLabel,
    interfaceRepoId,
    interfaceName
    ));


  unsigned int operationsCount;
  if ((operationsCount = fullDescr->operations.length()) > 0)
  {
    for(unsigned int i = 0; i < operationsCount; ++i)
    {
      listOperation(&(fullDescr->operations[i]));
    }
  }

  unsigned int attributesCount;
  if ((attributesCount = fullDescr->attributes.length()) > 0)
  {
    ACE_DEBUG((LM_INFO,
      ACE_TEXT("%s: %s has %d attribute(s).\n"),
      programLabel,
      interfaceName,
      attributesCount
      ));
  }

  ACE_DEBUG((LM_INFO, "\n\t}\n"));
}

void listOperation (CORBA::OperationDescription const * operationDescr)
{
  char const * operationName =
    operationDescr->name;
  char const * operationRepoId =
    operationDescr->id;
  char const * operationResult =
    decodeTypeCode(operationDescr->result.in());
  char const * operationMode =
    decodeOperationMode(operationDescr->mode);

  ACE_DEBUG((LM_INFO,
    ACE_TEXT("\n\t\t// %s \n\t\t%s %s %s"),
    operationRepoId,
    operationResult,
    operationName,
    operationMode
    ));

  CORBA::ParDescriptionSeq const * params = &(operationDescr->parameters);
  CORBA::ULong paramsCount = params->length();
  if (paramsCount > 0)
  {
    ACE_DEBUG((LM_INFO, "\n\t\t(\n\t\t"));
    for(CORBA::ULong i =0; i < paramsCount; ++i)
    {
      listParameter(&((*params)[i]));
      if(i < (paramsCount - 1))
      {
        ACE_DEBUG((LM_INFO, ",\n\t\t"));
      }
    }
    ACE_DEBUG((LM_INFO, "\n\t\t);\n"));
  }
  else
  {
    ACE_DEBUG((LM_INFO, "();\n"));
  }
}

void listParameter (CORBA::ParameterDescription const * parameterDescr)
{
  char const * typCode =
    decodeTypeCode(parameterDescr->type.in());
  char const * paramMode =
    decodeParameterMode(parameterDescr->mode);
  ACE_DEBUG((LM_INFO,
    ACE_TEXT("%s %s %s"),
    paramMode,
    typCode,
    parameterDescr->name.in()
    ));
}

char const * decodeTypeCode (CORBA::TypeCode_ptr typeCode)
{
  char const * code = "";
  if (typeCode->equivalent(CORBA::_tc_void)) {
    code = "void";
  } else if (typeCode->equivalent(CORBA::_tc_boolean)) {
    code = "boolean";
  } else if (typeCode->equivalent(CORBA::_tc_string)) {
    code = "string";
  }
  return code;
}

char const * decodeParameterMode (CORBA::ParameterMode mode)
{
  char const * paramMode;
  switch(mode)
  {
  case CORBA::PARAM_IN:
    {
      paramMode = "in";
      break;
    }
  case CORBA::PARAM_OUT:
    {
      paramMode = "out";
      break;
    }
  case CORBA::PARAM_INOUT:
    {
      paramMode = "inout";
      break;
    }
  default:
      paramMode = "";
  }
  return paramMode;
}

char const * decodeOperationMode (CORBA::OperationMode mode)
{
  return (mode == CORBA::OP_NORMAL) ? "synchronous" : "asynchronous";
}
