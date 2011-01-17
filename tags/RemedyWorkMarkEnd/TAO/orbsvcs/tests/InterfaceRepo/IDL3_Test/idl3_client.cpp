// -*- C++ -*-
// $Id$

#include "idl3_client.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"

// All the magic quantities are here at the top.

const char *COMPONENT_ID = "IDL:mod/test_component:1.0";
const char *COMPONENT_SCOPED_NAME = "::mod::test_component";
const char *COMP_BASE_ID = "IDL:help/c_base:1.0";

const char *VALUETYPE_ID = "IDL:mod/test_valuetype:1.0";
const char *VALUETYPE_SCOPED_NAME = "::mod::test_valuetype";
const char *VT_BASE_ID = "IDL:help/v_base:1.0";

const char *HOME_ID = "IDL:mod/test_home:1.0";
const char *HOME_SCOPED_NAME = "::mod::test_home";
const char *HOME_BASE_ID = "IDL:help/h_base:1.0";
const char *HOME_KEY_ID = "IDL:help/h_key:1.0";

const char *EVENTTYPE_ID = "IDL:mod/test_eventtype:1.0";

const CORBA::ULong ATTRS_LEN = 1;
const CORBA::ULong OPS_LEN = 1;
const CORBA::ULong FACTORY_LEN = 2;

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

const char *OP_NAMES[] =
  {
    "v_op"
  };

const CORBA::TCKind OP_RET_KINDS[] =
  {
    CORBA::tk_string
  };

const CORBA::ULong PARAMS_LEN[] =
  {
    3
  };

const char *PARAM_NAMES[] =
  {
    "inoutarg",
    "inarg",
    "outarg"
  };

const CORBA::ULong OP_EXCEP_LEN[] =
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

const CORBA::ULong MEM_LEN = 2;

const CORBA::Visibility MEM_VIS[] =
  {
    CORBA::PUBLIC_MEMBER,
    CORBA::PRIVATE_MEMBER
  };

const char *MEM_NAMES[] =
  {
    "test_mem1",
    "test_mem2",
  };

const CORBA::ULong VT_FACTORY_PARAM_LENS[] =
  {
    1,
    2
  };

const CORBA::ULong VT_FACTORY_EXCEP_LENS[] =
  {
    0,
    2
  };

const char *VT_FACTORY_PARAM_NAMES[][2] =
  {
    {"set_tm1", 0},
    {"set_tm1a", "set_tm2"}
  };

const char *VT_FACTORY_EXCEP_NAMES[][2] =
  {
    {0, 0},
    {"whups", "doh"}
  };

const CORBA::ULong HOME_SUPPORTED_LEN = 2;

const char *HOME_SUPPORTED_IDS[] =
  {
    "IDL:help/h_supp1:1.0",
    "IDL:help/h_supp2:1.0"
  };

const CORBA::ULong HOME_FACTORY_LEN = 1;
const CORBA::ULong HOME_FINDER_LEN = 1;

const CORBA::ULong HOME_FACTORY_PARAM_LENS[] =
  {
    1
  };

const char *HOME_FACTORY_PARAM_NAMES[][1] =
  {
    {"set_uid"}
  };

const CORBA::ULong HOME_FACTORY_EXCEP_LENS[] =
  {
    1
  };

const char *HOME_FACTORY_EXCEP_NAMES[][1] =
  {
    {"doh"}
  };

const CORBA::ULong HOME_FINDER_PARAM_LENS[] =
  {
    3
  };

const char *HOME_FINDER_PARAM_NAMES[][3] =
  {
    {"id_number", "id_string", "pkey"}
  };

const CORBA::ULong HOME_FINDER_EXCEP_LENS[] =
  {
    1
  };

const char *HOME_FINDER_EXCEP_NAMES[][1] =
  {
    {"whups"}
  };

IDL3_Client::IDL3_Client (void)
  : debug_ (false)
{
}

IDL3_Client::~IDL3_Client (void)
{
}

