/* -*- C++ -*- */
// $Id$

#include "StructDef_i.h"
#include "Repository_i.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           StructDef_i, 
           "$Id$")

TAO_StructDef_i::TAO_StructDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo),
    TAO_Contained_i (repo),
    TAO_TypedefDef_i (repo),
    TAO_Container_i (repo)
{
}

TAO_StructDef_i::~TAO_StructDef_i (void)
{
}

CORBA::DefinitionKind
TAO_StructDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Struct;
}

void
TAO_StructDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_StructDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_StructDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_StructDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::StructMemberSeq_var members = this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_struct_tc (id.c_str (),
                                                       name.c_str (),
                                                       members.in ()
                                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::StructMemberSeq *
TAO_StructDef_i::members (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::StructMemberSeq *
TAO_StructDef_i::members_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;
  ACE_Unbounded_Queue<ACE_TString> name_queue;

  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "refs",
                                        0,
                                        refs_key);

  u_int count = 0;
  this->repo_->config ()->get_integer_value (refs_key,
                                             "count",
                                             count);

  for (u_int i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      this->repo_->config ()->open_section (refs_key,
                                            this->int_to_string (i),
                                            0,
                                            member_key);

      ACE_TString path;
      this->repo_->config ()->get_string_value (member_key,
                                                "path",
                                                path);

      ACE_Configuration_Section_Key entry_key;
      int status =
        this->repo_->config ()->expand_path (this->repo_->root_key (),
                                             path,
                                             entry_key,
                                             0);

      // This entry may have been removed.
      if (status == 0)
        {
          path_queue.enqueue_tail (path);

          ACE_TString name;
          this->repo_->config ()->get_string_value (member_key,
                                                    "name",
                                                    name);

          name_queue.enqueue_tail (name);

          u_int kind = 0;
          this->repo_->config ()->get_integer_value (entry_key,
                                                     "def_kind",
                                                     kind);

          CORBA::DefinitionKind def_kind =
            ACE_static_cast (CORBA::DefinitionKind, kind);

          kind_queue.enqueue_tail (def_kind);
        }
    }

  size_t size = kind_queue.size ();

  CORBA::StructMemberSeq *members = 0;
  ACE_NEW_THROW_EX (members,
                    CORBA::StructMemberSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  members->length (size);

  CORBA::StructMemberSeq_var retval = members;

  ACE_TString name, path;
  CORBA::DefinitionKind kind = CORBA::dk_none;
  CORBA::Object_var obj;
  ACE_Configuration_Section_Key member_key;
  TAO_IDLType_i *impl = 0;

  for (size_t k = 0; k < size; ++k)
    {
      name_queue.dequeue_head (name);
      retval[k].name = name.c_str ();
      kind_queue.dequeue_head (kind);
      path_queue.dequeue_head (path);

      obj = this->create_objref (kind,
                                 path.c_str ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[k].type_def = CORBA::IDLType::_narrow (obj.in ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           path,
                                           member_key,
                                           0);

      impl = this->path_to_idltype (path);

      retval[k].type = impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_StructDef_i::members (const CORBA::StructMemberSeq &members
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
TAO_StructDef_i::members_i (const CORBA::StructMemberSeq &members
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our old members, both refs and defns.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong count = members.length ();

  ACE_TString section_name;
  ACE_Configuration_Section_Key refs_key;

  this->repo_->config ()->open_section (this->section_key_,
                                        "refs",
                                        1,
                                        refs_key);
  char *path = 0;

  // Create a section for each new member. We just store the
  // member name and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      this->repo_->config ()->open_section (refs_key,
                                            this->int_to_string (i),
                                            1,
                                            member_key);

      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                members[i].name.in ());

      path = 
        this->reference_to_path (members[i].type_def.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                path);
    }

  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count);
}

