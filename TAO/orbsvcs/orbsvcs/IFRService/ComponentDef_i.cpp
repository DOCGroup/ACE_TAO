// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/ComponentDef_i.h"
#include "orbsvcs/IFRService/ProvidesDef_i.h"
#include "orbsvcs/IFRService/UsesDef_i.h"
#include "orbsvcs/IFRService/EmitsDef_i.h"
#include "orbsvcs/IFRService/PublishesDef_i.h"
#include "orbsvcs/IFRService/ConsumesDef_i.h"
#include "orbsvcs/IFRService/ExtAttributeDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Specialization.
template<>
void
TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::UsesDescriptionSeq>::get_is_multiple (
    CORBA::ComponentIR::UsesDescriptionSeq &desc_seq,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key,
    CORBA::ULong index
  )
{
  CORBA::ULong is_multiple = 0;
  config->get_integer_value (key,
                             "is_multiple",
                             is_multiple);
  desc_seq[index].is_multiple =
    static_cast<CORBA::Boolean> (is_multiple);
}

// Specialization.
template<>
void
TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::EventPortDescriptionSeq>::port_base_type (
    CORBA::ComponentIR::EventPortDescriptionSeq &desc_seq,
    ACE_TString &holder,
    CORBA::ULong index
  )
{
  desc_seq[index].event = holder.fast_rep ();
}

/// Specialization.
template<>
void
TAO_Port_Utils<CORBA::ComponentIR::UsesDef>::set_is_multiple (
    CORBA::Boolean is_multiple,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key
  )
{
  config->set_integer_value (key,
                             "is_multiple",
                             is_multiple);
}

// ==============================================================

TAO_ComponentDef_i::TAO_ComponentDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo),
    TAO_InterfaceAttrExtension_i (repo),
    TAO_ExtInterfaceDef_i (repo)
{
}

TAO_ComponentDef_i::~TAO_ComponentDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ComponentDef_i::def_kind (void)
{
  return CORBA::dk_Component;
}

void
TAO_ComponentDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_ComponentDef_i::destroy_i (void)
{
  // Destroy our special subsections first, then call destroy_i
  // on our base class.

  TAO_IFR_Generic_Utils<TAO_ProvidesDef_i>::destroy_special (
      "provides",
      this->repo_,
      this->section_key_
    );

  TAO_IFR_Generic_Utils<TAO_UsesDef_i>::destroy_special (
      "uses",
      this->repo_,
      this->section_key_
    );

  TAO_IFR_Generic_Utils<TAO_EmitsDef_i>::destroy_special (
      "emits",
      this->repo_,
      this->section_key_
    );

  TAO_IFR_Generic_Utils<TAO_PublishesDef_i>::destroy_special (
      "publishes",
      this->repo_,
      this->section_key_
    );

  TAO_IFR_Generic_Utils<TAO_ConsumesDef_i>::destroy_special (
      "consumes",
      this->repo_,
      this->section_key_
    );

  this->TAO_ExtInterfaceDef_i::destroy_i ();
}

