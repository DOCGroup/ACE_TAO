// $Id$

#include "orbsvcs/IFRService/InterfaceDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/AttributeDef_i.h"
#include "orbsvcs/IFRService/OperationDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_InterfaceDef_i::TAO_InterfaceDef_i (TAO_Repository_i *repo)
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
TAO_InterfaceDef_i::def_kind (void)
{
  return CORBA::dk_Interface;
}

void
TAO_InterfaceDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_InterfaceDef_i::destroy_i (void)
{
  // Destroy our members.
  this->TAO_Container_i::destroy_i ();

  // These will get rid of the repo ids, which Contained_i::destroy()'s
  // call to remove_section (recursive = 1) will not get, and also
  // destroy the attribute's anonymous type, if any.

  TAO_IFR_Generic_Utils<TAO_AttributeDef_i>::destroy_special (
      "attrs",
      this->repo_,
      this->section_key_
    );

  TAO_IFR_Generic_Utils<TAO_OperationDef_i>::destroy_special (
      "ops",
      this->repo_,
      this->section_key_
    );

  // Destroy ourself.
  this->TAO_Contained_i::destroy_i ();
}

CORBA::Contained::Description *
TAO_InterfaceDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_InterfaceDef_i::describe_i (void)
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind ();

  CORBA::InterfaceDescription ifd;
  ACE_TString holder;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            holder);
  ifd.name = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            holder);
  ifd.id = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            holder);
  ifd.defined_in = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "version",
                                            holder);
  ifd.version = holder.fast_rep ();

  CORBA::ULong i = 0;
  CORBA::InterfaceDefSeq_var bases =
    this->base_interfaces_i ();

  CORBA::ULong length = bases->length ();
  CORBA::RepositoryIdSeq repo_ids (length);
  repo_ids.length (length);

  char *base_path = 0;
  ACE_Configuration_Section_Key base_key;

  for (i = 0; i < length; ++i)
    {
      base_path = TAO_IFR_Service_Utils::reference_to_path (bases[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           base_path,
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      repo_ids[i] = impl.id_i ();
    }

  ifd.base_interfaces = repo_ids;
  retval->value <<= ifd;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_InterfaceDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_InterfaceDef_i::type_i (void)
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
                                       );
}

CORBA::InterfaceDefSeq *
TAO_InterfaceDef_i::base_interfaces (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->base_interfaces_i ();
}

CORBA::InterfaceDefSeq *
TAO_InterfaceDef_i::base_interfaces_i (void)
{
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  this->base_interfaces_recursive (kind_queue,
                                   path_queue);

  CORBA::ULong size = static_cast<CORBA::ULong> (kind_queue.size ());

  CORBA::InterfaceDefSeq *seq = 0;
  ACE_NEW_THROW_EX (seq,
                    CORBA::InterfaceDefSeq (size),
                    CORBA::NO_MEMORY ());

  seq->length (size);
  CORBA::InterfaceDefSeq_var retval = seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      CORBA::DefinitionKind def_kind;
      kind_queue.dequeue_head (def_kind);

      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        TAO_IFR_Service_Utils::path_to_ir_object (path,
                                                  this->repo_);

      retval[i] = CORBA::InterfaceDef::_narrow (obj.in ());
    }

  return retval._retn ();
}

void
TAO_InterfaceDef_i::base_interfaces (const CORBA::InterfaceDefSeq &base_interfaces)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->base_interfaces_i (base_interfaces);
}

