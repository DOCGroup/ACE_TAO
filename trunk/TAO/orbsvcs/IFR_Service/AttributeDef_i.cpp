/* -*- C++ -*- */
// $Id$

#include "AttributeDef_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "ExceptionDef_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, AttributeDef_i, "$Id$")

TAO_AttributeDef_i::TAO_AttributeDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key)
{
}

TAO_AttributeDef_i::~TAO_AttributeDef_i (void)
{
}

IR::DefinitionKind
TAO_AttributeDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Attribute;
}

IR::Contained::Description *
TAO_AttributeDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
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

  retval->value <<= this->make_description (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

CORBA::TypeCode_ptr 
TAO_AttributeDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  ACE_Configuration_Section_Key type_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       type_path,
                                       type_key,
                                       0);

  TAO_IDLType_i *impl =
    this->repo_->servant_factory ()->create_idltype (type_key,
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  return impl->type (ACE_TRY_ENV);
}

IR::IDLType_ptr 
TAO_AttributeDef_i::type_def (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString type_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "type_path",
                                            type_path);

  ACE_Configuration_Section_Key type_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       type_path,
                                       type_key,
                                       0);

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (type_key,
                                             "def_kind",
                                             kind);

  IR::DefinitionKind def_kind =
    ACE_static_cast (IR::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    type_path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::IDLType::_nil ());

  IR::IDLType_var retval = IR::IDLType::_narrow (obj.in (),
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::IDLType::_nil ());

  return retval._retn ();
}

void 
TAO_AttributeDef_i::type_def (IR::IDLType_ptr type_def,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid = 
    this->repo_->ir_poa ()->reference_to_id (type_def,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var type_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            type_path.in ());
}

IR::AttributeMode 
TAO_AttributeDef_i::mode (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int mode = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "mode",
                                             mode);

  return ACE_static_cast (IR::AttributeMode, mode);
}

void 
TAO_AttributeDef_i::mode (IR::AttributeMode mode,
                          CORBA::Environment &A)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

IR::AttributeDescription
TAO_AttributeDef_i::make_description (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IR::AttributeDescription ad;

  ad.name = this->name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ad);

  ad.id = this->id (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ad);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ad.defined_in = container_id.c_str ();

  ad.version = this->version (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ad);

  ad.type = this->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ad);

  ad.mode = this->mode (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ad);

  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;
  int index = 0;
  int status;
  ACE_Configuration::VALUETYPE type;
  ACE_TString field_name;
  CORBA::ULong i = 0;

  // 'get' exceptions

  ACE_Configuration_Section_Key get_excepts_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "get_excepts",
                                          0,
                                          get_excepts_key);

  // This section may not have been created.
  if (status == 0)
    {
      while (this->repo_->config ()->enumerate_values (get_excepts_key,
                                                       index,
                                                       field_name,
                                                       type)
              == 0)
        {
          ACE_TString get_except_path;
          this->repo_->config ()->get_string_value (get_excepts_key,
                                                    field_name.c_str (),
                                                    get_except_path);

          ACE_Configuration_Section_Key get_except_key;
          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               get_except_path,
                                               get_except_key,
                                               0);

          key_queue.enqueue_tail (get_except_key);
          ++index;
        }
    }

  IR::ExcDescriptionSeq get_ed_seq (index);
  get_ed_seq.length (index);

  for (i = 0; i < (CORBA::ULong) index; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      IR::ExceptionDescription get_ed;

      ACE_TString name;
      this->repo_->config ()->get_string_value (key,
                                                "name",
                                                name);

      get_ed.name = name.c_str ();

      ACE_TString id;
      this->repo_->config ()->get_string_value (key,
                                                "id",
                                                id);

      get_ed.id = id.c_str ();

      ACE_TString container_id;
      this->repo_->config ()->get_string_value (key,
                                                "container_id",
                                                container_id);

      get_ed.defined_in = container_id.c_str ();

      ACE_TString version;
      this->repo_->config ()->get_string_value (key,
                                                "version",
                                                version);

      get_ed.version = version.c_str ();

      TAO_ExceptionDef_i impl (this->repo_,
                               key);

      get_ed.type = impl.type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (ad);

      get_ed_seq[i] = get_ed;
    }

  ad.get_exceptions = get_ed_seq;
  index = 0;

  // 'put' exceptions

  ACE_Configuration_Section_Key put_excepts_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "put_excepts",
                                          0,
                                          put_excepts_key);

  // This section may not have been created.
  if (status == 0)
    {
      while (this->repo_->config ()->enumerate_values (put_excepts_key,
                                                       index,
                                                       field_name,
                                                       type)
              == 0)
        {
          ACE_TString put_except_path;
          this->repo_->config ()->get_string_value (put_excepts_key,
                                                    field_name.c_str (),
                                                    put_except_path);

          ACE_Configuration_Section_Key put_except_key;
          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               put_except_path,
                                               put_except_key,
                                               0);

          key_queue.enqueue_tail (put_except_key);
          ++index;
        }
    }

  IR::ExcDescriptionSeq put_ed_seq (index);
  put_ed_seq.length (index);

  for (i = 0; i < (CORBA::ULong) index; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      IR::ExceptionDescription put_ed;

      ACE_TString name;
      this->repo_->config ()->get_string_value (key,
                                                "name",
                                                name);

      put_ed.name = name.c_str ();

      ACE_TString id;
      this->repo_->config ()->get_string_value (key,
                                                "id",
                                                id);

      put_ed.id = id.c_str ();

      ACE_TString container_id;
      this->repo_->config ()->get_string_value (key,
                                                "container_id",
                                                container_id);

      put_ed.defined_in = container_id.c_str ();

      ACE_TString version;
      this->repo_->config ()->get_string_value (key,
                                                "version",
                                                version);

      put_ed.version = version.c_str ();

      TAO_ExceptionDef_i impl (this->repo_,
                               key);

      put_ed.type = impl.type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      put_ed_seq[i] = put_ed;
    }

  ad.put_exceptions = put_ed_seq;

  return ad;
}

