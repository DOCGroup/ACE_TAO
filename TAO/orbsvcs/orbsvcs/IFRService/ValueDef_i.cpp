/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ValueDef_i.h"
#include "AttributeDef_i.h"
#include "OperationDef_i.h"
#include "IFR_Service_Utils.h"
#include "IFR_Service_Utils_T.h"

ACE_RCSID (IFRService, 
           ValueDef_i, 
           "$Id$")

TAO_ValueDef_i::TAO_ValueDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_ValueDef_i::~TAO_ValueDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Value;
}

void
TAO_ValueDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ValueDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // These will get rid of the repo ids, which Contained_i::destroy()'s
  // call to remove_section (recursive = 1) will not get, and also
  // destroy the attribute's anonymous type, if any.

  TAO_IFR_Generic_Utils<TAO_AttributeDef_i>::destroy_special (
      "attrs",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;

  TAO_IFR_Generic_Utils<TAO_OperationDef_i>::destroy_special (
      "ops",
      this->repo_,
      this->section_key_
      ACE_ENV_ARG_PARAMETER
    );
  ACE_CHECK;
}

CORBA::Contained::Description *
TAO_ValueDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ValueDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ValueDescription *vd = 0;
  ACE_NEW_RETURN (vd,
                  CORBA::ValueDescription,
                  0);
  CORBA::ValueDescription_var safe_vd = vd;
       
  this->fill_value_description (safe_vd.inout ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  
  CORBA::Contained::Description *cd = 0;
  ACE_NEW_RETURN (cd,
                  CORBA::Contained::Description,
                  0);
                  
  cd->kind = CORBA::dk_Value;
  cd->value <<= safe_vd._retn ();
  return cd;
}

CORBA::TypeCode_ptr
TAO_ValueDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ValueDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);
  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);
  CORBA::ValueModifier tm = CORBA::VM_NONE;
  CORBA::Boolean is_it = 
    this->is_abstract_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
    
  if (is_it)
    {
      tm = CORBA::VM_ABSTRACT;
    }
   else
    {
      is_it = this->is_custom (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
      
      if (is_it)
        {
          tm = CORBA::VM_CUSTOM;
        }
      else
        {
          is_it = this->is_truncatable (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
          
          if (is_it)
            {
              tm = CORBA::VM_TRUNCATABLE;
            }
        }
    }
    
  ACE_TString holder;
  int status =
    this->repo_->config ()->get_string_value (this->section_key_,
                                              "base_value",
                                              holder);
  CORBA::TypeCode_var base_tc = CORBA::TypeCode::_nil ();
                                              
  if (status == 0)
    {
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                holder.fast_rep (),
                                                holder);
      TAO_IDLType_i *base_type =
        TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                this->repo_);
      base_tc = base_type->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
    }
    
  CORBA::ValueMemberSeq vm_seq;
  this->fill_vm_seq (vm_seq
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
  
  return 
    this->repo_->tc_factory ()->create_value_tc (name.c_str (),
                                                 id.c_str (),
                                                 tm,
                                                 base_tc.in (),
                                                 vm_seq
                                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->supported_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::InterfaceDefSeq *seq = 0;
  ACE_NEW_RETURN (seq,
                  CORBA::InterfaceDefSeq,
                  0);
  CORBA::InterfaceDefSeq_var retval = seq;
  
  ACE_Configuration_Section_Key supported_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "supported",
                                          0,
                                          supported_key);
                                          
  if (status != 0)
    {
      seq->length (0);
      return retval._retn ();
    }
    
  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (supported_key,
                                             "count",
                                             count);
  seq->length (count);
  char *stringified = 0;
  ACE_TString holder;
  CORBA::Object_var obj;
  
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (supported_key,
                                                stringified,
                                                holder);
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                holder.fast_rep (),
                                                holder);
      obj = TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                      this->repo_
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA::InterfaceDef::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
    
  return retval._retn ();
}

