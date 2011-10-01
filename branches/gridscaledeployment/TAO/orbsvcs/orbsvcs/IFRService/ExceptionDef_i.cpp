// $Id$

#include "orbsvcs/IFRService/ExceptionDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ExceptionDef_i::TAO_ExceptionDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_Container_i (repo)
{
}

TAO_ExceptionDef_i::~TAO_ExceptionDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ExceptionDef_i::def_kind (void)
{
  return CORBA::dk_Exception;
}

void
TAO_ExceptionDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_ExceptionDef_i::destroy_i (void)
{
  // Destroy our members.
  TAO_Container_i::destroy_i ();

  // Destroy ourself.
  TAO_Contained_i::destroy_i ();
}

CORBA::Contained::Description *
TAO_ExceptionDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_ExceptionDef_i::describe_i (void)
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind ();

  CORBA::ExceptionDescription ed;

  ed.name = this->name_i ();

  ed.id = this->id_i ();

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ed.defined_in = container_id.c_str ();

  ed.version = this->version_i ();

  ed.type = this->type_i ();

  retval->value <<= ed;

  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_ExceptionDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_ExceptionDef_i::type_i (void)
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
    this->members_i ();

  return this->repo_->tc_factory ()->create_exception_tc (
                                         id.c_str (),
                                         name.c_str (),
                                         members.in ()
                                       );
}

CORBA::StructMemberSeq *
TAO_ExceptionDef_i::members (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->members_i ();
}

CORBA::StructMemberSeq *
TAO_ExceptionDef_i::members_i (void)
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
            static_cast<CORBA::DefinitionKind> (kind);

          kind_queue.enqueue_tail (def_kind);
        }
    }

  CORBA::ULong size = static_cast<CORBA::ULong> (kind_queue.size ());

  CORBA::StructMemberSeq *members = 0;
  ACE_NEW_THROW_EX (members,
                    CORBA::StructMemberSeq (size),
                    CORBA::NO_MEMORY ());

  members->length (size);

  CORBA::StructMemberSeq_var retval = members;

  ACE_TString name;
  ACE_TString path;
  CORBA::DefinitionKind kind = CORBA::dk_none;
  CORBA::Object_var obj;
  ACE_Configuration_Section_Key member_key;
  TAO_IDLType_i *impl = 0;

  // Store to replace below.
  ACE_Configuration_Section_Key key_holder = this->section_key_;

  for (CORBA::ULong k = 0; k < size; ++k)
    {
      name_queue.dequeue_head (name);
      retval[k].name = name.c_str ();

      kind_queue.dequeue_head (kind);
      path_queue.dequeue_head (path);

      obj =
        TAO_IFR_Service_Utils::create_objref (kind,
                                              path.c_str (),
                                              this->repo_);

      retval[k].type_def = CORBA::IDLType::_narrow (obj.in ());

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           path,
                                           member_key,
                                           0);

      impl = TAO_IFR_Service_Utils::path_to_idltype (path,
                                                     this->repo_);

      retval[k].type = impl->type_i ();

      // If this struct contains a nested struct (of another type) at
      // some level, the above code will have changed the section key
      // so we have to replace it with the value we stored above.
      this->section_key (key_holder);
    }

  return retval._retn ();
}

void
TAO_ExceptionDef_i::members (const CORBA::StructMemberSeq &members)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->members_i (members);
}

void
TAO_ExceptionDef_i::members_i (const CORBA::StructMemberSeq &members)
{
  // Destroy our old refs.
  TAO_Container_i::destroy_references_i ();

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

TAO_END_VERSIONED_NAMESPACE_DECL
