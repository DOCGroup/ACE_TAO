/* -*- C++ -*- */
// $Id$

#include "OperationDef_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "ExceptionDef_i.h"
#include "IFR_Service_Utils.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           OperationDef_i, 
           "$Id$")

TAO_OperationDef_i::TAO_OperationDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo)
{
}

TAO_OperationDef_i::~TAO_OperationDef_i (void)
{
}

CORBA::DefinitionKind
TAO_OperationDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Operation;
}

CORBA::Contained::Description *
TAO_OperationDef_i::describe (ACE_ENV_SINGLE_ARG_DECL )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_OperationDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL )
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

  retval->value <<= this->make_description (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_OperationDef_i::result (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->result_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_OperationDef_i::result_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString result_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "result",
                                            result_path);


  TAO_IDLType_i *impl = 
    TAO_IFR_Service_Utils::path_to_idltype (result_path,
                                            this->repo_);

  return impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_OperationDef_i::result_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return this->result_def_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::IDLType_ptr
TAO_OperationDef_i::result_def_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString result_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "result",
                                            result_path);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (result_path,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::IDLType::_nil ());

  return CORBA::IDLType::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::result_def (CORBA::IDLType_ptr result_def
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->result_def_i (result_def
                      ACE_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::result_def_i (CORBA::IDLType_ptr result_def
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *result_path = 
    TAO_IFR_Service_Utils::reference_to_path (result_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "result",
                                            result_path);
}

CORBA::ParDescriptionSeq *
TAO_OperationDef_i::params (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->params_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ParDescriptionSeq *
TAO_OperationDef_i::params_i (ACE_ENV_SINGLE_ARG_DECL)
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
          ACE_Configuration_Section_Key param_key;
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          status =
            this->repo_->config ()->open_section (params_key,
                                                  stringified,
                                                  0,
                                                  param_key);

          if (status == 0)
            {
              key_queue.enqueue_tail (param_key);
            }
        }
    }

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, key_queue.size ());

  CORBA::ParDescriptionSeq *pd_seq = 0;
  ACE_NEW_THROW_EX (pd_seq,
                    CORBA::ParDescriptionSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  pd_seq->length (size);

  if (size == 0)
    {
      return pd_seq;
    }

  CORBA::ParDescriptionSeq_var retval = pd_seq;

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

      TAO_IDLType_i *impl = 
        TAO_IFR_Service_Utils::path_to_idltype (type_path,
                                                this->repo_);

      retval[i].type = impl->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Object_var obj = 
        TAO_IFR_Service_Utils::path_to_ir_object (type_path,
                                                  this->repo_
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i].type_def = CORBA::IDLType::_narrow (obj.in ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_OperationDef_i::params (const CORBA::ParDescriptionSeq &params
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->params_i (params
                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::params_i (const CORBA::ParDescriptionSeq &params
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "params",
                                          1);

  CORBA::ULong length = params.length ();

  if (length == 0)
    {
      return;
    }

  ACE_Configuration_Section_Key params_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "params",
                                        1,
                                        params_key);

  this->repo_->config ()->set_integer_value (params_key,
                                             "count",
                                             length);
  char *type_path = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
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
        TAO_IFR_Service_Utils::reference_to_path (params[i].type_def.in ());

      this->repo_->config ()->set_string_value (param_key,
                                                "type_path",
                                                type_path);

      this->repo_->config ()->set_integer_value (param_key,
                                                 "mode",
                                                 params[i].mode);
    }
}

CORBA::OperationMode
TAO_OperationDef_i::mode (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::OP_NORMAL);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OP_NORMAL);

  return this->mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::OperationMode
