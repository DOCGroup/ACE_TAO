/* -*- C++ -*- */
// $Id$

#include "ExtAttributeDef_i.h"
#include "Repository_i.h"
#include "ExceptionDef_i.h"
#include "IFR_Service_Utils.h"

ACE_RCSID (IFRService, 
           ExtAttributeDef_i, 
           "$Id$")

TAO_ExtAttributeDef_i::TAO_ExtAttributeDef_i (TAO_Repository_i *repo)
  : TAO_AttributeDef_i (repo),
    TAO_Contained_i (repo),
    TAO_IRObject_i (repo)
{
}

TAO_ExtAttributeDef_i::~TAO_ExtAttributeDef_i (void)
{
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::get_exceptions (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->get_exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::get_exceptions_i (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ExcDescriptionSeq *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ExcDescriptionSeq,
                  0);
  CORBA::ExcDescriptionSeq_var safe_retval = retval;

  this->fill_exceptions (*retval,
                         "get_excepts"
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return safe_retval._retn ();
}

void 
TAO_ExtAttributeDef_i::get_exceptions (
    const CORBA::ExcDescriptionSeq &get_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->get_exceptions_i (get_exceptions
                          ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ExtAttributeDef_i::get_exceptions_i (
    const CORBA::ExcDescriptionSeq &get_exceptions
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->exceptions ("get_excepts",
                    get_exceptions);
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::set_exceptions (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->set_exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::set_exceptions_i (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ExcDescriptionSeq *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ExcDescriptionSeq,
                  0);
  CORBA::ExcDescriptionSeq_var safe_retval = retval;

  this->fill_exceptions (*retval,
                         "set_excepts"
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return safe_retval._retn ();
}

void 
TAO_ExtAttributeDef_i::set_exceptions (
    const CORBA::ExcDescriptionSeq &set_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->set_exceptions_i (set_exceptions
                          ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ExtAttributeDef_i::set_exceptions_i (
    const CORBA::ExcDescriptionSeq &set_exceptions
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->exceptions ("set_excepts",
                    set_exceptions);
}

CORBA::ExtAttributeDescription *
TAO_ExtAttributeDef_i::describe_attribute (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_attribute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExtAttributeDescription *
TAO_ExtAttributeDef_i::describe_attribute_i (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ExtAttributeDescription *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ExtAttributeDescription,
                  0);
  CORBA::ExtAttributeDescription_var safe_retval = retval;

  this->fill_description (*retval
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return safe_retval._retn ();
}

void 
TAO_ExtAttributeDef_i::fill_description (
    CORBA::ExtAttributeDescription &desc
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  desc.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  desc.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            holder);
  desc.defined_in = holder.fast_rep ();

  desc.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  desc.type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  desc.mode = this->mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->fill_exceptions (desc.get_exceptions,
                         "get_excepts"
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->fill_exceptions (desc.put_exceptions,
                         "put_excepts"
                         ACE_ENV_ARG_PARAMETER);
}

void
TAO_ExtAttributeDef_i::exceptions (const char *sub_section,
                                   const CORBA::ExceptionDefSeq &exceptions)
{
  CORBA::ULong length = exceptions.length ();

  if (length == 0)
    {
      return;
    }

  // Create new subsection because this function should be called
  // only on a new TAO_ExtAttributeDef_i.
  ACE_Configuration_Section_Key excepts_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        sub_section,
                                        1,
                                        excepts_key);
  char *type_path = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      type_path = 
        TAO_IFR_Service_Utils::reference_to_path (exceptions[i].in ());

      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (excepts_key,
                                                stringified,
                                                type_path);
    }
}

void
TAO_ExtAttributeDef_i::fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                                        const char *sub_section
                                        ACE_ENV_ARG_DECL)
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
      exceptions[i].type = impl.type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

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
