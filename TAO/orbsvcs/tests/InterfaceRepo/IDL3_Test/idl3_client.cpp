// -*- C++ -*-
// $Id$

#include "idl3_client.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Application_Test, 
           ifr_dii_client, 
           "$Id$")

// All the magic quantities are here at the top.

const char *COMPONENT_ID = "IDL:mod/test_component:1.0";
const char *COMPONENT_SCOPED_NAME = "::mod::test_component";
const char *COMP_BASE_ID = "IDL:help/c_base:1.0";

const char *VALUETYPE_ID = "IDL:mod/test_valuetype:1.0";
const char *VALUETYPE_SCOPED_NAME = "::mod::test_valuetype";
const char *VT_BASE_ID = "IDL:help/v_base:1.0";

const CORBA::ULong ATTRS_LEN = 1;

const char *ATTR_LOCAL_NAMES[] = 
  {
    "c_attr1"
  };

const CORBA::TCKind ATTR_TC_KINDS[] =
  {
    CORBA::tk_long
  };

const CORBA::ULong GET_EXCEP_LEN[] =
  {
    1
  };

const CORBA::ULong PUT_EXCEP_LEN[] =
  {
    2
  };

const CORBA::ULong COMP_SUPPORTED_LEN = 2;

const char *COMP_SUPPORTED_IDS[] = 
  {
    "IDL:help/c_supp1:1.0",
    "IDL:help/c_supp2:1.0"
  };

const CORBA::ULong PROVIDES_LEN = 1;
const CORBA::ULong USES_LEN = 2;
const CORBA::ULong EMITS_LEN = 1;
const CORBA::ULong PUBLISHES_LEN = 1;
const CORBA::ULong CONSUMES_LEN = 1;

const char *PROVIDES_NAMES[] =
  {
    "test_provides1"
  };

const char *PROVIDES_TYPE_IDS[] =
  {
    "IDL:help/c_provides1:1.0"
  };

const char *USES_NAMES[] =
  {
    "test_uses1",
    "test_uses2"
  };

const char *USES_TYPE_IDS[] =
  {
    "IDL:help/c_uses1:1.0",
    "IDL:help/c_uses2:1.0"
  };

const CORBA::Boolean USES_MULTIPLE_FLAGS[] =
  {
    0,
    1
  };

const char *EMITS_NAMES[] =
  {
    "test_emits1"
  };

const char *PUBLISHES_NAMES[] =
  {
    "test_publishes1"
  };

const char *CONSUMES_NAMES[] =
  {
    "test_consumes1"
  };

const char *EMITS_IDS[] =
  {
    "IDL:help/c_emits1:1.0"
  };

const char *PUBLISHES_IDS[] =
  {
    "IDL:help/c_publishes1:1.0"
  };

const char *CONSUMES_IDS[] =
  {
    "IDL:help/c_consumes1:1.0"
  };

const CORBA::ULong VT_SUPPORTED_LEN = 2;