void
TAO_ValueDef_i::supported_interfaces (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supported_interfaces_i (supported_interfaces
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::supported_interfaces_i (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "supported",
                                          0);
  ACE_Configuration_Section_Key supported_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "supported",
                                        1,
                                        supported_key);
  CORBA::ULong count = supported_interfaces.length ();
  this->repo_->config ()->set_integer_value (supported_key,
                                             "count",
                                             count);
  char *stringified = 0;
  const char *supported_id = 0;
  const char *supported_path = 0;
  CORBA::ULong kind = 0;
  CORBA::Boolean concrete_seen = 0;
  CORBA::DefinitionKind def_kind;
  
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      supported_path = 
        TAO_IFR_Service_Utils::reference_to_path (
            supported_interfaces[i].in ()
          );
          
      // Get the servant's key into the temporary key holder, because
      // the name clash checker for base interfaces is static, and has
      // no other way to know about a specific key.
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           supported_path,
                                           TAO_IFR_Service_Utils::tmp_key_,
                                           0);
      this->repo_->config ()->get_integer_value (
                                  TAO_IFR_Service_Utils::tmp_key_,
                                  "def_kind",
                                  kind
                                );
      def_kind = ACE_static_cast (CORBA::DefinitionKind, kind);
      
      if (def_kind == CORBA::dk_Interface)
        {
          if (concrete_seen == 0)
            {
              concrete_seen = 1;
            }
          else
            {
              ACE_THROW (CORBA::BAD_PARAM (12,
                                           CORBA::COMPLETED_NO));
              return;
            }
        }                            
      
      TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                          this->section_key_,
                                          this->repo_,
                                          CORBA::dk_Value
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      supported_id = supported_interfaces[i]->_interface_repository_id ();
      this->repo_->config ()->set_string_value (supported_key,
                                                stringified,
                                                supported_id);
    }
}

CORBA::InitializerSeq *
TAO_ValueDef_i::initializers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->initializers_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InitializerSeq *
TAO_ValueDef_i::initializers_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::InitializerSeq *iseq = 0;
  ACE_NEW_RETURN (iseq,
                  CORBA::InitializerSeq,
                  0);
  CORBA::InitializerSeq_var retval = iseq;
  
  ACE_Configuration_Section_Key initializers_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "initializers",
                                          0,
                                          initializers_key);
                                          
  if (status != 0)
    {
      iseq->length (0);
      return retval._retn ();
    }
    
  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (initializers_key,
                                             "count",
                                             count);
  iseq->length (count);
  char *stringified = 0;
  ACE_Configuration_Section_Key initializer_key, params_key, arg_key;
  ACE_TString holder;
  CORBA::ULong arg_count = 0;
  
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (initializers_key,
                                            stringified,
                                            0,
                                            initializer_key);
      this->repo_->config ()->get_string_value (initializer_key,
                                                "name",
                                                holder);
      retval[i].name = holder.fast_rep ();
      status =
        this->repo_->config ()->open_section (initializer_key,
                                              "params",
                                              0,
                                              params_key);
                                              
      if (status != 0)
        {
          retval[i].members.length (0);
          continue;
        }
        
      this->repo_->config ()->get_integer_value (params_key,
                                                 "count",
                                                 arg_count);
      retval[i].members.length (arg_count);
      CORBA::Object_var obj;
      
      for (CORBA::ULong j = 0; j < arg_count; ++j)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (j);
          this->repo_->config ()->open_section (params_key,
                                                stringified,
                                                0,
                                                arg_key);
          this->repo_->config ()->get_string_value (arg_key,
                                                    "arg_name",
                                                    holder);
          retval[i].members[j].name = holder.fast_rep ();
          this->repo_->config ()->get_string_value (arg_key,
                                                    "arg_path",
                                                    holder);
          TAO_IDLType_i *impl =
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          retval[i].members[j].type = 
            impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          
          obj = 
            TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                      this->repo_
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          
          retval[i].members[j].type_def =
            CORBA::IDLType::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }
    
  return retval._retn ();
}

