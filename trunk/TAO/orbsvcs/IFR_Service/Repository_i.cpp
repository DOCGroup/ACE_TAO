/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "IDLType_i.h"
#include "ExceptionDef_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, Repository_i, "$Id$")

TAO_Repository_i::TAO_Repository_i (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr poa,
                                    ACE_Configuration *config)
  : TAO_IRObject_i (0, ACE_Configuration_Section_Key ()),
    TAO_Container_i (0, ACE_Configuration_Section_Key ()),
    orb_ (orb),
    ir_poa_ (poa),
    config_ (config),
    servant_factory_ (0),
    extension_ (CORBA::string_dup ("TAO_IFR_name_extension"))
{
  ACE_TRY_NEW_ENV
    {
      ACE_NEW_THROW_EX (this->servant_factory_,
                        IFR_Servant_Factory (this),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("TypeCodeFactory",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (object.in ()))
        {
          this->tc_factory_ =
            CORBA::TypeCodeFactory::_narrow (object.in (),
                                             ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (this->tc_factory_.in ()))
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("Repository: TypeCodeFactory narrow failed\n")
              ));
        }
      else
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("Repository: TypeCodeFactory resolve failed\n")
            ));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("Repository: Initialization failed\n")
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  this->config_->open_section (config_->root_section (),
                               "root",
                               1, // Will create if IFR is not persistent.
                               this->root_key_);

  this->config_->open_section (this->root_key_, 
                               "defns", 
                               1, // Will create if IFR is not persistent.
                               this->named_objs_key_);

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

      u_int num_pkinds = this->num_pkinds ();
      for (u_int i = 0; i < num_pkinds; i++)
        {
          ACE_Configuration_Section_Key key;
          IR::PrimitiveKind pkind = ACE_static_cast (IR::PrimitiveKind,
                                                     i);

          this->config_->open_section (this->pkinds_key_,
                                       this->pkind_to_string (pkind),
                                       1,
                                       key);
                 
          this->config_->set_integer_value (key,
                                            "def_kind",
                                            IR::dk_Primitive);

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

  if (status == -3) // Field "count" does not exist.
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
                                    IR::dk_Repository);
}

TAO_Repository_i::~TAO_Repository_i (void)
{
  delete this->servant_factory_;
}

IR::DefinitionKind
TAO_Repository_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Repository;
}

void 
TAO_Repository_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::BAD_INV_ORDER (2, CORBA::COMPLETED_NO));
}