const char *VT_SUPPORTED_IDS[] = 
  {
    "IDL:help/v_supp1:1.0",
    "IDL:help/v_supp2:1.0"
  };

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

  status = this->valuetype_test (ACE_ENV_SINGLE_ARG_PARAMETER);
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
    this->repo_->lookup_id (COMPONENT_ID
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

  if (ACE_OS::strcmp (tmp, COMPONENT_SCOPED_NAME) != 0)
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

  if (tmp == 0 || ACE_OS::strcmp (tmp, COMPONENT_ID) != 0)
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

  int status = this->component_attribute_test (desc
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

  status = this->component_port_test (comp_def
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
  CORBA::Contained_var result =
    this->repo_->lookup_id (VALUETYPE_ID
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (result.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "valuetype_test: lookup by id failed\n"));
        }

      return -1;
    }

  CORBA::ExtValueDef_var evd = 
    CORBA::ExtValueDef::_narrow (result.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (evd.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "valuetype_test: narrow to ExtValueDef failed\n"));
        }

      return -1;
    }

  CORBA::ExtValueDef::ExtFullValueDescription_var desc =
    evd->describe_ext_value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  int status = this->valuetype_attribute_test (desc
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  status = this->valuetype_inheritance_test (evd
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::eventtype_test (ACE_ENV_SINGLE_ARG_DECL)
{
  return 0;
}

int
IDL3_Client::component_attribute_test (
    CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var &desc
    ACE_ENV_ARG_DECL
  )
{
  if (desc->attributes.length () != ATTRS_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_attribute_test: wrong number of attrs\n"));
        }

      return -1;
    }

  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < ATTRS_LEN; ++i)
    {
      tmp = desc->attributes[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, ATTR_LOCAL_NAMES[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "component_attribute_test: "
                          "wrong local name for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }

      CORBA::TCKind kind = 
        desc->attributes[i].type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (kind != ATTR_TC_KINDS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "component_attribute_test: "
                          "wrong TCKind for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }

      if (desc->attributes[i].get_exceptions.length () != GET_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "component_attribute_test: "
                          "wrong number of get-exceptions"
                          " for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }

      if (desc->attributes[i].put_exceptions.length () != PUT_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "component_attribute_test: "
                          "wrong number of put-exceptions"
                          " for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }
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

  if (length != COMP_SUPPORTED_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_inheritance_test: "
                      "wrong number of supported interfaces\n"));
        }

      return -1;
    }

  CORBA::String_var str;
  
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      str = supported[i].in ()->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (str.in () == 0 
          || ACE_OS::strcmp (str.in (), COMP_SUPPORTED_IDS[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "component_inheritance_test: "
                          "bad id on supported interface #%d\n",
                          i + 1));
            }

          return -1;
        }
    }

  CORBA::ComponentIR::ComponentDef_var comp_base =
    comp_def->base_component (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (comp_base.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_inheritance_test: "
                      "base component is null\n"));
        }

      return -1;
    }

  str = comp_base->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (str.in () == 0 || ACE_OS::strcmp (str.in (), COMP_BASE_ID) != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_inheritance_test: "
                      "bad id on base component\n"));
        }

      return -1;
    }

  return 0;
}

