// $Id$

#include "orbsvcs/IFRService/ExtAttributeDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/ExceptionDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ExtAttributeDef_i::TAO_ExtAttributeDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_AttributeDef_i (repo)
{
}

TAO_ExtAttributeDef_i::~TAO_ExtAttributeDef_i (void)
{
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::get_exceptions (
  )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->get_exceptions_i ();
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::get_exceptions_i (
  )
{
  CORBA::ExcDescriptionSeq *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ExcDescriptionSeq,
                  0);
  CORBA::ExcDescriptionSeq_var safe_retval = retval;

  this->fill_exceptions (*retval,
                         "get_excepts");

  return safe_retval._retn ();
}

void
TAO_ExtAttributeDef_i::get_exceptions (
    const CORBA::ExcDescriptionSeq &get_exceptions
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->get_exceptions_i (get_exceptions);
}

void
TAO_ExtAttributeDef_i::get_exceptions_i (
    const CORBA::ExcDescriptionSeq &get_exceptions
  )
{
  this->exceptions ("get_excepts",
                    get_exceptions);
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::set_exceptions (
  )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->set_exceptions_i ();
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::set_exceptions_i (
  )
{
  CORBA::ExcDescriptionSeq *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ExcDescriptionSeq,
                  0);
  CORBA::ExcDescriptionSeq_var safe_retval = retval;

  this->fill_exceptions (*retval,
                         "put_excepts");

  return safe_retval._retn ();
}

void
TAO_ExtAttributeDef_i::set_exceptions (
    const CORBA::ExcDescriptionSeq &set_exceptions
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->set_exceptions_i (set_exceptions);
}

void
TAO_ExtAttributeDef_i::set_exceptions_i (
    const CORBA::ExcDescriptionSeq &set_exceptions
  )
{
  this->exceptions ("put_excepts",
                    set_exceptions);
}

CORBA::ExtAttributeDescription *
TAO_ExtAttributeDef_i::describe_attribute (
  )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_attribute_i ();
}

CORBA::ExtAttributeDescription *
TAO_ExtAttributeDef_i::describe_attribute_i (
  )
{
  CORBA::ExtAttributeDescription *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ExtAttributeDescription,
                  0);
  CORBA::ExtAttributeDescription_var safe_retval = retval;

  this->fill_description (*retval);

  return safe_retval._retn ();
}

void
TAO_ExtAttributeDef_i::fill_description (
    CORBA::ExtAttributeDescription &desc
  )
{
  desc.name = this->name_i ();

  desc.id = this->id_i ();

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            holder);
  desc.defined_in = holder.fast_rep ();

  desc.id = this->id_i ();

  desc.type = this->type_i ();

  desc.mode = this->mode_i ();

  this->fill_exceptions (desc.get_exceptions,
                         "get_excepts");

  this->fill_exceptions (desc.put_exceptions,
                         "put_excepts");
}

void
TAO_ExtAttributeDef_i::fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                                        const char *sub_section)
{
  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          sub_section,
                                          0,
                                          excepts_key);

  if (status != 0)
    {
      exceptions.length (0);
      return;
    }

  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (excepts_key,
                                             "count",
                                             count);
  exceptions.length (count);
  char *stringified = 0;
  ACE_TString holder;
  ACE_Configuration_Section_Key except_key;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (excepts_key,
                                                stringified,
                                                holder);
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           except_key,
                                           0);
      this->repo_->config ()->get_string_value (except_key,
                                                "name",
                                                holder);
      exceptions[i].name = holder.fast_rep ();
      this->repo_->config ()->get_string_value (except_key,
                                                "id",
                                                holder);
      exceptions[i].id = holder.fast_rep ();

      TAO_ExceptionDef_i impl (this->repo_);
      impl.section_key (except_key);
      exceptions[i].type = impl.type_i ();

      this->repo_->config ()->get_string_value (except_key,
                                                "container_id",
                                                holder);
      exceptions[i].defined_in = holder.fast_rep ();
      this->repo_->config ()->get_string_value (except_key,
                                                "version",
                                                holder);
      exceptions[i].version = holder.fast_rep ();
    }
}

void
TAO_ExtAttributeDef_i::exceptions (const char *sub_section,
                                   const CORBA::ExcDescriptionSeq &exceptions)
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          sub_section,
                                          0);
  ACE_Configuration_Section_Key new_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        sub_section,
                                        1,
                                        new_key);
  CORBA::ULong count = exceptions.length ();
  this->repo_->config ()->set_integer_value (new_key,
                                             "count",
                                             count);
  char *stringified = 0;
  ACE_TString path;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                exceptions[i].id.in (),
                                                path);
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (new_key,
                                                stringified,
                                                path);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