void
TAO_ValueDef_i::initializers (const CORBA::InitializerSeq &initializers
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->initializers_i (initializers
                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::initializers_i (const CORBA::InitializerSeq &initializers
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "initializers",
                                          1);
  TAO_IFR_Service_Utils::set_initializers (initializers,
                                           this->repo_->config (),
                                           this->section_key_);
}

CORBA::ValueDef_ptr
TAO_ValueDef_i::base_value (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ValueDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueDef::_nil ());

  return this->base_value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDef_ptr
TAO_ValueDef_i::base_value_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString holder;
  int status = 
    this->repo_->config ()->get_string_value (this->section_key_,
                                              "base_value",
                                              holder);
                               
  if (status != 0)
    {
      return CORBA::ValueDef::_nil ();
    }
    
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            holder.fast_rep (),
                                            holder);
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueDef::_nil ());

  return CORBA::ValueDef::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::base_value (CORBA::ValueDef_ptr base_value
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->base_value_i (base_value
                      ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::base_value_i (CORBA::ValueDef_ptr base_value
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (base_value))
    {
      this->repo_->config ()->remove_value (this->section_key_,
                                            "base_value");
      return;
    }
    
  const char *base_path = 
    TAO_IFR_Service_Utils::reference_to_path (base_value);
      
  // Get the servant's key into the temporary key holder, because
  // the name clash checker for base valuetypes is static, and has
  // no other way to know about a specific key.
  this->repo_->config ()->expand_path (
                              this->repo_->root_key (),
                              base_path,
                              TAO_IFR_Service_Utils::tmp_key_,
                              0
                            );
  TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                      this->section_key_,
                                      this->repo_,
                                      CORBA::dk_Value
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
          
  this->repo_->config ()->set_string_value (
                              this->section_key_,
                              "base_value",
                              base_value->_interface_repository_id ()
                            );
}

CORBA::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->abstract_base_values_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ValueDefSeq *vd_seq = 0;
  ACE_NEW_RETURN (vd_seq,
                  CORBA::ValueDefSeq,
                  0);
  CORBA::ValueDefSeq_var retval = vd_seq;
  
  ACE_Configuration_Section_Key bases_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "abstract_bases",
                                          0,
                                          bases_key);
                                          
  if (status != 0)
    {
      vd_seq->length (0);
      return retval._retn ();
    }
    
  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (bases_key,
                                             "count",
                                             count);
  vd_seq->length (count);
  char *stringified = 0;
  ACE_TString holder;
  CORBA::Object_var obj;
  
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (bases_key,
                                                stringified,
                                                holder);
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                holder.fast_rep (),
                                                holder);
      obj = TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                      this->repo_
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA::ValueDef::_narrow (obj.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
    
  return retval._retn ();
}

void
TAO_ValueDef_i::abstract_base_values (
    const CORBA::ValueDefSeq &abstract_base_values
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->abstract_base_values_i (abstract_base_values
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::abstract_base_values_i (
    const CORBA::ValueDefSeq &abstract_base_values
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "abstract_bases",
                                          0);
  CORBA::ULong count = abstract_base_values.length ();
  
  if (count == 0)
    {
      return;
    }
    
  ACE_Configuration_Section_Key bases_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "abstract_bases",
                                        0,
                                        bases_key);
  this->repo_->config ()->set_integer_value (bases_key,
                                             "count",
                                             count);                          
  const char *base_id = 0;
  const char *base_path = 0;
  char *stringified = 0;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      base_path = 
        TAO_IFR_Service_Utils::reference_to_path (
            abstract_base_values[i].in ()
          );
          
      // Get the servant's key into the temporary key holder, because
      // the name clash checker for base interfaces is static, and has
      // no other way to know about a specific key.
      this->repo_->config ()->expand_path (
                                  this->repo_->root_key (),
                                  base_path,
                                  TAO_IFR_Service_Utils::tmp_key_,
                                  0
                                );
      TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                          this->section_key_,
                                          this->repo_,
                                          CORBA::dk_Value
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      base_id = abstract_base_values[i]->_interface_repository_id ();
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (bases_key,
                                                stringified,
                                                base_id);
    }
}

CORBA::Boolean
TAO_ValueDef_i::is_abstract (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_abstract_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_abstract_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong is_it = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_abstract",
                                             is_it);
  return ACE_static_cast (CORBA::Boolean,
                          is_it);
}