CORBA::Contained::Description *
TAO_ComponentDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_ComponentDef_i::describe_i (void)
{
  CORBA::ComponentIR::ComponentDescription cd;
  TAO_IFR_Desc_Utils<CORBA::ComponentIR::ComponentDescription,
                     TAO_ComponentDef_i>::fill_desc_begin (
                                              cd,
                                              this->repo_,
                                              this->section_key_
                                            );

  ACE_TString holder;
  int status =
    this->repo_->config ()->get_string_value (this->section_key_,
                                              "base_component",
                                              holder);

  if (status == 0)
    {
      ACE_Configuration_Section_Key base_key;
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           base_key,
                                           0);
      this->repo_->config ()->get_string_value (base_key,
                                                "id",
                                                holder);
    }

  // If status wasn't 0, holder will contain the empty string.
  cd.base_component = holder.fast_rep ();

  CORBA::ULong count = 0;
  ACE_Configuration_Section_Key supports_key;
  status = this->repo_->config ()->open_section (this->section_key_,
                                                 "supported",
                                                 0,
                                                 supports_key);

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (supports_key,
                                                 "count",
                                                 count);
    }

  cd.supported_interfaces.length (count);
  char *stringified = 0;
  CORBA::ULong i = 0;

  for (i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (supports_key,
                                                stringified,
                                                holder);

      cd.supported_interfaces[i] = holder.c_str ();
    }

  TAO_Port_Desc_Seq_Utils<
      CORBA::ComponentIR::ProvidesDescriptionSeq
    >::port_descriptions (cd.provided_interfaces,
       this->repo_->config (),
       this->section_key_,
       "provides");

  TAO_Port_Desc_Seq_Utils<
      CORBA::ComponentIR::UsesDescriptionSeq
    >::port_descriptions (cd.used_interfaces,
       this->repo_->config (),
       this->section_key_,
       "uses");

  TAO_Port_Desc_Seq_Utils<
      CORBA::ComponentIR::EventPortDescriptionSeq
    >::port_descriptions (cd.emits_events,
      this->repo_->config (),
      this->section_key_,
      "emits");

  TAO_Port_Desc_Seq_Utils<
      CORBA::ComponentIR::EventPortDescriptionSeq
    >::port_descriptions (cd.publishes_events,
      this->repo_->config (),
      this->section_key_,
      "publishes");

  TAO_Port_Desc_Seq_Utils<
      CORBA::ComponentIR::EventPortDescriptionSeq
    >::port_descriptions (cd.consumes_events,
      this->repo_->config (),
      this->section_key_,
      "consumes");

  count = 0;
  ACE_Configuration_Section_Key attrs_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "attrs",
                                          0,
                                          attrs_key);
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (attrs_key,
                                                 "count",
                                                 count);
    }

  cd.attributes.length (count);
  ACE_Configuration_Section_Key attr_key;

  for (i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (attrs_key,
                                            stringified,
                                            0,
                                            attr_key);

      TAO_ExtAttributeDef_i impl (this->repo_);
      impl.section_key (attr_key);
      impl.fill_description (cd.attributes[i]);
    }

  cd.type = this->type_i ();

  CORBA::Contained::Description *cont_desc_ptr = 0;
  ACE_NEW_THROW_EX (cont_desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());
  CORBA::Contained::Description_var retval = cont_desc_ptr;

  cont_desc_ptr->kind = CORBA::dk_Component;

  cont_desc_ptr->value <<= cd;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_ComponentDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_ComponentDef_i::type_i (void)
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  return this->repo_->tc_factory ()->create_component_tc (
                                         id.c_str (),
                                         name.c_str ()
                                       );
}

CORBA::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->supported_interfaces_i ();
}

CORBA::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces_i (void)
{
  CORBA::InterfaceDefSeq *seq = 0;
  ACE_NEW_RETURN (seq,
                  CORBA::InterfaceDefSeq (0),
                  0);
  CORBA::InterfaceDefSeq_var retval = seq;
  retval->length (0);

  CORBA::ULong count = 0;
  ACE_Configuration_Section_Key supported_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "supported",
                                          0,
                                          supported_key);

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (supported_key,
                                                 "count",
                                                 count);
      retval->length (count);
      char *stringified = 0;
      ACE_TString path;
      CORBA::Object_var tmp;

      for (CORBA::ULong i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->get_string_value (supported_key,
                                                    stringified,
                                                    path);
          tmp =
            TAO_IFR_Service_Utils::path_to_ir_object (path,
                                                      this->repo_);

          retval[i] = CORBA::InterfaceDef::_narrow (tmp.in ());
        }
    }

  return retval._retn ();
}

void
TAO_ComponentDef_i::supported_interfaces (
    const CORBA::InterfaceDefSeq &supported_interfaces
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->supported_interfaces_i (supported_interfaces);
}

