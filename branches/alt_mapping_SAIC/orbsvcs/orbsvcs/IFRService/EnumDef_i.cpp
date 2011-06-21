// $Id$

#include "orbsvcs/IFRService/EnumDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EnumDef_i::TAO_EnumDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_TypedefDef_i (repo)
{
}

TAO_EnumDef_i::~TAO_EnumDef_i (void)
{
}

CORBA::DefinitionKind
TAO_EnumDef_i::def_kind (void)
{
  return CORBA::dk_Enum;
}

CORBA::TypeCode_ptr
TAO_EnumDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_EnumDef_i::type_i (void)
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  CORBA::EnumMemberSeq_var members = this->members_i ();

  return this->repo_->tc_factory ()->create_enum_tc (id.c_str (),
                                                     name.c_str (),
                                                     members.in ());
}

CORBA::EnumMemberSeq *
TAO_EnumDef_i::members (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->members_i ();
}

CORBA::EnumMemberSeq *
TAO_EnumDef_i::members_i (void)
{
  u_int count = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "count",
                                             count);

  CORBA::EnumMemberSeq *retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA::EnumMemberSeq (count),
                    CORBA::NO_MEMORY ());

  retval->length (count);

  for (u_int i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (this->section_key_,
                                            stringified,
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
TAO_EnumDef_i::members (const CORBA::EnumMemberSeq &members)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->members_i (members);
}

void
TAO_EnumDef_i::members_i (const CORBA::EnumMemberSeq &members)
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "members",
                                          1);

  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "count",
                                             count);

 for (CORBA::ULong i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (this->section_key_,
                                            stringified,
                                            1,
                                            member_key);

      ACE_TString member_name (members[i]);
      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                member_name);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