void
TAO_ValueDef_i::is_abstract (CORBA::Boolean is_abstract
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_abstract (is_abstract
                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_abstract_i (CORBA::Boolean is_abstract
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (
                              this->section_key_,
                              "is_abstract",
                              ACE_static_cast (CORBA::ULong,
                                               is_abstract));
}

CORBA::Boolean
TAO_ValueDef_i::is_custom (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_custom_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_custom_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong is_it = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_custom",
                                             is_it);
  return ACE_static_cast (CORBA::Boolean,
                          is_it);
}

void
TAO_ValueDef_i::is_custom (CORBA::Boolean is_custom
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_custom_i (is_custom
                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_custom_i (CORBA::Boolean is_custom
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (
                              this->section_key_,
                              "is_custom",
                              ACE_static_cast (CORBA::ULong,
                                               is_custom));
}

CORBA::Boolean
TAO_ValueDef_i::is_truncatable (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_truncatable_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_truncatable_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong is_it = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_truncatable",
                                             is_it);
  return ACE_static_cast (CORBA::Boolean,
                          is_it);
}

void
TAO_ValueDef_i::is_truncatable (CORBA::Boolean is_truncatable
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_truncatable_i (is_truncatable
                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_truncatable_i (CORBA::Boolean is_truncatable
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (
                              this->section_key_,
                              "is_truncatable",
                              ACE_static_cast (CORBA::ULong,
                                               is_truncatable));
}

CORBA::Boolean
TAO_ValueDef_i::is_a (const char *id
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_a_i (id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_a_i (const char *id
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (id, "IDL:omg.org/CORBA/ValueBase:1.0") == 0)
    {
      return 1;
    }

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            holder);

  // Is it our type?
  if (ACE_OS::strcmp (holder.fast_rep (), id) == 0)
    {
      return 1;
    }

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_value",
                                            holder);

  // Is it our concrete base type?
  if (ACE_OS::strcmp (holder.fast_rep (), id) == 0)
    {
      return 1;
    }

  ACE_Configuration_Section_Key bases_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "abstract_bases",
                                          0,
                                          bases_key);
                                          
  if (status != 0)
    {
      return 0;
    }
        
  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (bases_key,
                                             "count",
                                             count);
  char *stringified = 0;
  ACE_Configuration_Section_Key base_key;
  CORBA::Boolean success = 0;
                                             
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (bases_key,
                                                stringified,
                                                holder);
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                holder.fast_rep (),
                                                holder);
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           base_key,
                                           0);
      TAO_ValueDef_i impl (this->repo_);
      impl.section_key (base_key);
      success = impl.is_a_i (id
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      
      if (success)
        {
          return 1;
        }
    }

  return 0;
}

