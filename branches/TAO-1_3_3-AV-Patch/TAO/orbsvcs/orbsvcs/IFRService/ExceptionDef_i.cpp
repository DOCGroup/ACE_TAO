/* -*- C++ -*- */
// $Id$

#include "ExceptionDef_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           ExceptionDef_i, 
           "$Id$")

TAO_ExceptionDef_i::TAO_ExceptionDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_Container_i (repo)
{
}

TAO_ExceptionDef_i::~TAO_ExceptionDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ExceptionDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Exception;
}

void
TAO_ExceptionDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ExceptionDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our members.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Destroy ourself.
  TAO_Contained_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Contained::Description *
TAO_ExceptionDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ExceptionDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ExceptionDescription ed;

  ed.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ed.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ed.defined_in = container_id.c_str ();

  ed.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ed.type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  retval->value <<= ed;

  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_ExceptionDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ExceptionDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::StructMemberSeq_var members = 
    this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_exception_tc (
                                         id.c_str (),
                                         name.c_str (),
                                         members.in ()
                                         ACE_ENV_ARG_PARAMETER
                                       );
}

CORBA::StructMemberSeq *
TAO_ExceptionDef_i::members (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::StructMemberSeq *
TAO_ExceptionDef_i::members_i (ACE_ENV_SINGLE_ARG_DECL)
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
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            stringified,
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

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, kind_queue.size ());

  CORBA::StructMemberSeq *members = 0;
  ACE_NEW_THROW_EX (members,
                    CORBA::StructMemberSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  members->length (size);

  CORBA::StructMemberSeq_var retval = members;

  ACE_TString name;
  ACE_TString path;
  CORBA::DefinitionKind kind = CORBA::dk_none;
  CORBA::Object_var obj;
  ACE_Configuration_Section_Key member_key;
  TAO_IDLType_i *impl = 0;

  for (CORBA::ULong k = 0; k < size; ++k)
    {
      name_queue.dequeue_head (name);
      retval[k].name = name.c_str ();

      kind_queue.dequeue_head (kind);
      path_queue.dequeue_head (path);

      obj =
        TAO_IFR_Service_Utils::create_objref (kind,
                                              path.c_str (),
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[k].type_def = CORBA::IDLType::_narrow (obj.in ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           path,
                                           member_key,
                                           0);

      impl = TAO_IFR_Service_Utils::path_to_idltype (path,
                                                     this->repo_);

      retval[k].type = impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_ExceptionDef_i::members (const CORBA::StructMemberSeq &members
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
TAO_ExceptionDef_i::members_i (const CORBA::StructMemberSeq &members
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our old members, both refs and defns.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong count = members.length ();

  // Exceptions can be empty.
  if (count > 0)
    {
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
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (refs_key,
                                                stringified,
                                                1,
                                                member_key);

          ACE_TString name (members[i].name);
          this->repo_->config ()->set_string_value (member_key,
                                                    "name",
                                                    name);

          path = 
            TAO_IFR_Service_Utils::reference_to_path (
                members[i].type_def.in ()
              );

          this->repo_->config ()->set_string_value (member_key,
                                                    "path",
                                                    path);
        }

      this->repo_->config ()->set_integer_value (refs_key,
                                                 "count",
                                                 count);
    }
}
