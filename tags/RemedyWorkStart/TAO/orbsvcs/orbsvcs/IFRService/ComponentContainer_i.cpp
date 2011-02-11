/* -*- C++ -*- */
// $Id$

#include "orbsvcs/IFRService/ComponentContainer_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/InterfaceDef_i.h"
#include "orbsvcs/IFRService/ComponentDef_i.h"
#include "orbsvcs/IFRService/ValueDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char *TAO_ComponentContainer_i::tmp_name_holder_ = 0;

TAO_ComponentContainer_i::TAO_ComponentContainer_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo)
{
}

TAO_ComponentContainer_i::~TAO_ComponentContainer_i (void)
{
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentContainer_i::create_component (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::ComponentDef_ptr base_component,
    const CORBA::InterfaceDefSeq &supports_interfaces
  )
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  this->update_key ();

  return this->create_component_i (id,
                                   name,
                                   version,
                                   base_component,
                                   supports_interfaces);
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentContainer_i::create_component_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::ComponentDef_ptr base_component,
    const CORBA::InterfaceDefSeq &supports_interfaces
  )
{
  TAO_ComponentContainer_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path =
    TAO_IFR_Service_Utils::create_common (
        this->def_kind (),
        CORBA::dk_Component,
        this->section_key_,
        new_key,
        this->repo_,
        id,
        name,
        &TAO_ComponentContainer_i::same_as_tmp_name,
        version,
        "defns"
      );

  if (! CORBA::is_nil (base_component))
    {
      const char *base_path =
        TAO_IFR_Service_Utils::reference_to_path (base_component);

      // Get the servant's key into the temporary key holder, because
      // the name clash checker for base valuetypes is static, and has
      // no other way to know about a specific key.
      this->repo_->config ()->expand_path (
                                  this->repo_->root_key (),
                                  base_path,
                                  TAO_IFR_Service_Utils::tmp_key_,
                                  0
                                );
      TAO_IFR_Service_Utils::name_exists (&TAO_ComponentDef_i::name_clash,
                                          new_key,
                                          this->repo_,
                                          CORBA::dk_Component);

      // Store the id for this - that's what ComponentDescription takes.
//      ACE_TString base_id;
//      this->repo_->config ()->get_string_value (TAO_IFR_Service_Utils::tmp_key_,
//                                                "id",
//                                                base_id);
      this->repo_->config ()->set_string_value (new_key,
                                                "base_component",
                                                base_path);
    }

  CORBA::ULong count = supports_interfaces.length ();
  CORBA::ULong i = 0;

  if (count > 0)
    {
      ACE_Configuration_Section_Key supports_key;
      this->repo_->config ()->open_section (new_key,
                                            "supported",
                                            1,
                                            supports_key);

      this->repo_->config ()->set_integer_value (supports_key,
                                                 "count",
                                                 count);

      const char *supported_path = 0;
      char *stringified = 0;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          supported_path =
            TAO_IFR_Service_Utils::reference_to_path (
                supports_interfaces[i]
              );
          this->repo_->config ()->set_string_value (supports_key,
                                                    stringified,
                                                    supported_path);
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Component,
                                          path.c_str (),
                                          this->repo_);

  return CORBA::ComponentIR::ComponentDef::_narrow (obj.in ());
}

CORBA::ComponentIR::HomeDef_ptr
TAO_ComponentContainer_i::create_home (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::HomeDef_ptr base_home,
    CORBA::ComponentIR::ComponentDef_ptr managed_component,
    const CORBA::InterfaceDefSeq &supports_interfaces,
    CORBA::ValueDef_ptr primary_key
  )
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::HomeDef::_nil ());

  this->update_key ();

  return this->create_home_i (id,
                              name,
                              version,
                              base_home,
                              managed_component,
                              supports_interfaces,
                              primary_key);
}