CORBA::ValueDef::FullValueDescription *
TAO_ValueDef_i::describe_value (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDef::FullValueDescription *
TAO_ValueDef_i::describe_value_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ValueDef::FullValueDescription *fv_desc = 0;
  ACE_NEW_RETURN (fv_desc,
                  CORBA::ValueDef::FullValueDescription,
                  0);
  CORBA::ValueDef::FullValueDescription_var retval = fv_desc;
  
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            holder);
  fv_desc->name = holder.fast_rep ();
  this->repo_->config ()->get_string_value  (this->section_key_,
                                             "id",
                                             holder);
  fv_desc->id = holder.fast_rep ();

  CORBA::ULong val = 0;

  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_abstract",
                                             val);
  fv_desc->is_abstract = ACE_static_cast (CORBA::Boolean, val);
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_custom",
                                             val);
  fv_desc->is_custom = ACE_static_cast (CORBA::Boolean, val);
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            holder);
  fv_desc->defined_in = holder.fast_rep ();
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "version",
                                            holder);
  fv_desc->version = holder.fast_rep ();

  // Operations.

  fv_desc->operations.length (0);

  ACE_Configuration_Section_Key ops_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "ops",
                                          0,
                                          ops_key);
  CORBA::ULong count = 0;
  CORBA::ULong param_count = 0;
  char *stringified = 0;
  CORBA::ULong i = 0;
  CORBA::ULong j = 0;
  TAO_IDLType_i *idl_type = 0;
  CORBA::Object_var obj;
  
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (ops_key,
                                                 "count",
                                                 count);
      fv_desc->operations.length (count);
      ACE_Configuration_Section_Key op_key, result_key, contexts_key;
      ACE_Configuration_Section_Key params_key, excepts_key;
      
      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (ops_key,
                                                stringified,
                                                0,
                                                op_key);
          this->repo_->config ()->get_string_value (op_key,
                                                    "name",
                                                    holder);
          fv_desc->operations[i].name = holder.fast_rep ();
          this->repo_->config ()->get_string_value (op_key,
                                                    "id",
                                                    holder);
          fv_desc->operations[i].id = holder.fast_rep ();
          this->repo_->config ()->get_string_value (this->section_key_,
                                                    "id",
                                                    holder);
          fv_desc->operations[i].defined_in = holder.fast_rep ();
          this->repo_->config ()->get_string_value (op_key,
                                                    "result",
                                                    holder);
          idl_type =
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          fv_desc->operations[i].result = 
            idl_type->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          this->repo_->config ()->get_integer_value (op_key,
                                                     "mode",
                                                     val);
          fv_desc->operations[i].mode = ACE_static_cast (CORBA::OperationMode,
                                                         val);
          CORBA::TCKind kind = 
            fv_desc->operations[i].result->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          
          if (fv_desc->operations[i].mode == CORBA::OP_ONEWAY 
              && kind != CORBA::tk_void)
            {
              ACE_THROW_RETURN (CORBA::BAD_PARAM (31,
                                                  CORBA::COMPLETED_NO),
                                0);
            }

          // Operation contexts.
            
          TAO_IFR_Strseq_Utils<CORBA::ContextIdSeq>::fill_string_seq (
              "contexts",
              this->repo_->config (),
              op_key,
              fv_desc->operations[i].contexts
            );

          // Operation parameters.

          fv_desc->operations[i].parameters.length (0);
          status =
            this->repo_->config ()->open_section (op_key,
                                                  "params",
                                                  0,
                                                  params_key);
          if (status == 0)
            {
              ACE_Configuration_Section_Key param_key;
              this->repo_->config ()->get_integer_value (params_key,
                                                        "count",
                                                        param_count);                                                  
              fv_desc->operations[i].parameters.length (param_count);
              
              for (j = 0; j < param_count; ++j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->open_section (params_key,
                                                        stringified,
                                                        0,
                                                        param_key);
                  this->repo_->config ()->get_string_value (param_key,
                                                            "name",
                                                            holder);
                  fv_desc->operations[i].parameters[j].name = 
                    holder.fast_rep ();
                  this->repo_->config ()->get_string_value (param_key,
                                                            "type_path",
                                                            holder);
                  idl_type = 
                    TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                            this->repo_);
                  fv_desc->operations[i].parameters[j].type =
                    idl_type->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_CHECK_RETURN (0);
                  
                  obj = 
                    TAO_IFR_Service_Utils::path_to_ir_object (
                                               holder,
                                               this->repo_
                                               ACE_ENV_ARG_PARAMETER
                                             );
                  ACE_CHECK_RETURN (0);

                  fv_desc->operations[i].parameters[j].type_def =
                    CORBA::IDLType::_narrow (obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK_RETURN (0);

                  this->repo_->config ()->get_integer_value (param_key,
                                                             "mode",
                                                             val);
                  fv_desc->operations[i].parameters[j].mode = 
                    ACE_static_cast (CORBA::ParameterMode, 
                                     val);
                }
            }

          // Operation exceptions.
          
          status =
            this->repo_->config ()->open_section (op_key,
                                                  "excepts",
                                                  0,
                                                  excepts_key);
          fv_desc->operations[i].exceptions.length (0);
          
          if (status == 0)
            {
              this->repo_->config ()->get_integer_value (excepts_key,
                                                         "count",
                                                         count);
              fv_desc->operations[i].exceptions.length (count);
              ACE_Configuration_Section_Key except_def_key;

              for (j = 0; j < count; ++j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->get_string_value (excepts_key,
                                                            stringified,
                                                            holder);

                  // Get the TAO_IDLType_i before 'holder' is clobbered.
                  idl_type = 
                    TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                            this->repo_);
                  this->repo_->config ()->expand_path (
                                              this->repo_->root_key (),
                                              holder,
                                              except_def_key,
                                              0
                                            );
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            "name",
                                                            holder);
                  fv_desc->operations[i].exceptions[j].name = 
                    holder.fast_rep ();
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            "id",
                                                            holder);
                  fv_desc->operations[i].exceptions[j].id = 
                    holder.fast_rep ();
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            "container_id",
                                                            holder);
                  fv_desc->operations[i].exceptions[j].defined_in = 
                    holder.fast_rep ();
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            "version",
                                                            holder);
                  fv_desc->operations[i].exceptions[j].version = 
                    holder.fast_rep ();

                  // Use the TAO_IDLType_i fetched above.
                  fv_desc->operations[i].exceptions[j].type =
                    idl_type->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_CHECK_RETURN (0);
                }
            }                          
        }
    }
    
  // Attributes..

  fv_desc->attributes.length (0);
  
  ACE_Configuration_Section_Key attrs_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "attrs",
                                          0,
                                          attrs_key);
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (attrs_key,
                                                 "count",
                                                 count);
      fv_desc->attributes.length (count);
      ACE_Configuration_Section_Key attr_key, attr_def_key;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (attrs_key,
                                                stringified,
                                                0,
                                                attr_key);
          this->repo_->config ()->get_string_value (attr_key,
                                                    "name",
                                                    holder);
          fv_desc->attributes[i].name = holder.fast_rep ();
          this->repo_->config ()->get_string_value (attr_key,
                                                    "id",
                                                    holder);
          fv_desc->attributes[i].id = holder.fast_rep ();
          this->repo_->config ()->get_string_value (attr_key,
                                                    "version",
                                                    holder);
          fv_desc->attributes[i].version = holder.fast_rep ();
          this->repo_->config ()->get_integer_value (attr_key,
                                                     "mode",
                                                     val);
          fv_desc->attributes[i].mode =
            ACE_static_cast (CORBA::AttributeMode, 
                             val);
          this->repo_->config ()->get_string_value (attr_key,
                                                    "type_path",
                                                    holder);

          // Use the type path for 'defined_in' and 'type'.

          idl_type = 
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          fv_desc->attributes[i].type =
            idl_type->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               holder,
                                               attr_def_key,
                                               0);
          this->repo_->config ()->get_string_value (attr_def_key,
                                                    "container_id",
                                                    holder);
          fv_desc->attributes[i].defined_in = holder.fast_rep ();
        }
    }

  // Members..

  fv_desc->members.length (0);
  
  ACE_Configuration_Section_Key members_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "members",
                                          0,
                                          members_key);
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (members_key,
                                                 "count",
                                                 count);
      fv_desc->members.length (count);
      ACE_Configuration_Section_Key member_key, member_def_key;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (members_key,
                                                stringified,
                                                0,
                                                member_key);
          this->repo_->config ()->get_string_value (member_key,
                                                    "name",
                                                    holder);
          fv_desc->members[i].name = holder.fast_rep ();
          this->repo_->config ()->get_string_value (member_key,
                                                    "id",
                                                    holder);
          fv_desc->members[i].id = holder.fast_rep ();
          this->repo_->config ()->get_string_value (member_key,
                                                    "container_id",
                                                    holder);
          fv_desc->members[i].defined_in = holder.fast_rep ();
          this->repo_->config ()->get_string_value (member_key,
                                                    "version",
                                                    holder);
          fv_desc->members[i].version = holder.fast_rep ();

          this->repo_->config ()->get_integer_value (member_key,
                                                     "access",
                                                     val);
          fv_desc->members[i].access =
            ACE_static_cast (CORBA::Visibility, 
                             val);

          // Use type path for 'type' and 'type_def',

          this->repo_->config ()->get_string_value (member_key,
                                                    "type_path",
                                                    holder);
          idl_type = 
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          fv_desc->members[i].type =
            idl_type->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          obj = 
            TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                      this->repo_
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          
          fv_desc->members[i].type_def =
            CORBA::IDLType::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }

  // Initializers

  fv_desc->initializers.length (0);
  
  ACE_Configuration_Section_Key initializers_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "initializers",
                                          0,
                                          initializers_key);

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (initializers_key,
                                                 "count",
                                                 count);
      fv_desc->initializers.length (count);
      ACE_Configuration_Section_Key initializer_key, params_key, arg_key;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (initializers_key,
                                                stringified,
                                                0,
                                                initializer_key);
          this->repo_->config ()->get_string_value (initializer_key,
                                                    "name",
                                                    holder);
          fv_desc->initializers[i].name = holder.fast_rep ();

          fv_desc->initializers[i].members.length (0);
          status =
            this->repo_->config ()->open_section (initializer_key,
                                                  stringified,
                                                  0,
                                                  params_key);

          if (status == 0)
            {
              this->repo_->config ()->get_integer_value (params_key,
                                                         "count",
                                                         param_count);
              fv_desc->initializers[i].members.length (param_count);

              for (j = 0; j < param_count; +j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->open_section (params_key,
                                                        stringified,
                                                        0,
                                                        arg_key);
                  this->repo_->config ()->get_string_value (arg_key,
                                                            "arg_name",
                                                            holder);
                  fv_desc->initializers[i].members[j].name = 
                    holder.fast_rep ();
                  this->repo_->config ()->get_string_value (arg_key,
                                                            "arg_path",
                                                            holder);
                  obj = 
                    TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                              this->repo_);
                  fv_desc->initializers[i].members[j].type_def =
                    CORBA::IDLType::_narrow (obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK_RETURN (0);
                }
            }
        }
    }
                                          
  TAO_IFR_Strseq_Utils<CORBA::RepositoryIdSeq>::fill_string_seq (
      "supported",
      this->repo_->config (),
      this->section_key_,
      fv_desc->supported_interfaces
    );
  
  TAO_IFR_Strseq_Utils<CORBA::RepositoryIdSeq>::fill_string_seq (
      "abstract_bases",
      this->repo_->config (),
      this->section_key_,
      fv_desc->abstract_base_values
    );

  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_truncatable",
                                             val);
  fv_desc->is_truncatable = ACE_static_cast (CORBA::Boolean,
                                             val);
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_value",
                                            holder);
  fv_desc->base_value = holder.fast_rep ();
  fv_desc->type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  
  return retval._retn ();
}