int
IDL3_Client::component_port_test (
    CORBA::ComponentIR::ComponentDef_var &comp_def
    ACE_ENV_ARG_DECL
  )
{
  CORBA::Contained::Description_var desc =
    comp_def->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::ComponentIR::ComponentDescription *cd = 0;

  if ((desc->value >>= cd) == 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "component_port_test: "
                      "Any extraction of component description failed\n"));
        }

      return -1;
    }

  int status = this->provides_test (cd->provided_interfaces
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  status = this->uses_test (cd->used_interfaces
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  status = this->event_port_test (cd->emits_events,
                                  EMITS_LEN,
                                  "emits",
                                  EMITS_NAMES,
                                  EMITS_IDS
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  status = this->event_port_test (cd->publishes_events,
                                  PUBLISHES_LEN,
                                  "publishes",
                                  PUBLISHES_NAMES,
                                  PUBLISHES_IDS
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  status = this->event_port_test (cd->consumes_events,
                                  CONSUMES_LEN,
                                  "consumes",
                                  CONSUMES_NAMES,
                                  CONSUMES_IDS
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::provides_test (CORBA::ComponentIR::ProvidesDescriptionSeq &pds
                            ACE_ENV_ARG_DECL)
{
  if (pds.length () != PROVIDES_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "provides_test: "
                      "wrong number of provides interfaces\n"));
        }

      return -1;
    }

  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < PROVIDES_LEN; ++i)
    {
      tmp = pds[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, PROVIDES_NAMES[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "provides_test: "
                          "wrong local name for provides #%d\n",
                          i + 1));
            }

          return -1;
        }

      tmp = pds[i].interface_type.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, PROVIDES_TYPE_IDS[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "provides_test: "
                          "wrong base interface type id for provides #%d\n",
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::uses_test (CORBA::ComponentIR::UsesDescriptionSeq &uds
                        ACE_ENV_ARG_DECL)
{
  if (uds.length () != USES_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "uses_test: "
                      "wrong number of uses interfaces\n"));
        }

      return -1;
    }

  const char *tmp = 0;
  CORBA::Boolean mult = 0;

  for (CORBA::ULong i = 0; i < USES_LEN; ++i)
    {
      tmp = uds[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, USES_NAMES[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "uses_test: "
                          "wrong local name for uses #%d\n",
                          i + 1));
            }

          return -1;
        }

      tmp = uds[i].interface_type.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, USES_TYPE_IDS[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "uses_test: "
                          "wrong base interface type id for uses #%d\n",
                          i + 1));
            }

          return -1;
        }

      mult = uds[i].is_multiple;

      if (mult != USES_MULTIPLE_FLAGS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "uses_test: "
                          "wrong is_multiple value for uses #%d\n",
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::event_port_test (CORBA::ComponentIR::EventPortDescriptionSeq &eds,
                              CORBA::ULong seq_length,
                              const char *port_type,
                              const char **names,
                              const char **ids
                              ACE_ENV_ARG_DECL)
{
  if (eds.length () != seq_length)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "event_port_test: "
                      "wrong number of event %s ports\n",
                      port_type));
        }

      return -1;
    }

  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < seq_length; ++i)
    {
      tmp = eds[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, names[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "event_port_test: "
                          "wrong local name for %s port #%d\n",
                          port_type,
                          i + 1));
            }

          return -1;
        }

      tmp = eds[i].event.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, ids[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "event_port_test: "
                          "wrong base event type id for %s port #%d\n",
                          port_type,
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_inheritance_test (CORBA::ExtValueDef_var &vd
                                         ACE_ENV_ARG_DECL)
{
  CORBA::ValueDef_var bvd = vd->base_value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (bvd.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "valuetype_inheritance_test: "
                      "base valuetype is null\n"));
        }

      return -1;
    }

  CORBA::String_var str = bvd->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (str.in () ==0 || ACE_OS::strcmp (str.in (), VT_BASE_ID) != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "valuetype_inheritance_test: "
                      "wrong repo id for base valuetype\n"));
        }

      return -1;
    }

  CORBA::InterfaceDefSeq_var supported = 
    vd->supported_interfaces (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::ULong length = supported->length ();

  if (length != VT_SUPPORTED_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "valuetype_inheritance_test: "
                      "wrong number of supported interfaces\n"));
        }

      return -1;
    }

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      str = supported[i].in ()->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (str.in () == 0 
          || ACE_OS::strcmp (str.in (), VT_SUPPORTED_IDS[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "valuetype_inheritance_test: "
                          "bad id on supported interface #%d\n",
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_attribute_test (
    CORBA::ExtValueDef::ExtFullValueDescription_var &desc
    ACE_ENV_ARG_DECL
  )
{
  if (desc->attributes.length () != ATTRS_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "valuetype_attribute_test: wrong number of attrs\n"));
        }

      return -1;
    }

  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < ATTRS_LEN; ++i)
    {
      tmp = desc->attributes[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, ATTR_LOCAL_NAMES[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "valuetype_attribute_test: "
                          "wrong local name for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }

      CORBA::TCKind kind = 
        desc->attributes[i].type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (kind != ATTR_TC_KINDS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "valuetype_attribute_test: "
                          "wrong TCKind for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }

      if (desc->attributes[i].get_exceptions.length () != GET_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "valuetype_attribute_test: "
                          "wrong number of get-exceptions"
                          " for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }

      if (desc->attributes[i].put_exceptions.length () != PUT_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "valuetype_attribute_test: "
                          "wrong number of put-exceptions"
                          " for attribute #%d\n",
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

