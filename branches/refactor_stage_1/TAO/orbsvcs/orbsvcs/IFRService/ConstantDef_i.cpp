/* -*- C++ -*- */
// $Id$

#include "ConstantDef_i.h"
#include "Repository_i.h"
#include "IFR_Service_Utils.h"
#include "IDLType_i.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           ConstantDef_i,
           "$Id$")

TAO_ConstantDef_i::TAO_ConstantDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ConstantDef_i::~TAO_ConstantDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ConstantDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Constant;
}

CORBA::Contained::Description *
TAO_ConstantDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ConstantDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::ConstantDescription cd;

  cd.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  cd.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);
  cd.defined_in = container_id.c_str ();

  cd.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  cd.type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Any_var val = this->value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  cd.value = val.in ();

  retval->value <<= cd;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_ConstantDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ConstantDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  TAO_IDLType_i *impl = TAO_IFR_Service_Utils::path_to_idltype (type_path,
                                                                this->repo_);

  return impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_ConstantDef_i::type_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_ConstantDef_i::type_def_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (type_path,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return CORBA::IDLType::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_ConstantDef_i::type_def (CORBA::IDLType_ptr type_def
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->type_def_i (type_def
                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_ConstantDef_i::type_def_i (CORBA::IDLType_ptr type_def
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            type_path);
}

CORBA::Any *
TAO_ConstantDef_i::value (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Any *
TAO_ConstantDef_i::value_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_var tc = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  void *ref = 0;
  size_t length = 0;

  this->repo_->config ()->get_binary_value (
                              this->section_key_,
                              "value",
                              ref,
                              length
                            );

  char *data = ACE_static_cast (char *, ref);
  ACE_Auto_Basic_Array_Ptr<char> safety (data);

  ACE_Message_Block mb (data,
                        length);
  mb.length (length);

  CORBA::Any *retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  TAO::Unknown_IDL_Type *impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO::Unknown_IDL_Type (tc._retn (),
                                           &mb,
                                           TAO_ENCAP_BYTE_ORDER,
                                           1),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  retval->replace (impl);
  safety.release ();
  return retval;
}

void
TAO_ConstantDef_i::value (const CORBA::Any &value
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->value_i (value
                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_ConstantDef_i::value_i (const CORBA::Any &value
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_var my_tc = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var val_tc = value.type ();

  if (!my_tc.in ()->equal (val_tc.in ()))
    {
      return;
    }

  ACE_Message_Block *mb = value._tao_get_cdr ();

  CORBA::TCKind kind = val_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  switch (kind)
  {
    // The data for these types will be aligned to an 8-byte
    // boundary, while the rd_ptr may not.
    case CORBA::tk_double:
    case CORBA::tk_ulonglong:
    case CORBA::tk_longlong:
    case CORBA::tk_longdouble:
      mb->rd_ptr (ACE_ptr_align_binary (mb->rd_ptr (),
                                        ACE_CDR::MAX_ALIGNMENT));
      break;
    default:
      break;
  }

  mb->crunch ();
  this->repo_->config ()->set_binary_value (this->section_key_,
                                            "value",
                                            mb->base (),
                                            mb->length ());
}