int
IDL3_Client::init (int argc,
                   ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("InterfaceRepository");

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "IDL3_Client - IFR resolution failed\n"),
                        -1);
    }

  this->repo_ =
    CORBA::ComponentIR::Repository::_narrow (obj.in ());

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
IDL3_Client::run (void)
{
  int status = this->component_test ();

  if (status != 0)
    {
      return status;
    }

  status = this->valuetype_test (VALUETYPE_ID,
                                 "value");

  if (status != 0)
    {
      return status;
    }

  status = this->home_test ();

  if (status != 0)
    {
      return status;
    }

  status = this->valuetype_test (EVENTTYPE_ID,
                                 "event");

  if (status != 0)
    {
      return status;
    }

  return 0;
}

int
IDL3_Client::parse_args (int argc,
                         ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("d"));
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'd':   // Turn on debugging outoput.
        this->debug_ = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s [-d]\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
IDL3_Client::component_test (void)
{
  CORBA::Contained_var result =
    this->repo_->lookup_id (COMPONENT_ID);

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
    result->absolute_name ();

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
    CORBA::ComponentIR::ComponentDef::_narrow (result.in ());

  CORBA::TypeCode_var comp_tc =
    comp_def->type ();

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

  tmp = comp_tc->id ();

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
    comp_def->describe_ext_interface ();

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

  int status = this->component_attribute_test (desc);

  if (status != 0)
    {
      return -1;
    }

  status = this->component_inheritance_test (comp_def);

  if (status != 0)
    {
      return -1;
    }

  status = this->component_port_test (comp_def);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::home_test (void)
{
  CORBA::Contained_var result =
    this->repo_->lookup_id (HOME_ID);

  if (CORBA::is_nil (result.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: lookup by id failed\n"));
        }

      return -1;
    }

  CORBA::ComponentIR::HomeDef_var home =
    CORBA::ComponentIR::HomeDef::_narrow (result.in ());

  if (CORBA::is_nil (result.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: home narrow failed\n"));
        }

      return -1;
    }

  CORBA::ComponentIR::ComponentDef_var managed =
    home->managed_component ();

  if (CORBA::is_nil (managed.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: managed component is null\n"));
        }

      return -1;
    }

  CORBA::String_var str = managed->id ();

  if (str.in () == 0 || ACE_OS::strcmp (str.in (), COMPONENT_ID) != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: bad id for managed component\n"));
        }

      return -1;
    }

  CORBA::ValueDef_var pkey =
    home->primary_key ();

  if (CORBA::is_nil (pkey.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: primary key is null\n"));
        }

      return -1;
    }

  str = pkey->id ();

  if (str.in () == 0 || ACE_OS::strcmp (str.in (), HOME_KEY_ID) != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: bad id for managed component\n"));
        }

      return -1;
    }

  int status = this->home_inheritance_test (home);

  if (status != 0)
    {
      return -1;
    }

  CORBA::Contained::Description_var desc =
    home->describe ();

  CORBA::ComponentIR::HomeDescription *home_desc = 0;

  if ((desc->value >>= home_desc) == 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_test: extraction of HomeDescription failed\n"));
        }

      return -1;
    }

  status = this->home_factory_test (home_desc);

  if (status != 0)
    {
      return -1;
    }

  status = this->home_finder_test (home_desc);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::valuetype_test (const char *repo_id,
                             const char *prefix)
{
  CORBA::Contained_var result =
    this->repo_->lookup_id (repo_id);

  if (CORBA::is_nil (result.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_test: lookup by id failed\n",
                      prefix));
        }

      return -1;
    }

  CORBA::ExtValueDef_var evd =
    CORBA::ExtValueDef::_narrow (result.in ());

  if (CORBA::is_nil (evd.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_test: narrow to ExtValueDef failed\n",
                      prefix));
        }

      return -1;
    }

  CORBA::ExtValueDef::ExtFullValueDescription_var desc =
    evd->describe_ext_value ();

  int status = this->valuetype_attribute_test (desc,
                                               prefix);

  if (status != 0)
    {
      return -1;
    }

  status = this->valuetype_inheritance_test (evd,
                                             prefix);

  if (status != 0)
    {
      return -1;
    }

  status = this->valuetype_operation_test (desc,
                                           prefix);

  if (status != 0)
    {
      return -1;
    }

  status = this->valuetype_member_test (desc,
                                        prefix);

  if (status != 0)
    {
      return -1;
    }

  status = this->valuetype_factory_test (desc,
                                         prefix);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::component_attribute_test (
    CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var &desc
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
        desc->attributes[i].type->kind ();

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
  )
{
  CORBA::ComponentIR::ComponentDef_var comp_base =
    comp_def->base_component ();

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

  CORBA::String_var str = comp_base->id ();

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

  CORBA::InterfaceDefSeq_var supported =
    comp_base->supported_interfaces ();

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

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      str = supported[i]->id ();

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

  return 0;
}

