/* -*- C++ -*- */
// $Id$

#include "concrete_classes.h"
#include "Repository_i.h"
#include "IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           Contained_i,
           "$Id$")
           
const char *TAO_Contained_i::tmp_name_holder_ = 0;

TAO_Contained_i::TAO_Contained_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo)
{
}

TAO_Contained_i::~TAO_Contained_i (void)
{
}

void
TAO_Contained_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Contained_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  // We'll get the last segment of this below.
  ACE_TString path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            id.c_str (),
                                            path);

  // Remove the repo id.
  this->repo_->config ()->remove_value (this->repo_->repo_ids_key (),
                                        id.c_str ());

  // Remove the database entry.
  ACE_TString container_id;
  ACE_Configuration_Section_Key parent_key;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  if (container_id == "")
    {
      parent_key = this->repo_->root_key ();
    }
  else
    {
      ACE_TString parent_path;
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                container_id.c_str (),
                                                parent_path);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           parent_path,
                                           parent_key,
                                           0);
    }

  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (parent_key,
                                        "defns",
                                        0,
                                        defns_key);

  ACE_TString last_seg = path.substr (path.rfind ('\\') + 1);
  this->repo_->config ()->remove_section (defns_key,
                                          last_seg.c_str (),
                                          1);
}

char *
TAO_Contained_i::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_Contained_i::id_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString retval;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            retval);

  return CORBA::string_dup (retval.c_str ());
}

void
TAO_Contained_i::id (const char *id
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->id_i (id
              ACE_ENV_ARG_PARAMETER);
}

void
TAO_Contained_i::id_i (const char *id
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString tmp;
  if (this->repo_->config ()->get_string_value (
                                  this->repo_->repo_ids_key (),
                                  id,
                                  tmp
                                )
       == 0)
    {
      // Repository id already exists.
      ACE_THROW (CORBA::BAD_PARAM (2, CORBA::COMPLETED_NO));
    }

  // Get the old id.
  ACE_TString old_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            old_id);

  ACE_TString path;

  // Fetch our path.
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            old_id.c_str (),
                                            path);

  // Remove the old id.
  this->repo_->config ()->remove_value (this->repo_->repo_ids_key (),
                                        old_id.c_str ());

  // Save our path under the new id.
  this->repo_->config ()->set_string_value (this->repo_->repo_ids_key (),
                                            id,
                                            path);

  // Store the new id locally as well.
  this->repo_->config ()->set_string_value (this->section_key_,
                                            "id",
                                            id);
}

char *
TAO_Contained_i::name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_Contained_i::name_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString retval;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            retval);

  return CORBA::string_dup (retval.c_str ());
}

char *
TAO_Contained_i::version (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_Contained_i::version_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString retval;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "version",
                                            retval);

  return CORBA::string_dup (retval.c_str ());
}

void
TAO_Contained_i::name (const char *name
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->name_i (name
                ACE_ENV_ARG_PARAMETER);
}

void
TAO_Contained_i::name_i (const char *name
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check if name already exists in this container.
  CORBA::Boolean exists = this->name_exists (name
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (exists)
    {
      ACE_THROW (CORBA::BAD_PARAM (3, CORBA::COMPLETED_NO));
    }

  // Reset the local name.
  this->repo_->config ()->set_string_value (this->section_key_,
                                            "name",
                                            name);

  // Update the absolute name.
  ACE_TString absolute_name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "absolute_name",
                                            absolute_name);

  absolute_name = absolute_name.substr (0, absolute_name.rfind (':') + 1);
  absolute_name += name;

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "absolute_name",
                                            absolute_name);

  this->contents_name_update (absolute_name,
                              this->section_key_);
}

void
TAO_Contained_i::version (const char *version
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->version_i (version
                   ACE_ENV_ARG_PARAMETER);
}

void
TAO_Contained_i::version_i (const char *version
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_string_value (this->section_key_,
                                            "version",
                                            version);
}

CORBA::Container_ptr
TAO_Contained_i::defined_in (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::Container::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Container::_nil ());

  return this->defined_in_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Container_ptr
TAO_Contained_i::defined_in_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  if (container_id == "")
    {
      // The repository itself is our container.
      return this->repo_->repo_objref ();
    }

  ACE_TString container_path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            container_id.c_str (),
                                            container_path);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (container_path,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Container::_nil ());

  CORBA::Container_var retval = CORBA::Container::_narrow (obj.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Container::_nil ());

  return retval._retn ();
}

