/* -*- C++ -*- */
// $Id$

#include "InterfaceDef_i.h"
#include "Repository_i.h"
#include "AttributeDef_i.h"
#include "OperationDef_i.h"

ACE_RCSID (IFRService, 
           InterfaceDef_i, 
           "$Id$")

TAO_InterfaceDef_i::TAO_InterfaceDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_InterfaceDef_i::~TAO_InterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_InterfaceDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Interface;
}

void
TAO_InterfaceDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_InterfaceDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our members.
  TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // This will get rid of the repo ids, which Contained_i::destroy()'s
  // call to remove_section (recursive = 1) will not get, and also
  // destroy the attribute's anonymous type, if any.
  this->destroy_special ("attrs" 
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_special ("ops" 
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Destroy ourself.
  TAO_Contained_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Contained::Description *
TAO_InterfaceDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_InterfaceDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::InterfaceDescription ifd;

  ifd.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ifd.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ifd.defined_in = container_id.c_str ();

  ifd.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong i = 0;
  CORBA::InterfaceDefSeq_var bases = 
    this->base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = bases->length ();
  CORBA::RepositoryIdSeq repo_ids (length);
  repo_ids.length (length);

  CORBA::String_var base_path;
  ACE_Configuration_Section_Key base_key;

  for (i = 0; i < length; ++i)
    {
      base_path = this->reference_to_path (bases[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           base_path.in (),
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      repo_ids[i] = impl.id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  ifd.base_interfaces = repo_ids;
  retval->value <<= ifd;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_InterfaceDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_InterfaceDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
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

  return this->repo_->tc_factory ()->create_interface_tc (
                                         id.c_str (),
                                         name.c_str ()
                                         ACE_ENV_ARG_PARAMETER
                                       );
}

CORBA::InterfaceDefSeq *
TAO_InterfaceDef_i::base_interfaces (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_InterfaceDef_i::base_interfaces_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  this->base_interfaces_recursive (kind_queue,
                                   path_queue);

  size_t size = kind_queue.size ();

  CORBA::InterfaceDefSeq *seq = 0;
  ACE_NEW_THROW_EX (seq,
                    CORBA::InterfaceDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  seq->length (size);
  CORBA::InterfaceDefSeq_var retval = seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      CORBA::DefinitionKind def_kind;
      kind_queue.dequeue_head (def_kind);

      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj = this->path_to_ir_object (path
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA::InterfaceDef::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_InterfaceDef_i::base_interfaces (const CORBA::InterfaceDefSeq &base_interfaces
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->base_interfaces_i (base_interfaces
                           ACE_ENV_ARG_PARAMETER);
}

void
TAO_InterfaceDef_i::base_interfaces_i (const CORBA::InterfaceDefSeq &base_interfaces
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If we are an abstract interface, all our base interfaces must be
  // abstract as well.
  CORBA::DefinitionKind def_kind = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (def_kind == CORBA::dk_AbstractInterface)
    {
      CORBA::ULong length = base_interfaces.length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          def_kind = base_interfaces[i]->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          if (def_kind != CORBA::dk_AbstractInterface)
            {
              ACE_THROW (CORBA::BAD_PARAM (11,
                                           CORBA::COMPLETED_NO));
            }
        }
    }

  // Remove the old base interfaces.
  this->repo_->config ()->remove_section (this->section_key_,
                                          "inherited",
                                          0);

  CORBA::ULong length = base_interfaces.length ();

  ACE_Configuration_Section_Key inherited_key;

  this->repo_->config ()->open_section (this->section_key_,
                                        "inherited",
                                        1,
                                        inherited_key);

  CORBA::String_var name, inherited_path, section_name;
  PortableServer::ObjectId_var oid;
  ACE_Configuration_Section_Key base_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      inherited_path = this->reference_to_path (base_interfaces[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           inherited_path.in (),
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      name = impl.name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // None of these names can clash with any we may already have.
      if (TAO_Container_i::name_exists (name.in ()))
        {
          return;
        }

      section_name = this->int_to_string (i);

      this->repo_->config ()->set_string_value (inherited_key,
                                                section_name.in (),
                                                inherited_path.in ());
    }
}

CORBA::Boolean
TAO_InterfaceDef_i::is_a (const char *interface_id
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_a_i (interface_id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_InterfaceDef_i::is_a_i (const char *interface_id
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    {
      return 1;
    }

  CORBA::String_var id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Is it our type?
  if (ACE_OS::strcmp (id.in (), interface_id) == 0)
    {
      return 1;
    }

  // Is it one of our ancestors' types?
  CORBA::InterfaceDefSeq_var bases = 
    this->base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = bases->length ();

  PortableServer::ObjectId_var oid;
  CORBA::String_var base_path;
  ACE_Configuration_Section_Key base_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      base_path = this->reference_to_path (bases[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           base_path.in (),
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      CORBA::Boolean success = impl.is_a_i (interface_id
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (success == 1)
        {
          return 1;
        }
    }

  return 0;
}

CORBA::InterfaceDef::FullInterfaceDescription *
TAO_InterfaceDef_i::describe_interface (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_interface_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDef::FullInterfaceDescription *
TAO_InterfaceDef_i::describe_interface_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::InterfaceDef::FullInterfaceDescription *fifd = 0;
  ACE_NEW_RETURN (fifd,
                  CORBA::InterfaceDef::FullInterfaceDescription,
                  0);

  CORBA::InterfaceDef::FullInterfaceDescription_var retval = fifd;

  fifd->name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  fifd->id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  fifd->defined_in = container_id.c_str ();

  fifd->version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong i = 0;
  ACE_TString section_name;
  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  // Operations
  this->inherited_operations (key_queue);

  ACE_Configuration_Section_Key ops_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "ops",
                                          0,
                                          ops_key);

  u_int count = 0;

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (ops_key,
                                                 "count",
                                                 count);

      for (u_int j = 0; j < count; ++j)
        {
          ACE_Configuration_Section_Key op_key;
          CORBA::String_var section_name = this->int_to_string (j);
          status =
            this->repo_->config ()->open_section (ops_key,
                                                  section_name.in (),
                                                  0,
                                                  op_key);

          if (status == 0)
            {
              key_queue.enqueue_tail (op_key);
            }
        }
    }

  size_t size = key_queue.size ();
  fifd->operations.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_OperationDef_i op (this->repo_);
      op.section_key (key);

      fifd->operations[i] = 
        op.make_description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  // Attributes
  this->inherited_attributes (key_queue);

  ACE_Configuration_Section_Key attrs_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "attrs",
                                          0,
                                          attrs_key);

  count = 0;

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (attrs_key,
                                                 "count",
                                                 count);

      for (u_int j = 0; j < count; ++j)
        {
          ACE_Configuration_Section_Key attr_key;
          CORBA::String_var section_name = this->int_to_string (j);
          status =
            this->repo_->config ()->open_section (attrs_key,
                                                  section_name.in (),
                                                  0,
                                                  attr_key);

          if (status == 0)
            {
              key_queue.enqueue_tail (attr_key);
            }
        }
    }

  size = key_queue.size ();
  fifd->attributes.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_AttributeDef_i attr (this->repo_);
      attr.section_key (key);

      fifd->attributes[i] = 
        attr.make_description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  CORBA::InterfaceDefSeq_var bases = 
    this->base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = bases->length ();

  CORBA::RepositoryIdSeq repo_ids (length);

  repo_ids.length (length);

  PortableServer::ObjectId_var oid;
  CORBA::String_var base_path;
  ACE_Configuration_Section_Key base_key;

  for (i = 0; i < length; ++i)
    {
      base_path = this->reference_to_path (bases[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           base_path.in (),
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      repo_ids[i] = impl.id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  fifd->base_interfaces = repo_ids;

  fifd->type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

CORBA::AttributeDef_ptr
TAO_InterfaceDef_i::create_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::AttributeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  return this->create_attribute_i (id,
                                   name,
                                   version,
                                   type,
                                   mode
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::AttributeDef_ptr
TAO_InterfaceDef_i::create_attribute_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  if (bad_params)
    {
      return CORBA::AttributeDef::_nil ();
    }

  bad_params = this->check_inherited_attrs (name
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  if (bad_params)
    {
      return CORBA::AttributeDef::_nil ();
    }

  ACE_Configuration_Section_Key attrs_key;

  // Create/open section for attributes.
  this->repo_->config ()->open_section (this->section_key_,
                                        "attrs",
                                        1,
                                        attrs_key);

  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path = this->create_common (attrs_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "attrs\\",
                                          CORBA::dk_Attribute);

  // Store the path to the attribute's type definition.
  CORBA::String_var type_path = this->reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path.in ());

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "mode",
                                             mode);

  // Create the set and/or get operations for this attribute.
  this->create_attr_ops (id,
                         name,
                         version,
                         type,
                         mode
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

#if 0 // CCM specific.

  CORBA::ULong i = 0;

  CORBA::ULong length = get_exceptions.length ();

  if (length > 0)
    {
      // Open a section for the 'get' exceptions.
      ACE_Configuration_Section_Key get_excepts_key;

      this->repo_->config ()->open_section (new_key,
                                            "get_excepts",
                                            1,
                                            get_excepts_key);

      // Store the paths to the 'get' exceptions.
      for (i = 0; i < length; ++i)
        {
          CORBA::String_var get_except_path =
            this->reference_to_path (get_exceptions[i]);

          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (get_excepts_key,
                                                    section_name.in (),
                                                    get_except_path.in ());
        }
    }

  length = put_exceptions.length ();

  if (length > 0)
    {
      // Open a section for the 'put' exceptions.
      ACE_Configuration_Section_Key put_excepts_key;
      this->repo_->config ()->open_section (new_key,
                                            "put_excepts",
                                            1,
                                            put_excepts_key);

      // Store the paths to the 'put' exceptions.
      for (i = 0; i < length; ++i)
        {
          CORBA::String_var put_except_path =
            this->reference_to_path (put_exceptions[i]);

          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (put_excepts_key,
                                                    section_name.in (),
                                                    put_except_path.in ());
        }
    }

#endif /* CCM specific. */

  // Create the object reference.
  CORBA::Object_var obj =
    this->create_objref (CORBA::dk_Attribute,
                         path.c_str ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  CORBA::AttributeDef_var retval =
    CORBA::AttributeDef::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  return retval._retn ();
}

CORBA::OperationDef_ptr
TAO_InterfaceDef_i::create_operation (const char *id,
                                      const char *name,
                                      const char *version,
                                      CORBA::IDLType_ptr result,
                                      CORBA::OperationMode mode,
                                      const CORBA::ParDescriptionSeq &params,
                                      const CORBA::ExceptionDefSeq &exceptions,
                                      const CORBA::ContextIdSeq &contexts
                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::OperationDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  return this->create_operation_i (id,
                                   name,
                                   version,
                                   result,
                                   mode,
                                   params,
                                   exceptions,
                                   contexts
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::OperationDef_ptr
TAO_InterfaceDef_i::create_operation_i (const char *id,
                                        const char *name,
                                        const char *version,
                                        CORBA::IDLType_ptr result,
                                        CORBA::OperationMode mode,
                                        const CORBA::ParDescriptionSeq &params,
                                        const CORBA::ExceptionDefSeq &exceptions,
                                        const CORBA::ContextIdSeq &contexts
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  if (bad_params)
    {
      return CORBA::OperationDef::_nil ();
    }

  ACE_Configuration_Section_Key ops_key;

  // Create/open section for attributes.
  this->repo_->config ()->open_section (this->section_key_,
                                        "ops",
                                        1,
                                        ops_key);

  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path = this->create_common (ops_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "ops\\",
                                          CORBA::dk_Operation);

  // Get the TypeCode for the return type.
  CORBA::String_var tmp = this->reference_to_path (result);

  ACE_TString result_path (tmp.in ());
  TAO_IDLType_i *result_impl = this->path_to_idltype (result_path);

  CORBA::TypeCode_var rettype = 
    result_impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  CORBA::TCKind kind = rettype->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  // Oneway operations cannot have a non-void return type.
  if (mode == CORBA::OP_ONEWAY && kind != CORBA::tk_void)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (31,
                                          CORBA::COMPLETED_NO),
                        CORBA::OperationDef::_nil ());
    }

  this->repo_->config ()->set_string_value (new_key,
                                            "result",
                                            result_path);

  // Store the operation mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "mode",
                                             mode);

  CORBA::ULong i = 0;

  // Store the operation's parameter info.
  CORBA::ULong length = params.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key params_key;
      this->repo_->config ()->open_section (new_key,
                                            "params",
                                            1,
                                            params_key);

      this->repo_->config ()->set_integer_value (params_key,
                                                 "count",
                                                 length);

      for (i = 0; i < length; ++i)
        {
          // Oneway operations cannot have INOUT or OUT parameters.
          if (mode == CORBA::OP_ONEWAY && params[i].mode != CORBA::PARAM_IN)
            {
              ACE_THROW_RETURN (CORBA::BAD_PARAM (31,
                                                  CORBA::COMPLETED_NO),
                                CORBA::OperationDef::_nil ());
            }

          ACE_Configuration_Section_Key param_key;
          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->open_section (params_key,
                                                section_name.in (),
                                                1,
                                                param_key);

          this->repo_->config ()->set_string_value (param_key,
                                                    "name",
                                                    params[i].name.in ());
          CORBA::String_var type_path =
            this->reference_to_path (params[i].type_def.in ());

          this->repo_->config ()->set_string_value (param_key,
                                                    "type_path",
                                                    type_path.in ());

          this->repo_->config ()->set_integer_value (param_key,
                                                     "mode",
                                                     params[i].mode);
       }
    }

  // Store the operation's exception info.
  length = exceptions.length ();

  if (length > 0)
    {
      // Oneway operations cannot throw any user exceptions.
      if (mode == CORBA::OP_ONEWAY)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (31,
                                              CORBA::COMPLETED_NO),
                            CORBA::OperationDef::_nil ());
        }

      ACE_Configuration_Section_Key excepts_key;

      this->repo_->config ()->open_section (new_key,
                                            "excepts",
                                            1,
                                            excepts_key);

      for (i = 0; i < length; ++i)
        {
          CORBA::String_var type_path = 
            this->reference_to_path (exceptions[i]);

          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (excepts_key,
                                                    section_name.in (),
                                                    type_path.in ());
        }
    }

  // Store the operation's context info.
  length = contexts.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key contexts_key;

      this->repo_->config ()->open_section (new_key,
                                            "contexts",
                                            1,
                                            contexts_key);

      for (i = 0; i < length; ++i)
        {
          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (contexts_key,
                                                    section_name.in (),
                                                    contexts[i].in ());
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->create_objref (CORBA::dk_Operation,
                         path.c_str ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  CORBA::OperationDef_var retval =
    CORBA::OperationDef::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  return retval._retn ();
}

void
TAO_InterfaceDef_i::interface_contents (
    ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
    ACE_Unbounded_Queue<ACE_TString> &path_queue,
    CORBA::DefinitionKind limit_type,
    CORBA::Boolean exclude_inherited
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            id.c_str (),
                                            path);

  ACE_TString section_name;
  int index = 0;
  int status = 0;

  // Attributes
  if (limit_type == CORBA::dk_Attribute
      || limit_type == CORBA::dk_all)
    {
      ACE_Configuration_Section_Key attrs_key;
      status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "attrs",
                                              0,
                                              attrs_key);

      // Only if we have any.
      if (status == 0)
        {
          while (this->repo_->config ()->enumerate_sections (attrs_key,
                                                             index++,
                                                             section_name)
                  == 0)
            {
              kind_queue.enqueue_tail (CORBA::dk_Attribute);

              path_queue.enqueue_tail (
                  path + "\\attrs\\" + section_name.c_str ()
                );
            }
        }
    }

  // Operations
  if (limit_type == CORBA::dk_Operation
      || limit_type == CORBA::dk_all)
    {
      index = 0;

      ACE_Configuration_Section_Key ops_key;
      status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "ops",
                                              0,
                                              ops_key);

      // Only if we have any.
      if (status == 0)
        {
          while (this->repo_->config ()->enumerate_sections (ops_key,
                                                             index++,
                                                             section_name)
                  == 0)
            {
              kind_queue.enqueue_tail (CORBA::dk_Operation);

              path_queue.enqueue_tail (
                  path + "\\ops\\" + section_name.c_str ()
                );
            }
        }
    }

  if (exclude_inherited == 0)
    {
      // Must recurse through the base interfaces.
      ACE_Configuration_Section_Key inherited_key;
      status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "inherited",
                                              0,
                                              inherited_key);

      if (status == 0)
        {
          ACE_TString base_path;
          ACE_Configuration_Section_Key base_key;
          ACE_Configuration::VALUETYPE type;
          index = 0;

          while (this->repo_->config ()->enumerate_values (inherited_key,
                                                           index++,
                                                           section_name,
                                                           type)
                  == 0)
            {
              this->repo_->config ()->get_string_value (inherited_key,
                                                        section_name.c_str (),
                                                        base_path);

              this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                   base_path,
                                                   base_key,
                                                   0);

              TAO_InterfaceDef_i base_iface (this->repo_);
              base_iface.section_key (base_key);

              base_iface.interface_contents (kind_queue,
                                             path_queue,
                                             limit_type,
                                             exclude_inherited
                                             ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }
}

void
TAO_InterfaceDef_i::destroy_special (const char *sub_section
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key sub_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          sub_section,
                                          0,
                                          sub_key);

  if (status == 0)
    {
      ACE_TString section_name;
      int index = 0;

      while (this->repo_->config ()->enumerate_sections (sub_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          ACE_Configuration_Section_Key member_key;
          this->repo_->config ()->open_section (sub_key,
                                                section_name.c_str (),
                                                0,
                                                member_key);

          if (ACE_OS::strcmp (sub_section, "attrs") == 0)
            {
              TAO_AttributeDef_i attr (this->repo_);
              attr.section_key (member_key);

              attr.destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              TAO_OperationDef_i op (this->repo_);
              op.section_key (member_key);

              op.destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }
}

void
TAO_InterfaceDef_i::create_attr_ops (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::IDLType_ptr type,
                                     CORBA::AttributeMode mode
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_CString the_get_name ("_get_");
  ACE_CString start (id);
  int pos = start.find (name);
  ACE_CString piece (start.substr (pos));
  ACE_CString the_get_id (start.substr (0, pos) + the_get_name + piece);
  the_get_name += name;

  CORBA::ParDescriptionSeq params (0);
  CORBA::ExceptionDefSeq excepts (0);
  CORBA::ContextIdSeq contexts (0);

  CORBA::OperationDef_var the_get_op =
    this->create_operation_i (the_get_id.c_str (),
                              the_get_name.c_str (),
                              version,
                              type,
                              CORBA::OP_NORMAL,
                              params,
                              excepts,
                              contexts
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (mode == CORBA::ATTR_NORMAL)
    {
      ACE_CString the_set_name ("_set_");
      ACE_CString the_set_id (start.substr (0, pos) + the_set_name + piece);
      the_set_name += name;

      CORBA::PrimitiveDef_var rettype =
        this->repo_->get_primitive (CORBA::pk_void
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      params.length (1);

      params[0].name = name;
      params[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);
      params[0].type_def = CORBA::IDLType::_duplicate (type);
      params[0].mode = CORBA::PARAM_IN;

      CORBA::OperationDef_var the_set_op =
        this->create_operation_i (the_set_id.c_str (),
                                  the_set_name.c_str (),
                                  version,
                                  rettype.in (),
                                  CORBA::OP_NORMAL,
                                  params,
                                  excepts,
                                  contexts
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_InterfaceDef_i::base_interfaces_recursive (
    ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
    ACE_Unbounded_Queue<ACE_TString> &path_queue
  )
{
  ACE_Configuration_Section_Key inherited_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "inherited",
                                          0,
                                          inherited_key);

  // No base interfaces.
  if (status != 0)
    {
      return;
    }

  int index = 0;
  u_int kind = 0;
  ACE_Configuration::VALUETYPE type;
  ACE_TString section_name, inherited_path;
  CORBA::DefinitionKind def_kind = CORBA::dk_none;
  ACE_Configuration_Section_Key base_key;

  while (this->repo_->config ()->enumerate_values (inherited_key,
                                                   index++,
                                                   section_name,
                                                   type)
          == 0)
    {
      this->repo_->config ()->get_string_value (inherited_key,
                                                section_name.c_str (),
                                                inherited_path);

      status =
        this->repo_->config ()->expand_path (this->repo_->root_key (),
                                             inherited_path,
                                             base_key,
                                             0);

      if (status == 0)
        {
          TAO_InterfaceDef_i tmp (this->repo_);
          tmp.section_key (base_key);

          tmp.base_interfaces_recursive (kind_queue,
                                         path_queue);

          path_queue.enqueue_tail (inherited_path);

          this->repo_->config ()->get_integer_value (base_key,
                                                     "def_kind",
                                                     kind);

          def_kind = ACE_static_cast (CORBA::DefinitionKind, kind);

          kind_queue.enqueue_tail (def_kind);
        }
    }
}

void
TAO_InterfaceDef_i::inherited_attributes (
    ACE_Unbounded_Queue<ACE_Configuration_Section_Key> &key_queue
  )
{
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  this->base_interfaces_recursive (kind_queue,
                                   path_queue);

  size_t size = path_queue.size ();
  ACE_Configuration_Section_Key base_key, attrs_key, attr_key;
  int status = 0;
  ACE_TString path_name;
  u_int count = 0;

  for (size_t i = 0; i < size; ++i)
    {
      path_queue.dequeue_head (path_name);

      status =
        this->repo_->config ()->expand_path (this->repo_->root_key (),
                                             path_name,
                                             base_key,
                                             0);

      if (status == 0)
        {
          this->repo_->config ()->open_section (base_key,
                                                "attrs",
                                                0,
                                                attrs_key);

          this->repo_->config ()->get_integer_value (attrs_key,
                                                     "count",
                                                     count);

          for (u_int j = 0; j < count; ++j)
            {
              this->repo_->config ()->open_section (attrs_key,
                                                    this->int_to_string (j),
                                                    0,
                                                    attr_key);

              key_queue.enqueue_tail (attr_key);
            }
        }
    }
}

void
TAO_InterfaceDef_i::inherited_operations (
    ACE_Unbounded_Queue<ACE_Configuration_Section_Key> &key_queue
  )
{
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  this->base_interfaces_recursive (kind_queue,
                                   path_queue);

  size_t size = path_queue.size ();
  ACE_Configuration_Section_Key base_key, ops_key, op_key;
  int status = 0;
  ACE_TString path_name;
  u_int count = 0;

  for (size_t i = 0; i < size; ++i)
    {
      path_queue.dequeue_head (path_name);

      status =
        this->repo_->config ()->expand_path (this->repo_->root_key (),
                                             path_name,
                                             base_key,
                                             0);

      if (status == 0)
        {
          this->repo_->config ()->open_section (base_key,
                                                "ops",
                                                0,
                                                ops_key);

          this->repo_->config ()->get_integer_value (ops_key,
                                                     "count",
                                                     count);

          for (u_int j = 0; j < count; ++j)
            {
              this->repo_->config ()->open_section (ops_key,
                                                    this->int_to_string (j),
                                                    0,
                                                    op_key);

              key_queue.enqueue_tail (op_key);
            }
        }
    }
}

CORBA::Boolean
TAO_InterfaceDef_i::check_inherited_attrs (const char *name
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  this->inherited_attributes (key_queue);

  size_t size = key_queue.size ();
  ACE_Configuration_Section_Key attr_key;
  ACE_TString attr_name;

  for (u_int i = 0; i < size; ++i)
    {
      key_queue.dequeue_head (attr_key);

      this->repo_->config ()->get_string_value (attr_key,
                                                "name",
                                                attr_name);

      if (attr_name == name)
        {
          ACE_THROW_RETURN (CORBA::BAD_PARAM (5,
                                              CORBA::COMPLETED_NO),
                            1);
        }
    }

  return 0;
}