CORBA::ValueMemberDef_ptr
TAO_ValueDef_i::create_value_member (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::IDLType_ptr type,
                                     CORBA::Visibility access
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ValueMemberDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueMemberDef::_nil ());

  return this->create_value_member_i (id,
                                      name,
                                      version,
                                      type,
                                      access
                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueMemberDef_ptr
TAO_ValueDef_i::create_value_member_i (const char *id,
                                       const char *name,
                                       const char *version,
                                       CORBA::IDLType_ptr type,
                                       CORBA::Visibility access
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (CORBA::dk_Value,
                                          CORBA::dk_ValueMember,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "members"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueMemberDef::_nil ());

  // Store the path to the attribute's type definition.
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path);

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "access",
                                             access);

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_ValueMember,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueMemberDef::_nil ());

  CORBA::ValueMemberDef_var retval =
    CORBA::ValueMemberDef::_narrow (obj.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueMemberDef::_nil ());

  return retval._retn ();
}

CORBA::AttributeDef_ptr
TAO_ValueDef_i::create_attribute (
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
TAO_ValueDef_i::create_attribute_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (CORBA::dk_Value,
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
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

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
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  CORBA::AttributeDef_var retval =
    CORBA::AttributeDef::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  return retval._retn ();
}

CORBA::OperationDef_ptr
TAO_ValueDef_i::create_operation (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr result,
    CORBA::OperationMode mode,
    const CORBA::ParDescriptionSeq &params,
    const CORBA::ExceptionDefSeq &exceptions,
    const CORBA::ContextIdSeq &contexts
    ACE_ENV_ARG_DECL
  )
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
TAO_ValueDef_i::create_operation_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr result,
    CORBA::OperationMode mode,
    const CORBA::ParDescriptionSeq &params,
    const CORBA::ExceptionDefSeq &exceptions,
    const CORBA::ContextIdSeq &contexts
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;

  // Common to all IR objects created in CORBA::Container.
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (CORBA::dk_Value,
                                          CORBA::dk_Operation,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "ops"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  // Get the TypeCode for the return type.
  ACE_TString result_path (TAO_IFR_Service_Utils::reference_to_path (result));
  TAO_IDLType_i *result_impl = 
    TAO_IFR_Service_Utils::path_to_idltype (result_path,
                                            this->repo_);

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
      char *type_path = 0;

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
          ACE_THROW_RETURN (CORBA::BAD_PARAM (31,
                                              CORBA::COMPLETED_NO),
                            CORBA::OperationDef::_nil ());
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
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  CORBA::OperationDef_var retval =
    CORBA::OperationDef::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  return retval._retn ();
}

int
TAO_ValueDef_i::name_clash (const char *name)
{
  ACE_TRY_NEW_ENV
    {
      TAO_Container_i::tmp_name_holder_ = name;
      TAO_IFR_Service_Utils::name_exists (&TAO_Container_i::same_as_tmp_name,
                                          TAO_IFR_Service_Utils::tmp_key_,
                                          TAO_IFR_Service_Utils::repo_,
                                          CORBA::dk_Value
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return 1;
    }
  ACE_ENDTRY;
  
  return 0;
}

void
TAO_ValueDef_i::fill_vm_seq (CORBA::ValueMemberSeq &vm_seq
                             ACE_ENV_ARG_DECL)
{
  ACE_Configuration_Section_Key members_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "members",
                                          0,
                                          members_key);
                                          
  if (status != 0)
    {
      vm_seq.length (0);
      return;
    }
    
  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (members_key,
                                             "count",
                                             count);
  vm_seq.length (count);
  char *stringified = 0;
  ACE_Configuration_Section_Key member_key, type_key;
  ACE_TString holder;
  CORBA::ULong access = 0;
  
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (members_key,
                                            stringified,
                                            0,
                                            member_key);
      this->repo_->config ()->get_string_value (member_key,
                                                "name",
                                                holder);
      vm_seq[i].name = holder.fast_rep ();
      this->repo_->config ()->get_string_value (member_key,
                                                "id",
                                                holder);
      vm_seq[i].id = holder.fast_rep ();
      this->repo_->config ()->get_string_value (this->section_key_,
                                                "id",
                                                holder);
      vm_seq[i].defined_in = holder.fast_rep ();
      this->repo_->config ()->get_string_value (member_key,
                                                "version",
                                                holder);
      vm_seq[i].version = holder.fast_rep ();
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                vm_seq[i].id.in (),
                                                holder);
      TAO_IDLType_i *impl =
        TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                this->repo_);
      vm_seq[i].type = impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      
      this->repo_->config ()->get_integer_value (member_key,
                                                 "access",
                                                 access);
      vm_seq[i].access = ACE_static_cast (CORBA::Visibility,
                                          access);                                   
    }
}

void
TAO_ValueDef_i::fill_value_description (CORBA::ValueDescription &desc 
                                        ACE_ENV_ARG_DECL)
{
  desc.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  
  desc.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  
  CORBA::ULong tmp = this->is_abstract_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  desc.is_abstract = ACE_static_cast (CORBA::Boolean,
                                      tmp);
                                     
  tmp = this->is_custom_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  desc.is_custom = ACE_static_cast (CORBA::Boolean,
                                    tmp);
    
  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            holder);                               
  desc.defined_in = holder.fast_rep ();
  
  desc.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  
  TAO_IFR_Strseq_Utils<CORBA::RepositoryIdSeq>::fill_string_seq (
      "supported",
      this->repo_->config (),
      this->section_key_,
      desc.supported_interfaces
    );
  
  TAO_IFR_Strseq_Utils<CORBA::RepositoryIdSeq>::fill_string_seq (
      "abstract_bases",
      this->repo_->config (),
      this->section_key_,
      desc.abstract_base_values
    );
  
  tmp = this->is_truncatable_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  desc.is_truncatable = ACE_static_cast (CORBA::Boolean,
                                         tmp);
                                        
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "base_value",
                                            holder);                               
  desc.base_value = holder.fast_rep ();
}

