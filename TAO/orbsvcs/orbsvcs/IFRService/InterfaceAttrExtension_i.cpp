/* -*- C++ -*- */
// $Id$

#include "InterfaceAttrExtension_i.h"
#include "Repository_i.h"
#include "InterfaceDef_i.h"
#include "OperationDef_i.h"
#include "AttributeDef_i.h"
#include "ExtAttributeDef_i.h"
#include "IFR_Service_Utils.h"

ACE_RCSID (IFRService, 
           InterfaceAttrExtension_i, 
           "$Id$")

// =====================================================================

TAO_InterfaceAttrExtension_i::TAO_InterfaceAttrExtension_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo)
{
}

TAO_InterfaceAttrExtension_i::~TAO_InterfaceAttrExtension_i (void)
{
}


CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
TAO_InterfaceAttrExtension_i::describe_ext_interface (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_ext_interface_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
TAO_InterfaceAttrExtension_i::describe_ext_interface_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *fifd = 0;
  ACE_NEW_RETURN (fifd,
                  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription,
                  0);

  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var retval = 
    fifd;

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

  // Operations
  TAO_InterfaceDef_i iface (this->repo_);
  iface.section_key (this->section_key_);
  iface.inherited_operations (key_queue);

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

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, key_queue.size ());
  fifd->operations.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_OperationDef_i op (this->repo_);
      op.section_key (key);

      op.make_description (fifd->operations[i]
                           ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  // Attributes.
  iface.inherited_attributes (key_queue);

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

  size = ACE_static_cast (CORBA::ULong, 
                          key_queue.size ());
  fifd->attributes.length (size);

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      TAO_ExtAttributeDef_i attr (this->repo_);
      attr.section_key (key);

      attr.fill_description (fifd->attributes[i]
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  CORBA::InterfaceDefSeq_var bases = 
    iface.base_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

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
      this->repo_->config ()->get_string_value (base_key,
                                                "id",
                                                holder);
      repo_ids[i] = holder.fast_rep ();
    }

  fifd->base_interfaces = repo_ids;

  fifd->type = iface.type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

CORBA::ExtAttributeDef_ptr 
TAO_InterfaceAttrExtension_i::create_ext_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode,
    const CORBA::ExceptionDefSeq &get_exceptions,
    const CORBA::ExceptionDefSeq &set_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ExtAttributeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  return this->create_ext_attribute_i (id,
                                       name,
                                       version,
                                       type,
                                       mode,
                                       get_exceptions,
                                       set_exceptions
                                       ACE_ENV_ARG_PARAMETER);
}
  
CORBA::ExtAttributeDef_ptr 
TAO_InterfaceAttrExtension_i::create_ext_attribute_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode,
    const CORBA::ExceptionDefSeq &get_exceptions,
    const CORBA::ExceptionDefSeq &set_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder (name);
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
                                          "attrs"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  // Store the path to the attribute's type definition.
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path);

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "mode",
                                             mode);

  TAO_ExtAttributeDef_i xattr (this->repo_);
  xattr.section_key (new_key);

  xattr.exceptions ("get_excepts",
                    get_exceptions);

  xattr.exceptions ("set_excepts",
                    set_exceptions);

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Attribute,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  CORBA::ExtAttributeDef_var retval =
    CORBA::ExtAttributeDef::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  return retval._retn ();
}

