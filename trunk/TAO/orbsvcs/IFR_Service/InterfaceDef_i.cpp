/* -*- C++ -*- */
// $Id$

#include "InterfaceDef_i.h"
#include "Repository_i.h"
#include "AttributeDef_i.h"
#include "OperationDef_i.h"
#include "Servant_Factory.h"

ACE_RCSID(IFR_Service, InterfaceDef_i, "$Id$")

TAO_InterfaceDef_i::TAO_InterfaceDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_InterfaceDef_i::~TAO_InterfaceDef_i (void)
{
}

IR::DefinitionKind
TAO_InterfaceDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Interface;
}

void 
TAO_InterfaceDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy our members.
  TAO_Container_i::destroy (ACE_TRY_ENV);
  ACE_CHECK;

  // This will get rid of the repo ids, which Contained_i::destroy()'s
  // call to remove_section (recursive = 1) will not get, and also
  // destroy the attribute's anonymous type, if any.
  this->destroy_special ("attrs", ACE_TRY_ENV);
  ACE_CHECK;
  this->destroy_special ("ops", ACE_TRY_ENV);
  ACE_CHECK;

  // Destroy ourself.
  TAO_Contained_i::destroy (ACE_TRY_ENV);
  ACE_CHECK;
}

IR::Contained::Description *
TAO_InterfaceDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
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

  IR::InterfaceDescription ifd;

  ifd.name = this->name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ifd.id = this->id (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ifd.defined_in = container_id.c_str ();

  ifd.version = this->version (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::ULong i = 0;
  ACE_TString section_name;
  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  // Operations
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
            key_queue.enqueue_tail (op_key);
        }
    }

  size_t size = key_queue.size ();
  ifd.operations.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_OperationDef_i op (this->repo_,
                             key);

      ifd.operations[i] = op.make_description (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  // Attributes
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
            key_queue.enqueue_tail (attr_key);
        }
    }

  size = key_queue.size ();
  ifd.attributes.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_AttributeDef_i attr (this->repo_,
                               key);

      ifd.attributes[i] = attr.make_description (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  IR::InterfaceDefSeq_var bases = this->base_interfaces (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = bases->length ();

  IR::RepositoryIdSeq repo_ids (length);

  repo_ids.length (length);

  for (i = 0; i < length; ++i)
    {
      repo_ids[i] = bases[i]->id (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  ifd.base_interfaces = repo_ids;

  ifd.type = this->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ifd.is_abstract = this->is_abstract (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ifd.is_local = this->is_local (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  retval->value <<= ifd;

  return retval._retn ();
}

CORBA::TypeCode_ptr 
TAO_InterfaceDef_i::type (CORBA::Environment &ACE_TRY_ENV)
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
  
  return this->repo_->tc_factory ()->create_interface_tc (id.c_str (),
                                                          name.c_str (),
                                                          ACE_TRY_ENV);
}

IR::InterfaceDefSeq *
TAO_InterfaceDef_i::base_interfaces (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key inherited_key;

  this->repo_->config ()->open_section (this->section_key_,
                                        "inherited",
                                        0,
                                        inherited_key);

  int index = 0;
  ACE_Configuration::VALUETYPE type;
  ACE_TString section_name;
  ACE_Unbounded_Queue<IR::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  while (this->repo_->config ()->enumerate_values (inherited_key,
                                                   index,
                                                   section_name,
                                                   type)
          == 0)
    {
      ACE_TString inherited_path;
      this->repo_->config ()->get_string_value (inherited_key,
                                                section_name.c_str (),
                                                inherited_path);

      path_queue.enqueue_tail (inherited_path);

      ACE_Configuration_Section_Key base_key;
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           inherited_path,
                                           base_key,
                                           0);

      u_int kind = 0;
      this->repo_->config ()->get_integer_value (base_key,
                                                 "def_kind",
                                                 kind);

      IR::DefinitionKind def_kind =
        ACE_static_cast (IR::DefinitionKind, kind);

      kind_queue.enqueue_tail (def_kind);
      ++index;
    }

  IR::InterfaceDefSeq *seq = 0;
  ACE_NEW_THROW_EX (seq,
                    IR::InterfaceDefSeq (index),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  seq->length (index);
  IR::InterfaceDefSeq_var retval = seq;

  for (CORBA::ULong i = 0; i < (CORBA::ULong) index; ++i)
    {
      IR::DefinitionKind def_kind;
      kind_queue.dequeue_head (def_kind);

      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj = 
        this->repo_->servant_factory ()->create_objref (
                                             def_kind,
                                             path.c_str (),
                                             ACE_TRY_ENV
                                           );
      ACE_CHECK_RETURN (0);

      retval[i] = IR::InterfaceDef::_narrow (obj.in (),
                                             ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void 
TAO_InterfaceDef_i::base_interfaces (const IR::InterfaceDefSeq &base_interfaces,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
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

  for (CORBA::ULong i = 0; i < length; i++)
    {
      CORBA::String_var name = base_interfaces[i]->name (ACE_TRY_ENV);
      ACE_CHECK;

      // None of these names can clash with any we may already have.
      if (TAO_Container_i::name_exists (name.in ()))
        return;

      PortableServer::ObjectId_var oid = 
        this->repo_->ir_poa ()->reference_to_id (base_interfaces[i],
                                                 ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::String_var inherited_path =
        PortableServer::ObjectId_to_string (oid.in ());

      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->set_string_value (inherited_key,
                                                section_name.in (),
                                                inherited_path.in ());
    }
}

CORBA::Boolean 
TAO_InterfaceDef_i::is_abstract (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int is_abstract = 0;

  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_abstract",
                                             is_abstract);
  
  return ACE_static_cast (CORBA::Boolean, is_abstract);;
}

void 
TAO_InterfaceDef_i::is_abstract (CORBA::Boolean is_abstract,
                             CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "is_abstract",
                                             is_abstract);
}

CORBA::Boolean 
TAO_InterfaceDef_i::is_local (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int is_local = 0;

  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_local",
                                             is_local);
  
  return ACE_static_cast (CORBA::Boolean, is_local);;
}

void 
TAO_InterfaceDef_i::is_local (CORBA::Boolean is_local,
                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "is_local",
                                             is_local);
}

CORBA::Boolean 
TAO_InterfaceDef_i::is_a (const char *interface_id,
                          CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var id = this->id (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Is it our type?
  if (ACE_OS::strcmp (id.in (), interface_id) == 0)
    return 1;

  // Is it one of our ancestors' types?
  IR::InterfaceDefSeq_var bases = this->base_interfaces (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = bases->length ();

  for (CORBA::ULong i = 0; i < length; i++)
    {
      CORBA::Boolean success = bases[i]->is_a (interface_id,
                                               ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (success == 1)
        return 1;
    }

  return 0;
}

IR::AttributeDef_ptr 
TAO_InterfaceDef_i::create_attribute (
    const char *id,
    const char *name,
    const char *version,
    IR::IDLType_ptr type,
    IR::AttributeMode mode,
    const IR::ExceptionDefSeq &get_exceptions,
    const IR::ExceptionDefSeq &put_exceptions,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean bad_params = this->pre_exist (id, 
                                               name, 
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AttributeDef::_nil ());

  if (bad_params)
    return IR::AttributeDef::_nil ();
  
  ACE_Configuration_Section_Key attrs_key;

  // Create/open section for attributes.
  this->repo_->config ()->open_section (this->section_key_,
                                        "attrs",
                                        1,
                                        attrs_key);

  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in IR::Container.
  ACE_TString path = this->create_common (attrs_key,
                                          new_key, 
                                          id, 
                                          name, 
                                          version,
                                          "attrs\\",
                                          IR::dk_Attribute);

  // Store the path to the attribute's type definition.
  PortableServer::ObjectId_var oid = 
    this->repo_->ir_poa ()->reference_to_id (type,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AttributeDef::_nil ());

  CORBA::String_var type_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path.in ());

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "mode",
                                             mode);

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
          PortableServer::ObjectId_var oid = 
            this->repo_->ir_poa ()->reference_to_id (get_exceptions[i],
                                                     ACE_TRY_ENV);

          ACE_CHECK_RETURN (IR::AttributeDef::_nil ());

          CORBA::String_var get_except_path =
            PortableServer::ObjectId_to_string (oid.in ());

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
          PortableServer::ObjectId_var oid = 
            this->repo_->ir_poa ()->reference_to_id (put_exceptions[i],
                                                     ACE_TRY_ENV);

          ACE_CHECK_RETURN (IR::AttributeDef::_nil ());

          CORBA::String_var put_except_path =
            PortableServer::ObjectId_to_string (oid.in ());

          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (put_excepts_key,
                                                    section_name.in (),
                                                    put_except_path.in ());
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Attribute,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AttributeDef::_nil ());

  IR::AttributeDef_var retval =
    IR::AttributeDef::_narrow (obj.in (),
                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AttributeDef::_nil ());

  return retval._retn ();
}

IR::OperationDef_ptr 
TAO_InterfaceDef_i::create_operation (const char *id,
                                      const char *name,
                                      const char *version,
                                      IR::IDLType_ptr result,
                                      IR::OperationMode mode,
                                      const IR::ParDescriptionSeq &params,
                                      const IR::ExceptionDefSeq &exceptions,
                                      const IR::ContextIdSeq &contexts,
                                      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean bad_params = this->pre_exist (id, 
                                               name, 
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::OperationDef::_nil ());

  if (bad_params)
    return IR::OperationDef::_nil ();
  
  ACE_Configuration_Section_Key ops_key;

  // Create/open section for attributes.
  this->repo_->config ()->open_section (this->section_key_,
                                        "ops",
                                        1,
                                        ops_key);

  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in IR::Container.
  ACE_TString path = this->create_common (ops_key,
                                          new_key, 
                                          id, 
                                          name, 
                                          version,
                                          "ops\\",
                                          IR::dk_Operation);

  // Get the path to our return type and store it.
  PortableServer::ObjectId_var oid = 
    this->repo_->ir_poa ()->reference_to_id (result,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::OperationDef::_nil ());

  CORBA::String_var result_path = 
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (new_key,
                                            "result",
                                            result_path.in ());

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
          ACE_Configuration_Section_Key param_key;
          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->open_section (params_key,
                                                section_name.in (),
                                                1,
                                                param_key);

          const char *name = params[i].name;

          this->repo_->config ()->set_string_value (param_key,
                                                    "name",
                                                    name);
          PortableServer::ObjectId_var oid = 
            this->repo_->ir_poa ()->reference_to_id (params[i].type_def,
                                                     ACE_TRY_ENV);

          ACE_CHECK_RETURN (IR::OperationDef::_nil ());

          CORBA::String_var type_path =
            PortableServer::ObjectId_to_string (oid.in ());

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
      ACE_Configuration_Section_Key excepts_key;

      this->repo_->config ()->open_section (new_key,
                                            "excepts",
                                            1,
                                            excepts_key);

      for (i = 0; i < length; ++i)
        {
          PortableServer::ObjectId_var oid = 
            this->repo_->ir_poa ()->reference_to_id (exceptions[i],
                                                     ACE_TRY_ENV);

          ACE_CHECK_RETURN (IR::OperationDef::_nil ());

          CORBA::String_var type_path =
            PortableServer::ObjectId_to_string (oid.in ());

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
          const char *context = contexts[i];

          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (contexts_key,
                                                    section_name.in (),
                                                    context);
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Operation,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::OperationDef::_nil ());

  IR::OperationDef_var retval =
    IR::OperationDef::_narrow (obj.in (),
                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::OperationDef::_nil ());

  return retval._retn ();
}

void 
TAO_InterfaceDef_i::inherited_contents (
      ACE_Unbounded_Queue<IR::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      IR::DefinitionKind limit_type,
      CORBA::Environment &ACE_TRY_ENV)
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

  // Attributes
  if (limit_type == IR::dk_Attribute
      || limit_type == IR::dk_all)
    {
      ACE_Configuration_Section_Key attrs_key;
      this->repo_->config ()->open_section (this->section_key_,
                                            "attrs",
                                            0,
                                            attrs_key);

      while (this->repo_->config ()->enumerate_sections (attrs_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          kind_queue.enqueue_tail (IR::dk_Attribute);

          path_queue.enqueue_tail (
              path + "\\attrs\\" + section_name.c_str ()
            );
        }
    }

  // Operations
  if (limit_type == IR::dk_Operation
      || limit_type == IR::dk_all)
    {
      index = 0;

      ACE_Configuration_Section_Key ops_key;
      this->repo_->config ()->open_section (this->section_key_,
                                            "ops",
                                            0,
                                            ops_key);

      while (this->repo_->config ()->enumerate_sections (ops_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          kind_queue.enqueue_tail (IR::dk_Operation);

          path_queue.enqueue_tail (
              path + "\\ops\\" + section_name.c_str ()
            );
        }
    }

  // Must recurse through the base interfaces.
  ACE_Configuration_Section_Key inherited_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "inherited",
                                        0,
                                        inherited_key);

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

      TAO_InterfaceDef_i base_iface (this->repo_,
                                     base_key);

      base_iface.inherited_contents (kind_queue,
                                     path_queue,
                                     limit_type,
                                     ACE_TRY_ENV);
      ACE_CHECK;          
    }
}

void
TAO_InterfaceDef_i::destroy_special (const char *sub_section,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key sub_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        sub_section,
                                        0,
                                        sub_key);

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
          TAO_AttributeDef_i attr (this->repo_,
                                   member_key);

          attr.destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }
      else
        { 
          TAO_OperationDef_i op (this->repo_,
                                 member_key);

          op.destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
                                                    
}
