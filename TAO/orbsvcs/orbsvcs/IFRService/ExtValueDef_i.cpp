/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ExtValueDef_i.h"
#include "ExceptionDef_i.h"
#include "IFR_Service_Utils.h"
#include "IFR_Service_Utils_T.h"

ACE_RCSID (IFRService, 
           ExtValueDef_i, 
           "$Id$")

TAO_ExtValueDef_i::TAO_ExtValueDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_ValueDef_i (repo)
{
}

TAO_ExtValueDef_i::~TAO_ExtValueDef_i (void)
{
}

CORBA::ExtInitializerSeq *
TAO_ExtValueDef_i::ext_initializers (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->ext_initializers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExtInitializerSeq *
TAO_ExtValueDef_i::ext_initializers_i (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ExtInitializerSeq *iseq = 0;
  ACE_NEW_RETURN (iseq,
                  CORBA::ExtInitializerSeq,
                  0);
  CORBA::ExtInitializerSeq_var retval = iseq;
  
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
                                                    "name",
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

      this->fill_exceptions (retval[i].exceptions,
                             initializer_key,
                             "excepts"
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
    
  return retval._retn ();
}

void 
TAO_ExtValueDef_i::ext_initializers (
    const CORBA::ExtInitializerSeq &ext_initializers
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->ext_initializers_i (ext_initializers
                            ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ExtValueDef_i::ext_initializers_i (
    const CORBA::ExtInitializerSeq &ext_initializers
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration *config = this->repo_->config ();
  config->remove_section (this->section_key_,
                          "initializers",
                          1);

  CORBA::ULong length = ext_initializers.length ();

  if (length == 0)
    {
      return;
    }
    
  ACE_Configuration_Section_Key initializers_key;
  config->open_section (this->section_key_,
                        "initializers",
                        1,
                        initializers_key);
  config->set_integer_value (initializers_key,
                             "count",
                             length);

  CORBA::ULong arg_count = 0;
  char *arg_path = 0;
  ACE_Configuration_Section_Key initializer_key;
  ACE_Configuration_Section_Key params_key;
  ACE_Configuration_Section_Key arg_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->open_section (initializers_key,
                            stringified,
                            1,
                            initializer_key);
      config->set_string_value (initializer_key,
                                "name",
                                ext_initializers[i].name.in ());

      arg_count = ext_initializers[i].members.length ();

      if (arg_count > 0)
        {
          config->open_section (initializer_key,
                                "params",
                                1,
                                params_key);
          config->set_integer_value (params_key,
                                     "count",
                                     arg_count);

          for (CORBA::ULong j = 0; j < arg_count; ++j)
            {
              char *stringified = 
                TAO_IFR_Service_Utils::int_to_string (j);
              config->open_section (params_key,
                                    stringified,
                                    1,
                                    arg_key);
              config->set_string_value (
                  arg_key,
                  "arg_name",
                  ext_initializers[i].members[j].name.in ()
                );
              arg_path = 
                TAO_IFR_Service_Utils::reference_to_path (
                    ext_initializers[i].members[j].type_def.in ()
                  );
              config->set_string_value (arg_key,
                                        "arg_path",
                                        arg_path);
            }
        }

      this->exceptions (initializer_key,
                        "excepts",
                        ext_initializers[i].exceptions);
    }
}

CORBA::ExtValueDef::ExtFullValueDescription *
TAO_ExtValueDef_i::describe_ext_value (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_ext_value (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExtValueDef::ExtFullValueDescription *
TAO_ExtValueDef_i::describe_ext_value_i (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ExtValueDef::ExtFullValueDescription *fv_desc = 0;
  ACE_NEW_RETURN (fv_desc,
                  CORBA::ExtValueDef::ExtFullValueDescription,
                  0);
  CORBA::ExtValueDef::ExtFullValueDescription_var retval = fv_desc;
  
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
  fv_desc->is_abstract = ACE_static_cast (CORBA::Boolean, 
                                          val);
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "is_custom",
                                             val);
  fv_desc->is_custom = ACE_static_cast (CORBA::Boolean, 
                                        val);
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
  ACE_Configuration_Section_Key params_key, excepts_key, except_def_key;
  
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (ops_key,
                                                 "count",
                                                 count);
      fv_desc->operations.length (count);
      ACE_Configuration_Section_Key op_key, result_key, contexts_key;
      
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


          this->fill_exceptions (fv_desc->attributes[i].get_exceptions,
                                 attr_key,
                                 "get_excepts"
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);


          this->fill_exceptions (fv_desc->attributes[i].put_exceptions,
                                 attr_key,
                                 "put_excepts"
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
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
      ACE_Configuration_Section_Key initializer_key, 
                                    params_key, 
                                    arg_key,
                                    excepts_key;

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

              for (j = 0; j < param_count; ++j)
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
                    TAO_IFR_Service_Utils::path_to_ir_object (
                                               holder,
                                               this->repo_
                                               ACE_ENV_ARG_PARAMETER
                                             );
                  ACE_CHECK_RETURN (0);

                  fv_desc->initializers[i].members[j].type_def =
                    CORBA::IDLType::_narrow (obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK_RETURN (0);
                }
            }

          this->fill_exceptions (fv_desc->initializers[i].exceptions,
                                 initializer_key,
                                 "excepts"
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
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

CORBA::ExtAttributeDef_ptr 
TAO_ExtValueDef_i::create_ext_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode,
    const CORBA::ExceptionDefSeq &get_exceptions,
    const CORBA::ExceptionDefSeq &set_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ExtAttributeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  return this->create_ext_attribute_i (id,
                                       name,
                                       version,
                                       type,
                                       mode,
                                       get_exceptions,
                                       set_exceptions
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::ExtAttributeDef_ptr 
TAO_ExtValueDef_i::create_ext_attribute_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode,
    const CORBA::ExceptionDefSeq &get_exceptions,
    const CORBA::ExceptionDefSeq &set_exceptions
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
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  // Store the path to the attribute's type definition.
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path);

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             "mode",
                                             mode);

  this->exceptions (new_key,
                    "get_excepts",
                    get_exceptions);

  this->exceptions (new_key,
                    "put_excepts",
                    set_exceptions);

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Attribute,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  CORBA::ExtAttributeDef_var retval =
    CORBA::ExtAttributeDef::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  return retval._retn ();
}

void
TAO_ExtValueDef_i::fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                                    ACE_Configuration_Section_Key &key,
                                    const char *sub_section
                                    ACE_ENV_ARG_DECL)
{
  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (key,
                                          sub_section,
                                          0,
                                          excepts_key);

  if (status != 0)
    {
      exceptions.length (0);
      return;
    }

  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (excepts_key,
                                             "count",
                                             count);
  exceptions.length (count);
  char *stringified = 0;
  ACE_TString holder;
  ACE_Configuration_Section_Key except_key;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (excepts_key,
                                                stringified,
                                                holder);
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           except_key,
                                           0);
      this->repo_->config ()->get_string_value (except_key,
                                                "name",
                                                holder);
      exceptions[i].name = holder.fast_rep ();
      this->repo_->config ()->get_string_value (except_key,
                                                "id",
                                                holder);
      exceptions[i].id = holder.fast_rep ();

      TAO_ExceptionDef_i impl (this->repo_);
      impl.section_key (except_key);
      exceptions[i].type = impl.type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->repo_->config ()->get_string_value (except_key,
                                                "container_id",
                                                holder);
      exceptions[i].defined_in = holder.fast_rep ();
      this->repo_->config ()->get_string_value (except_key,
                                                "version",
                                                holder);
      exceptions[i].version = holder.fast_rep ();
    }
}

void 
TAO_ExtValueDef_i::exceptions (ACE_Configuration_Section_Key &key,
                               const char *sub_section,
                               const CORBA::ExcDescriptionSeq &exceptions)
{
  CORBA::ULong length = exceptions.length ();

  if (length == 0)
    {
      return;
    }

  ACE_Configuration_Section_Key new_key;
  this->repo_->config ()->open_section (key,
                                        sub_section,
                                        1,
                                        new_key);
  this->repo_->config ()->set_integer_value (new_key,
                                             "count",
                                             length);
  char *stringified = 0;
  ACE_TString path;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                exceptions[i].id.in (),
                                                path);
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (new_key,
                                                stringified,
                                                path);
    }
}

void 
TAO_ExtValueDef_i::exceptions (ACE_Configuration_Section_Key &key,
                               const char *sub_section,
                               const CORBA::ExceptionDefSeq &exceptions)
{
  CORBA::ULong length = exceptions.length ();

  if (length == 0)
    {
      return;
    }

  ACE_Configuration_Section_Key excepts_key;
  this->repo_->config ()->open_section (key,
                                        sub_section,
                                        1,
                                        excepts_key);

  this->repo_->config ()->set_integer_value (excepts_key,
                                             "count",
                                             length);
  char *stringified = 0;
  char *type_path = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      type_path = 
        TAO_IFR_Service_Utils::reference_to_path (exceptions[i].in ());

      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (excepts_key,
                                                stringified,
                                                type_path);
    }
}

