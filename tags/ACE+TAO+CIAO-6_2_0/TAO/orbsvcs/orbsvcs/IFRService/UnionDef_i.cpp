// $Id$

#include "orbsvcs/IFRService/UnionDef_i.h"
#include "orbsvcs/IFRService/RecursDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO_UnionDef_i::def_kind (void)
{
  return CORBA::dk_Union;
}

void
TAO_UnionDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_UnionDef_i::destroy_i (void)
{
  // Destroy our members.
  TAO_Container_i::destroy_i ();

  // Destroy ourself.
  TAO_Contained_i::destroy_i ();
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::type_i (void)
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  //---------------------------------------------------------------------------
  // Have we already seen this union definition at an outer scope?
  // If yes, return a recursive type code to signal the nesting.
  // If not, record this new union id in our stack (it will automatically be
  // removed when NowSeenThis goes out of scope).
  //---------------------------------------------------------------------------

  if (TAO_RecursiveDef_OuterScopes::SeenBefore( id ))
    return this->repo_->tc_factory ()->
                 create_recursive_tc ( id.c_str ());

  TAO_RecursiveDef_OuterScopes NowSeenThis( id );

  //---------------------------------------------------------------------------
  // Create a new type code for this structure; the create_union_tc() call
  // that follows may recursivly call this method again if one of its children
  // refers to a union (which is the point of the above NowSeenThis stack).
  //---------------------------------------------------------------------------

  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  CORBA::TypeCode_var tc = this->discriminator_type_i ();

  CORBA::UnionMemberSeq_var members = this->members_i ();

  return this->repo_->tc_factory ()->create_union_tc (id.c_str (),
                                                      name.c_str (),
                                                      tc.in (),
                                                      members.in ());
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::discriminator_type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->discriminator_type_i ();
}

CORBA::TypeCode_ptr
TAO_UnionDef_i::discriminator_type_i (void)
{
  ACE_TString disc_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "disc_path",
                                            disc_path);

  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (disc_path,
                                            this->repo_);
  if (0 == impl)
  {
     throw CORBA::OBJECT_NOT_EXIST();
  }

  return impl->type_i ();
}

CORBA::IDLType_ptr
TAO_UnionDef_i::discriminator_type_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->discriminator_type_def_i ();
}

CORBA::IDLType_ptr
TAO_UnionDef_i::discriminator_type_def_i (void)
{
  ACE_TString disc_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "disc_path",
                                            disc_path);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (disc_path,
                                              this->repo_);

  return CORBA::IDLType::_narrow (obj.in ());
}

void
TAO_UnionDef_i::discriminator_type_def (
    CORBA::IDLType_ptr discriminator_type_def
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->discriminator_type_def_i (discriminator_type_def);
}

void
TAO_UnionDef_i::discriminator_type_def_i (
    CORBA::IDLType_ptr discriminator_type_def
  )
{
  char *disc_path =
    TAO_IFR_Service_Utils::reference_to_path (discriminator_type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "disc_path",
                                            disc_path);
}

CORBA::UnionMemberSeq *
TAO_UnionDef_i::members (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->members_i ();
}

CORBA::UnionMemberSeq *
TAO_UnionDef_i::members_i (void)
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

  CORBA::ULong size = static_cast<CORBA::ULong> (key_queue.size ());

  CORBA::UnionMemberSeq *members = 0;
  ACE_NEW_THROW_EX (members,
                    CORBA::UnionMemberSeq (size),
                    CORBA::NO_MEMORY ());

  members->length (size);
  CORBA::UnionMemberSeq_var retval = members;

  ACE_TString name, path;
  ACE_Configuration_Section_Key next_key, entry_key;
  CORBA::Object_var obj;
  TAO_IDLType_i *impl = 0;

  // Store to replace below.
  ACE_Configuration_Section_Key key_holder = this->section_key_;

  for (CORBA::ULong k = 0; k < size; ++k)
    {
      key_queue.dequeue_head (next_key);

      this->repo_->config ()->get_string_value (next_key,
                                                "name",
                                                name);

      retval[k].name = name.c_str ();

      this->fetch_label (next_key,
                         retval[k]);

      this->repo_->config ()->get_string_value (next_key,
                                                "path",
                                                path);

      obj = TAO_IFR_Service_Utils::path_to_ir_object (path,
                                                      this->repo_);

      retval[k].type_def = CORBA::IDLType::_narrow (obj.in ());

      impl = TAO_IFR_Service_Utils::path_to_idltype (path,
                                                     this->repo_);
      if (0 == impl)
      {
        throw CORBA::OBJECT_NOT_EXIST();
      }

      retval[k].type = impl->type_i ();

      // If this union contains a nested union (of another type) at
      // some level, the above code will have changed the section key
      // so we have to replace it with the value we stored above.
      this->section_key (key_holder);
    }

  return retval._retn ();
}

void
TAO_UnionDef_i::members (const CORBA::UnionMemberSeq &members)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->members_i (members);
}

void
TAO_UnionDef_i::members_i (const CORBA::UnionMemberSeq &members)
{
  // Destroy our old refs.
  TAO_Container_i::destroy_references_i ();

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
                         members[i].label);
    }
}

void
TAO_UnionDef_i::fetch_label (const ACE_Configuration_Section_Key member_key,
                             CORBA::UnionMember &member)
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
    this->discriminator_type_i ();

  CORBA::TCKind kind = tc->kind ();

  switch (kind)
  {
    case CORBA::tk_char:
      member.label <<= CORBA::Any::from_char (static_cast<CORBA::Char> (value));
      break;
    case CORBA::tk_wchar:
      member.label <<= CORBA::Any::from_wchar (static_cast<CORBA::WChar> (value));
      break;
    case CORBA::tk_boolean:
      member.label <<= CORBA::Any::from_boolean (static_cast<CORBA::Boolean> (value));
      break;
    case CORBA::tk_short:
      member.label <<= static_cast<CORBA::Short> (value);
      break;
    case CORBA::tk_ushort:
      member.label <<= static_cast<CORBA::UShort> (value);
      break;
    case CORBA::tk_long:
      member.label <<= static_cast<CORBA::Long> (value);
      break;
    case CORBA::tk_ulong:
      member.label <<= static_cast<CORBA::ULong> (value);
      break;
    case CORBA::tk_longlong:
      member.label <<= static_cast<CORBA::LongLong> (value);
      break;
    case CORBA::tk_ulonglong:
      member.label <<= static_cast<CORBA::ULongLong> (value);
      break;
    case CORBA::tk_enum:
    {
      TAO_OutputCDR cdr;
      cdr.write_ulong (static_cast<CORBA::ULong> (value));
      TAO_InputCDR in_cdr (cdr);
      TAO::Unknown_IDL_Type *impl = 0;
      ACE_NEW (impl,
               TAO::Unknown_IDL_Type (tc.in (),
                                      in_cdr));
      member.label.replace (impl);
      break;
    }
    default:
      break;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
