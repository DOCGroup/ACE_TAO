/* -*- C++ -*- */
// $Id$

#include "AttributeDef_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "ExceptionDef_i.h"
#include "IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           AttributeDef_i, 
           "$Id$")

TAO_AttributeDef_i::TAO_AttributeDef_i (
    TAO_Repository_i *repo
  )
  : TAO_Contained_i (repo),
    TAO_IRObject_i (repo)
{
}

TAO_AttributeDef_i::~TAO_AttributeDef_i (void)
{
}

CORBA::DefinitionKind
TAO_AttributeDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Attribute;
}

CORBA::Contained::Description *
TAO_AttributeDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_AttributeDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = CORBA::dk_Attribute;

  CORBA::AttributeDescription *ad = 0;
  ACE_NEW_RETURN (ad,
                  CORBA::AttributeDescription,
                  0);

  this->make_description (*ad
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  retval->value <<= ad;
  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_AttributeDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_AttributeDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  TAO_IDLType_i *impl = 
    TAO_IFR_Service_Utils::path_to_idltype (type_path,
                                            this->repo_);

  return impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_AttributeDef_i::type_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->type_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_AttributeDef_i::type_def_i (ACE_ENV_SINGLE_ARG_DECL)
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

  CORBA::IDLType_var retval = CORBA::IDLType::_narrow (obj.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return retval._retn ();
}

void
TAO_AttributeDef_i::type_def (CORBA::IDLType_ptr type_def
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
TAO_AttributeDef_i::type_def_i (CORBA::IDLType_ptr type_def
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *type_path =
    TAO_IFR_Service_Utils::reference_to_path (type_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            type_path);
}

CORBA::AttributeMode
TAO_AttributeDef_i::mode (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ATTR_NORMAL);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ATTR_NORMAL);

  return this->mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::AttributeMode
TAO_AttributeDef_i::mode_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int mode = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "mode",
                                             mode);

  return ACE_static_cast (CORBA::AttributeMode, mode);
}

void
TAO_AttributeDef_i::mode (CORBA::AttributeMode mode
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->mode_i (mode
                ACE_ENV_ARG_PARAMETER);
}

void
TAO_AttributeDef_i::mode_i (CORBA::AttributeMode mode
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

void
TAO_AttributeDef_i::make_description (
    CORBA::AttributeDescription &ad
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ad.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ad.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ad.defined_in = container_id.c_str ();

  ad.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ad.type = this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ad.mode = this->mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::ExceptionDefSeq *
TAO_AttributeDef_i::get_exceptions (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<ACE_TString> path_queue;
  int index = 0;
  ACE_Configuration::VALUETYPE type;
  ACE_TString field_name;

  ACE_Configuration_Section_Key get_excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "get_excepts",
                                          0,
                                          get_excepts_key);

  // This section may not have been created.
  if (status == 0)
    {
      ACE_TString get_except_path;
      ACE_Configuration_Section_Key dummy;

      while (this->repo_->config ()->enumerate_values (get_excepts_key,
                                                       index++,
                                                       field_name,
                                                       type)
              == 0)
        {
          this->repo_->config ()->get_string_value (get_excepts_key,
                                                    field_name.c_str (),
                                                    get_except_path);

          // This exception may have been destroyed
          status =
            this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                 get_except_path,
                                                 dummy,
                                                 0);

          if (status == 0)
            {
              path_queue.enqueue_tail (get_except_path);
            }
        }
    }

  size_t size = path_queue.size ();

  CORBA::ExceptionDefSeq *get_ed_seq = 0;
  ACE_NEW_THROW_EX (get_ed_seq,
                    CORBA::ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  get_ed_seq->length (size);
  CORBA::ExceptionDefSeq_var retval = get_ed_seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        TAO_IFR_Service_Utils::create_objref (CORBA::dk_Exception,
                                              path.c_str (),
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA::ExceptionDef::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

CORBA::ExceptionDefSeq *
TAO_AttributeDef_i::put_exceptions (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<ACE_TString> path_queue;
  int index = 0;
  ACE_Configuration::VALUETYPE type;
  ACE_TString field_name;

  ACE_Configuration_Section_Key put_excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "put_excepts",
                                          0,
                                          put_excepts_key);

  // This section may not have been created.
  if (status == 0)
    {
      ACE_TString put_except_path;
      ACE_Configuration_Section_Key dummy;

      while (this->repo_->config ()->enumerate_values (put_excepts_key,
                                                       index++,
                                                       field_name,
                                                       type)
              == 0)
        {
          this->repo_->config ()->get_string_value (put_excepts_key,
                                                    field_name.c_str (),
                                                    put_except_path);

          // This exception may have been destroyed
          status =
            this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                 put_except_path,
                                                 dummy,
                                                 0);

          if (status == 0)
            {
              path_queue.enqueue_tail (put_except_path);
            }
        }
    }

  size_t size = path_queue.size ();

  CORBA::ExceptionDefSeq *put_ed_seq = 0;
  ACE_NEW_THROW_EX (put_ed_seq,
                    CORBA::ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  put_ed_seq->length (size);
  CORBA::ExceptionDefSeq_var retval = put_ed_seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        TAO_IFR_Service_Utils::create_objref (CORBA::dk_Exception,
                                              path.c_str (),
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA::ExceptionDef::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}


