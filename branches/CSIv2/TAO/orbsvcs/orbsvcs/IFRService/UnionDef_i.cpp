/* -*- C++ -*- */
// $Id$

#include "UnionDef_i.h"
#include "Repository_i.h"
#include "IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           UnionDef_i, 
           "$Id$")

TAO_UnionDef_i::TAO_UnionDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_TypedefDef_i (repo),
    TAO_Container_i (repo)
{
}

TAO_UnionDef_i::~TAO_UnionDef_i (void)
{
}

CORBA::DefinitionKind
TAO_UnionDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Union;
}

void
TAO_UnionDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_UnionDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our members.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Destroy ourself.
  TAO_Contained_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::TypeCode_var tc = this->discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  CORBA::UnionMemberSeq_var members = this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_union_tc (id.c_str (),
                                                      name.c_str (),
                                                      tc.in (),
                                                      members.in ()
                                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::discriminator_type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::discriminator_type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString disc_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "disc_path",
                                            disc_path);

  TAO_IDLType_i *impl = 
    TAO_IFR_Service_Utils::path_to_idltype (disc_path,
                                            this->repo_);

  return impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_UnionDef_i::discriminator_type_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->discriminator_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_UnionDef_i::discriminator_type_def_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString disc_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "disc_path",
                                            disc_path);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (disc_path,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return CORBA::IDLType::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_UnionDef_i::discriminator_type_def (
    CORBA::IDLType_ptr discriminator_type_def
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->discriminator_type_def_i (discriminator_type_def
                                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_UnionDef_i::discriminator_type_def_i (
    CORBA::IDLType_ptr discriminator_type_def
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *disc_path = 
    TAO_IFR_Service_Utils::reference_to_path (discriminator_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "disc_path",
                                            disc_path);
}

CORBA::UnionMemberSeq *
TAO_UnionDef_i::members (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::UnionMemberSeq *
TAO_UnionDef_i::members_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "refs",
                                        0,
                                        refs_key);

  u_int count;
  this->repo_->config ()->get_integer_value (refs_key,
                                             "count",
                                             count);

  for (u_int i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      if (this->repo_->config ()->open_section (refs_key,
                                                stringified,
                                                0,
                                                member_key)
           == 0)
        {
          ACE_TString path;
          this->repo_->config ()->get_string_value (member_key,
                                                    "path",
                                                    path);

          ACE_Configuration_Section_Key entry_key;

          // This entry may have been removed.
          if (this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                   path,
                                                   entry_key,
                                                   0)
               == 0)
            {
              key_queue.enqueue_tail (member_key);
            }
        }
    }

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, key_queue.size ());

  CORBA::UnionMemberSeq *members = 0;
  ACE_NEW_THROW_EX (members,
                    CORBA::UnionMemberSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  members->length (size);
  CORBA::UnionMemberSeq_var retval = members;

  ACE_TString name, path;
  ACE_Configuration_Section_Key next_key, entry_key;
  CORBA::Object_var obj;
  TAO_IDLType_i *impl = 0;

  for (CORBA::ULong k = 0; k < size; ++k)
    {
      key_queue.dequeue_head (next_key);

      this->repo_->config ()->get_string_value (next_key,
                                                "name",
                                                name);

      retval[k].name = name.c_str ();

      this->fetch_label (next_key,
                         retval[k]
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->repo_->config ()->get_string_value (next_key,
                                                "path",
                                                path);

      obj = TAO_IFR_Service_Utils::path_to_ir_object (path,
                                                      this->repo_
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[k].type_def = CORBA::IDLType::_narrow (obj.in ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      impl = TAO_IFR_Service_Utils::path_to_idltype (path,
                                                     this->repo_);

      retval[k].type = impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_UnionDef_i::members (const CORBA::UnionMemberSeq &members
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->members_i (members
                   ACE_ENV_ARG_PARAMETER);
}

void
TAO_UnionDef_i::members_i (const CORBA::UnionMemberSeq &members
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our old members, both refs and defns.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TString section_name;
  ACE_Configuration_Section_Key refs_key;

  this->repo_->config ()->open_section (this->section_key_,
                                        "refs",
                                        1,
                                        refs_key);

  // Store the new member count of the union.
  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count);
  char *member_path = 0;

  // Create a section for each member. We store the member
  // name, its label value, and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; i++)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            stringified,
                                            1,
                                            member_key);

      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                members[i].name.in ());

      member_path = 
        TAO_IFR_Service_Utils::reference_to_path (members[i].type_def.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path);

      this->store_label (member_key,
                         members[i].label
                         ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_UnionDef_i::fetch_label (const ACE_Configuration_Section_Key member_key,
                             CORBA::UnionMember &member
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration::VALUETYPE vt;
  this->repo_->config ()->find_value (member_key,
                                      "label",
                                      vt);

  if (vt == ACE_Configuration::STRING)
    {
      member.label <<= CORBA::Any::from_octet (0);

      return;
    }

  u_int value = 0;
  this->repo_->config ()->get_integer_value (member_key,
                                             "label",
                                             value);

  CORBA::TypeCode_var tc = 
    this->discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  switch (kind)
  {
    case CORBA::tk_char:
      member.label <<= CORBA::Any::from_char (ACE_static_cast (CORBA::Char,
                                                               value));
      break;
    case CORBA::tk_wchar:
      member.label <<= CORBA::Any::from_wchar (ACE_static_cast (CORBA::WChar,
                                                                value));
      break;
    case CORBA::tk_boolean:
      member.label <<= CORBA::Any::from_boolean (ACE_static_cast (CORBA::Boolean,
                                                                  value));
      break;
    case CORBA::tk_short:
      member.label <<= ACE_static_cast (CORBA::Short, value);
      break;
    case CORBA::tk_ushort:
      member.label <<= ACE_static_cast (CORBA::UShort, value);
      break;
    case CORBA::tk_long:
      member.label <<= ACE_static_cast (CORBA::Long, value);
      break;
    case CORBA::tk_ulong:
      member.label <<= ACE_static_cast (CORBA::ULong, value);
      break;
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_longlong:
      member.label <<= ACE_static_cast (CORBA::LongLong, value);
      break;
#endif /* ACE_LACKS_LONGLONG_T */
    case CORBA::tk_ulonglong:
      member.label <<= ACE_static_cast (CORBA::ULongLong, value);
      break;
    case CORBA::tk_enum:
    {
      TAO_OutputCDR cdr;
      cdr.write_ulong (ACE_static_cast (CORBA::ULong, value));
      TAO::Unknown_IDL_Type *impl = 0;
      ACE_NEW (impl,
               TAO::Unknown_IDL_Type (tc._retn (),
                                      cdr.begin (),
                                      TAO_ENCAP_BYTE_ORDER,
                                      1));
      member.label.replace (impl);
      break;
    }
    default:
      break;
  }
}
