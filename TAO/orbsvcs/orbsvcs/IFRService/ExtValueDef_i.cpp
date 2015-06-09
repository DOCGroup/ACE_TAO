#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/ExtValueDef_i.h"
#include "orbsvcs/IFRService/ExceptionDef_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->ext_initializers_i ();
}

CORBA::ExtInitializerSeq *
TAO_ExtValueDef_i::ext_initializers_i (
  )
{
  CORBA::ExtInitializerSeq *iseq = 0;
  ACE_NEW_RETURN (iseq,
                  CORBA::ExtInitializerSeq,
                  0);
  CORBA::ExtInitializerSeq_var retval = iseq;

  ACE_Configuration_Section_Key initializers_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          ACE_TEXT("initializers"),
                                          0,
                                          initializers_key);

  if (status != 0)
    {
      iseq->length (0);
      return retval._retn ();
    }

  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (initializers_key,
                                             ACE_TEXT("count"),
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
                                            ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                            0,
                                            initializer_key);
      this->repo_->config ()->get_string_value (initializer_key,
                                                ACE_TEXT("name"),
                                                holder);
      retval[i].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
      status =
        this->repo_->config ()->open_section (initializer_key,
                                              ACE_TEXT("params"),
                                              0,
                                              params_key);

      if (status != 0)
        {
          retval[i].members.length (0);
          continue;
        }

      this->repo_->config ()->get_integer_value (params_key,
                                                 ACE_TEXT("count"),
                                                 arg_count);
      retval[i].members.length (arg_count);
      CORBA::Object_var obj;

      for (CORBA::ULong j = 0; j < arg_count; ++j)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (j);
          this->repo_->config ()->open_section (params_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                0,
                                                arg_key);
          this->repo_->config ()->get_string_value (arg_key,
                                                    ACE_TEXT("arg_name"),
                                                    holder);
          retval[i].members[j].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (arg_key,
                                                    ACE_TEXT("arg_path"),
                                                    holder);
          TAO_IDLType_i *impl =
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          retval[i].members[j].type =
            impl->type_i ();

          obj =
            TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                      this->repo_);

          retval[i].members[j].type_def =
            CORBA::IDLType::_narrow (obj.in ());
        }

      this->fill_exceptions (retval[i].exceptions,
                             initializer_key,
                             "excepts");
    }

  return retval._retn ();
}

void
TAO_ExtValueDef_i::ext_initializers (
    const CORBA::ExtInitializerSeq &ext_initializers
  )
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->ext_initializers_i (ext_initializers);
}

void
TAO_ExtValueDef_i::ext_initializers_i (
    const CORBA::ExtInitializerSeq &ext_initializers
  )
{
  ACE_Configuration *config = this->repo_->config ();
  config->remove_section (this->section_key_,
                          ACE_TEXT("initializers"),
                          1);

  /// This does not handle the initializer exceptions, so we do that below.
  TAO_IFR_Generic_Utils<CORBA::ExtInitializerSeq>::set_initializers (
      ext_initializers,
      this->repo_->config (),
      this->section_key_
    );

  CORBA::ULong length = ext_initializers.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key initializers_key, initializer_key;
      char *stringified = 0;

      this->repo_->config ()->open_section (this->section_key_,
                                            ACE_TEXT("initializers"),
                                            0,
                                            initializers_key);

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (initializers_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                0,
                                                initializer_key);

          this->exceptions (initializer_key,
                            "excepts",
                            ext_initializers[i].exceptions);
        }
    }
}

CORBA::ExtValueDef::ExtFullValueDescription *
TAO_ExtValueDef_i::describe_ext_value (
  )
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->describe_ext_value_i ();
}

