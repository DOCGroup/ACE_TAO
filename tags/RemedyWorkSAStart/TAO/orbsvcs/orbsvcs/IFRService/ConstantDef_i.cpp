// $Id$

#include "orbsvcs/IFRService/ConstantDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IFRService/IDLType_i.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ConstantDef_i::TAO_ConstantDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_ConstantDef_i::~TAO_ConstantDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ConstantDef_i::def_kind (void)
{
  return CORBA::dk_Constant;
}

CORBA::Contained::Description *
TAO_ConstantDef_i::describe (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_ConstantDef_i::describe_i (void)
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind ();

  CORBA::ConstantDescription cd;

  cd.name = this->name_i ();

  cd.id = this->id_i ();

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);
  cd.defined_in = container_id.c_str ();

  cd.version = this->version_i ();

  cd.type = this->type_i ();

  CORBA::Any_var val = this->value_i ();

  cd.value = val.in ();

  retval->value <<= cd;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_ConstantDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_ConstantDef_i::type_i (void)
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  TAO_IDLType_i *impl = TAO_IFR_Service_Utils::path_to_idltype (type_path,
                                                                this->repo_);

  return impl->type_i ();
}

CORBA::IDLType_ptr
TAO_ConstantDef_i::type_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->type_def_i ();
}

CORBA::IDLType_ptr
TAO_ConstantDef_i::type_def_i (void)
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (type_path,
                                              this->repo_);

  return CORBA::IDLType::_narrow (obj.in ());
}

void
TAO_ConstantDef_i::type_def (CORBA::IDLType_ptr type_def)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->type_def_i (type_def);
}

void
TAO_ConstantDef_i::type_def_i (CORBA::IDLType_ptr type_def)
{
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            type_path);
}

CORBA::Any *
TAO_ConstantDef_i::value (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->value_i ();
}

CORBA::Any *
TAO_ConstantDef_i::value_i (void)
{
  CORBA::TypeCode_var tc = this->type_i ();

  void *ref = 0;
  size_t length = 0;

  this->repo_->config ()->get_binary_value (
                              this->section_key_,
                              "value",
                              ref,
                              length
                            );

  char *data = static_cast<char *> (ref);
  ACE_Auto_Basic_Array_Ptr<char> safety (data);

  ACE_Message_Block mb (data,
                        length);
  mb.length (length);
  TAO_InputCDR in_cdr (&mb);

  CORBA::Any *retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());

  TAO::Unknown_IDL_Type *impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO::Unknown_IDL_Type (tc.in (),
                                           in_cdr),
                    CORBA::NO_MEMORY ());

  retval->replace (impl);
  return retval;
}

void
TAO_ConstantDef_i::value (const CORBA::Any &value)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->value_i (value);
}

void
TAO_ConstantDef_i::value_i (const CORBA::Any &value)
{
  CORBA::TypeCode_var my_tc =
    this->type_i ();

  CORBA::TypeCode_var val_tc = value.type ();

  CORBA::Boolean const equal_tc =
    my_tc.in ()->equal (val_tc.in ());

  if (!equal_tc)
    {
      return;
    }

  ACE_Message_Block *mb = 0;
  TAO::Any_Impl *impl = value.impl ();

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      mb = unk->_tao_get_cdr ().steal_contents ();
    }
  else
    {
      TAO_OutputCDR out;
      impl->marshal_value (out);
      TAO_InputCDR in (out);
      mb = in.steal_contents ();
    }
  ACE_Auto_Ptr<ACE_Message_Block> safe (mb);

  CORBA::TCKind kind = val_tc->kind ();

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

TAO_END_VERSIONED_NAMESPACE_DECL