int
IDL3_Client::component_port_test (
    CORBA::ComponentIR::ComponentDef_var &comp_def
  )
{
  CORBA::Contained::Description_var desc =
    comp_def->describe ();

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

  int status = this->provides_test (cd->provided_interfaces);

  if (status != 0)
    {
      return -1;
    }

  status = this->uses_test (cd->used_interfaces);

  if (status != 0)
    {
      return -1;
    }

  status = this->event_port_test (cd->emits_events,
                                  EMITS_LEN,
                                  "emits",
                                  EMITS_NAMES,
                                  EMITS_IDS);

  if (status != 0)
    {
      return -1;
    }

  status = this->event_port_test (cd->publishes_events,
                                  PUBLISHES_LEN,
                                  "publishes",
                                  PUBLISHES_NAMES,
                                  PUBLISHES_IDS);

  if (status != 0)
    {
      return -1;
    }

  status = this->event_port_test (cd->consumes_events,
                                  CONSUMES_LEN,
                                  "consumes",
                                  CONSUMES_NAMES,
                                  CONSUMES_IDS);

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
IDL3_Client::provides_test (CORBA::ComponentIR::ProvidesDescriptionSeq &pds)
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
IDL3_Client::uses_test (CORBA::ComponentIR::UsesDescriptionSeq &uds)
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
                              const char **ids)
{
  if (eds.length () != seq_length)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "event_port_test: "
                      "wrong number of event %C ports\n",
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
                          "wrong local name for %C port #%d\n",
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
                          "wrong base event type id for %C port #%d\n",
                          port_type,
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_inheritance_test (CORBA::ExtValueDef_var &vd,
                                         const char *prefix)
{
  CORBA::ValueDef_var bvd = vd->base_value ();

  if (CORBA::is_nil (bvd.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_inheritance_test: "
                      "base valuetype is null\n",
                      prefix));
        }

      return -1;
    }

  CORBA::String_var str = bvd->id ();

  if (str.in () == 0 || ACE_OS::strcmp (str.in (), VT_BASE_ID) != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_inheritance_test: "
                      "wrong repo id for base valuetype\n",
                      prefix));
        }

      return -1;
    }

  CORBA::InterfaceDefSeq_var supported =
    vd->supported_interfaces ();

  CORBA::ULong length = supported->length ();

  if (length != VT_SUPPORTED_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_inheritance_test: "
                      "wrong number of supported interfaces\n",
                      prefix));
        }

      return -1;
    }

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      str = supported[i]->id ();

      if (str.in () == 0
          || ACE_OS::strcmp (str.in (), VT_SUPPORTED_IDS[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_inheritance_test: "
                          "bad id on supported interface #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_attribute_test (
    CORBA::ExtValueDef::ExtFullValueDescription_var &desc,
    const char *prefix
  )
{
  if (desc->attributes.length () != ATTRS_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_attribute_test: wrong number of attrs\n",
                      prefix));
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
                          "%Ctype_attribute_test: "
                          "wrong local name for attribute #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      CORBA::TCKind kind =
        desc->attributes[i].type->kind ();

      if (kind != ATTR_TC_KINDS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_attribute_test: "
                          "wrong TCKind for attribute #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      if (desc->attributes[i].get_exceptions.length () != GET_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_attribute_test: "
                          "wrong number of get-exceptions"
                          " for attribute #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      if (desc->attributes[i].put_exceptions.length () != PUT_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_attribute_test: "
                          "wrong number of put-exceptions"
                          " for attribute #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_operation_test (
    CORBA::ExtValueDef::ExtFullValueDescription_var &desc,
    const char *prefix
  )
{
  CORBA::ULong ops_length = desc->operations.length ();

  if (ops_length != OPS_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_operation_test: "
                      "wrong number of operations\n",
                      prefix));
        }

      return -1;
    }

  const char *tmp = 0;
  CORBA::ULong j = 0;
  CORBA::ULong length = 0;

  for (CORBA::ULong i = 0; i < ops_length; ++i)
    {
      tmp = desc->operations[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, OP_NAMES[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_operation_test: "
                          "wrong name for operation #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      CORBA::TCKind ret_kind =
        desc->operations[i].result.in ()->kind ();

      if (ret_kind != OP_RET_KINDS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_operation_test: "
                          "wrong return type\n",
                          prefix));
            }

          return -1;
        }

      length = desc->operations[i].parameters.length ();

      if (length != PARAMS_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_operation_test: "
                          "wrong number of parameters in operation #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      for (j = 0; j < length; ++j)
        {
          tmp = desc->operations[i].parameters[j].name.in ();

          if (tmp == 0 || ACE_OS::strcmp (tmp, PARAM_NAMES[j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "%Ctype_operation_test: "
                              "wrong name for operation #%d,"
                              "parameter #%d\n",
                              prefix,
                              i + 1,
                              j + 1));
                }

              return -1;
            }
        }

      length = desc->operations[i].exceptions.length ();

      if (length != OP_EXCEP_LEN[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_operation_test: "
                          "wrong number of exceptions in operation #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_member_test (
    CORBA::ExtValueDef::ExtFullValueDescription_var &desc,
    const char *prefix
  )
{
  CORBA::ULong length = desc->members.length ();

  if (length != MEM_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_member_test: "
                      "wrong number of members\n",
                      prefix));
        }

      return -1;
    }

  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      if (desc->members[i].access != MEM_VIS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_member_test: "
                          "wrong access value in member #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      tmp = desc->members[i].name.in ();

      if (tmp == 0 || ACE_OS::strcmp (tmp, MEM_NAMES[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_member_test: "
                          "wrong repo id for member #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::valuetype_factory_test (
    CORBA::ExtValueDef::ExtFullValueDescription_var &desc,
    const char *prefix
  )
{
  CORBA::ULong length = desc->initializers.length ();

  if (length != FACTORY_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%Ctype_factory_test: "
                      "wrong number of factories\n",
                      prefix));
        }

      return -1;
    }

  CORBA::ULong inside_len = 0;
  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < FACTORY_LEN; ++i)
    {
      inside_len = desc->initializers[i].members.length ();

      if (inside_len != VT_FACTORY_PARAM_LENS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_factory_test: "
                          "wrong number of params in factory #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      CORBA::ULong j = 0;

      for (j = 0; j < VT_FACTORY_PARAM_LENS[i]; ++j)
        {
          tmp = desc->initializers[i].members[j].name.in ();

          if (tmp == 0
              || ACE_OS::strcmp (tmp, VT_FACTORY_PARAM_NAMES[i][j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "%Ctype_factory_test: "
                              "wrong name for arg #%d in factory #%d\n",
                              prefix,
                              j + 1,
                              i + 1));
                }

              return -1;
            }
        }

      inside_len = desc->initializers[i].exceptions.length ();

      if (inside_len != VT_FACTORY_EXCEP_LENS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%Ctype_factory_test: "
                          "wrong number of exceptions in factory #%d\n",
                          prefix,
                          i + 1));
            }

          return -1;
        }

      for (j = 0; j < VT_FACTORY_EXCEP_LENS[i]; ++j)
        {
          tmp = desc->initializers[i].exceptions[j].name.in ();

          if (tmp == 0
              || ACE_OS::strcmp (tmp, VT_FACTORY_EXCEP_NAMES[i][j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "%Ctype_factory_test: "
                              "wrong name for exception #%d in factory #%d\n",
                              prefix,
                              j + 1,
                              i + 1));
                }

              return -1;
            }
        }
    }

  return 0;
}