CORBA::ComponentIR::HomeDef_ptr
TAO_ComponentContainer_i::create_home_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::ComponentIR::HomeDef_ptr base_home,
    CORBA::ComponentIR::ComponentDef_ptr managed_component,
    const CORBA::InterfaceDefSeq &supports_interfaces,
    CORBA::ValueDef_ptr primary_key
  )
{
  TAO_ComponentContainer_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path =
    TAO_IFR_Service_Utils::create_common (
        this->def_kind (),
        CORBA::dk_Home,
        this->section_key_,
        new_key,
        this->repo_,
        id,
        name,
        &TAO_ComponentContainer_i::same_as_tmp_name,
        version,
        "defns"
      );

  char *base_path = 0;

  if (! CORBA::is_nil (base_home))
    {
      base_path = TAO_IFR_Service_Utils::reference_to_path (base_home);
      this->repo_->config ()->set_string_value (new_key,
                                                "base_home",
                                                base_path);
    }

  if (! CORBA::is_nil (managed_component))
    {
      base_path =
        TAO_IFR_Service_Utils::reference_to_path (managed_component);
      this->repo_->config ()->set_string_value (new_key,
                                                "managed",
                                                base_path);
    }

  CORBA::ULong length = supports_interfaces.length ();
  CORBA::ULong i = 0;

  if (length > 0)
    {
      ACE_Configuration_Section_Key supports_key;
      this->repo_->config ()->open_section (new_key,
                                            "supported",
                                            1,
                                            supports_key);
      this->repo_->config ()->set_integer_value (supports_key,
                                                 "count",
                                                 length);

      char *supported_path = 0;
      char *stringified = 0;

      for (i = 0; i < length; ++i)
        {
          supported_path =
            TAO_IFR_Service_Utils::reference_to_path (
                supports_interfaces[i]
              );
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (supports_key,
                                                    stringified,
                                                    supported_path);
        }
    }

  if (! CORBA::is_nil (primary_key))
    {
      char *primary_key_path =
        TAO_IFR_Service_Utils::reference_to_path (primary_key);
        this->repo_->config ()->set_string_value (new_key,
                                                  "primary_key",
                                                  primary_key_path);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Home,
                                          path.c_str (),
                                          this->repo_);

  return CORBA::ComponentIR::HomeDef::_narrow (obj.in ());
}

CORBA::ComponentIR::EventDef_ptr
TAO_ComponentContainer_i::create_event (
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces,
    const CORBA::ExtInitializerSeq &initializers
  )
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::EventDef::_nil ());

  this->update_key ();

  return this->create_event_i (id,
                               name,
                               version,
                               is_custom,
                               is_abstract,
                               base_value,
                               is_truncatable,
                               abstract_base_values,
                               supported_interfaces,
                               initializers);
}