void
TAO_InterfaceDef_i::base_interfaces_i (const CORBA::InterfaceDefSeq &base_interfaces)
{
  // If we are an abstract interface, all our base interfaces must be
  // abstract as well.
  CORBA::DefinitionKind def_kind = this->def_kind ();

  if (def_kind == CORBA::dk_AbstractInterface)
    {
      CORBA::ULong length = base_interfaces.length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          def_kind = base_interfaces[i]->def_kind ();

          if (def_kind != CORBA::dk_AbstractInterface)
            {
              throw CORBA::BAD_PARAM (
                CORBA::OMGVMCID | 11,
                CORBA::COMPLETED_NO);
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

  CORBA::String_var name;
  char *inherited_path = 0;
  PortableServer::ObjectId_var oid;
  ACE_Configuration_Section_Key base_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      inherited_path =
        TAO_IFR_Service_Utils::reference_to_path (base_interfaces[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           inherited_path,
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      name = impl.name_i ();

      // None of these names can clash with any we may already have.
      TAO_Container_i::tmp_name_holder_ = name.in ();
      TAO_IFR_Service_Utils::name_exists (&TAO_Container_i::same_as_tmp_name,
                                          this->section_key_,
                                          this->repo_,
                                          this->def_kind ());

      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (inherited_key,
                                                stringified,
                                                inherited_path);
    }
}

CORBA::Boolean
TAO_InterfaceDef_i::is_a (const char *interface_id)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->is_a_i (interface_id);
}

CORBA::Boolean
TAO_InterfaceDef_i::is_a_i (const char *interface_id)
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    {
      return 1;
    }

  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  // Is it our type?
  if (ACE_OS::strcmp (id.fast_rep (), interface_id) == 0)
    {
      return 1;
    }

  // Is it one of our ancestors' types?
  CORBA::InterfaceDefSeq_var bases =
    this->base_interfaces_i ();

  CORBA::ULong length = bases->length ();

  PortableServer::ObjectId_var oid;
  char *base_path = 0;
  ACE_Configuration_Section_Key base_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      base_path = TAO_IFR_Service_Utils::reference_to_path (bases[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           base_path,
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      CORBA::Boolean success = impl.is_a_i (interface_id);

      if (success == 1)
        {
          return 1;
        }
    }

  return 0;
}

CORBA::InterfaceDef::FullInterfaceDescription *
TAO_InterfaceDef_i::describe_interface (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_interface_i ();
}

CORBA::InterfaceDef::FullInterfaceDescription *
TAO_InterfaceDef_i::describe_interface_i (void)
{
  CORBA::InterfaceDef::FullInterfaceDescription *fifd = 0;
  ACE_NEW_RETURN (fifd,
                  CORBA::InterfaceDef::FullInterfaceDescription,
                  0);

  CORBA::InterfaceDef::FullInterfaceDescription_var retval = fifd;

  ACE_TString holder;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            holder);
  fifd->name = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            holder);
  fifd->id = holder.fast_rep ();

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            holder);

  fifd->defined_in = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "version",
                                            holder);
  fifd->version = holder.fast_rep ();

  CORBA::ULong i = 0;
  CORBA::ULong j = 0;
  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  // Store our section key for later restoration after we have
  // traversed entries for inherited interfaces.
  ACE_Configuration_Section_Key key_holder = this->section_key_;

  // Operations
  this->inherited_operations (key_queue);

  // Restore our original section key.
  //   I am not sure this is needed but it will not hurt.
  this->section_key (key_holder);

  ACE_Configuration_Section_Key ops_key, op_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "ops",
                                          0,
                                          ops_key);

  CORBA::ULong count = 0;

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (ops_key,
                                                 "count",
                                                 count);

      for (j = 0; j < count; ++j)
        {
          char *stringified = TAO_IFR_Service_Utils::int_to_string (j);
          status =
            this->repo_->config ()->open_section (ops_key,
                                                  stringified,
                                                  0,
                                                  op_key);

          if (status == 0)
            {
              key_queue.enqueue_tail (op_key);
            }
        }
    }

  CORBA::ULong size = static_cast<CORBA::ULong> (key_queue.size ());
  fifd->operations.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_OperationDef_i op (this->repo_);
      op.section_key (key);

      op.make_description (fifd->operations[i]);
    }

  // Restore our original section key.
  //   It may have been overwritten by a superclass key as part of the
  //   make_description() call.
  this->section_key (key_holder);

  // Attributes
  this->inherited_attributes (key_queue);

  // Restore our original section key.
  //   I am not sure this is needed but it will not hurt.
  this->section_key (key_holder);

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

      for (j = 0; j < count; ++j)
        {
          ACE_Configuration_Section_Key attr_key;
          char *stringified = TAO_IFR_Service_Utils::int_to_string (j);
          status =
            this->repo_->config ()->open_section (attrs_key,
                                                  stringified,
                                                  0,
                                                  attr_key);

          if (status == 0)
            {
              key_queue.enqueue_tail (attr_key);
            }
        }
    }

  size = static_cast<CORBA::ULong> (key_queue.size ());
  fifd->attributes.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_AttributeDef_i attr (this->repo_);
      attr.section_key (key);

      attr.make_description (fifd->attributes[i]);
    }

  // Restore our original section key.
  //   It may have been overwritten by a superclass key as part of the
  //   make_description() call.
  this->section_key (key_holder);

  CORBA::InterfaceDefSeq_var bases =
    this->base_interfaces_i ();

  CORBA::ULong length = bases->length ();

  CORBA::RepositoryIdSeq repo_ids (length);

  repo_ids.length (length);

  PortableServer::ObjectId_var oid;
  char *base_path = 0;
  ACE_Configuration_Section_Key base_key;

  for (i = 0; i < length; ++i)
    {
      base_path = TAO_IFR_Service_Utils::reference_to_path (bases[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           base_path,
                                           base_key,
                                           0);

      TAO_InterfaceDef_i impl (this->repo_);
      impl.section_key (base_key);

      repo_ids[i] = impl.id_i ();
    }

  fifd->base_interfaces = repo_ids;

  fifd->type = this->type_i ();

  return retval._retn ();
}