CORBA::ExtValueDef::ExtFullValueDescription *
TAO_ExtValueDef_i::describe_ext_value_i (
  )
{
  CORBA::ExtValueDef::ExtFullValueDescription *fv_desc = 0;
  ACE_NEW_RETURN (fv_desc,
                  CORBA::ExtValueDef::ExtFullValueDescription,
                  0);
  CORBA::ExtValueDef::ExtFullValueDescription_var retval = fv_desc;

  ACE_TString holder;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            ACE_TEXT("name"),
                                            holder);
  fv_desc->name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
  this->repo_->config ()->get_string_value  (this->section_key_,
                                             ACE_TEXT("id"),
                                             holder);
  fv_desc->id = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());

  CORBA::ULong val = 0;

  this->repo_->config ()->get_integer_value (this->section_key_,
                                             ACE_TEXT("is_abstract"),
                                             val);
  fv_desc->is_abstract = static_cast<CORBA::Boolean> (val);
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             ACE_TEXT("is_custom"),
                                             val);
  fv_desc->is_custom = static_cast<CORBA::Boolean> (val);
  this->repo_->config ()->get_string_value (this->section_key_,
                                            ACE_TEXT("container_id"),
                                            holder);
  fv_desc->defined_in = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
  this->repo_->config ()->get_string_value (this->section_key_,
                                            ACE_TEXT("version"),
                                            holder);
  fv_desc->version = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());

  // Operations.

  fv_desc->operations.length (0);

  ACE_Configuration_Section_Key ops_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          ACE_TEXT("ops"),
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
                                                 ACE_TEXT("count"),
                                                 count);
      fv_desc->operations.length (count);
      ACE_Configuration_Section_Key op_key, result_key, contexts_key;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (ops_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                0,
                                                op_key);
          this->repo_->config ()->get_string_value (op_key,
                                                    ACE_TEXT("name"),
                                                    holder);
          fv_desc->operations[i].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (op_key,
                                                    ACE_TEXT("id"),
                                                    holder);
          fv_desc->operations[i].id = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (this->section_key_,
                                                    ACE_TEXT("id"),
                                                    holder);
          fv_desc->operations[i].defined_in = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (op_key,
                                                    ACE_TEXT("result"),
                                                    holder);
          idl_type =
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          fv_desc->operations[i].result =
            idl_type->type_i ();

          this->repo_->config ()->get_integer_value (op_key,
                                                     ACE_TEXT("mode"),
                                                     val);
          fv_desc->operations[i].mode = static_cast<CORBA::OperationMode> (val);
          CORBA::TCKind kind =
            fv_desc->operations[i].result->kind ();

          if (fv_desc->operations[i].mode == CORBA::OP_ONEWAY
              && kind != CORBA::tk_void)
            {
              throw CORBA::BAD_PARAM (
                CORBA::OMGVMCID | 31,
                CORBA::COMPLETED_NO);
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
                                                  ACE_TEXT("params"),
                                                  0,
                                                  params_key);
          if (status == 0)
            {
              ACE_Configuration_Section_Key param_key;
              this->repo_->config ()->get_integer_value (params_key,
                                                        ACE_TEXT("count"),
                                                        param_count);
              fv_desc->operations[i].parameters.length (param_count);

              for (j = 0; j < param_count; ++j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->open_section (params_key,
                                                        ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                        0,
                                                        param_key);
                  this->repo_->config ()->get_string_value (param_key,
                                                            ACE_TEXT("name"),
                                                            holder);
                  fv_desc->operations[i].parameters[j].name =
                    ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
                  this->repo_->config ()->get_string_value (param_key,
                                                            ACE_TEXT("type_path"),
                                                            holder);
                  idl_type =
                    TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                            this->repo_);
                  fv_desc->operations[i].parameters[j].type =
                    idl_type->type_i ();

                  obj =
                    TAO_IFR_Service_Utils::path_to_ir_object (
                                               holder,
                                               this->repo_
                                             );

                  fv_desc->operations[i].parameters[j].type_def =
                    CORBA::IDLType::_narrow (obj.in ());

                  this->repo_->config ()->get_integer_value (param_key,
                                                             ACE_TEXT("mode"),
                                                             val);
                  fv_desc->operations[i].parameters[j].mode =
                    static_cast<CORBA::ParameterMode> (val);
                }
            }

          // Operation exceptions.

          status =
            this->repo_->config ()->open_section (op_key,
                                                  ACE_TEXT("excepts"),
                                                  0,
                                                  excepts_key);
          fv_desc->operations[i].exceptions.length (0);

          if (status == 0)
            {
              CORBA::ULong excep_count = 0;
              this->repo_->config ()->get_integer_value (excepts_key,
                                                         ACE_TEXT("count"),
                                                         excep_count);
              fv_desc->operations[i].exceptions.length (excep_count);
              ACE_Configuration_Section_Key except_def_key;

              for (j = 0; j < excep_count; ++j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->get_string_value (excepts_key,
                                                            ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                            holder);

                  this->repo_->config ()->expand_path (
                                              this->repo_->root_key (),
                                              holder,
                                              except_def_key,
                                              0
                                            );
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            ACE_TEXT("name"),
                                                            holder);
                  fv_desc->operations[i].exceptions[j].name =
                    ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            ACE_TEXT("id"),
                                                            holder);
                  fv_desc->operations[i].exceptions[j].id =
                    ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            ACE_TEXT("container_id"),
                                                            holder);
                  fv_desc->operations[i].exceptions[j].defined_in =
                    ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
                  this->repo_->config ()->get_string_value (except_def_key,
                                                            ACE_TEXT("version"),
                                                            holder);
                  fv_desc->operations[i].exceptions[j].version =
                    ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());

                  TAO_ExceptionDef_i impl (this->repo_);
                  impl.section_key (except_def_key);
                  fv_desc->operations[i].exceptions[j].type =
                    impl.type_i ();
                }
            }
        }
    }

  // Attributes..

  fv_desc->attributes.length (0);

  ACE_Configuration_Section_Key attrs_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          ACE_TEXT("attrs"),
                                          0,
                                          attrs_key);
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (attrs_key,
                                                 ACE_TEXT("count"),
                                                 count);
      fv_desc->attributes.length (count);
      ACE_Configuration_Section_Key attr_key, attr_def_key;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (attrs_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                0,
                                                attr_key);
          this->repo_->config ()->get_string_value (attr_key,
                                                    ACE_TEXT("name"),
                                                    holder);
          fv_desc->attributes[i].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (attr_key,
                                                    ACE_TEXT("id"),
                                                    holder);
          fv_desc->attributes[i].id = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (attr_key,
                                                    ACE_TEXT("version"),
                                                    holder);
          fv_desc->attributes[i].version = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_integer_value (attr_key,
                                                     ACE_TEXT("mode"),
                                                     val);
          fv_desc->attributes[i].mode =
            static_cast<CORBA::AttributeMode> (val);
          this->repo_->config ()->get_string_value (attr_key,
                                                    ACE_TEXT("type_path"),
                                                    holder);

          // Use the type path for 'defined_in' and 'type'.

          idl_type =
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          fv_desc->attributes[i].type =
            idl_type->type_i ();

          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               holder,
                                               attr_def_key,
                                               0);
          this->repo_->config ()->get_string_value (attr_def_key,
                                                    ACE_TEXT("container_id"),
                                                    holder);
          fv_desc->attributes[i].defined_in = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());


          this->fill_exceptions (fv_desc->attributes[i].get_exceptions,
                                 attr_key,
                                 "get_excepts");


          this->fill_exceptions (fv_desc->attributes[i].put_exceptions,
                                 attr_key,
                                 "put_excepts");
        }
    }

  // Members..

  fv_desc->members.length (0);

  ACE_Configuration_Section_Key members_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          ACE_TEXT("members"),
                                          0,
                                          members_key);
  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (members_key,
                                                 ACE_TEXT("count"),
                                                 count);
      fv_desc->members.length (count);
      ACE_Configuration_Section_Key member_key, member_def_key;

      for (i = 0; i < count; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (members_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                0,
                                                member_key);
          this->repo_->config ()->get_string_value (member_key,
                                                    ACE_TEXT("name"),
                                                    holder);
          fv_desc->members[i].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (member_key,
                                                    ACE_TEXT("id"),
                                                    holder);
          fv_desc->members[i].id = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (member_key,
                                                    ACE_TEXT("container_id"),
                                                    holder);
          fv_desc->members[i].defined_in = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
          this->repo_->config ()->get_string_value (member_key,
                                                    ACE_TEXT("version"),
                                                    holder);
          fv_desc->members[i].version = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());

          this->repo_->config ()->get_integer_value (member_key,
                                                     ACE_TEXT("access"),
                                                     val);
          fv_desc->members[i].access =
            static_cast<CORBA::Visibility> (val);

          // Use type path for 'type' and 'type_def',

          this->repo_->config ()->get_string_value (member_key,
                                                    ACE_TEXT("type_path"),
                                                    holder);
          idl_type =
            TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                    this->repo_);
          fv_desc->members[i].type =
            idl_type->type_i ();

          obj =
            TAO_IFR_Service_Utils::path_to_ir_object (holder,
                                                      this->repo_);

          fv_desc->members[i].type_def =
            CORBA::IDLType::_narrow (obj.in ());
        }
    }

  // Initializers

  fv_desc->initializers.length (0);

  ACE_Configuration_Section_Key initializers_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          ACE_TEXT("initializers"),
                                          0,
                                          initializers_key);

  if (status == 0)
    {
      this->repo_->config ()->get_integer_value (initializers_key,
                                                 ACE_TEXT("count"),
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
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                0,
                                                initializer_key);
          this->repo_->config ()->get_string_value (initializer_key,
                                                    ACE_TEXT("name"),
                                                    holder);
          fv_desc->initializers[i].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());

          fv_desc->initializers[i].members.length (0);

          status =
            this->repo_->config ()->open_section (initializer_key,
                                                  ACE_TEXT("params"),
                                                  0,
                                                  params_key);

          if (status == 0)
            {
              this->repo_->config ()->get_integer_value (params_key,
                                                         ACE_TEXT("count"),
                                                         param_count);
              fv_desc->initializers[i].members.length (param_count);

              for (j = 0; j < param_count; ++j)
                {
                  stringified = TAO_IFR_Service_Utils::int_to_string (j);
                  this->repo_->config ()->open_section (params_key,
                                                        ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                        0,
                                                        arg_key);
                  this->repo_->config ()->get_string_value (arg_key,
                                                            ACE_TEXT("arg_name"),
                                                            holder);
                  fv_desc->initializers[i].members[j].name =
                    ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
                  this->repo_->config ()->get_string_value (arg_key,
                                                            ACE_TEXT("arg_path"),
                                                            holder);
                  TAO_IDLType_i *impl =
                    TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                             this->repo_);
                  fv_desc->initializers[i].members[j].type =
                    impl->type_i ();

                  obj =
                    TAO_IFR_Service_Utils::path_to_ir_object (
                                               holder,
                                               this->repo_
                                             );

                  fv_desc->initializers[i].members[j].type_def =
                    CORBA::IDLType::_narrow (obj.in ());
                }
            }

          this->fill_exceptions (fv_desc->initializers[i].exceptions,
                                 initializer_key,
                                 "excepts");
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
                                             ACE_TEXT("is_truncatable"),
                                             val);
  fv_desc->is_truncatable = static_cast<CORBA::Boolean> (val);
  status =
    this->repo_->config ()->get_string_value (this->section_key_,
                                              ACE_TEXT("base_value"),
                                              holder);

  if (status == 0)
    {
      ACE_Configuration_Section_Key base_key;
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           base_key,
                                           0);
      this->repo_->config ()->get_string_value (base_key,
                                                ACE_TEXT("id"),
                                                holder);
    }

  // If status isn't 0, then holder will be empty anyway.
  fv_desc->base_value = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
  fv_desc->type = this->type_i ();

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
  )
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ExtAttributeDef::_nil ());

  this->update_key ();

  return this->create_ext_attribute_i (id,
                                       name,
                                       version,
                                       type,
                                       mode,
                                       get_exceptions,
                                       set_exceptions);
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
  )
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
                                          "attrs");

  // Store the path to the attribute's type definition.
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            ACE_TEXT("type_path"),
                                            ACE_TEXT_CHAR_TO_TCHAR(type_path));

  // Store the attribute's mode.
  this->repo_->config ()->set_integer_value (new_key,
                                             ACE_TEXT("mode"),
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
                                          ACE_TEXT_ALWAYS_CHAR(path.c_str ()),
                                          this->repo_);

  CORBA::ExtAttributeDef_var retval =
    CORBA::ExtAttributeDef::_narrow (obj.in ());

  return retval._retn ();
}