TAO_OperationDef_i::mode_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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
TAO_OperationDef_i::mode_i (CORBA::OperationMode mode
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

CORBA::ContextIdSeq *
TAO_OperationDef_i::contexts (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->contexts_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ContextIdSeq *
TAO_OperationDef_i::contexts_i (ACE_ENV_SINGLE_ARG_DECL)
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
      ACE_TString context;

      // Don't have to worry about gaps here - contexts are not
      // IR objects so they can't be destroyed one at a time. To
      // make a change one has to call the mutator version of this
      // function and make a completely new list.
      char *stringified = TAO_IFR_Service_Utils::int_to_string (index);
      while (this->repo_->config ()->get_string_value (contexts_key,
                                                       stringified,
                                                       context)
              == 0)
        {
          context_queue.enqueue_tail (context);
          ++index;
          stringified = 
            TAO_IFR_Service_Utils::int_to_string (index);
        }
    }

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, context_queue.size ());

  CORBA::ContextIdSeq *ci_seq = 0;
  ACE_NEW_THROW_EX (ci_seq,
                    CORBA::ContextIdSeq (size),
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
TAO_OperationDef_i::contexts (const CORBA::ContextIdSeq &contexts
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->contexts_i (contexts
                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::contexts_i (const CORBA::ContextIdSeq &contexts
                                ACE_ENV_ARG_DECL_NOT_USED)
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

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (contexts_key,
                                                stringified,
                                                contexts[i].in ());
    }
}

CORBA::ExceptionDefSeq *
TAO_OperationDef_i::exceptions (ACE_ENV_SINGLE_ARG_DECL )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExceptionDefSeq *
TAO_OperationDef_i::exceptions_i (ACE_ENV_SINGLE_ARG_DECL )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "excepts",
                                          0,
                                          excepts_key);

  int index = 0;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  // This section may not have been created.
  if (status == 0)
    {
      ACE_TString field_name, type_path;
      ACE_Configuration::VALUETYPE type;

      while (this->repo_->config ()->enumerate_values (excepts_key,
                                                       index++,
                                                       field_name,
                                                       type)
              == 0)
        {
          this->repo_->config ()->get_string_value (excepts_key,
                                                    field_name.c_str (),
                                                    type_path);

          path_queue.enqueue_tail (type_path);
        }
    }

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, path_queue.size ());

  CORBA::ExceptionDefSeq *ed_seq = 0;
  ACE_NEW_THROW_EX (ed_seq,
                    CORBA::ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  ed_seq->length (size);

  if (index == 0)
    {
      return ed_seq;
    }

  CORBA::ExceptionDefSeq_var retval = ed_seq;

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

void
TAO_OperationDef_i::exceptions (const CORBA::ExceptionDefSeq &exceptions
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->exceptions_i (exceptions
                      ACE_ENV_ARG_PARAMETER);
}

void
TAO_OperationDef_i::exceptions_i (const CORBA::ExceptionDefSeq &exceptions
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "excepts",
                                          0);

  CORBA::ULong length = exceptions.length ();

  if (length == 0)
    {
      return;
    }

  ACE_Configuration_Section_Key excepts_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "excepts",
                                        1,
                                        excepts_key);
  char *type_path = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      type_path = TAO_IFR_Service_Utils::reference_to_path (exceptions[i]);
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (excepts_key,
                                                stringified,
                                                type_path);
    }

}

CORBA::OperationDescription
TAO_OperationDef_i::make_description (
      ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::OperationDescription od;

  od.name = this->name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.id = this->id_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  od.defined_in = container_id.c_str ();

  od.version = this->version_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.result = this->result_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.mode = this->mode_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  CORBA::ContextIdSeq_var cid_seq = 
    this->contexts_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (od);

  od.contexts = cid_seq.in ();

  CORBA::ParDescriptionSeq_var pd_seq = 
    this->params_i (ACE_ENV_SINGLE_ARG_PARAMETER);
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

      CORBA::ULong size = ACE_static_cast (CORBA::ULong, path_queue.size ());

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

          TAO_ExceptionDef_i impl (this->repo_);
          impl.section_key (type_key);

          od.exceptions[i].type = impl.type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (od);
        }
    }
  else
    {
      od.exceptions.length (0);
    }

  return od;
}
