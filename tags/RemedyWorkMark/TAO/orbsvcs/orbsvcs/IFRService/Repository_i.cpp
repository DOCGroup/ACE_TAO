// $Id$

#include "orbsvcs/IFRService/concrete_classes.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/Options.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "tao/ORB.h"
#include "tao/Object_KeyC.h"

#include "ace/Auto_Ptr.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Repository_i::TAO_Repository_i (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr poa,
                                    ACE_Configuration *config)
  : TAO_IRObject_i (0),
    TAO_Container_i (0),
    orb_ (orb),
    root_poa_ (poa),
    config_ (config),
    extension_ (CORBA::string_dup ("TAO_IFR_name_extension")),
    lock_ (0)
{
}

TAO_Repository_i::~TAO_Repository_i (void)
{
  delete this->lock_;
}

CORBA::DefinitionKind
TAO_Repository_i::def_kind (void)
{
  return CORBA::dk_Repository;
}

void
TAO_Repository_i::destroy (void)
{
  throw CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
}

CORBA::Contained_ptr
TAO_Repository_i::lookup_id (const char *search_id)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::Contained::_nil ());

  return this->lookup_id_i (search_id);
}

CORBA::Contained_ptr
TAO_Repository_i::lookup_id_i (const char *search_id)
{
  if (ACE_OS::strcmp (search_id, "IDL:omg.org/CORBA/Object:1.0") == 0
      || ACE_OS::strcmp (search_id, "IDL:omg.org/CORBA/ValueBase:1.0") == 0)
    {
      return CORBA::Contained::_nil ();
    }

  ACE_TString path;
  if (this->config_->get_string_value (this->repo_ids_key_,
                                       search_id,
                                       path)
       != 0)
    {
      return CORBA::Contained::_nil ();
    }

  ACE_Configuration_Section_Key key;
  this->config_->expand_path (this->root_key_,
                              path,
                              key);

  u_int kind = 0;
  this->config_->get_integer_value (key,
                                    "def_kind",
                                    kind);

  CORBA::DefinitionKind def_kind =
    static_cast<CORBA::DefinitionKind> (kind);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (def_kind,
                                          path.c_str (),
                                          this->repo_);

  return CORBA::Contained::_narrow (obj.in ());
}

CORBA::TypeCode_ptr
TAO_Repository_i::get_canonical_typecode (CORBA::TypeCode_ptr tc)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->get_canonical_typecode_i (tc);
}

CORBA::TypeCode_ptr
TAO_Repository_i::get_canonical_typecode_i (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind kind = tc->kind ();

  switch (kind)
  {
    // For all the TCKinds not covered below, no change is needed.
    default:
      return CORBA::TypeCode::_duplicate (tc);
    case CORBA::tk_fixed:
      throw CORBA::NO_IMPLEMENT ();
    case CORBA::tk_array:
    {
      CORBA::ULong length = tc->length ();

      CORBA::TypeCode_var ctype = tc->content_type ();

      CORBA::TypeCode_var canon_ctype =
        this->get_canonical_typecode_i (ctype.in ());

      return this->tc_factory ()->create_array_tc (length,
                                                   canon_ctype.in ());
    }
    case CORBA::tk_sequence:
    {
      CORBA::ULong length = tc->length ();

      CORBA::TypeCode_var ctype = tc->content_type ();

      CORBA::TypeCode_var canon_ctype =
        this->get_canonical_typecode_i (ctype.in ());

      return this->tc_factory ()->create_sequence_tc (length,
                                                      canon_ctype.in ());
    }
    case CORBA::tk_alias:
    case CORBA::tk_objref:
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_enum:
    case CORBA::tk_except:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
    case CORBA::tk_native:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_component:
    case CORBA::tk_home:
    {
      CORBA::String_var id = tc->id ();

      ACE_TString path;
      int status =
        this->config ()->get_string_value (this->repo_ids_key (),
                                           id.in (),
                                           path);

      // TODO - something in case the repo id is an empty string,
      //        or if it is not found in this repository
      if (status != 0)
        {
          return CORBA::TypeCode::_nil ();
        }

      ACE_Configuration_Section_Key key;
      this->config ()->expand_path (this->root_key (),
                                    path,
                                    key,
                                    0);

      // An ExceptionDef is not an IDLType.
      if (kind == CORBA::tk_except)
        {
          TAO_ExceptionDef_i impl (this->repo_);
          impl.section_key (key);
          return impl.type_i ();
        }
      else
        {
          TAO_IDLType_i *impl =
            TAO_IFR_Service_Utils::path_to_idltype (path,
                                                    this);
          impl->section_key (key);
          return impl->type_i ();
        }
    }
  }
}