int
IDL3_Client::home_inheritance_test (CORBA::ComponentIR::HomeDef_var &hd)
{
  CORBA::ComponentIR::HomeDef_var bhd =
    hd->base_home ();

  if (CORBA::is_nil (bhd.in ()))
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_inheritance_test: "
                      "base home is null\n"));
        }

      return -1;
    }

  CORBA::String_var str = bhd->id ();

  if (str.in () == 0 || ACE_OS::strcmp (str.in (), HOME_BASE_ID) != 0)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_inheritance_test: "
                      "wrong repo id for base home\n"));
        }

      return -1;
    }

  CORBA::InterfaceDefSeq_var supported =
    bhd->supported_interfaces ();

  CORBA::ULong length = supported->length ();

  if (length != HOME_SUPPORTED_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_inheritance_test: "
                      "wrong number of supported interfaces\n"));
        }

      return -1;
    }

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      str = supported[i]->id ();

      if (str.in () == 0
          || ACE_OS::strcmp (str.in (), HOME_SUPPORTED_IDS[i]) != 0)
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "home_inheritance_test: "
                          "bad id on supported interface #%d\n",
                          i + 1));
            }

          return -1;
        }
    }

  return 0;
}

int
IDL3_Client::home_factory_test (CORBA::ComponentIR::HomeDescription *hd)
{
  CORBA::ULong length = hd->factories.length ();

  if (length != HOME_FACTORY_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_factory_test: "
                      "wrong number of factories\n"));
        }

      return -1;
    }

  CORBA::ULong inside_len = 0;
  CORBA::ULong j = 0;
  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < HOME_FACTORY_LEN; ++i)
    {
      inside_len = hd->factories[i].parameters.length ();

      if (inside_len != HOME_FACTORY_PARAM_LENS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "home_factory_test: "
                          "wrong number of params in factory #%d\n",
                          i + 1));
            }

          return -1;
        }

      for (j = 0; j < inside_len; ++j)
        {
          tmp = hd->factories[i].parameters[j].name.in ();

          if (tmp == 0
              || ACE_OS::strcmp (tmp, HOME_FACTORY_PARAM_NAMES[i][j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "home_factory_test: "
                              "wrong name for param #%d in factory #%d\n",
                              j + 1,
                              i + 1));
                }

              return -1;
            }
        }

      inside_len = hd->factories[i].exceptions.length ();

      if (inside_len != HOME_FACTORY_EXCEP_LENS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "home_factory_test: "
                          "wrong number of exceptions in factory #%d\n",
                          i + 1));
            }

          return -1;
        }

      for (j = 0; j < inside_len; ++j)
        {
          tmp = hd->factories[i].exceptions[j].name.in ();

          if (tmp == 0
              || ACE_OS::strcmp (tmp, HOME_FACTORY_EXCEP_NAMES[i][j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "home_factory_test: "
                              "wrong name for exception #%d in factory #%d\n",
                              j + 1,
                              i + 1));
                }

              return -1;
            }
        }
    }

  return 0;
}

