// -*- C++ -*-
// $Id$

#include "idl3_client.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Application_Test, 
           ifr_dii_client, 
           "$Id$")

IDL3_Client::IDL3_Client (void)
  : debug_ (0)
{
}

IDL3_Client::~IDL3_Client (void)
{
}

int
IDL3_Client::init (int argc,
                   char *argv[]
                   ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("InterfaceRepository"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "IDL3_Client - IFR resolution failed\n"),
                        -1);
    }

  this->repo_ = 
    CORBA::ComponentIR::Repository::_narrow (obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (this->repo_.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "IDL3_Client - IFR narrow failed\n"),
                        -1);
    }

  if (this->parse_args (argc, argv) == -1)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int status = this->component_test (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return status;
    }

  return 0;
}

int
IDL3_Client::parse_args (int argc,
                         char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "d");
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'd':   // Turn on debugging outoput.
        this->debug_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s"
                           " [-d]"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
IDL3_Client::component_test (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Contained_var result =
    this->repo_->lookup_id ("IDL:mod/test_component:1.0"
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (result.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: lookup by id failed\n"));
        }

      return -1;
    }

  CORBA::String_var str = 
    result->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  const char *tmp = str.in ();

  if (tmp == 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: component has null scoped name\n"));
        }

      return -1;
    }

  if (ACE_OS::strcmp (tmp, "::mod::test_component") != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "component has incorrect scoped name\n"));
        }

      return -1;
    }

  CORBA::ComponentIR::ComponentDef_var comp_def =
    CORBA::ComponentIR::ComponentDef::_narrow (result.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::TypeCode_var comp_tc = 
    comp_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (comp_tc.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "type code creation failed\n"));
        }

      return -1;
    }

  tmp = comp_tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (tmp == 0 || ACE_OS::strcmp (tmp, "IDL:mod/test_component:1.0") != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "bad id from type code\n"));
        }

      return -1;
    }

  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var desc =
    comp_def->describe_ext_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (desc.ptr () == 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "describe_ext_interface return null\n"));
        }

      return -1;
    }

  int status = this->component_attr_test (desc
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  status = this->component_inheritance_test (comp_def
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::home_test (ACE_ENV_SINGLE_ARG_DECL)
{
  return 0;
}

int
IDL3_Client::valuetype_test (ACE_ENV_SINGLE_ARG_DECL)
{
  return 0;
}

int
IDL3_Client::eventtype_test (ACE_ENV_SINGLE_ARG_DECL)
{
  return 0;
}

int
IDL3_Client::component_attr_test (
    CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var &desc
    ACE_ENV_ARG_DECL
  )
{
  const char *tmp = desc->attributes[0].name.in ();

  if (tmp == 0 || ACE_OS::strcmp (tmp, "c_attr") != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: bad attribute name\n"));
        }

      return -1;
    }

  CORBA::TCKind kind = 
    desc->attributes[0].type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (kind != CORBA::tk_long)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "bad attribute type code kind\n"));
        }

      return -1;
    }

  if (desc->attributes[0].get_exceptions.length () != 1)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "wrong number of attribute get-exceptions\n"));
        }

      return -1;
    }

  if (desc->attributes[0].put_exceptions.length () != 2)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "wrong number of attribute put-exceptions\n"));
        }

      return -1;
    }

  return 0;
}

int
IDL3_Client::component_inheritance_test (
    CORBA::ComponentIR::ComponentDef_var &comp_def
    ACE_ENV_ARG_DECL
  )
{
  CORBA::InterfaceDefSeq_var supported =
    comp_def->supported_interfaces (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::ULong length = supported->length ();

  if (length != 2)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "wrong number of supported interfaces\n"));
        }

      return -1;
    }

  CORBA::String_var str = supported[1U].in()->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  const char *tmp = str.in ();

  if (tmp == 0 || ACE_OS::strcmp (tmp, "IDL:help/c_supp2:1.0") != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "bad id on supported interface\n"));
        }

      return -1;
    }

  CORBA::ComponentIR::ComponentDef_var comp_base =
    comp_def->base_component (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (comp_base.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "base component is null\n"));
        }

      return -1;
    }

  str = comp_base->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  tmp = str.in ();

  if (tmp == 0 || ACE_OS::strcmp (tmp, "IDL:help/c_base:1.0") != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_test: "
                      "bad id on base component\n"));
        }

      return -1;
    }

  return 0;
}

