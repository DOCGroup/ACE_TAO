// $Id$

#include "orbsvcs/IFRService/OperationDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/ExceptionDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO_OperationDef_i::def_kind (void)
{
  return CORBA::dk_Operation;
}

CORBA::Contained::Description *
TAO_OperationDef_i::describe ( )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_i ();
}

CORBA::Contained::Description *
TAO_OperationDef_i::describe_i ( )
{
  CORBA::Contained::Description *desc_ptr = 0;
  ACE_NEW_THROW_EX (desc_ptr,
                    CORBA::Contained::Description,
                    CORBA::NO_MEMORY ());

  CORBA::Contained::Description_var retval = desc_ptr;

  retval->kind = this->def_kind ();

  CORBA::OperationDescription od;
  this->make_description (od);

  retval->value <<= od;

  return retval._retn ();
}

CORBA::TypeCode_ptr
TAO_OperationDef_i::result (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->result_i ();
}

CORBA::TypeCode_ptr
TAO_OperationDef_i::result_i (void)
{
  ACE_TString result_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "result",
                                            result_path);


  TAO_IDLType_i *impl =
    TAO_IFR_Service_Utils::path_to_idltype (result_path,
                                            this->repo_);

  return impl->type_i ();
}

CORBA::IDLType_ptr
TAO_OperationDef_i::result_def (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::IDLType::_nil ());

  this->update_key ();

  return this->result_def_i ();
}

CORBA::IDLType_ptr
TAO_OperationDef_i::result_def_i (void)
{
  ACE_TString result_path;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "result",
                                            result_path);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (result_path,
                                              this->repo_);

  return CORBA::IDLType::_narrow (obj.in ());
}

void
TAO_OperationDef_i::result_def (CORBA::IDLType_ptr result_def)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->result_def_i (result_def);
}

void
TAO_OperationDef_i::result_def_i (CORBA::IDLType_ptr result_def)
{
  char *result_path =
    TAO_IFR_Service_Utils::reference_to_path (result_def);

  this->repo_->config ()->set_string_value (this->section_key_,
                                            "result",
                                            result_path);
}

CORBA::ParDescriptionSeq *
TAO_OperationDef_i::params (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->params_i ();
}

CORBA::ParDescriptionSeq *
TAO_OperationDef_i::params_i (void)
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

  CORBA::ULong size = static_cast<CORBA::ULong> (key_queue.size ());

  CORBA::ParDescriptionSeq *pd_seq = 0;
  ACE_NEW_THROW_EX (pd_seq,
                    CORBA::ParDescriptionSeq (size),
                    CORBA::NO_MEMORY ());

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

      retval[i].mode = static_cast<CORBA::ParameterMode> (mode);

      ACE_TString type_path;
      this->repo_->config ()->get_string_value (key,
                                                "type_path",
                                                type_path);

      TAO_IDLType_i *impl =
        TAO_IFR_Service_Utils::path_to_idltype (type_path,
                                                this->repo_);

      // We have an undefined interface
      if (impl == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Encountered undefined parameter type "
                                "for variable: %s\n"),
                      name.c_str ()));

          throw CORBA::INTF_REPOS ();
        }

      retval[i].type = impl->type_i ();

      CORBA::Object_var obj =
        TAO_IFR_Service_Utils::path_to_ir_object (type_path,
                                                  this->repo_);

      retval[i].type_def = CORBA::IDLType::_narrow (obj.in ());
    }

  return retval._retn ();
}

void
TAO_OperationDef_i::params (const CORBA::ParDescriptionSeq &params)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->params_i (params);
}

void
TAO_OperationDef_i::params_i (const CORBA::ParDescriptionSeq &params)
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
TAO_OperationDef_i::mode (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::OP_NORMAL);

  this->update_key ();

  return this->mode_i ();
}

CORBA::OperationMode
TAO_OperationDef_i::mode_i (void)
{
  u_int mode;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "mode",
                                             mode);

  return static_cast<CORBA::OperationMode> (mode);
}

void
TAO_OperationDef_i::mode (CORBA::OperationMode mode)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->mode_i (mode);
}

void
TAO_OperationDef_i::mode_i (CORBA::OperationMode mode)
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "mode",
                                             mode);
}

CORBA::ContextIdSeq *
TAO_OperationDef_i::contexts (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->contexts_i ();
}

CORBA::ContextIdSeq *
TAO_OperationDef_i::contexts_i (void)
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

  CORBA::ULong size = static_cast<CORBA::ULong> (context_queue.size ());

  CORBA::ContextIdSeq *ci_seq = 0;
  ACE_NEW_THROW_EX (ci_seq,
                    CORBA::ContextIdSeq (size),
                    CORBA::NO_MEMORY ());

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
TAO_OperationDef_i::contexts (const CORBA::ContextIdSeq &contexts)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->contexts_i (contexts);
}

void
TAO_OperationDef_i::contexts_i (const CORBA::ContextIdSeq &contexts)
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
TAO_OperationDef_i::exceptions ( )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->exceptions_i ();
}

CORBA::ExceptionDefSeq *
TAO_OperationDef_i::exceptions_i ( )
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

  CORBA::ULong size = static_cast<CORBA::ULong> (path_queue.size ());

  CORBA::ExceptionDefSeq *ed_seq = 0;
  ACE_NEW_THROW_EX (ed_seq,
                    CORBA::ExceptionDefSeq (size),
                    CORBA::NO_MEMORY ());

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
                                              this->repo_);

      retval[i] = CORBA::ExceptionDef::_narrow (obj.in ());
    }

  return retval._retn ();
}

void
TAO_OperationDef_i::exceptions (const CORBA::ExceptionDefSeq &exceptions)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->exceptions_i (exceptions);
}

void
TAO_OperationDef_i::exceptions_i (const CORBA::ExceptionDefSeq &exceptions)
{
  this->repo_->config ()->remove_section (this->section_key_,
                                          "excepts",
                                          0);

  TAO_IFR_Service_Utils::set_exceptions (this->repo_->config (),
                                         this->section_key_,
                                         "excepts",
                                         exceptions);
}

void
TAO_OperationDef_i::make_description (
    CORBA::OperationDescription &od)
{
  od.name = this->name_i ();

  od.id = this->id_i ();

  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "container_id",
                                            container_id);

  od.defined_in = container_id.c_str ();

  od.version = this->version_i ();

  od.result = this->result_i ();

  od.mode = this->mode_i ();

  CORBA::ContextIdSeq_var cid_seq =
    this->contexts_i ();

  od.contexts = cid_seq.in ();

  CORBA::ParDescriptionSeq_var pd_seq =
    this->params_i ();

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

      CORBA::ULong size = static_cast<CORBA::ULong> (path_queue.size ());

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

          od.exceptions[i].type = impl.type_i ();
        }
    }
  else
    {
      od.exceptions.length (0);
    }
}

CORBA::TypeCode_ptr
TAO_OperationDef_i::type_i (void)
{
  return this->result_i ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