CORBA::PrimitiveDef_ptr
TAO_Repository_i::get_primitive (CORBA::PrimitiveKind kind)
{
  ACE_TString obj_id ("pkinds\\");

  obj_id += this->pkind_to_string (kind);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Primitive,
                                          obj_id.c_str (),
                                          this->repo_);

  return CORBA::PrimitiveDef::_narrow (obj.in ());
}

CORBA::StringDef_ptr
TAO_Repository_i::create_string (CORBA::ULong bound)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::StringDef::_nil ());

  return this->create_string_i (bound);
}

CORBA::StringDef_ptr
TAO_Repository_i::create_string_i (CORBA::ULong bound)
{
  u_int count = 0;
  this->config_->get_integer_value (this->strings_key_,
                                    "count",
                                    count);

  char *name = TAO_IFR_Service_Utils::int_to_string (count++);
  this->config_->set_integer_value (this->strings_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->strings_key_,
                               name,
                               1,
                               new_key);

  this->config_->set_integer_value (new_key,
                                    "bound",
                                    bound);

  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    CORBA::dk_String);

  this->config_->set_string_value (new_key,
                                   "name",
                                   name);

  // Create the object reference.
  ACE_TString obj_id ("strings\\");
  obj_id += name;

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_String,
                                          obj_id.c_str (),
                                          this->repo_);

  return CORBA::StringDef::_narrow (obj.in ());
}

CORBA::WstringDef_ptr
TAO_Repository_i::create_wstring (CORBA::ULong bound)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::WstringDef::_nil ());

  return this->create_wstring_i (bound);
}

CORBA::WstringDef_ptr
TAO_Repository_i::create_wstring_i (CORBA::ULong bound)
{
  u_int count = 0;
  this->config_->get_integer_value (this->wstrings_key_,
                                    "count",
                                    count);

  char *name = TAO_IFR_Service_Utils::int_to_string (count++);
  this->config_->set_integer_value (this->wstrings_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->wstrings_key_,
                               name,
                               1,
                               new_key);

  this->config_->set_integer_value (new_key,
                                    "bound",
                                    bound);

  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    CORBA::dk_Wstring);

  this->config_->set_string_value (new_key,
                                   "name",
                                   name);

  // Create the object reference.
  ACE_TString obj_id ("wstrings\\");
  obj_id += name;

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Wstring,
                                               obj_id.c_str (),
                                               this->repo_);

  return CORBA::WstringDef::_narrow (obj.in ());
}

CORBA::SequenceDef_ptr
TAO_Repository_i::create_sequence (CORBA::ULong bound,
                                   CORBA::IDLType_ptr element_type)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::SequenceDef::_nil ());

  return this->create_sequence_i (bound,
                                  element_type);
}

CORBA::SequenceDef_ptr
TAO_Repository_i::create_sequence_i (CORBA::ULong bound,
                                     CORBA::IDLType_ptr element_type)
{
  u_int count = 0;
  this->config_->get_integer_value (this->sequences_key_,
                                    "count",
                                    count);

  char *name = TAO_IFR_Service_Utils::int_to_string (count++);
  this->config_->set_integer_value (this->sequences_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->sequences_key_,
                               name,
                               1,
                               new_key);

  // Set the bound attribute.
  this->config_->set_integer_value (new_key,
                                    "bound",
                                    bound);

  // Set the def_kind attribute.
  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    CORBA::dk_Sequence);

  // Set the "name" for destroy to use.
  this->config_->set_string_value (new_key,
                                   "name",
                                   name);

  char *element_path =
    TAO_IFR_Service_Utils::reference_to_path (element_type);

  // To get key to element type.
  this->config_->set_string_value (new_key,
                                   "element_path",
                                   element_path);

  // Create the object reference.
  ACE_TString obj_id ("sequences\\");
  obj_id += name;

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Sequence,
                                          obj_id.c_str (),
                                          this->repo_);

  return CORBA::SequenceDef::_narrow (obj.in ());
}