void
TAO_ComponentDef_i::supported_interfaces_i (
    const CORBA::InterfaceDefSeq &supported_interfaces
  )
{
  // Remove the old supported interfaces.
  this->repo_->config ()->remove_section (this->section_key_,
                                          "supported",
                                          0);

  CORBA::ULong length = supported_interfaces.length ();
  ACE_Configuration_Section_Key inherited_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "supported",
                                        1,
                                        inherited_key);

  CORBA::String_var name;
  char *supported_path = 0;
  ACE_Configuration_Section_Key supported_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      supported_path =
        TAO_IFR_Service_Utils::reference_to_path (
            supported_interfaces[i]
          );

      // Get the servant's key into the temporary key holder, because
      // the name clash checker for base interfaces is static, and has
      // no other way to know about a specific key.
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                            supported_path,
                                            TAO_IFR_Service_Utils::tmp_key_,
                                            0);
      TAO_IFR_Service_Utils::name_exists (&TAO_InterfaceDef_i::name_clash,
                                          this->section_key_,
                                          this->repo_,
                                          this->def_kind ());

      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (supported_key,
                                                stringified,
                                                supported_path);
    }
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentDef_i::base_component (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  this->update_key ();

  return this->base_component_i ();
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentDef_i::base_component_i (void)
{
  ACE_TString base_path;
  int status =
    this->repo_->config ()->get_string_value (this->section_key_,
                                              "base_component",
                                              base_path);

  if (status != 0)
    {
      return CORBA::ComponentIR::ComponentDef::_nil ();
    }

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (base_path,
                                              this->repo_);

  return CORBA::ComponentIR::ComponentDef::_narrow (obj.in ());
}

void
TAO_ComponentDef_i::base_component (
    CORBA::ComponentIR::ComponentDef_ptr base_component
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->base_component_i (base_component);
}

void
TAO_ComponentDef_i::base_component_i (
    CORBA::ComponentIR::ComponentDef_ptr base_component
  )
{
  if (CORBA::is_nil (base_component))
    {
      this->repo_->config ()->remove_value (this->section_key_,
                                            "base_component");
      return;
    }

  const char *base_path =
    TAO_IFR_Service_Utils::reference_to_path (base_component);

  // Get the servant's key into the temporary key holder, because
  // the name clash checker for base components is static, and has
  // no other way to know about a specific key.
  this->repo_->config ()->expand_path (
                              this->repo_->root_key (),
                              base_path,
                              TAO_IFR_Service_Utils::tmp_key_,
                              0
                            );
  TAO_IFR_Service_Utils::name_exists (&TAO_ComponentDef_i::name_clash,
                                      this->section_key_,
                                      this->repo_,
                                      CORBA::dk_Component);

  this->repo_->config ()->set_string_value (
                              this->section_key_,
                              "base_component",
                              base_path
                            );
}

CORBA::ComponentIR::ProvidesDef_ptr
TAO_ComponentDef_i::create_provides (
    const char *id,
    const char *name,
    const char *version,
    CORBA::InterfaceDef_ptr interface_type
  )
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ProvidesDef::_nil ());

  this->update_key ();

  return this->create_provides_i (id,
                                  name,
                                  version,
                                  interface_type);
}

CORBA::ComponentIR::ProvidesDef_ptr
TAO_ComponentDef_i::create_provides_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::InterfaceDef_ptr interface_type
  )
{
  return
    TAO_Port_Utils<
        CORBA::ComponentIR::ProvidesDef
      >::create_entry (id,
                       name,
                       version,
                       "provides",
                       interface_type,
                       0,
                       this->repo_,
                       CORBA::dk_Provides,
                       this->section_key_);
}

