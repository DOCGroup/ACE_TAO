/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ComponentDef_i.h"
#include "ProvidesDef_i.h"
#include "UsesDef_i.h"
#include "EmitsDef_i.h"
#include "PublishesDef_i.h"
#include "ConsumesDef_i.h"
#include "ExtAttributeDef_i.h"
#include "IFR_Service_Utils.h"

ACE_RCSID (IFRService,
           ComponentDef_i,
           "$Id$")

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
    ACE_static_cast (CORBA::Boolean,
                     is_multiple);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::UsesDescriptionSeq>;
template class TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::EventPortDescriptionSeq>;
template class TAO_Port_Utils<CORBA::ComponentIR::UsesDef>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::UsesDescriptionSeq>
#pragma instantiate TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::EventPortDescriptionSeq>
#pragma instantiate TAO_Port_Utils<CORBA::ComponentIR::UsesDef>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION*/

// ==============================================================

TAO_ComponentDef_i::TAO_ComponentDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo),
    TAO_ExtInterfaceDef_i (repo),
    TAO_InterfaceAttrExtension_i (repo)
{
}

TAO_ComponentDef_i::~TAO_ComponentDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ComponentDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Component;
}

void
TAO_ComponentDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our special subsections first, then call destroy_i
  // on our base class.

  TAO_IFR_Generic_Utils<TAO_ProvidesDef_i>::destroy_special (
      "provides",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;

  TAO_IFR_Generic_Utils<TAO_UsesDef_i>::destroy_special (
      "uses",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;

  TAO_IFR_Generic_Utils<TAO_EmitsDef_i>::destroy_special (
      "emits",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;

  TAO_IFR_Generic_Utils<TAO_PublishesDef_i>::destroy_special (
      "publishes",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;

  TAO_IFR_Generic_Utils<TAO_ConsumesDef_i>::destroy_special (
      "consumes",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;

  this->TAO_ExtInterfaceDef_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ComponentDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ComponentDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ComponentIR::ComponentDescription cd;
  TAO_IFR_Desc_Utils<CORBA::ComponentIR::ComponentDescription,
                     TAO_ComponentDef_i>::fill_desc_begin (
                                              cd,
                                              this->repo_,
                                              this->section_key_
                                              ACE_ENV_ARG_PARAMETER
                                            );
  ACE_CHECK_RETURN (0);

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_component",
                                            holder);
  cd.base_component = holder.fast_rep ();

  CORBA::ULong count = 0;
  ACE_Configuration_Section_Key supports_key;
  int status = this->repo_->config ()->open_section (this->section_key_,
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
      this->repo_->config ()->get_string_value (attrs_key,
                                                stringified,
                                                holder);
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           attr_key,
                                           0);
      TAO_ExtAttributeDef_i impl (this->repo_);
      impl.section_key (attr_key);
      impl.fill_description (cd.attributes[i]
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  cd.type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Contained::Description *cont_desc_ptr = 0;
  ACE_NEW_THROW_EX (cont_desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  CORBA::Contained::Description_var retval = cont_desc_ptr;

  cont_desc_ptr->kind = CORBA::dk_Component;

  cont_desc_ptr->value <<= cd;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_ComponentDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ComponentDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                                         ACE_ENV_ARG_PARAMETER
                                       );
}

CORBA::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->supported_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                                                      this->repo_
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          retval[i] = CORBA::InterfaceDef::_narrow (tmp.in ()
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }

  return retval._retn ();
}

void
TAO_ComponentDef_i::supported_interfaces (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supported_interfaces_i (supported_interfaces
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::supported_interfaces_i (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
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
            supported_interfaces[i].in ()
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
                                          this->def_kind ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (supported_key,
                                                stringified,
                                                supported_path);
    }
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentDef_i::base_component (ACE_ENV_SINGLE_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  return this->base_component_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentDef_i::base_component_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString base_id;
  int status =
    this->repo_->config ()->get_string_value (this->section_key_,
                                              "base_component",
                                              base_id);

  if (status != 0)
    {
      return CORBA::ComponentIR::ComponentDef::_nil ();
    }

  CORBA::Contained_var obj = this->repo_->lookup_id (base_id.fast_rep ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  return CORBA::ComponentIR::ComponentDef::_narrow (obj.in ()
                                                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::base_component (
    CORBA::ComponentIR::ComponentDef_ptr base_component
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->base_component_i (base_component
                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::base_component_i (
    CORBA::ComponentIR::ComponentDef_ptr base_component
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  // the name clash checker for base valuetypes is static, and has
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
                                      CORBA::dk_Component
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->repo_->config ()->set_string_value (
                              this->section_key_,
                              "base_component",
                              base_component->_interface_repository_id ()
                            );
}

CORBA::ComponentIR::ProvidesDef_ptr
TAO_ComponentDef_i::create_provides (
    const char *id,
    const char *name,
    const char *version,
    CORBA::InterfaceDef_ptr interface_type
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ProvidesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ProvidesDef::_nil ());

  return this->create_provides_i (id,
                                  name,
                                  version,
                                  interface_type
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::ProvidesDef_ptr
TAO_ComponentDef_i::create_provides_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::InterfaceDef_ptr interface_type
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                       this->section_key_
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::UsesDef_ptr
TAO_ComponentDef_i::create_uses (const char *id,
                                 const char *name,
                                 const char *version,
                                 CORBA::InterfaceDef_ptr interface_type,
                                 CORBA::Boolean is_multiple
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::UsesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::UsesDef::_nil ());

  return this->create_uses_i (id,
                              name,
                              version,
                              interface_type,
                              is_multiple
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::UsesDef_ptr
TAO_ComponentDef_i::create_uses_i (const char *id,
                                   const char *name,
                                   const char *version,
                                   CORBA::InterfaceDef_ptr interface_type,
                                   CORBA::Boolean is_multiple
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                       this->section_key_
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::EmitsDef_ptr
TAO_ComponentDef_i::create_emits (const char *id,
                                  const char *name,
                                  const char *version,
                                  CORBA::ValueDef_ptr value
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::EmitsDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::EmitsDef::_nil ());

  return this->create_emits_i (id,
                               name,
                               version,
                               value
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::EmitsDef_ptr
TAO_ComponentDef_i::create_emits_i (const char *id,
                                    const char *name,
                                    const char *version,
                                    CORBA::ValueDef_ptr value
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                       this->section_key_
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::PublishesDef_ptr
TAO_ComponentDef_i::create_publishes (const char *id,
                                      const char *name,
                                      const char *version,
                                      CORBA::ValueDef_ptr value
                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::PublishesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::PublishesDef::_nil ());

  return this->create_publishes_i (id,
                                   name,
                                   version,
                                   value
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::PublishesDef_ptr
TAO_ComponentDef_i::create_publishes_i (const char *id,
                                        const char *name,
                                        const char *version,
                                        CORBA::ValueDef_ptr value
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                       this->section_key_
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::ConsumesDef_ptr
TAO_ComponentDef_i::create_consumes (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::ValueDef_ptr value
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ConsumesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ConsumesDef::_nil ());

  return this->create_consumes_i (id,
                                  name,
                                  version,
                                  value
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::ConsumesDef_ptr
TAO_ComponentDef_i::create_consumes_i (const char *id,
                                       const char *name,
                                       const char *version,
                                       CORBA::ValueDef_ptr value
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    TAO_Port_Utils<
        CORBA::ComponentIR::ConsumesDef
      >::create_entry (id,
                       name,
                       version,
                       "comsumes",
                       value,
                       0,
                       this->repo_,
                       CORBA::dk_Consumes,
                       this->section_key_
                       ACE_ENV_ARG_PARAMETER);
}

int
TAO_ComponentDef_i::name_clash (const char *name)
{
  ACE_TRY_NEW_ENV
    {
      TAO_Container_i::tmp_name_holder_ = name;
      TAO_IFR_Service_Utils::name_exists (&TAO_Container_i::same_as_tmp_name,
                                          TAO_IFR_Service_Utils::tmp_key_,
                                          TAO_IFR_Service_Utils::repo_,
                                          CORBA::dk_Component
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
