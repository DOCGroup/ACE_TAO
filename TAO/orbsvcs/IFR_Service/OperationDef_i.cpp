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

IR::DefinitionKind
TAO_OperationDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Operation;
}

IR::Contained::Description *
TAO_OperationDef_i::describe (CORBA::Environment &ACE_TRY_ENV )
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
TAO_OperationDef_i::result (CORBA::Environment &ACE_TRY_ENV)
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
    this->repo_->servant_factory ()->create_idltype (result_key,
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  auto_ptr<TAO_IDLType_i> safety (impl);

  return impl->type (ACE_TRY_ENV);
}

IR::IDLType_ptr 
TAO_OperationDef_i::result_def (CORBA::Environment &ACE_TRY_ENV)
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

  IR::DefinitionKind def_kind =
    ACE_static_cast (IR::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    result_path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::IDLType::_nil ());

  return IR::IDLType::_narrow (obj.in (),
                               ACE_TRY_ENV);
}

void 
TAO_OperationDef_i::result_def (IR::IDLType_ptr result_def,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid = 
    this->repo_->ir_poa ()->reference_to_id (result_def,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var result_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "result",
                                            result_path.in ());
}

IR::ParDescriptionSeq *
TAO_OperationDef_i::params (CORBA::Environment &ACE_TRY_ENV)
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

  IR::ParDescriptionSeq *pd_seq = 0;
  ACE_NEW_THROW_EX (pd_seq,
                    IR::ParDescriptionSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  pd_seq->length (size);

  if (size == 0)
    return pd_seq;

  IR::ParDescriptionSeq_var retval = pd_seq;

  for (i = 0; i < size; ++i)
    {
      ACE_Configuration_Section_Key key;
      key_queue.dequeue_head (key);

      IR::ParameterDescription pd;

      ACE_TString name;
      this->repo_->config ()->get_string_value (key,
                                                "name",
                                                name);

      pd.name = name.c_str ();

      u_int mode = 0;
      this->repo_->config ()->get_integer_value (key,
                                                 "mode",
                                                 mode);

      pd.mode = ACE_static_cast (IR::ParameterMode, mode);

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
        this->repo_->servant_factory ()->create_idltype (type_key,
                                                         ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_IDLType_i> safety (impl);

      pd.type = impl->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

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
      ACE_CHECK_RETURN (0);

      pd.type_def = IR::IDLType::_narrow (obj.in (),
                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[i] = pd;
    }

  return retval._retn ();
}

void 
TAO_OperationDef_i::params (const IR::ParDescriptionSeq &params,
                            CORBA::Environment &ACE_TRY_ENV)
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

      const char *name = params[i].name;
      this->repo_->config ()->set_string_value (param_key,
                                                "name",
                                                name);
      PortableServer::ObjectId_var oid = 
        this->repo_->ir_poa ()->reference_to_id (params[i].type_def,
                                                 ACE_TRY_ENV);
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

IR::OperationMode 
TAO_OperationDef_i::mode (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int mode;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "mode",
                                             mode);

  return ACE_static_cast (IR::OperationMode, mode);
}

void 
TAO_OperationDef_i::mode (IR::OperationMode mode,
                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

IR::ContextIdSeq *
TAO_OperationDef_i::contexts (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key contexts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "contexts",
                                          0,
                                          contexts_key);

  int index = 0;
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

  IR::ContextIdSeq *ci_seq = 0;
  ACE_NEW_THROW_EX (ci_seq,
                    IR::ContextIdSeq (size),
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
TAO_OperationDef_i::contexts (const IR::ContextIdSeq &contexts,
                              CORBA::Environment &)
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
      const char *context = contexts[i];
      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->set_string_value (contexts_key,
                                                section_name.in (),
                                                context);
    }
}

IR::ExceptionDefSeq *
TAO_OperationDef_i::exceptions (CORBA::Environment &ACE_TRY_ENV )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "excepts",
                                          0,
                                          excepts_key);

  int index = 0;
  ACE_Unbounded_Queue<IR::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  // This section may not have been created.
  if (status == 0)
    {
      ACE_TString field_name;
      ACE_Configuration::VALUETYPE type;

      while (this->repo_->config ()->enumerate_values (excepts_key,
                                                       index,
                                                       field_name,
                                                       type)
              == 0)
        {
          ACE_TString type_path;
          this->repo_->config ()->get_string_value (excepts_key,
                                                    field_name.c_str (),
                                                    type_path);

          path_queue.enqueue_tail (type_path);

          ACE_Configuration_Section_Key except_key;
          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               type_path,
                                               except_key,
                                               0);

          u_int kind = 0;
          this->repo_->config ()->get_integer_value (except_key,
                                                     "def_kind",
                                                     kind);

          IR::DefinitionKind def_kind = 
            ACE_static_cast (IR::DefinitionKind, kind);

          kind_queue.enqueue_tail (def_kind);
          ++index;
        }
    }

  IR::ExceptionDefSeq *ed_seq = 0;
  ACE_NEW_THROW_EX (ed_seq,
                    IR::ExceptionDefSeq (index),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  ed_seq->length (index);

  if (index == 0)
    return ed_seq;

  IR::ExceptionDefSeq_var retval = ed_seq;

  for (CORBA::ULong i = 0; i < (CORBA::ULong) index; ++i)
    {
      IR::DefinitionKind def_kind;
      kind_queue.dequeue_head (def_kind);

      ACE_TString path;
      path_queue.dequeue_head (path);

      CORBA::Object_var obj =
        this->repo_->servant_factory ()->create_objref (def_kind,
                                                        path.c_str (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[i] = IR::ExceptionDef::_narrow (obj.in (),
                                             ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void 
TAO_OperationDef_i::exceptions (const IR::ExceptionDefSeq &exceptions,
                                CORBA::Environment &ACE_TRY_ENV)
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
        this->repo_->ir_poa ()->reference_to_id (exceptions[i],
                                                 ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::String_var type_path =
        PortableServer::ObjectId_to_string (oid.in ());

      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->set_string_value (excepts_key,
                                                section_name.in (),
                                                type_path.in ());
    }

}

IR::OperationDescription
TAO_OperationDef_i::make_description (
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IR::OperationDescription od;

  od.name = this->name (ACE_TRY_ENV);
  ACE_CHECK_RETURN (od);

  od.id = this->id (ACE_TRY_ENV);
  ACE_CHECK_RETURN (od);

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  od.defined_in = container_id.c_str ();

  od.version = this->version (ACE_TRY_ENV);
  ACE_CHECK_RETURN (od);

  od.result = this->result (ACE_TRY_ENV);
  ACE_CHECK_RETURN (od);

  od.mode = this->mode (ACE_TRY_ENV);
  ACE_CHECK_RETURN (od);

  IR::ContextIdSeq_var cid_seq = this->contexts (ACE_TRY_ENV);
  ACE_CHECK_RETURN (od);

  od.contexts = cid_seq.in ();

  IR::ParDescriptionSeq_var pd_seq = this->params (ACE_TRY_ENV);
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
      ACE_TString field_name;
      ACE_Configuration::VALUETYPE type;
      ACE_Unbounded_Queue<ACE_TString> path_queue;

      while (this->repo_->config ()->enumerate_values (excepts_key,
                                                       index,
                                                       field_name,
                                                       type)
              == 0)
        {
          ACE_TString type_path;
          this->repo_->config ()->get_string_value (excepts_key,
                                                    field_name.c_str (),
                                                    type_path);

          path_queue.enqueue_tail (type_path);
          ++index;
        }

      od.exceptions.length (index);

      for (CORBA::ULong i = 0; i < (CORBA::ULong) index; ++i)
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

          od.exceptions[i].type = impl.type (ACE_TRY_ENV);
          ACE_CHECK_RETURN (od);
        }
    }
  else
    {
      od.exceptions.length (0);
    }

  return od;
}

