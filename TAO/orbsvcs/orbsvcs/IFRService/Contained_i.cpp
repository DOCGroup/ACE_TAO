/* -*- C++ -*- */
// $Id$

#include "concrete_classes.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, Contained_i, "$Id$")

TAO_Contained_i::TAO_Contained_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key)
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
                                            path.c_str ());

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

CORBA_Container_ptr
TAO_Contained_i::defined_in (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_Container::_nil ());

  return this->defined_in_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Container_ptr
TAO_Contained_i::defined_in_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  if (container_id == "")
    // The repository itself it our container.
    return this->repo_->repo_objref (); // already duplicated.

  ACE_TString container_path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            container_id.c_str (),
                                            container_path);

  ACE_Configuration_Section_Key container_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       container_path,
                                       container_key);

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (container_key,
                                             "def_kind",
                                             kind);

  CORBA::DefinitionKind def_kind =
    ACE_static_cast (CORBA::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    container_path.c_str ()
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_Container::_nil ());

  CORBA_Container_var retval = CORBA_Container::_narrow (obj.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_Container::_nil ());

  return retval._retn ();
}

char *
TAO_Contained_i::absolute_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

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

CORBA_Repository_ptr
TAO_Contained_i::containing_repository (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_Repository::_nil ());

  return this->containing_repository_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Repository_ptr
TAO_Contained_i::containing_repository_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->repo_->repo_objref ();
}