CORBA::ArrayDef_ptr
TAO_Repository_i::create_array (CORBA::ULong length,
                                CORBA::IDLType_ptr element_type)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ArrayDef::_nil ());

  return this->create_array_i (length,
                               element_type);
}

CORBA::ArrayDef_ptr
TAO_Repository_i::create_array_i (CORBA::ULong length,
                                  CORBA::IDLType_ptr element_type)
{
  u_int count = 0;
  this->config_->get_integer_value (this->arrays_key_,
                                    "count",
                                    count);

  char *name = TAO_IFR_Service_Utils::int_to_string (count++);
  this->config_->set_integer_value (this->arrays_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->arrays_key_,
                               name,
                               1,
                               new_key);

  // Set the length attribute.
  this->config_->set_integer_value (new_key,
                                    "length",
                                    length);

  // Set the def_kind attribute.
  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    CORBA::dk_Array);

  // Set the "name" for destroy to use.
  this->config_->set_string_value (new_key,
                                   "name",
                                   name);

  char *element_path =
    TAO_IFR_Service_Utils::reference_to_path (element_type);

  // To get key to element type.
  this->config_->set_string_value (new_key,
                                   "element_path",
                                   element_path);

  // Create the object reference.
  ACE_TString obj_id ("arrays\\");
  obj_id += name;

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Array,
                                          obj_id.c_str (),
                                          this->repo_);

  return CORBA::ArrayDef::_narrow (obj.in ());
}

CORBA::FixedDef_ptr
TAO_Repository_i::create_fixed (CORBA::UShort digits,
                                CORBA::Short scale)
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::FixedDef::_nil ());

  return this->create_fixed_i (digits,
                               scale);
}

CORBA::FixedDef_ptr
TAO_Repository_i::create_fixed_i (CORBA::UShort /* digits */,
                                  CORBA::Short /* scale */
                                  )
{
  throw CORBA::NO_IMPLEMENT ();
}

int
TAO_Repository_i::repo_init (CORBA::Repository_ptr repo_ref,
                             PortableServer::POA_ptr repo_poa)
{
  this->repo_objref_ = repo_ref;
  this->repo_poa_ = repo_poa;

  // Create the appropriate lock.
  if (OPTIONS::instance ()->enable_locking ())
    {
      ACE_NEW_THROW_EX (this->lock_,
                        ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      ACE_NEW_THROW_EX (this->lock_,
                        ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                        CORBA::NO_MEMORY ());
    }

  // Resolve and narrow the TypeCodeFactory.
  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("TypeCodeFactory");

  if (!CORBA::is_nil (object.in ()))
    {
      this->tc_factory_ =
        CORBA::TypeCodeFactory::_narrow (object.in ());

      if (CORBA::is_nil (this->tc_factory_.in ()))
        ACE_ERROR_RETURN ((
            LM_ERROR,
            ACE_TEXT ("Repository: TypeCodeFactory narrow failed\n")
          ),
          -1
        );
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("Repository: TypeCodeFactory resolve failed\n")
        ),
        -1
      );
    }

  // Resolve and narrow the POACurrent object.
  object =
    this->orb_->resolve_initial_references ("POACurrent");

  if (!CORBA::is_nil (object.in ()))
    {
      this->poa_current_ =
        PortableServer::Current::_narrow (object.in ());

      if (CORBA::is_nil (this->poa_current_.in ()))
        ACE_ERROR_RETURN ((
            LM_ERROR,
            ACE_TEXT ("Repository: POACurrent narrow failed\n")
          ),
          -1
        );
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("Repository: POACurrent resolve failed\n")
        ),
        -1
      );
    }

  if (this->create_sections () != 0)
    {
      return -1;
    }

  int status = this->create_servants_and_poas ();

  if (status != 0)
    {
      return -1;
    }

  return 0;
}