void
TAO_ExtValueDef_i::fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                                    ACE_Configuration_Section_Key &key,
                                    const char *sub_section)
{
  ACE_Configuration_Section_Key excepts_key;
  int status =
    this->repo_->config ()->open_section (key,
                                          ACE_TEXT_CHAR_TO_TCHAR(sub_section),
                                          0,
                                          excepts_key);

  if (status != 0)
    {
      exceptions.length (0);
      return;
    }

  CORBA::ULong count = 0;
  this->repo_->config ()->get_integer_value (excepts_key,
                                             ACE_TEXT("count"),
                                             count);
  exceptions.length (count);
  char *stringified = 0;
  ACE_TString holder;
  ACE_Configuration_Section_Key except_key;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->get_string_value (excepts_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                holder);
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           holder,
                                           except_key,
                                           0);
      this->repo_->config ()->get_string_value (except_key,
                                                ACE_TEXT("name"),
                                                holder);
      exceptions[i].name = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
      this->repo_->config ()->get_string_value (except_key,
                                                ACE_TEXT("id"),
                                                holder);
      exceptions[i].id = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());

      TAO_ExceptionDef_i impl (this->repo_);
      impl.section_key (except_key);
      exceptions[i].type = impl.type_i ();

      this->repo_->config ()->get_string_value (except_key,
                                                ACE_TEXT("container_id"),
                                                holder);
      exceptions[i].defined_in = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
      this->repo_->config ()->get_string_value (except_key,
                                                ACE_TEXT("version"),
                                                holder);
      exceptions[i].version = ACE_TEXT_ALWAYS_CHAR(holder.fast_rep ());
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
                                        ACE_TEXT_CHAR_TO_TCHAR(sub_section),
                                        1,
                                        new_key);
  this->repo_->config ()->set_integer_value (new_key,
                                             ACE_TEXT("count"),
                                             length);
  char *stringified = 0;
  ACE_TString path;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                ACE_TEXT_CHAR_TO_TCHAR(exceptions[i].id.in ()),
                                                path);
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (new_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
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
                                        ACE_TEXT_CHAR_TO_TCHAR(sub_section),
                                        1,
                                        excepts_key);

  this->repo_->config ()->set_integer_value (excepts_key,
                                             ACE_TEXT("count"),
                                             length);
  char *stringified = 0;
  char *type_path = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      type_path =
        TAO_IFR_Service_Utils::reference_to_path (exceptions[i]);

      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->set_string_value (excepts_key,
                                                ACE_TEXT_CHAR_TO_TCHAR(stringified),
                                                ACE_TEXT_CHAR_TO_TCHAR(type_path));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