CORBA::ComponentIR::UsesDef_ptr
TAO_ComponentDef_i::create_uses (const char *id,
                                 const char *name,
                                 const char *version,
                                 CORBA::InterfaceDef_ptr interface_type,
                                 CORBA::Boolean is_multiple)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::UsesDef::_nil ());

  this->update_key ();

  return this->create_uses_i (id,
                              name,
                              version,
                              interface_type,
                              is_multiple);
}

CORBA::ComponentIR::UsesDef_ptr
TAO_ComponentDef_i::create_uses_i (const char *id,
                                   const char *name,
                                   const char *version,
                                   CORBA::InterfaceDef_ptr interface_type,
                                   CORBA::Boolean is_multiple)
{
  return
    TAO_Port_Utils<
        CORBA::ComponentIR::UsesDef
      >::create_entry (id,
                       name,
                       version,
                       "uses",
                       interface_type,
                       is_multiple,
                       this->repo_,
                       CORBA::dk_Uses,
                       this->section_key_);
}

CORBA::ComponentIR::EmitsDef_ptr
TAO_ComponentDef_i::create_emits (const char *id,
                                  const char *name,
                                  const char *version,
                                  CORBA::ValueDef_ptr value)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::EmitsDef::_nil ());

  this->update_key ();

  return this->create_emits_i (id,
                               name,
                               version,
                               value);
}

CORBA::ComponentIR::EmitsDef_ptr
TAO_ComponentDef_i::create_emits_i (const char *id,
                                    const char *name,
                                    const char *version,
                                    CORBA::ValueDef_ptr value)
{
  return
    TAO_Port_Utils<
        CORBA::ComponentIR::EmitsDef
      >::create_entry (id,
                       name,
                       version,
                       "emits",
                       value,
                       0,
                       this->repo_,
                       CORBA::dk_Emits,
                       this->section_key_);
}

CORBA::ComponentIR::PublishesDef_ptr
TAO_ComponentDef_i::create_publishes (const char *id,
                                      const char *name,
                                      const char *version,
                                      CORBA::ValueDef_ptr value)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::PublishesDef::_nil ());

  this->update_key ();

  return this->create_publishes_i (id,
                                   name,
                                   version,
                                   value);
}

CORBA::ComponentIR::PublishesDef_ptr
TAO_ComponentDef_i::create_publishes_i (const char *id,
                                        const char *name,
                                        const char *version,
                                        CORBA::ValueDef_ptr value)
{
  return
    TAO_Port_Utils<
        CORBA::ComponentIR::PublishesDef
      >::create_entry (id,
                       name,
                       version,
                       "publishes",
                       value,
                       0,
                       this->repo_,
                       CORBA::dk_Publishes,
                       this->section_key_);
}

CORBA::ComponentIR::ConsumesDef_ptr
TAO_ComponentDef_i::create_consumes (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::ValueDef_ptr value)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ConsumesDef::_nil ());

  this->update_key ();

  return this->create_consumes_i (id,
                                  name,
                                  version,
                                  value);
}

CORBA::ComponentIR::ConsumesDef_ptr
TAO_ComponentDef_i::create_consumes_i (const char *id,
                                       const char *name,
                                       const char *version,
                                       CORBA::ValueDef_ptr value)
{
  return
    TAO_Port_Utils<
        CORBA::ComponentIR::ConsumesDef
      >::create_entry (id,
                       name,
                       version,
                       "consumes",
                       value,
                       0,
                       this->repo_,
                       CORBA::dk_Consumes,
                       this->section_key_);
}

int
TAO_ComponentDef_i::name_clash (const char *name)
{
  try
    {
      TAO_Container_i::tmp_name_holder_ = name;
      TAO_IFR_Service_Utils::name_exists (&TAO_Container_i::same_as_tmp_name,
                                          TAO_IFR_Service_Utils::tmp_key_,
                                          TAO_IFR_Service_Utils::repo_,
                                          CORBA::dk_Component);
    }
  catch (const CORBA::Exception&)
    {
      return 1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