int
TAO_Repository_i::create_sections (void)
{
  this->config_->open_section (config_->root_section (),
                               "root",
                               1, // Will create if IFR is not persistent.
                               this->root_key_);

  this->config_->open_section (this->root_key_,
                               "repo_ids",
                               1, // Will create if IFR is not persistent.
                               this->repo_ids_key_);

  int status =
    this->config_->open_section (this->root_key_,
                                 "pkinds",
                                 0, // Don't create
                                 this->pkinds_key_);

  if (status != 0)
    // IFR is non-persistent or is being run for the first time.
    // Must add entries for primitive kinds.
    {
      this->config_->open_section (this->root_key_,
                                   "pkinds",
                                   1, // It doesn't exist so create it.
                                   this->pkinds_key_);

      u_int const num_pkinds = this->num_pkinds ();

      for (u_int i = 0; i < num_pkinds; ++i)
        {
          ACE_Configuration_Section_Key key;
          CORBA::PrimitiveKind pkind = static_cast<CORBA::PrimitiveKind> (i);

          this->config_->open_section (this->pkinds_key_,
                                       this->pkind_to_string (pkind),
                                       1,
                                       key);

          this->config_->set_integer_value (key,
                                            "def_kind",
                                            CORBA::dk_Primitive);

          this->config_->set_integer_value (key,
                                            "pkind",
                                            i);

        }
    }

  this->config_->open_section (this->root_key_,
                               "strings",
                               1,
                               this->strings_key_);

  this->config_->open_section (this->root_key_,
                               "wstrings",
                               1,
                               this->wstrings_key_);

  this->config_->open_section (this->root_key_,
                               "fixeds",
                               1,
                               this->fixeds_key_);

  this->config_->open_section (this->root_key_,
                               "arrays",
                               1,
                               this->arrays_key_);

  this->config_->open_section (this->root_key_,
                               "sequences",
                               1,
                               this->sequences_key_);

  u_int count = 0;

  // If repository is not persistent, we need to set all
  // the "count" fields to 0. We test just one for existence.
  status =
    this->config_->get_integer_value (this->strings_key_,
                                      "count",
                                      count);

  if (status == -1) // Field "count" does not exist.
    {
      this->config_->set_integer_value (this->strings_key_,
                                        "count",
                                        0);

      this->config_->set_integer_value (this->wstrings_key_,
                                        "count",
                                        0);

      this->config_->set_integer_value (this->fixeds_key_,
                                        "count",
                                        0);

      this->config_->set_integer_value (this->arrays_key_,
                                        "count",
                                        0);

      this->config_->set_integer_value (this->sequences_key_,
                                        "count",
                                        0);
    }

  // The following are done because we are also a Container.
  this->repo_ = this;
  this->section_key_ = this->root_key_;

  this->config_->set_string_value (this->section_key_,
                                   "absolute_name",
                                   "");

  this->config_->set_string_value (this->section_key_,
                                   "id",
                                   ""); // required by spec

  this->config_->set_string_value (this->section_key_,
                                   "name",
                                   "");

  this->config_->set_integer_value (this->section_key_,
                                    "def_kind",
                                    CORBA::dk_Repository);

  return 0;
}

int
TAO_Repository_i::create_servants_and_poas (void)
{
  CORBA::PolicyList policies (5);
  policies.length (5);

  // ID Assignment Policy.
  policies[0] =
    this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

  // Lifespan Policy.
  policies[1] =
    this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

  // Request Processing Policy.
  policies[2] =
    this->root_poa_->create_request_processing_policy (
        PortableServer::USE_DEFAULT_SERVANT
      );

  // Servant Retention Policy.
  policies[3] =
    this->root_poa_->create_servant_retention_policy (
        PortableServer::NON_RETAIN
      );

  // Id Uniqueness Policy.
  policies[4] =
    this->root_poa_->create_id_uniqueness_policy (
        PortableServer::MULTIPLE_ID
      );

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager ();

#define GEN_IR_OBJECT(name) \
  this-> name ## _poa_ = \
    this->root_poa_->create_POA (#name "_poa", \
                                 poa_manager.in (), \
                                 policies); \
\
  TAO_ ## name ## _i * name ## _impl = 0; \
  ACE_NEW_RETURN (name ## _impl, \
                  TAO_ ## name ## _i (this), \
                  -1); \
  ACE_NEW_RETURN (this-> name ## _servant_, \
                  POA_CORBA:: name ## _tie<TAO_ ## name ## _i> ( \
                      name ## _impl, \
                      this-> name ## _poa_.in (), \
                      1 \
                    ), \
                  -1); \
  PortableServer::ServantBase_var name ## _safety ( \
      this-> name ## _servant_ \
    ); \
  this-> name ## _poa_->set_servant (this-> name ## _servant_);

  CONCRETE_IR_OBJECT_TYPES