char *
TAO_Contained_i::absolute_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->absolute_name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_Contained_i::absolute_name_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString absolute_name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "absolute_name",
                                            absolute_name);

  return CORBA::string_dup (absolute_name.c_str ());
}

CORBA::Repository_ptr
TAO_Contained_i::containing_repository (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->repo_->repo_objref ();
}

void
TAO_Contained_i::move (CORBA::Container_ptr new_container,
                       const char *new_name,
                       const char *new_version
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->move_i (new_container,
                new_name,
                new_version,
                1
                ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_Contained_i::name_exists (const char *name
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  if (container_id == "")
    {
      TAO_Contained_i::tmp_name_holder_ = name;
      ACE_Configuration_Section_Key root_key = this->repo_->root_key ();
      TAO_IFR_Service_Utils::name_exists (&TAO_Contained_i::same_as_tmp_name,
                                          root_key,
                                          this->repo_,
                                          CORBA::dk_Repository
                                          ACE_ENV_ARG_PARAMETER);
      return 0;
    }

  ACE_TString container_path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            container_id.c_str (),
                                            container_path);

  ACE_Configuration_Section_Key container_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       container_path,
                                       container_key,
                                       0);

  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (container_key,
                                        "defns",
                                        0,
                                        defns_key);

  ACE_TString section_name;
  int index = 0;

  while (this->repo_->config ()->enumerate_sections (defns_key,
                                                     index++,
                                                     section_name)
          == 0)
  {
    ACE_Configuration_Section_Key defn_key;
    this->repo_->config ()->open_section (defns_key,
                                          section_name.c_str (),
                                          0,
                                          defn_key);

    ACE_TString defn_name;
    this->repo_->config ()->get_string_value (defn_key,
                                              "name",
                                              defn_name);

    if (defn_name == name)
      {
        return 1;
      }
  }

  return 0;
}

void
TAO_Contained_i::contents_name_update (ACE_TString stem,
                                       ACE_Configuration_Section_Key key)
{
  ACE_Configuration_Section_Key defns_key;
  if (this->repo_->config ()->open_section (key,
                                            "defns",
                                            0,
                                            defns_key)
       != 0)
    {
      return;
    }

  int index = 0;
  ACE_TString sub_section;
  while (this->repo_->config ()->enumerate_sections (defns_key,
                                                     index++,
                                                     sub_section)
          == 0)
    {
      ACE_Configuration_Section_Key defn_key;

      this->repo_->config ()->open_section (defns_key,
                                            sub_section.c_str (),
                                            0,
                                            defn_key);

      ACE_TString name;
      this->repo_->config ()->get_string_value (defn_key,
                                                "name",
                                                name);

      ACE_TString absolute_name = stem + "::" + name;
      this->repo_->config ()->set_string_value (defn_key,
                                                "absolute_name",
                                                absolute_name);

      this->contents_name_update (absolute_name,
                                  defn_key);
    }
}

int
TAO_Contained_i::same_as_tmp_name (const char *name)
{
  return ACE_OS::strcmp (TAO_Contained_i::tmp_name_holder_, name) == 0;
}

