/* -*- C++ -*- */
// $Id$

#include "EnumDef_i.h"
#include "Repository_i.h"
#include "Servant_Factory.h"

ACE_RCSID(IFR_Service, EnumDef_i, "$Id$")

TAO_EnumDef_i::TAO_EnumDef_i (TAO_Repository_i *repo,
                              ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_TypedefDef_i (repo, section_key)
{
}

TAO_EnumDef_i::~TAO_EnumDef_i (void)
{
}

CORBA::DefinitionKind
TAO_EnumDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Enum;
}

CORBA::TypeCode_ptr
TAO_EnumDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_EnumDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA_EnumMemberSeq_var members = this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_enum_tc (id.c_str (),
                                                     name.c_str (),
                                                     members.in ()
                                                     ACE_ENV_ARG_PARAMETER);
}

CORBA_EnumMemberSeq *
TAO_EnumDef_i::members (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_EnumMemberSeq *
TAO_EnumDef_i::members_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int count = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "count",
                                             count);

  CORBA_EnumMemberSeq *retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA_EnumMemberSeq (count),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  retval->length (count);

  for (u_int i = 0; i < count; i++)
    {
      ACE_Configuration_Section_Key member_key;
      CORBA::String_var section_name = this->int_to_string (i);
      this->repo_->config ()->open_section (this->section_key_,
                                            section_name.in (),
                                            0,
                                            member_key);

      ACE_TString member_name;
      this->repo_->config ()->get_string_value (member_key,
                                                "name",
                                                member_name);

      (*retval)[i] = member_name.c_str ();
    }

  return retval;
}

void
TAO_EnumDef_i::members (const CORBA_EnumMemberSeq &members
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->members_i (members
                   ACE_ENV_ARG_PARAMETER);
}

void
TAO_EnumDef_i::members_i (const CORBA_EnumMemberSeq &members
                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "members",
                                          1);

  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "count",
                                             count);

 for (CORBA::ULong i = 0; i < count; i ++)
    {
      ACE_Configuration_Section_Key member_key;
      CORBA::String_var section_name = this->int_to_string (i);
      this->repo_->config ()->open_section (this->section_key_,
                                            section_name.in (),
                                            1,
                                            member_key);

      ACE_TString member_name (members[i]);
      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                member_name);
    }
}