int
IDL3_Client::home_finder_test (CORBA::ComponentIR::HomeDescription *hd)
{
  CORBA::ULong length = hd->finders.length ();

  if (length != HOME_FINDER_LEN)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "home_finder_test: "
                      "wrong number of finders\n"));
        }

      return -1;
    }

  CORBA::ULong inside_len = 0;
  CORBA::ULong j = 0;
  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < HOME_FINDER_LEN; ++i)
    {
      inside_len = hd->finders[i].parameters.length ();

      if (inside_len != HOME_FINDER_PARAM_LENS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "home_finder_test: "
                          "wrong number of params in finder #%d\n",
                          i + 1));
            }

          return -1;
        }

      for (j = 0; j < inside_len; ++j)
        {
          tmp = hd->finders[i].parameters[j].name.in ();

          if (tmp == 0
              || ACE_OS::strcmp (tmp, HOME_FINDER_PARAM_NAMES[i][j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "home_finder_test: "
                              "wrong name for param #%d in finder #%d\n",
                              j + 1,
                              i + 1));
                }

              return -1;
            }
        }

      inside_len = hd->finders[i].exceptions.length ();

      if (inside_len != HOME_FINDER_EXCEP_LENS[i])
        {
          if (this->debug_)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "home_finder_test: "
                          "wrong number of exceptions in finder #%d\n",
                          i + 1));
            }

          return -1;
        }

      for (j = 0; j < inside_len; ++j)
        {
          tmp = hd->finders[i].exceptions[j].name.in ();

          if (tmp == 0
              || ACE_OS::strcmp (tmp, HOME_FINDER_EXCEP_NAMES[i][j]) != 0)
            {
              if (this->debug_)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "home_finder_test: "
                              "wrong name for exception #%d in finder #%d\n",
                              j + 1,
                              i + 1));
                }

              return -1;
            }
        }
    }

  return 0;
}