void
TAO_Contained_i::move_i (CORBA::Container_ptr new_container,
                         const char *new_name,
                         const char *new_version,
                         CORBA::Boolean cleanup
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Repository_var my_repo = this->repo_->repo_objref ();

  CORBA::DefinitionKind container_dk = 
    TAO_IFR_Service_Utils::reference_to_def_kind (new_container,
                                                  this->repo_);

  ACE_Configuration_Section_Key container_key;
  TAO_Container_i *container_impl = 0;
  TAO_Contained_i *contained_impl = 0;

  // Check that we're not trying to move to a different repository,
  // and set the container impl, used in each switch case below.
  if (container_dk == CORBA::dk_Repository)
    {
      if (my_repo.in () != new_container)
        {
          ACE_THROW (CORBA::BAD_PARAM (4, 
                                       CORBA::COMPLETED_NO));
        }

      container_key = this->repo_->root_key ();

      container_impl = this->repo_;
    }
  else
    {
      // The only case where a container is not also a contained is
      // CORBA::Repository, which is covered by the other IF branch, so
      // we're ok here.
      ACE_TString container_path (
          TAO_IFR_Service_Utils::reference_to_path (new_container)
        );
      contained_impl = 
        TAO_IFR_Service_Utils::path_to_contained (container_path,
                                                  this->repo_);

      CORBA::Repository_var your_repo =
        contained_impl->containing_repository (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (my_repo.in () != your_repo.in ())
        {
          ACE_THROW (CORBA::BAD_PARAM (4, CORBA::COMPLETED_NO));
        }

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           container_path,
                                           container_key,
                                           0);

      container_impl = 
        TAO_IFR_Service_Utils::path_to_container (container_path,
                                                  this->repo_);
    }

  CORBA::DefinitionKind my_dk = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // If an exception is thrown, this must be restored.
  ACE_TString my_path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            id.in (),
                                            my_path);

  // Remove the repo id, because the create_* function call will add it
  // again with a different path value.
  this->repo_->config ()->remove_value (this->repo_->repo_ids_key (),
                                        id.in ());

  // Used for cleanup at the end of this function.
  ACE_Configuration_Section_Key old_section_key = this->section_key_;

  CORBA::String_var container_id = 
    contained_impl->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TString new_id (container_id.in ());
  new_id = 
    new_id.substr (0, new_id.rfind (':')) + "/" + new_name + ":" + new_version;

  ACE_TRY
    {
      // Call the appropriate create function.
      switch (my_dk)
      {
        case CORBA::dk_Enum:
        {
          TAO_EnumDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::EnumMemberSeq_var members = 
            impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Struct
              || container_dk == CORBA::dk_Union
              || container_dk == CORBA::dk_Exception)
            {
              this->move_pre_process (new_container,
                                      my_path.c_str (),
                                      new_name
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          CORBA::EnumDef_var new_defn =
            container_impl->create_enum_i (new_id.c_str (),
                                           new_name,
                                           new_version,
                                           members.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Alias:
        {
          TAO_AliasDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::IDLType_var otype = 
            impl.original_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          container_impl->create_alias_i (new_id.c_str (),
                                          new_name,
                                          new_version,
                                          otype.in ()
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Native:
        {
          container_impl->create_native_i (new_id.c_str (),
                                           new_name,
                                           new_version
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_ValueBox:
        {
          TAO_ValueBoxDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::IDLType_var otype = 
            impl.original_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          container_impl->create_value_box_i (new_id.c_str (),
                                              new_name,
                                              new_version,
                                              otype.in ()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Struct:
        {
          TAO_StructDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::StructMemberSeq_var members = 
            impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Struct
              || container_dk == CORBA::dk_Union
              || container_dk == CORBA::dk_Exception)
            {
              this->move_pre_process (new_container,
                                      my_path.c_str (),
                                      new_name
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          CORBA::StructDef_var new_defn =
            container_impl->create_struct_i (new_id.c_str (),
                                             new_name,
                                             new_version,
                                             members.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->move_contents (new_defn.in ()
                               ACE_ENV_ARG_PARAMETER);

          break;
        }
        case CORBA::dk_Union:
        {
          TAO_UnionDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::IDLType_var disc_type =
            impl.discriminator_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::UnionMemberSeq_var members = 
            impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Struct
              || container_dk == CORBA::dk_Union
              || container_dk == CORBA::dk_Exception)
            {
              this->move_pre_process (new_container,
                                      my_path.c_str (),
                                      new_name
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          CORBA::UnionDef_var new_defn =
            container_impl->create_union_i (new_id.c_str (),
                                            new_name,
                                            new_version,
                                            disc_type.in (),
                                            members.in ()
                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->move_contents (new_defn.in ()
                               ACE_ENV_ARG_PARAMETER);

          break;
        }
        case CORBA::dk_Module:
        {
          CORBA::ModuleDef_var new_defn =
            container_impl->create_module_i (new_id.c_str (),
                                             new_name,
                                             new_version
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->move_contents (new_defn.in ()
                               ACE_ENV_ARG_PARAMETER);

          break;
        }
        case CORBA::dk_Exception:
        {
          TAO_ExceptionDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::StructMemberSeq_var members = 
            impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ExceptionDef_var new_defn =
            container_impl->create_exception_i (new_id.c_str (),
                                                new_name,
                                                new_version,
                                                members.in ()
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->move_contents (new_defn.in ()
                               ACE_ENV_ARG_PARAMETER);

          break;
        }
        case CORBA::dk_Interface:
        {
          TAO_InterfaceDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::InterfaceDefSeq_var bases =
            impl.base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::InterfaceDef_var new_defn =
            container_impl->create_interface_i (new_id.c_str (),
                                                new_name,
                                                new_version,
                                                bases.in ()
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->move_contents (new_defn.in ()
                               ACE_ENV_ARG_PARAMETER);

          break;
        }
        case CORBA::dk_Constant:
        {
          TAO_ConstantDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::IDLType_var type_def = 
            impl.type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Any_var value = impl.value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          container_impl->create_constant_i (new_id.c_str (),
                                             new_name,
                                             new_version,
                                             type_def.in (),
                                             value.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Attribute:
        {
          TAO_AttributeDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::IDLType_var type_def = 
            impl.type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::AttributeMode mode = 
            impl.mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Interface)
            {
              TAO_InterfaceDef_i idef (this->repo_);
              idef.section_key (container_key);

              idef.create_attribute_i (new_id.c_str (),
                                       new_name,
                                       new_version,
                                       type_def.in (),
                                       mode
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;
            }
          else if (container_dk == CORBA::dk_Value)
            {
              TAO_ValueDef_i vdef (this->repo_);
              vdef.section_key (container_key);

              vdef.create_attribute_i (new_id.c_str (),
                                       new_name,
                                       new_version,
                                       type_def.in (),
                                       mode
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;
            }
          else
            {
              ACE_THROW (CORBA::BAD_PARAM (4, CORBA::COMPLETED_NO));
            }
        }
        case CORBA::dk_Operation:
        {
          TAO_OperationDef_i impl (this->repo_);
          impl.section_key (this->section_key_);

          CORBA::IDLType_var result = 
            impl.result_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::OperationMode mode = 
            impl.mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ParDescriptionSeq_var params = 
            impl.params_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ExceptionDefSeq_var exceptions =
            impl.exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ContextIdSeq_var contexts = 
            impl.contexts_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Interface)
            {
              TAO_InterfaceDef_i idef (this->repo_);
              idef.section_key (container_key);

              idef.create_operation_i (new_id.c_str (),
                                       new_name,
                                       new_version,
                                       result.in (),
                                       mode,
                                       params.in (),
                                       exceptions.in (),
                                       contexts.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;
            }
          else if (container_dk == CORBA::dk_Value)
            {
              TAO_ValueDef_i vdef (this->repo_);
              impl.section_key (container_key);

              vdef.create_operation_i (new_id.c_str (),
                                       new_name,
                                       new_version,
                                       result.in (),
                                       mode,
                                       params.in (),
                                       exceptions.in (),
                                       contexts.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;
            }
          else
            {
              ACE_THROW (CORBA::BAD_PARAM (4, CORBA::COMPLETED_NO));
            }
        }
        case CORBA::dk_Value:
        case CORBA::dk_Home:
        case CORBA::dk_Component:
        case CORBA::dk_Factory:
        case CORBA::dk_Finder:
        case CORBA::dk_Provides:
        case CORBA::dk_Uses:
        case CORBA::dk_Emits:
        case CORBA::dk_Publishes:
        case CORBA::dk_Consumes:
        case CORBA::dk_ValueMember:
        case CORBA::dk_Event:
          // TODO
        default:
          break;
      }
    }
  ACE_CATCHANY
    {
      // If the move was not successful, we must restore this.
      this->repo_->config ()->set_string_value (
                                  this->repo_->repo_ids_key (),
                                  id.in (),
                                  my_path
                                );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  if (cleanup)
    {
      // Remove the old database entry.
      ACE_TString container_id;
      ACE_Configuration_Section_Key parent_key;

      this->repo_->config ()->get_string_value (old_section_key,
                                                "container_id",
                                                container_id);

      if (container_id == "")
        {
          parent_key = this->repo_->root_key ();
        }
      else
        {
          ACE_TString parent_path;
          this->repo_->config ()->get_string_value (
                                      this->repo_->repo_ids_key (),
                                      container_id.c_str (),
                                      parent_path
                                    );

          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               parent_path,
                                               parent_key,
                                               0);
        }

      ACE_Configuration_Section_Key defns_key;
      this->repo_->config ()->open_section (parent_key,
                                            "defns",
                                            0,
                                            defns_key);

      ACE_TString last_seg = my_path.substr (my_path.rfind ('\\') + 1);
      this->repo_->config ()->remove_section (defns_key,
                                              last_seg.c_str (),
                                              1);
    }
}

void
TAO_Contained_i::move_pre_process (CORBA::Container_ptr container,
                                   const char *contained_path,
                                   const char *name
                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *container_path = 
    TAO_IFR_Service_Utils::reference_to_path (container);

  ACE_Configuration_Section_Key container_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       container_path,
                                       container_key,
                                       0);

  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (container_key,
                                        "refs",
                                        0,
                                        refs_key);

  int index = 0;
  ACE_TString section_name;
  while (this->repo_->config ()->enumerate_sections (refs_key,
                                                     index++,
                                                     section_name)
          == 0)
    {
      ACE_Configuration_Section_Key ref_key;
      this->repo_->config ()->open_section (refs_key,
                                            section_name.c_str (),
                                            0,
                                            ref_key);

      ACE_TString ref_name;
      this->repo_->config ()->get_string_value (ref_key,
                                                "name",
                                                ref_name);

      ACE_TString path;
      this->repo_->config ()->get_string_value (ref_key,
                                                "path",
                                                path);

      if (ref_name == name && path == contained_path)
        {
          ref_name += this->repo_->extension ();
          this->repo_->config ()->set_string_value (ref_key,
                                                    "name",
                                                    ref_name);

          break;
        }
    }
}

void
TAO_Contained_i::move_contents (CORBA::Container_ptr new_container
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString name;
  ACE_TString version;

  ACE_Configuration_Section_Key defns_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "defns",
                                          0,
                                          defns_key);

  if (status == 0)
    {
      u_int count = 0;
      this->repo_->config ()->get_integer_value (defns_key,
                                                 "count",
                                                 count);

      for (u_int i = 0; i < count; ++i)
        {
          ACE_Configuration_Section_Key defn_key;
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          status =
            this->repo_->config ()->open_section (defns_key,
                                                  stringified,
                                                  0,
                                                  defn_key);

          if (status == 0)
            {
              this->repo_->config ()->get_string_value (defn_key,
                                                        "name",
                                                        name);

              this->repo_->config ()->get_string_value (defn_key,
                                                        "version",
                                                        version);
              u_int kind = 0;
              this->repo_->config ()->get_integer_value (defn_key,
                                                         "def_kind",
                                                         kind);
              CORBA::DefinitionKind def_kind =
                ACE_static_cast (CORBA::DefinitionKind, kind);

              TAO_Contained_i *impl = this->repo_->select_contained (def_kind);
              impl->section_key (defn_key);

              impl->move_i (new_container,
                            name.c_str (),
                            version.c_str (),
                            0
                            ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }

  CORBA::DefinitionKind kind = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Specific to InterfaceDef_i and ValueDef_i
  if (kind == CORBA::dk_Interface || kind == CORBA::dk_Value)
    {
      int index = 0;
      ACE_TString section_name;

      // Attributes
      ACE_Configuration_Section_Key attrs_key;
      int status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "attrs",
                                              0,
                                              attrs_key);

      if (status == 0)
        {
          while (this->repo_->config ()->enumerate_sections (attrs_key,
                                                             index++,
                                                             section_name)
                  == 0)
            {
              ACE_Configuration_Section_Key attr_key;
              this->repo_->config ()->open_section (attrs_key,
                                                    section_name.c_str (),
                                                    0,
                                                    attr_key);

              this->repo_->config ()->get_string_value (attr_key,
                                                        "name",
                                                        name);

              this->repo_->config ()->get_string_value (attr_key,
                                                        "version",
                                                        version);

              TAO_AttributeDef_i impl (this->repo_);
              impl.section_key (attr_key);

              impl.move_i (new_container,
                           name.c_str (),
                           version.c_str (),
                           0
                           ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }

      index = 0;

      // Operations
      ACE_Configuration_Section_Key ops_key;
      status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "ops",
                                              0,
                                              ops_key);

      if (status == 0)
        {
          while (this->repo_->config ()->enumerate_sections (ops_key,
                                                             index++,
                                                             section_name)
                  == 0)
            {
              ACE_Configuration_Section_Key op_key;
              this->repo_->config ()->open_section (ops_key,
                                                    section_name.c_str (),
                                                    0,
                                                    op_key);

              this->repo_->config ()->get_string_value (op_key,
                                                        "name",
                                                        name);

              this->repo_->config ()->get_string_value (op_key,
                                                        "version",
                                                        version);

              TAO_OperationDef_i impl (this->repo_);
              impl.section_key (op_key);

              impl.move_i (new_container,
                           name.c_str (),
                           version.c_str (),
                           0
                           ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }
}