IR::Contained_ptr 
TAO_Repository_i::lookup_id (const char *search_id,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString path;
  if (this->config_->get_string_value (this->repo_ids_key_,
                                       search_id,
                                       path)
       == 0)
    {
      return IR::Contained::_nil ();
    }

  ACE_Configuration_Section_Key key;
  this->config_->expand_path (this->root_key_,
                              path,
                              key);

  u_int kind = 0;
  this->config_->get_integer_value (key,
                                    "def_kind",
                                    kind);

  IR::DefinitionKind def_kind = ACE_static_cast (IR::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->servant_factory_->create_objref (def_kind,
                                           path.c_str (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::Contained::_nil ());

  return IR::Contained::_narrow (obj.in (),
                                 ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_Repository_i::get_canonical_typecode (CORBA::TypeCode_ptr tc,
                                          CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TCKind kind = tc->kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  switch (kind)
  {
    // For all the TCKinds not covered below, no change is needed.
    default:
      return CORBA::TypeCode::_duplicate (tc);
    case CORBA::tk_fixed:
      ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::TypeCode::_nil ());
    case CORBA::tk_array:
    {
      CORBA::ULong length = tc->length (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      CORBA::TypeCode_var ctype = tc->content_type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      CORBA::TypeCode_var canon_ctype = 
        this->get_canonical_typecode (ctype.in (),
                                      ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      return this->tc_factory ()->create_array_tc (length,
                                                   canon_ctype.in (),
                                                   ACE_TRY_ENV);
    }
    case CORBA::tk_sequence:
    {
      CORBA::ULong length = tc->length (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      CORBA::TypeCode_var ctype = tc->content_type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      CORBA::TypeCode_var canon_ctype = 
        this->get_canonical_typecode (ctype.in (),
                                      ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      return this->tc_factory ()->create_sequence_tc (length,
                                                      canon_ctype.in (),
                                                      ACE_TRY_ENV);
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
      CORBA::String_var id = tc->id (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      ACE_TString path;
      int status =
        this->config ()->get_string_value (this->repo_ids_key (),
                                           id.in (),
                                           path);

      // TODO - something in case the repo id is an empty string,
      //        or if it is not found in this repository
      if (status != 0)
        return CORBA::TypeCode::_nil ();

      ACE_Configuration_Section_Key key;
      this->config ()->expand_path (this->root_key (),
                                    path,
                                    key,
                                    0);

      // An ExceptionDef is not an IDLType.
      if (kind == CORBA::tk_except)
        {
          TAO_ExceptionDef_i impl (this,
                                   key);

          return impl.type (ACE_TRY_ENV);
        }
      else
        {
          TAO_IDLType_i *impl = 
            this->servant_factory ()->create_idltype (key,
                                                      ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

          auto_ptr<TAO_IDLType_i> safety (impl);

          return impl->type (ACE_TRY_ENV);
        }
    }
  }
}

IR::PrimitiveDef_ptr 
TAO_Repository_i::get_primitive (IR::PrimitiveKind kind,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TString obj_id ("pkinds\\");

  obj_id += this->pkind_to_string (kind);

  CORBA::Object_var obj =
    this->servant_factory_->create_objref (IR::dk_Primitive,
                                           obj_id.c_str (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::PrimitiveDef::_nil ());

  return IR::PrimitiveDef::_narrow (obj.in (),
                                    ACE_TRY_ENV);
}

IR::StringDef_ptr 
TAO_Repository_i::create_string (CORBA::ULong bound,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int count = 0;
  this->config_->get_integer_value (this->strings_key_,
                                    "count",
                                    count);

  CORBA::String_var name = this->int_to_string (count++);
  this->config_->set_integer_value (this->strings_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->strings_key_,
                               name.in (),
                               1,
                               new_key);

  this->config_->set_integer_value (new_key,
                                    "bound",
                                    bound);

  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    IR::dk_String);

  this->config_->set_string_value (new_key,
                                   "name",
                                   name.in ());

  // Create the object reference.
  ACE_TString obj_id ("strings\\");  
  obj_id += name.in ();

  CORBA::Object_var obj =
    this->servant_factory_->create_objref (IR::dk_String,
                                           obj_id.c_str (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::StringDef::_nil ());

  return IR::StringDef::_narrow (obj.in (),
                                 ACE_TRY_ENV);
}

IR::WstringDef_ptr 
TAO_Repository_i::create_wstring (CORBA::ULong bound,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  u_int count = 0;
  this->config_->get_integer_value (this->wstrings_key_,
                                    "count",
                                    count);

  CORBA::String_var name = this->int_to_string (count++);
  this->config_->set_integer_value (this->wstrings_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->wstrings_key_,
                               name.in (),
                               1,
                               new_key);

  this->config_->set_integer_value (new_key,
                                    "bound",
                                    bound);

  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    IR::dk_Wstring);

  this->config_->set_string_value (new_key,
                                   "name",
                                   name.in ());

  // Create the object reference.
  ACE_TString obj_id ("wstrings\\");  
  obj_id += name.in ();

  CORBA::Object_var obj =
    this->servant_factory_->create_objref (IR::dk_Wstring,
                                           obj_id.c_str (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::WstringDef::_nil ());

  return IR::WstringDef::_narrow (obj.in (),
                                  ACE_TRY_ENV);
}

IR::SequenceDef_ptr 
TAO_Repository_i::create_sequence (CORBA::ULong bound,
                                   IR::IDLType_ptr element_type,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid = 
    this->ir_poa_->reference_to_id (element_type,
                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::SequenceDef::_nil ());

  CORBA::String_var element_path = 
    PortableServer::ObjectId_to_string (oid.in ());

  u_int count = 0;
  this->config_->get_integer_value (this->sequences_key_,
                                    "count",
                                    count);

  CORBA::String_var name = this->int_to_string (count++);
  this->config_->set_integer_value (this->sequences_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->sequences_key_,
                               name.in (),
                               1,
                               new_key);

  // Set the bound attribute.
  this->config_->set_integer_value (new_key,
                                    "bound",
                                    bound);

  // Set the def_kind attribute.
  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    IR::dk_Sequence);

  // Set the "name" for destroy to use.
  this->config_->set_string_value (new_key,
                                   "name",
                                   name.in ());

  // To get key to element type.
  this->config_->set_string_value (new_key,
                                   "element_path",
                                   element_path.in ());

  // Create the object reference.
  ACE_TString obj_id ("sequences\\");
  obj_id += name.in ();

  CORBA::Object_var obj =
    this->servant_factory_->create_objref (IR::dk_Sequence,
                                           obj_id.c_str (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::SequenceDef::_nil ());

  return IR::SequenceDef::_narrow (obj.in (),
                                   ACE_TRY_ENV);
}

IR::ArrayDef_ptr 
TAO_Repository_i::create_array (CORBA::ULong length,
                                IR::IDLType_ptr element_type,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid = 
    this->ir_poa_->reference_to_id (element_type,
                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ArrayDef::_nil ());

  CORBA::String_var element_path = 
    PortableServer::ObjectId_to_string (oid.in ());

  u_int count = 0;
  this->config_->get_integer_value (this->arrays_key_,
                                    "count",
                                    count);

  CORBA::String_var name = this->int_to_string (count++);
  this->config_->set_integer_value (this->arrays_key_,
                                    "count",
                                    count);

  // Make new database entry.
  ACE_Configuration_Section_Key new_key;
  this->config_->open_section (this->arrays_key_,
                               name.in (),
                               1,
                               new_key);

  // Set the length attribute.
  this->config_->set_integer_value (new_key,
                                    "length",
                                    length);

  // Set the def_kind attribute.
  this->config_->set_integer_value (new_key,
                                    "def_kind",
                                    IR::dk_Array);

  // Set the "name" for destroy to use.
  this->config_->set_string_value (new_key,
                                   "name",
                                   name.in ());

  // To get key to element type.
  this->config_->set_string_value (new_key,
                                   "element_path",
                                   element_path.in ());

  // Create the object reference.
  ACE_TString obj_id ("arrays\\");
  obj_id += name.in ();

  CORBA::Object_var obj =
    this->servant_factory_->create_objref (IR::dk_Array,
                                           obj_id.c_str (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ArrayDef::_nil ());

  return IR::ArrayDef::_narrow (obj.in (),
                                ACE_TRY_ENV);
}

IR::FixedDef_ptr 
TAO_Repository_i::create_fixed (CORBA::UShort /* digits */,
                                CORBA::Short /* scale */,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), IR::FixedDef::_nil ());
}

PortableServer::POA_ptr 
TAO_Repository_i::ir_poa (void) const
{
  return this->ir_poa_;
}

ACE_Configuration *
TAO_Repository_i::config (void) const
{
  return this->config_;
}

IFR_Servant_Factory *
TAO_Repository_i::servant_factory (void) const
{
  return this->servant_factory_;
}

CORBA::TypeCodeFactory_ptr
TAO_Repository_i::tc_factory (void) const
{
  return this->tc_factory_.in ();
}

IR::Repository_ptr
TAO_Repository_i::repo_objref (void) const
{
  return IR::Repository::_duplicate (this->repo_objref_);
}

void 
TAO_Repository_i::repo_objref (IR::Repository_ptr objref)
{
  this->repo_objref_ = objref;
}

ACE_Configuration_Section_Key
TAO_Repository_i::root_key (void) const
{
  return this->root_key_;
}

ACE_Configuration_Section_Key
TAO_Repository_i::named_objs_key (void) const
{
  return this->named_objs_key_;
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
TAO_Repository_i::extension (void)
{
  return this->extension_.in ();
}

const char *TAO_Repository_i::TAO_IFR_primitive_kinds[] =
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
TAO_Repository_i::pkind_to_string (IR::PrimitiveKind pkind) const
{
  return TAO_Repository_i::TAO_IFR_primitive_kinds[pkind];
}

u_int 
TAO_Repository_i::num_pkinds (void) const
{
  return sizeof (TAO_Repository_i::TAO_IFR_primitive_kinds) / sizeof (char*);
}

void 
TAO_Repository_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