IR::ExceptionDefSeq *
TAO_AttributeDef_i::get_exceptions (CORBA::Environment &ACE_TRY_ENV)
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
      while (this->repo_->config ()->enumerate_values (get_excepts_key,
                                                       index,
                                                       field_name,
                                                       type)
              == 0)
        {
          ACE_TString get_except_path;
          this->repo_->config ()->get_string_value (get_excepts_key,
                                                    field_name.c_str (),
                                                    get_except_path);

          path_queue.enqueue_tail (get_except_path);
          ++index;
        }
    }

  IR::ExceptionDefSeq *get_ed_seq = 0;
  ACE_NEW_THROW_EX (get_ed_seq,
                    IR::ExceptionDefSeq (index),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  get_ed_seq->length (index);

  IR::ExceptionDefSeq_var retval = get_ed_seq;

  for (CORBA::ULong i = 0; i < (CORBA::ULong) index; ++i)
    {
      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj = 
        this->repo_->servant_factory ()->create_objref (IR::dk_Exception,
                                                        path.c_str (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[i] = IR::ExceptionDef::_narrow (obj.in (),
                                             ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

IR::ExceptionDefSeq *
TAO_AttributeDef_i::put_exceptions (CORBA::Environment &ACE_TRY_ENV)
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
      while (this->repo_->config ()->enumerate_values (put_excepts_key,
                                                       index,
                                                       field_name,
                                                       type)
              == 0)
        {
          ACE_TString put_except_path;
          this->repo_->config ()->get_string_value (put_excepts_key,
                                                    field_name.c_str (),
                                                    put_except_path);

          path_queue.enqueue_tail (put_except_path);
          ++index;
        }
    }

  IR::ExceptionDefSeq *put_ed_seq = 0;
  ACE_NEW_THROW_EX (put_ed_seq,
                    IR::ExceptionDefSeq (index),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  put_ed_seq->length (index);

  IR::ExceptionDefSeq_var retval = put_ed_seq;

  for (CORBA::ULong i = 0; i < (CORBA::ULong) index; ++i)
    {
      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj = 
        this->repo_->servant_factory ()->create_objref (IR::dk_Exception,
                                                        path.c_str (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[i] = IR::ExceptionDef::_narrow (obj.in (),
                                             ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