CORBA::AttributeDef_ptr
TAO_InterfaceDef_i::create_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
  )
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::AttributeDef::_nil ());

  this->update_key ();

  return this->create_attribute_i (id,
                                   name,
                                   version,
                                   type,
                                   mode);
}

CORBA::AttributeDef_ptr
TAO_InterfaceDef_i::create_attribute_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
  )
{
  // This will throw an exception if a name clash is found.
  // create_common() will check for all other errors.
  this->check_inherited (name,
                         CORBA::dk_Attribute);

  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path =
    TAO_IFR_Service_Utils::create_common (CORBA::dk_Interface,
                                          CORBA::dk_Attribute,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "attrs");

  // Store the path to the attribute's type definition.
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path);

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "mode",
                                             mode);

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Attribute,
                                          path.c_str (),
                                          this->repo_);

  CORBA::AttributeDef_var retval =
    CORBA::AttributeDef::_narrow (obj.in ());

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
                                      const CORBA::ContextIdSeq &contexts)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::OperationDef::_nil ());

  this->update_key ();

  return this->create_operation_i (id,
                                   name,
                                   version,
                                   result,
                                   mode,
                                   params,
                                   exceptions,
                                   contexts);
}

CORBA::OperationDef_ptr
TAO_InterfaceDef_i::create_operation_i (const char *id,
                                        const char *name,
                                        const char *version,
                                        CORBA::IDLType_ptr result,
                                        CORBA::OperationMode mode,
                                        const CORBA::ParDescriptionSeq &params,
                                        const CORBA::ExceptionDefSeq &exceptions,
                                        const CORBA::ContextIdSeq &contexts)
{
  // This will throw an exception if a name clash is found.
  // create_common() will check for all other errors.
  this->check_inherited (name,
                         CORBA::dk_Operation);

  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path =
    TAO_IFR_Service_Utils::create_common (CORBA::dk_Interface,
                                          CORBA::dk_Operation,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "ops");

  // Get the TypeCode for the return type.
  ACE_TString result_path (TAO_IFR_Service_Utils::reference_to_path (result));
  TAO_IDLType_i *result_impl =
    TAO_IFR_Service_Utils::path_to_idltype (result_path,
                                            this->repo_);

  CORBA::TypeCode_var rettype =
    result_impl->type_i ();

  CORBA::TCKind kind = rettype->kind ();

  // Oneway operations cannot have a non-void return type.
  if (mode == CORBA::OP_ONEWAY && kind != CORBA::tk_void)
    {
      throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 31, CORBA::COMPLETED_NO);
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
      char *type_path = 0;

      for (i = 0; i < length; ++i)
        {
          // Oneway operations cannot have INOUT or OUT parameters.
          if (mode == CORBA::OP_ONEWAY && params[i].mode != CORBA::PARAM_IN)
            {
              throw CORBA::BAD_PARAM (31, CORBA::COMPLETED_NO);
            }

          ACE_Configuration_Section_Key param_key;
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (params_key,
                                                stringified,
                                                1,
                                                param_key);

          this->repo_->config ()->set_string_value (param_key,
                                                    "name",
                                                    params[i].name.in ());
          type_path =
            TAO_IFR_Service_Utils::reference_to_path (
                params[i].type_def.in ()
              );

          this->repo_->config ()->set_string_value (param_key,
                                                    "type_path",
                                                    type_path);

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
          throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 31, CORBA::COMPLETED_NO);
        }

      ACE_Configuration_Section_Key excepts_key;
      this->repo_->config ()->open_section (new_key,
                                            "excepts",
                                            1,
                                            excepts_key);
      char *type_path = 0;

      for (i = 0; i < length; ++i)
        {
          type_path =
            TAO_IFR_Service_Utils::reference_to_path (exceptions[i]);

          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (excepts_key,
                                                    stringified,
                                                    type_path);
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
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (contexts_key,
                                                    stringified,
                                                    contexts[i].in ());
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Operation,
                                          path.c_str (),
                                          this->repo_);

  CORBA::OperationDef_var retval =
    CORBA::OperationDef::_narrow (obj.in ());

  return retval._retn ();
}