void
TAO_Contained_i::move (CORBA_Container_ptr new_container,
                       const char *new_name,
                       const char *new_version
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

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
      return this->repo_->name_exists (name
                                       ACE_ENV_ARG_PARAMETER);
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
      return 1;
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

void
TAO_Contained_i::move_i (CORBA_Container_ptr new_container,
                         const char *new_name,
                         const char *new_version,
                         CORBA::Boolean cleanup
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA_Repository_var my_repo =
    this->containing_repository_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::DefinitionKind container_dk =
    new_container->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_Configuration_Section_Key container_key;
  TAO_Container_i *container_impl = 0;

  // Check that we're not trying to move to a different repository,
  // and set the container impl, used in each switch case below.
  if (container_dk == CORBA::dk_Repository)
    {
      if (my_repo.in () != new_container)
        {
          ACE_THROW (CORBA::BAD_PARAM (4, CORBA::COMPLETED_NO));
        }

      container_key = this->repo_->root_key ();

      container_impl = this->repo_;
    }
  else
    {
      PortableServer::ObjectId_var oid =
        this->repo_->ir_poa ()->reference_to_id (new_container
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var container_path =
        PortableServer::ObjectId_to_string (oid.in ());

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           container_path.in (),
                                           container_key,
                                           0);

      TAO_Contained_i *impl =
        this->repo_->servant_factory ()->create_contained (container_key
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      auto_ptr<TAO_Contained_i> safety (impl);

      CORBA_Repository_var your_repo =
        impl->containing_repository_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (my_repo.in () != your_repo.in ())
        {
          ACE_THROW (CORBA::BAD_PARAM (4, CORBA::COMPLETED_NO));
        }

      container_impl =
        this->repo_->servant_factory ()->create_container (container_key
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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

  ACE_TRY
    {
      // Call the appropriate create function.
      switch (my_dk)
      {
        case CORBA::dk_Enum:
        {
          TAO_EnumDef_i impl (this->repo_,
                              this->section_key_);

          CORBA_EnumMemberSeq_var members = impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
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

          CORBA_EnumDef_var new_defn =
            container_impl->create_enum_i (id.in (),
                                           new_name,
                                           new_version,
                                           members.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Alias:
        {
          TAO_AliasDef_i impl (this->repo_,
                               this->section_key_);

          CORBA_IDLType_var otype = impl.original_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          container_impl->create_alias_i (id.in (),
                                          new_name,
                                          new_version,
                                          otype.in ()
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Native:
        {
          container_impl->create_native_i (id.in (),
                                           new_name,
                                           new_version
                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_ValueBox:
        {
          TAO_ValueBoxDef_i impl (this->repo_,
                                  this->section_key_);

          CORBA_IDLType_var otype = impl.original_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          container_impl->create_value_box_i (id.in (),
                                              new_name,
                                              new_version,
                                              otype.in ()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          break;
        }
        case CORBA::dk_Struct:
        {
          TAO_StructDef_i impl (this->repo_,
                                this->section_key_);

          CORBA_StructMemberSeq_var members = impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
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

          CORBA_StructDef_var new_defn =
            container_impl->create_struct_i (id.in (),
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
          TAO_UnionDef_i impl (this->repo_,
                               this->section_key_);

          CORBA_IDLType_var disc_type =
            impl.discriminator_type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA_UnionMemberSeq_var members = impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
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

          CORBA_UnionDef_var new_defn =
            container_impl->create_union_i (id.in (),
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
          CORBA_ModuleDef_var new_defn =
            container_impl->create_module_i (id.in (),
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
          TAO_ExceptionDef_i impl (this->repo_,
                                   this->section_key_);

          CORBA_StructMemberSeq_var members = impl.members_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA_ExceptionDef_var new_defn =
            container_impl->create_exception_i (id.in (),
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
          TAO_InterfaceDef_i impl (this->repo_,
                                   this->section_key_);

          CORBA_InterfaceDefSeq_var bases =
            impl.base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA_InterfaceDef_var new_defn =
            container_impl->create_interface_i (id.in (),
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
          TAO_ConstantDef_i impl (this->repo_,
                                  this->section_key_);

          CORBA_IDLType_var type_def = impl.type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Any_var value = impl.value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          container_impl->create_constant_i (id.in (),
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
          TAO_AttributeDef_i impl (this->repo_,
                                   this->section_key_);

          CORBA_IDLType_var type_def = impl.type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::AttributeMode mode = impl.mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Interface)
            {
              TAO_InterfaceDef_i idef (this->repo_,
                                       container_key);

              idef.create_attribute_i (id.in (),
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
              TAO_ValueDef_i vdef (this->repo_,
                                   container_key);

              vdef.create_attribute_i (id.in (),
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
          TAO_OperationDef_i impl (this->repo_,
                                   this->section_key_);

          CORBA_IDLType_var result = impl.result_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::OperationMode mode = impl.mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA_ParDescriptionSeq_var params = impl.params_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA_ExceptionDefSeq_var exceptions =
            impl.exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA_ContextIdSeq_var contexts = impl.contexts_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (container_dk == CORBA::dk_Interface)
            {
              TAO_InterfaceDef_i idef (this->repo_,
                                       container_key);

              idef.create_operation_i (id.in (),
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
              TAO_ValueDef_i vdef (this->repo_,
                                   container_key);

              vdef.create_operation_i (id.in (),
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
        case CORBA::dk_PrimaryKey:
        case CORBA::dk_Provides:
        case CORBA::dk_Uses:
        case CORBA::dk_Emits:
        case CORBA::dk_Publishes:
        case CORBA::dk_Consumes:
        case CORBA::dk_ValueMember:
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
                                  my_path.c_str ()
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

      ACE_TString last_seg = my_path.substr (my_path.rfind ('\\') + 1);
      this->repo_->config ()->remove_section (defns_key,
                                              last_seg.c_str (),
                                              1);
    }

  if (container_dk != CORBA::dk_Repository)
    delete container_impl;
}

void
TAO_Contained_i::move_pre_process (CORBA_Container_ptr container,
                                   const char *contained_path,
                                   const char *name
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var container_oid =
    this->repo_->ir_poa ()->reference_to_id (container
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var container_path =
    PortableServer::ObjectId_to_string (container_oid.in ());

  ACE_Configuration_Section_Key container_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       container_path.in (),
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
TAO_Contained_i::move_contents (CORBA_Container_ptr new_container
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
          CORBA::String_var section_name = this->int_to_string (i);
          status =
            this->repo_->config ()->open_section (defns_key,
                                                  section_name.in (),
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

              TAO_Contained_i *impl =
                this->repo_->servant_factory ()->create_contained (
                                                     defn_key
                                                     ACE_ENV_ARG_PARAMETER
                                                   );
              ACE_CHECK;

              auto_ptr<TAO_Contained_i> safety (impl);

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

              TAO_AttributeDef_i impl (this->repo_,
                                       attr_key);

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

              TAO_OperationDef_i impl (this->repo_,
                                       op_key);

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
