/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ValueDef_i.h"

ACE_RCSID (IFRService, 
           ValueDef_i, 
           "$Id$")

TAO_ValueDef_i::TAO_ValueDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_ValueDef_i::~TAO_ValueDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Value;
}

void
TAO_ValueDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ValueDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_ValueDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ValueDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_ValueDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ValueDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->supported_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong size = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "supports_count",
                                             size);
  CORBA::InterfaceDefSeq_var retval;
  retval->length (size);

  if (size == 0)
    {
      return retval._retn ();
    }

  CORBA::ULong index = 0;
  u_int kind = 0;
  CORBA::DefinitionKind def_kind = CORBA::dk_none;
  ACE_Configuration::VALUETYPE type;
  ACE_Configuration_Section_Key supports_key, supported_key;
  ACE_TString supported_name, supported_path;
  this->repo_->config ()->open_section (this->section_key_,
                                        "supports",
                                        0,
                                        supports_key);

  while (this->repo_->config ()->enumerate_values (supports_key,
                                                   index,
                                                   supported_name,
                                                   type)
           == 0)
    {
      this->repo_->config ()->get_string_value (supports_key,
                                                supported_name.c_str (),
                                                supported_path);
      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           supported_path.c_str (),
                                           supported_key,
                                           0);
      this->repo_->config ()->get_integer_value (supported_key,
                                                 "def_kind",
                                                 kind);
      def_kind = ACE_static_cast (CORBA::DefinitionKind,
                                  kind);
      CORBA::Object_var obj = this->create_objref (def_kind,
                                                   supported_path.c_str ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[index++] = CORBA::InterfaceDef::_narrow (obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return retval._retn ();
}

void
TAO_ValueDef_i::supported_interfaces (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supported_interfaces_i (supported_interfaces
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::supported_interfaces_i (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_PARAMETER
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Remove the old base interfaces.
  this->repo_->config ()->remove_section (this->section_key_,
                                          "supports",
                                          0);

  CORBA::ULong length = supported_interfaces.length ();
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "supports_count",
                                             length);

  ACE_Configuration_Section_Key supports_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "supports",
                                        1,
                                        supports_key);

  CORBA::String_var supported_name, supported_path;
  PortableServer::ObjectId_var oid;
  ACE_Configuration_Section_Key supported_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      supported_path = this->reference_to_path (supported_interfaces[i]);

      this->repo_->config ()->expand_path (this->repo_->root_key (),
                                           supported_path.in (),
                                           supported_key,
                                           1);

      TAO_ValueDef_i impl (this->repo_);
      impl.section_key (supported_key);

      supported_name = impl.name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // None of these names may clash with any we may already have.
      // The called function throws the BAD_PARAM exception if there is one.
      if (TAO_Container_i::name_exists (supported_name.in ()))
        {
          return;
        }

      this->repo_->config ()->set_string_value (supported_key,
                                                supported_name.in (),
                                                supported_path.in ());
    }
}

CORBA::InitializerSeq *
TAO_ValueDef_i::initializers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->initializers_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InitializerSeq *
TAO_ValueDef_i::initializers_i (ACE_ENV_SINGLE_ARG_PARAMETER)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong size = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "initializer_count",
                                             size);

  CORBA::InitializerSeq_var retval;
  retval->length (size);

  if (size == 0)
    {
      return retval._retn ();
    }

  u_int kind = 0;
  CORBA::ULong member_count = 0;
  CORBA::DefinitionKind def_kind = CORBA::dk_none;
  ACE_Configuration_Section_Key initializers_key, initializer_key, member_key;
  ACE_Configuration_Section_Key member_type_key;
  ACE_TString initializer_name, member_name, member_path;
  this->repo_->config ()->open_section (this->section_key_,
                                        "initializers",
                                        0,
                                        initializers_key);

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      this->repo_->config ()->enumerate_sections (initializers_key,
                                                  i,
                                                  initializer_name);
      retval[i].name = initializer_name.c_str ();

      ACE_Configuration_Section_Key initializer_key;
      this->repo_->config ()->open_section (initializers_key,
                                            initializer_name.c_str (),
                                            0,
                                            initializer_key);
      this->repo_->config ()->get_integer_value (initializer_key,
                                                 "member_count",
                                                 member_count);
      retval[i].members.length (member_count);

      for (CORBA::ULong j = 0; j < member_count; ++j)
        {
          CORBA::String_var section_name = this->int_to_string (j);
          this->repo_->config ()->open_section (initializer_key,
                                                section_name.in (),
                                                0,
                                                member_key);
          this->repo_->config ()->get_string_value (
                                      member_key,
                                      "name",
                                      member_name
                                    );
          retval[i].members[j].name = member_name.c_str ();
          this->repo_->config ()->get_string_value (member_key,
                                                    "path",
                                                    member_path);
          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               member_path,
                                               member_type_key,
                                               0);
          this->repo_->config ()->get_integer_value (member_type_key,
                                                     "def_kind",
                                                     kind);
          def_kind = ACE_static_cast (CORBA::DefinitionKind,
                                      kind);
          CORBA::Object_var member_obj = this->create_objref (
                                                   def_kind,
                                                   member_path.c_str ()
                                                   ACE_ENV_ARG_PARAMETER
                                                 );
          ACE_CHECK_RETURN (0);

          retval[i].members[j].type_def = 
            CORBA::IDLType::_narrow (member_obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }

  return retval._retn ();
}