#undef GEN_IR_OBJECT
#undef CONCRETE_IR_OBJECT_TYPES

  CORBA::ULong length = policies.length ();

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy ();
    }

  return 0;
}

TAO_IDLType_i *
TAO_Repository_i::select_idltype (CORBA::DefinitionKind def_kind) const
{
  switch (def_kind)
  {
    case CORBA::dk_AbstractInterface:
      return this->ExtAbstractInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_Alias:
      return this->AliasDef_servant_->_tied_object ();
    case CORBA::dk_Array:
      return this->ArrayDef_servant_->_tied_object ();
    case CORBA::dk_Enum:
      return this->EnumDef_servant_->_tied_object ();
    case CORBA::dk_Fixed:
      return this->FixedDef_servant_->_tied_object ();
    case CORBA::dk_Interface:
      return this->ExtInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_LocalInterface:
      return this->ExtLocalInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_Native:
      return this->NativeDef_servant_->_tied_object ();
    case CORBA::dk_Primitive:
      return this->PrimitiveDef_servant_->_tied_object ();
    case CORBA::dk_Sequence:
      return this->SequenceDef_servant_->_tied_object ();
    case CORBA::dk_String:
      return this->StringDef_servant_->_tied_object ();
    case CORBA::dk_Struct:
      return this->StructDef_servant_->_tied_object ();
    case CORBA::dk_Union:
      return this->UnionDef_servant_->_tied_object ();
    case CORBA::dk_ValueBox:
      return this->ValueBoxDef_servant_->_tied_object ();
    case CORBA::dk_Value:
      return this->ExtValueDef_servant_->_tied_object ();
    case CORBA::dk_Wstring:
      return this->WstringDef_servant_->_tied_object ();
    default:
      return 0;
  }
}

TAO_Container_i *
TAO_Repository_i::select_container (CORBA::DefinitionKind def_kind) const
{
  switch (def_kind)
  {
    case CORBA::dk_AbstractInterface:
      return this->ExtAbstractInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_Exception:
      return this->ExceptionDef_servant_->_tied_object ();
    case CORBA::dk_Interface:
      return this->ExtInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_LocalInterface:
      return this->ExtLocalInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_Repository:
      return const_cast<TAO_Repository_i *> (this);
    case CORBA::dk_Struct:
      return this->StructDef_servant_->_tied_object ();
    case CORBA::dk_Union:
      return this->UnionDef_servant_->_tied_object ();
    case CORBA::dk_Value:
      return this->ExtValueDef_servant_->_tied_object ();
    default:
      return 0;
  }
}

TAO_Contained_i *
TAO_Repository_i::select_contained (CORBA::DefinitionKind def_kind) const
{
  switch (def_kind)
  {
    case CORBA::dk_AbstractInterface:
      return this->ExtAbstractInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_Alias:
      return this->AliasDef_servant_->_tied_object ();
    case CORBA::dk_Attribute:
      return this->AttributeDef_servant_->_tied_object ();
    case CORBA::dk_Constant:
      return this->ConstantDef_servant_->_tied_object ();
    case CORBA::dk_Enum:
      return this->EnumDef_servant_->_tied_object ();
    case CORBA::dk_Exception:
      return this->ExceptionDef_servant_->_tied_object ();
    case CORBA::dk_Interface:
      return this->ExtInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_LocalInterface:
      return this->ExtLocalInterfaceDef_servant_->_tied_object ();
    case CORBA::dk_Native:
      return this->NativeDef_servant_->_tied_object ();
    case CORBA::dk_Operation:
      return this->OperationDef_servant_->_tied_object ();
    case CORBA::dk_Struct:
      return this->StructDef_servant_->_tied_object ();
    case CORBA::dk_Union:
      return this->UnionDef_servant_->_tied_object ();
    case CORBA::dk_ValueBox:
      return this->ValueBoxDef_servant_->_tied_object ();
    case CORBA::dk_Value:
      return this->ExtValueDef_servant_->_tied_object ();
    case CORBA::dk_ValueMember:
      return this->ValueMemberDef_servant_->_tied_object ();
    default:
      return 0;
  }
}