CORBA::ComponentIR::EventDef_ptr
TAO_ComponentContainer_i::create_event_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces,
    const CORBA::ExtInitializerSeq &initializers
  )
{
  TAO_ComponentContainer_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path =
    TAO_IFR_Service_Utils::create_common (
        this->def_kind (),
        CORBA::dk_Event,
        this->section_key_,
        new_key,
        this->repo_,
        id,
        name,
        &TAO_ComponentContainer_i::same_as_tmp_name,
        version,
        "defns"
      );

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_custom",
                                             (CORBA::ULong) is_custom);

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_abstract",
                                             (CORBA::ULong) is_abstract);

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_truncatable",
                                             (CORBA::ULong) is_truncatable);

  if (!CORBA::is_nil (base_value))
    {
      const char *base_path =
        TAO_IFR_Service_Utils::reference_to_path (base_value);

      // Get the servant's key into the temporary key holder, because
      // the name clash checker for base valuetypes is static, and has
      // no other way to know about a specific key.
      this->repo_->config ()->expand_path (
                                  this->repo_->root_key (),
                                  base_path,
                                  TAO_IFR_Service_Utils::tmp_key_,
                                  0
                                );
      TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                          new_key,
                                          this->repo_,
                                          CORBA::dk_Value);

      this->repo_->config ()->set_string_value (new_key,
                                                "base_value",
                                                base_path);
    }

  CORBA::ULong length = abstract_base_values.length ();
  CORBA::ULong i = 0;

  if (length > 0)
    {
      ACE_Configuration_Section_Key bases_key;
      this->repo_->config ()->open_section (new_key,
                                            "abstract_bases",
                                            1,
                                            bases_key);

      char *base_path = 0;

      for (i = 0; i < length; ++i)
        {
          base_path =
            TAO_IFR_Service_Utils::reference_to_path (
                abstract_base_values[i]
              );
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (bases_key,
                                                    stringified,
                                                    base_path);
        }
    }

  length = supported_interfaces.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key supported_key;
      this->repo_->config ()->open_section (new_key,
                                            "supported",
                                            1,
                                            supported_key);

      this->repo_->config ()->set_integer_value (supported_key,
                                                 "count",
                                                 length);
      char *supported_path = 0;

      for (i = 0; i < length; ++i)
        {
          supported_path =
            TAO_IFR_Service_Utils::reference_to_path (
                supported_interfaces[i]
              );
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (supported_key,
                                                    stringified,
                                                    supported_path);
        }
    }

  length = initializers.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key initializers_key;
      this->repo_->config ()->open_section (new_key,
                                            "initializers",
                                            1,
                                            initializers_key);

      CORBA::ULong count = 0;
      char *path = 0;
      char *stringified = 0;
      ACE_Configuration_Section_Key initializer_key;
      ACE_Configuration_Section_Key outer_key;
      ACE_Configuration_Section_Key inner_key;
      ACE_TString excep_path;

      for (i = 0; i < length; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (initializers_key,
                                                stringified,
                                                1,
                                                initializer_key);
          this->repo_->config ()->set_string_value (
                                      initializer_key,
                                      "name",
                                      initializers[i].name.in ()
                                    );

          count = initializers[i].members.length ();

          if (count > 0)
            {
              this->repo_->config ()->open_section (initializer_key,
                                                    "params",
                                                    1,
                                                    outer_key);

              this->repo_->config ()->set_integer_value (outer_key,
                                                         "count",
                                                         count);

              for (CORBA::ULong j = 0; j < count; ++j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->open_section (outer_key,
                                                        stringified,
                                                        1,
                                                        inner_key);

                  this->repo_->config ()->set_string_value (
                      inner_key,
                      "arg_name",
                      initializers[i].members[j].name.in ()
                    );

                  path =
                    TAO_IFR_Service_Utils::reference_to_path (
                              initializers[i].members[j].type_def.in ()
                            );

                  this->repo_->config ()->set_string_value (inner_key,
                                                            "arg_path",
                                                            path);
                }
            }

          count = initializers[i].exceptions.length ();

          if (count > 0)
            {
              this->repo_->config ()->open_section (initializer_key,
                                                    "excepts",
                                                    1,
                                                    outer_key);

              this->repo_->config ()->set_integer_value (outer_key,
                                                         "count",
                                                         count);

              for (CORBA::ULong k = 0; k < count; ++k)
                {
                  this->repo_->config ()->get_string_value (
                      this->repo_->repo_ids_key (),
                      initializers[i].exceptions[k].id.in (),
                      excep_path
                    );

                  stringified = TAO_IFR_Service_Utils::int_to_string (k);
                  this->repo_->config ()->set_string_value (outer_key,
                                                            stringified,
                                                            excep_path);
                }
            }
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Event,
                                          path.c_str (),
                                          this->repo_);

  return CORBA::ComponentIR::EventDef::_narrow (obj.in ());
}

int
TAO_ComponentContainer_i::same_as_tmp_name (const char *name)
{
  return
    ACE_OS::strcmp (TAO_ComponentContainer_i::tmp_name_holder_, name) == 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
