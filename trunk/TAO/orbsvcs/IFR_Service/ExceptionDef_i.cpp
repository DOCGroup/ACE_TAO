/* -*- C++ -*- */
// $Id$

#include "ExceptionDef_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, ExceptionDef_i, "$Id$")

TAO_ExceptionDef_i::TAO_ExceptionDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key)
{
}

TAO_ExceptionDef_i::~TAO_ExceptionDef_i (void)
{
}

IR::DefinitionKind
TAO_ExceptionDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Exception;
}

void 
TAO_ExceptionDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_ExceptionDef_i::destroy_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our members.
  TAO_Container_i::destroy_i (ACE_TRY_ENV);
  ACE_CHECK;
    
  // Destroy ourself.
  TAO_Contained_i::destroy_i (ACE_TRY_ENV);
  ACE_CHECK;
}

IR::Contained::Description *
TAO_ExceptionDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

IR::Contained::Description *
TAO_ExceptionDef_i::describe_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IR::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    IR::Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  IR::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  IR::ExceptionDescription ed;

  ed.name = this->name_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ed.id = this->id_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ed.defined_in = container_id.c_str ();

  ed.version = this->version_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ed.type = this->type_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  retval->value <<= ed;

  return retval._retn ();
}

CORBA::TypeCode_ptr 
TAO_ExceptionDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_ExceptionDef_i::type_i (CORBA::Environment &ACE_TRY_ENV)
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

  IR::StructMemberSeq_var members = this->members_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->repo_->tc_factory ()->create_exception_tc (id.c_str (),
                                                          name.c_str (),
                                                          members.in (),
                                                          ACE_TRY_ENV);
}

IR::StructMemberSeq *
TAO_ExceptionDef_i::members (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->members_i (ACE_TRY_ENV);
}

IR::StructMemberSeq *
TAO_ExceptionDef_i::members_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<IR::DefinitionKind> kind_queue;
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
      CORBA::String_var section_name = this->int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            section_name.in (),
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

          IR::DefinitionKind def_kind = 
            ACE_static_cast (IR::DefinitionKind, kind);

          kind_queue.enqueue_tail (def_kind);
        }
    }

  size_t size = kind_queue.size ();
                    
  IR::StructMemberSeq *members = 0;
  ACE_NEW_THROW_EX (members,
                    IR::StructMemberSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  members->length (size);

  IR::StructMemberSeq_var retval = members;

  ACE_TString name;
  ACE_TString path;
  IR::DefinitionKind kind = IR::dk_none;
  CORBA::Object_var obj;
  ACE_Configuration_Section_Key member_key;
  TAO_IDLType_i *impl = 0;

  for (size_t k = 0; k < size; k++)
    {
      name_queue.dequeue_head (name);

      retval[k].name = name.c_str ();

      kind_queue.dequeue_head (kind);

      path_queue.dequeue_head (path);

      obj = 
        this->repo_->servant_factory ()->create_objref (kind,
                                                        path.c_str (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    
      retval[k].type_def = IR::IDLType::_narrow (obj.in (),
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           path,
                                           member_key,
                                           0);

      impl =
        this->repo_->servant_factory ()->create_idltype (member_key,
                                                         ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_IDLType_i> safety (impl);
    
      retval[k].type = impl->type_i (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void 
TAO_ExceptionDef_i::members (const IR::StructMemberSeq &members,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->members_i (members,
                   ACE_TRY_ENV);
}

void 
TAO_ExceptionDef_i::members_i (const IR::StructMemberSeq &members,
                               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our old members, both refs and defns.
  TAO_Container_i::destroy_i (ACE_TRY_ENV);
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

      // Create a section for each new member. We just store the
      // member name and the path to its database entry.
      for (CORBA::ULong i = 0; i < count; i++)
        {
          ACE_Configuration_Section_Key member_key;
          CORBA::String_var section_name = this->int_to_string (i);
          this->repo_->config ()->open_section (refs_key,
                                                section_name.in (),
                                                1,
                                                member_key);

          ACE_TString name (members[i].name);
          this->repo_->config ()->set_string_value (member_key,
                                                    "name",
                                                    name);

          PortableServer::ObjectId_var oid = 
            this->repo_->ir_poa ()->reference_to_id (members[i].type_def,
                                                     ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::String_var path =
            PortableServer::ObjectId_to_string (oid.in ());

          this->repo_->config ()->set_string_value (member_key,
                                                    "path",
                                                    path.in ());
        }

      this->repo_->config ()->set_integer_value (refs_key,
                                                 "count",
                                                 count);
    }
}