PortableServer::POA_ptr
TAO_Repository_i::select_poa (CORBA::DefinitionKind def_kind) const
{
  switch (def_kind)
  {
    case CORBA::dk_AbstractInterface:
      return this->ExtAbstractInterfaceDef_poa_.in ();
    case CORBA::dk_Alias:
      return this->AliasDef_poa_.in ();
    case CORBA::dk_Array:
      return this->ArrayDef_poa_.in ();
    case CORBA::dk_Attribute:
      return this->AttributeDef_poa_.in ();
    case CORBA::dk_Constant:
      return this->ConstantDef_poa_.in ();
    case CORBA::dk_Enum:
      return this->EnumDef_poa_.in ();
    case CORBA::dk_Exception:
      return this->ExceptionDef_poa_.in ();
    case CORBA::dk_Fixed:
      return this->FixedDef_poa_.in ();
    case CORBA::dk_Interface:
      return this->ExtInterfaceDef_poa_.in ();
    case CORBA::dk_LocalInterface:
      return this->ExtLocalInterfaceDef_poa_.in ();
    case CORBA::dk_Native:
      return this->NativeDef_poa_.in ();
    case CORBA::dk_Operation:
      return this->OperationDef_poa_.in ();
    case CORBA::dk_Primitive:
      return this->PrimitiveDef_poa_.in ();
    case CORBA::dk_Sequence:
      return this->SequenceDef_poa_.in ();
    case CORBA::dk_String:
      return this->StringDef_poa_.in ();
    case CORBA::dk_Struct:
      return this->StructDef_poa_.in ();
    case CORBA::dk_Union:
      return this->UnionDef_poa_.in ();
    case CORBA::dk_ValueBox:
      return this->ValueBoxDef_poa_.in ();
    case CORBA::dk_Value:
      return this->ExtValueDef_poa_.in ();
    case CORBA::dk_ValueMember:
      return this->ValueMemberDef_poa_.in ();
    case CORBA::dk_Wstring:
      return this->WstringDef_poa_.in ();
    default:
      return PortableServer::POA::_nil ();
  }
}

PortableServer::Current_ptr
TAO_Repository_i::poa_current (void) const
{
  return this->poa_current_.in ();
}

ACE_Configuration *
TAO_Repository_i::config (void) const
{
  return this->config_;
}

CORBA::TypeCodeFactory_ptr
TAO_Repository_i::tc_factory (void) const
{
  return this->tc_factory_.in ();
}

CORBA::Repository_ptr
TAO_Repository_i::repo_objref (void) const
{
  return CORBA::Repository::_duplicate (this->repo_objref_.in ());
}

void
TAO_Repository_i::repo_objref (CORBA::Repository_ptr objref)
{
  this->repo_objref_ = objref;
}

ACE_Configuration_Section_Key
TAO_Repository_i::root_key (void) const
{
  return this->root_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::repo_ids_key (void) const
{
  return this->repo_ids_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::pkinds_key (void) const
{
  return this->pkinds_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::strings_key (void) const
{
  return this->strings_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::wstrings_key (void) const
{
  return this->wstrings_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::fixeds_key (void) const
{
  return this->fixeds_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::arrays_key (void) const
{
  return this->arrays_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::sequences_key (void) const
{
  return this->sequences_key_;
}

const char *
TAO_Repository_i::extension (void) const
{
  return this->extension_.in ();
}

ACE_Lock &
TAO_Repository_i::lock (void) const
{
  return *this->lock_;
}

const char * TAO_Repository_i::TAO_IFR_primitive_kinds[] =
  {
    "pk_null",
    "pk_void",
    "pk_short",
    "pk_long",
    "pk_ushort",
    "pk_ulong",
    "pk_float",
    "pk_double",
    "pk_boolean",
    "pk_char",
    "pk_octet",
    "pk_any",
    "pk_TypeCode",
    "pk_Principal",
    "pk_string",
    "pk_objref",
    "pk_longlong",
    "pk_ulonglong",
    "pk_longdouble",
    "pk_wchar",
    "pk_wstring",
    "pk_value_base"
  };

const char *
TAO_Repository_i::pkind_to_string (CORBA::PrimitiveKind pkind) const
{
  return TAO_Repository_i::TAO_IFR_primitive_kinds[pkind];
}

u_int
TAO_Repository_i::num_pkinds (void) const
{
  return sizeof (TAO_Repository_i::TAO_IFR_primitive_kinds) / sizeof (char*);
}

// For debugging use only.
void
TAO_Repository_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
