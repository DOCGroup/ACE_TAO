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

CORBA::DefinitionKind
TAO_AttributeDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Attribute;
}

CORBA_Contained::Description *
TAO_AttributeDef_i::describe (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Contained::Description *
TAO_AttributeDef_i::describe_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA_Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA_Contained::Description,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA_Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  retval->value <<= this->make_description (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_AttributeDef_i::type (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_AttributeDef_i::type_i (TAO_ENV_SINGLE_ARG_DECL)
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
    this->repo_->servant_factory ()->create_idltype (type_key
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  return impl->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_AttributeDef_i::type_def (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_IDLType::_nil ());

  return this->type_def_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_AttributeDef_i::type_def_i (TAO_ENV_SINGLE_ARG_DECL)
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

  CORBA::DefinitionKind def_kind =
    ACE_static_cast (CORBA::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    type_path.c_str ()
                                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_IDLType::_nil ());

  CORBA_IDLType_var retval = CORBA_IDLType::_narrow (obj.in ()
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_IDLType::_nil ());

  return retval._retn ();
}

void
TAO_AttributeDef_i::type_def (CORBA_IDLType_ptr type_def
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->type_def_i (type_def
                    TAO_ENV_ARG_PARAMETER);
}

void
TAO_AttributeDef_i::type_def_i (CORBA_IDLType_ptr type_def
                                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (type_def
                                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var type_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "type_path",
                                            type_path.in ());
}

CORBA::AttributeMode
TAO_AttributeDef_i::mode (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ATTR_NORMAL);

  return this->mode_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::AttributeMode
TAO_AttributeDef_i::mode_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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
                          TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->mode_i (mode
                TAO_ENV_ARG_PARAMETER);
}

void
TAO_AttributeDef_i::mode_i (CORBA::AttributeMode mode
                            TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

CORBA_AttributeDescription
TAO_AttributeDef_i::make_description (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA_AttributeDescription ad;

  ad.name = this->name_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ad);

  ad.id = this->id_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ad);

  ACE_TString container_id;

  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  ad.defined_in = container_id.c_str ();

  ad.version = this->version_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ad);

  ad.type = this->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ad);

  ad.mode = this->mode_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ad);
/*
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

  CORBA_ExcDescriptionSeq get_ed_seq (index);
  get_ed_seq.length (index);

  for (i = 0; i < (CORBA::ULong) index; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      CORBA_ExceptionDescription get_ed;

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

      get_ed.type = impl.type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
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

  CORBA_ExcDescriptionSeq put_ed_seq (index);
  put_ed_seq.length (index);

  for (i = 0; i < (CORBA::ULong) index; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      CORBA_ExceptionDescription put_ed;

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

      put_ed.type = impl.type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (ad);

      put_ed_seq[i] = put_ed;
    }

  ad.put_exceptions = put_ed_seq;
*/
  return ad;
}

CORBA_ExceptionDefSeq *
TAO_AttributeDef_i::get_exceptions (TAO_ENV_SINGLE_ARG_DECL)
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

  CORBA_ExceptionDefSeq *get_ed_seq = 0;
  ACE_NEW_THROW_EX (get_ed_seq,
                    CORBA_ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  get_ed_seq->length (size);

  CORBA_ExceptionDefSeq_var retval = get_ed_seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        this->repo_->servant_factory ()->create_objref (CORBA::dk_Exception,
                                                        path.c_str ()
                                                        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA_ExceptionDef::_narrow (obj.in ()
                                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

CORBA_ExceptionDefSeq *
TAO_AttributeDef_i::put_exceptions (TAO_ENV_SINGLE_ARG_DECL)
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

  CORBA_ExceptionDefSeq *put_ed_seq = 0;
  ACE_NEW_THROW_EX (put_ed_seq,
                    CORBA_ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  put_ed_seq->length (size);

  CORBA_ExceptionDefSeq_var retval = put_ed_seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        this->repo_->servant_factory ()->create_objref (CORBA::dk_Exception,
                                                        path.c_str ()
                                                        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA_ExceptionDef::_narrow (obj.in ()
                                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