void
TAO_InterfaceDef_i::interface_contents (
    ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
    ACE_Unbounded_Queue<ACE_TString> &path_queue,
    CORBA::DefinitionKind limit_type,
    CORBA::Boolean exclude_inherited
  )
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
                                             exclude_inherited);
            }
        }
    }
}

int
TAO_InterfaceDef_i::name_clash (const char *name)
{
  try
    {
      TAO_Container_i::tmp_name_holder_ = name;
      TAO_IFR_Service_Utils::name_exists (&TAO_Container_i::same_as_tmp_name,
                                          TAO_IFR_Service_Utils::tmp_key_,
                                          TAO_IFR_Service_Utils::repo_,
                                          CORBA::dk_Interface);
    }
  catch (const CORBA::Exception&)
    {
      return 1;
    }

  return 0;
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

          def_kind = static_cast<CORBA::DefinitionKind> (kind);

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
              char *stringified = TAO_IFR_Service_Utils::int_to_string (j);
              this->repo_->config ()->open_section (attrs_key,
                                                    stringified,
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
              char *stringified = TAO_IFR_Service_Utils::int_to_string (j);
              this->repo_->config ()->open_section (ops_key,
                                                    stringified,
                                                    0,
                                                    op_key);

              key_queue.enqueue_tail (op_key);
            }
        }
    }
}

void
TAO_InterfaceDef_i::check_inherited (const char *name,
                                     CORBA::DefinitionKind kind)
{
  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  switch (kind)
  {
    case CORBA::dk_Attribute:
      this->inherited_attributes (key_queue);
      break;
    case CORBA::dk_Operation:
      this->inherited_operations (key_queue);
      break;
    default:
      break;
  }

  size_t size = key_queue.size ();
  ACE_Configuration_Section_Key inherited_key;
  ACE_TString inherited_name;

  for (u_int i = 0; i < size; ++i)
    {
      key_queue.dequeue_head (inherited_key);

      this->repo_->config ()->get_string_value (inherited_key,
                                                "name",
                                                inherited_name);

      if (inherited_name == name)
        {
          throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 5, CORBA::COMPLETED_NO);
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
