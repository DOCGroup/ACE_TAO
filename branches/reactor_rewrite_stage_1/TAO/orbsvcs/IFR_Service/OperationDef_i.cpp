/* -*- C++ -*- */
// $Id$

#include "OperationDef_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "ExceptionDef_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, OperationDef_i, "$Id$")

TAO_OperationDef_i::TAO_OperationDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key)
{
}

TAO_OperationDef_i::~TAO_OperationDef_i (void)
{
}

CORBA::DefinitionKind
TAO_OperationDef_i::def_kind (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Operation;
}

CORBA_Contained::Description *
TAO_OperationDef_i::describe (TAO_ENV_SINGLE_ARG_DECL )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_Contained::Description *
TAO_OperationDef_i::describe_i (TAO_ENV_SINGLE_ARG_DECL )
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
TAO_OperationDef_i::result (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->result_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_OperationDef_i::result_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString result_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "result",
                                            result_path);

  ACE_Configuration_Section_Key result_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       result_path,
                                       result_key,
                                       0);

  TAO_IDLType_i *impl =
    this->repo_->servant_factory ()->create_idltype (result_key
                                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  return impl->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_OperationDef_i::result_def (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA_IDLType::_nil ());

  return this->result_def_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_IDLType_ptr
TAO_OperationDef_i::result_def_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString result_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "result",
                                            result_path);

  ACE_Configuration_Section_Key result_key;
  this->repo_->config ()->expand_path (this->repo_->root_key (),
                                       result_path,
                                       result_key,
                                       0);

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (result_key,
                                             "def_kind",
                                             kind);

  CORBA::DefinitionKind def_kind =
    ACE_static_cast (CORBA::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    result_path.c_str ()
                                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_IDLType::_nil ());

  return CORBA_IDLType::_narrow (obj.in ()
                                 TAO_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::result_def (CORBA_IDLType_ptr result_def
                                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->result_def_i (result_def
                      TAO_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::result_def_i (CORBA_IDLType_ptr result_def
                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (result_def
                                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var result_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "result",
                                            result_path.in ());
}

CORBA_ParDescriptionSeq *
TAO_OperationDef_i::params (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->params_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_ParDescriptionSeq *
TAO_OperationDef_i::params_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong i = 0;
  ACE_Configuration_Section_Key params_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "params",
                                          0,
                                          params_key);

  ACE_Unbounded_Queue<ACE_Configuration_Section_Key> key_queue;

  // This section may not have been created.
  if (status == 0)
    {
      u_int count = 0;
      this->repo_->config ()->get_integer_value (params_key,
                                                 "count",
                                                 count);

      for (i = 0; i < count; ++i)
        {
          CORBA::String_var section_name = this->int_to_string (i);
          ACE_Configuration_Section_Key param_key;
          status =
            this->repo_->config ()->open_section (params_key,
                                                  section_name.in (),
                                                  0,
                                                  param_key);

          if (status == 0)
            key_queue.enqueue_tail (param_key);
        }
    }

  size_t size = key_queue.size ();

  CORBA_ParDescriptionSeq *pd_seq = 0;
  ACE_NEW_THROW_EX (pd_seq,
                    CORBA_ParDescriptionSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  pd_seq->length (size);

  if (size == 0)
    return pd_seq;

  CORBA_ParDescriptionSeq_var retval = pd_seq;

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      ACE_TString name;
      this->repo_->config ()->get_string_value (key,
                                                "name",
                                                name);

      retval[i].name = name.c_str ();

      u_int mode = 0;
      this->repo_->config ()->get_integer_value (key,
                                                 "mode",
                                                 mode);

      retval[i].mode = ACE_static_cast (CORBA::ParameterMode, mode);

      ACE_TString type_path;
      this->repo_->config ()->get_string_value (key,
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
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_IDLType_i> safety (impl);

      retval[i].type = impl->type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

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
      ACE_CHECK_RETURN (0);

      retval[i].type_def = CORBA_IDLType::_narrow (obj.in ()
                                                   TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_OperationDef_i::params (const CORBA_ParDescriptionSeq &params
                            TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->params_i (params
                  TAO_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::params_i (const CORBA_ParDescriptionSeq &params
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "params",
                                          1);

  CORBA::ULong length = params.length ();

  if (length == 0)
    return;

  ACE_Configuration_Section_Key params_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "params",
                                        1,
                                        params_key);

  this->repo_->config ()->set_integer_value (params_key,
                                             "count",
                                             length);

  for (CORBA::ULong i = 0; i < length; i++)
    {
      ACE_Configuration_Section_Key param_key;
      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->open_section (params_key,
                                            section_name.in (),
                                            1,
                                            param_key);

      this->repo_->config ()->set_string_value (param_key,
                                                "name",
                                                params[i].name.in ());
      PortableServer::ObjectId_var oid =
        this->repo_->ir_poa ()->reference_to_id (params[i].type_def.in ()
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

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

CORBA::OperationMode
TAO_OperationDef_i::mode (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::OP_NORMAL);

  return this->mode_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::OperationMode
TAO_OperationDef_i::mode_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int mode;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "mode",
                                             mode);

  return ACE_static_cast (CORBA::OperationMode, mode);
}

void
TAO_OperationDef_i::mode (CORBA::OperationMode mode
                          TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->mode_i (mode
                TAO_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::mode_i (CORBA::OperationMode mode
                            TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

CORBA_ContextIdSeq *
TAO_OperationDef_i::contexts (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->contexts_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_ContextIdSeq *
TAO_OperationDef_i::contexts_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key contexts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "contexts",
                                          0,
                                          contexts_key);

  ACE_Unbounded_Queue<ACE_TString> context_queue;

  // This section may not have been created.
  if (status == 0)
    {
      int index = 0;
      CORBA::String_var field_name = this->int_to_string (index);
      ACE_TString context;

      // Don't have to worry about gaps here - contexts are not
      // IR objects so they can't be destroyed one at a time. To
      // make a change one has to call the mutator version of this
      // function and make a completely new list.
      while (this->repo_->config ()->get_string_value (contexts_key,
                                                       field_name.in (),
                                                       context)
              == 0)
        {
          context_queue.enqueue_tail (context);
          field_name = this->int_to_string (++index);
        }
    }

  size_t size = context_queue.size ();

  CORBA_ContextIdSeq *ci_seq = 0;
  ACE_NEW_THROW_EX (ci_seq,
                    CORBA_ContextIdSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  ci_seq->length (size);

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      ACE_TString context_string;
      context_queue.dequeue_head (context_string);

      (*ci_seq)[i] = context_string.c_str ();
    }

  return ci_seq;
}

void
TAO_OperationDef_i::contexts (const CORBA_ContextIdSeq &contexts
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->contexts_i (contexts
                    TAO_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::contexts_i (const CORBA_ContextIdSeq &contexts
                                TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "contexts",
                                          0);

  CORBA::ULong length = contexts.length ();

  if (length == 0)
    return;

  ACE_Configuration_Section_Key contexts_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "contexts",
                                        1,
                                        contexts_key);

  for (CORBA::ULong i = 0; i < length; i++)
    {
      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->set_string_value (contexts_key,
                                                section_name.in (),
                                                contexts[i].in ());
    }
}

CORBA_ExceptionDefSeq *
TAO_OperationDef_i::exceptions (TAO_ENV_SINGLE_ARG_DECL )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->exceptions_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CORBA_ExceptionDefSeq *
TAO_OperationDef_i::exceptions_i (TAO_ENV_SINGLE_ARG_DECL )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "excepts",
                                          0,
                                          excepts_key);

  int index = 0;
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  // This section may not have been created.
  if (status == 0)
    {
      ACE_TString field_name, type_path;
      ACE_Configuration::VALUETYPE type;
      u_int kind = 0;
      CORBA::DefinitionKind def_kind = CORBA::dk_none;
      ACE_Configuration_Section_Key except_key;

      while (this->repo_->config ()->enumerate_values (excepts_key,
                                                       index++,
                                                       field_name,
                                                       type)
              == 0)
        {
          this->repo_->config ()->get_string_value (excepts_key,
                                                    field_name.c_str (),
                                                    type_path);

          status =
            this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                 type_path,
                                                 except_key,
                                                 0);

          if (status == 0)
            {
              path_queue.enqueue_tail (type_path);

              this->repo_->config ()->get_integer_value (except_key,
                                                         "def_kind",
                                                         kind);

              def_kind =
                ACE_static_cast (CORBA::DefinitionKind, kind);

              kind_queue.enqueue_tail (def_kind);
            }
        }
    }

  size_t size = kind_queue.size ();

  CORBA_ExceptionDefSeq *ed_seq = 0;
  ACE_NEW_THROW_EX (ed_seq,
                    CORBA_ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  ed_seq->length (size);

  if (index == 0)
    return ed_seq;

  CORBA_ExceptionDefSeq_var retval = ed_seq;

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      CORBA::DefinitionKind def_kind;
      kind_queue.dequeue_head (def_kind);

      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        this->repo_->servant_factory ()->create_objref (def_kind,
                                                        path.c_str ()
                                                        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = CORBA_ExceptionDef::_narrow (obj.in ()
                                               TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_OperationDef_i::exceptions (const CORBA_ExceptionDefSeq &exceptions
                                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->exceptions_i (exceptions
                      TAO_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::exceptions_i (const CORBA_ExceptionDefSeq &exceptions
                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "excepts",
                                          0);

  CORBA::ULong length = exceptions.length ();

  if (length == 0)
    return;

  ACE_Configuration_Section_Key excepts_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "excepts",
                                        1,
                                        excepts_key);

  for (CORBA::ULong i = 0; i < length; i++)
    {
      PortableServer::ObjectId_var oid =
        this->repo_->ir_poa ()->reference_to_id (exceptions[i]
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var type_path =
        PortableServer::ObjectId_to_string (oid.in ());

      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->set_string_value (excepts_key,
                                                section_name.in (),
                                                type_path.in ());
    }

}

CORBA_OperationDescription
TAO_OperationDef_i::make_description (
      TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA_OperationDescription od;

  od.name = this->name_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.id = this->id_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  od.defined_in = container_id.c_str ();

  od.version = this->version_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.result = this->result_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.mode = this->mode_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  CORBA_ContextIdSeq_var cid_seq = this->contexts_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.contexts = cid_seq.in ();

  CORBA_ParDescriptionSeq_var pd_seq = this->params_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.parameters = pd_seq.in ();

  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "excepts",
                                          0,
                                          excepts_key);

  // This section may not have been created.
  if (status == 0)
    {
      int index = 0;
      int status = 0;
      ACE_TString field_name, type_path;
      ACE_Configuration::VALUETYPE type;
      ACE_Unbounded_Queue<ACE_TString> path_queue;

      while (this->repo_->config ()->enumerate_values (excepts_key,
                                                       index++,
                                                       field_name,
                                                       type)
              == 0)
        {
          status =
            this->repo_->config ()->get_string_value (excepts_key,
                                                      field_name.c_str (),
                                                      type_path);

          if (status == 0)
            {
              path_queue.enqueue_tail (type_path);
            }
        }

      size_t size = path_queue.size ();

      od.exceptions.length (size);

      for (CORBA::ULong i = 0; i < size; ++i)
        {
          ACE_TString type_path;
          path_queue.dequeue_head (type_path);

          ACE_Configuration_Section_Key type_key;
          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               type_path,
                                               type_key,
                                               0);

          ACE_TString name;
          this->repo_->config ()->get_string_value (type_key,
                                                    "name",
                                                    name);

          od.exceptions[i].name = name.c_str ();

          ACE_TString id;
          this->repo_->config ()->get_string_value (type_key,
                                                    "id",
                                                    id);

          od.exceptions[i].id = id.c_str ();

          ACE_TString container_id;
          this->repo_->config ()->get_string_value (type_key,
                                                    "container_id",
                                                    container_id);

          od.exceptions[i].defined_in = container_id.c_str ();

          ACE_TString version;
          this->repo_->config ()->get_string_value (type_key,
                                                    "version",
                                                    version);

          od.exceptions[i].version = version.c_str ();

          TAO_ExceptionDef_i impl (this->repo_,
                                   type_key);

          od.exceptions[i].type = impl.type_i (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (od);
        }
    }
  else
    {
      od.exceptions.length (0);
    }

  return od;
}