void
TAO_ValueDef_i::initializers (const CORBA::InitializerSeq &initializers
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->initializers_i (initializers
                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::initializers_i (const CORBA::InitializerSeq & /* initializers */
                                ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ValueDef_ptr
TAO_ValueDef_i::base_value (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ValueDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValuoeDef::_nil ());

  return this->base_value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDef_ptr
TAO_ValueDef_i::base_value_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::base_value (CORBA::ValueDef_ptr base_value
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->base_value_i (base_value
                      ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::base_value_i (CORBA::ValueDef_ptr /* base_value */
                              ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->abstract_base_values_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::abstract_base_values (
    const CORBA::ValueDefSeq &abstract_base_values
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->abstract_base_values_i (abstract_base_values
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::abstract_base_values_i (
    const CORBA::ValueDefSeq & /* abstract_base_values */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_abstract (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_abstract_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_abstract_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::is_abstract (CORBA::Boolean is_abstract
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_abstract (is_abstract
                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_abstract_i (CORBA::Boolean /* is_abstract */
                             ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_custom (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_custom_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_custom_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::is_custom (CORBA::Boolean is_custom
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_custom_i (is_custom
                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_custom_i (CORBA::Boolean /* is_custom */
                             ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_truncatable (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_truncatable_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_truncatable_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::is_truncatable (CORBA::Boolean is_truncatable
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->is_truncatable_i (is_truncatable
                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_truncatable_i (CORBA::Boolean /* is_truncatable */
                                  ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_a (const char *id
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->is_a_i (id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_a_i (const char *id
                        ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (id, "IDL:omg.org/CORBA/ValueBase:1.0") == 0)
    {
      return 1;
    }

  // TODO
  return 0;
}

CORBA::ValueDef::FullValueDescription *
TAO_ValueDef_i::describe_value (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDef::FullValueDescription *
TAO_ValueDef_i::describe_value_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ValueMemberDef_ptr
TAO_ValueDef_i::create_value_member (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::IDLType_ptr type,
                                     CORBA::Visibility access
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ValueMemberDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueMember::_nil ());

  return this->create_value_member_i (id,
                                      name,
                                      version,
                                      type,
                                      access
                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueMemberDef_ptr
TAO_ValueDef_i::create_value_member_i (const char * /* id */,
                                       const char * /* name */,
                                       const char * /* version */,
                                       CORBA::IDLType_ptr /* type */,
                                       CORBA::Visibility /* access */
                                       ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::AttributeDef_ptr
TAO_ValueDef_i::create_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::AttributeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AttributeDef::_nil ());

  return this->create_attribute_i (id,
                                   name,
                                   version,
                                   type,
                                   mode
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::AttributeDef_ptr
TAO_ValueDef_i::create_attribute_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    CORBA::IDLType_ptr /* type */,
    CORBA::AttributeMode /* mode */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::OperationDef_ptr
TAO_ValueDef_i::create_operation (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr result,
    CORBA::OperationMode mode,
    const CORBA::ParDescriptionSeq &params,
    const CORBA::ExceptionDefSeq &exceptions,
    const CORBA::ContextIdSeq &contexts
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::OperationDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::OperationDef::_nil ());

  return this->create_operation_i (id,
                                   name,
                                   version,
                                   result,
                                   mode,
                                   params,
                                   exceptions,
                                   contexts
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::OperationDef_ptr
TAO_ValueDef_i::create_operation_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    CORBA::IDLType_ptr /* result */,
    CORBA::OperationMode /* mode */,
    const CORBA::ParDescriptionSeq & /* params */,
    const CORBA::ExceptionDefSeq & /* exceptions */,
    const CORBA::ContextIdSeq & /* contexts */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

