// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   CosPropertyService_i.cpp
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/Property/CosPropertyService_i.h"

ACE_RCSID(Property, CosPropertyService_i, "$Id$")

// = Methods to deal with ACE_Hash_Map_Manager.

CosProperty_Hash_Key::CosProperty_Hash_Key (void)
{
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const char * &name)
  : pname_ (CORBA::string_dup (name))
{
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const CosPropertyService::PropertyName &name)
  : pname_ (CORBA::string_dup (name))
{
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const CosProperty_Hash_Key &src)
  : pname_ (src.pname_)
{
}

int
CosProperty_Hash_Key::operator == (const CosProperty_Hash_Key &hash_key) const
{
  return ACE_OS::strcmp (this->pname_.in (),
                         hash_key.pname_.in ()) == 0;
}

u_long
CosProperty_Hash_Key::hash (void) const
{
  u_long ret = ACE::hash_pjw (this->pname_.in ());

  return ret;
}

CosProperty_Hash_Key::~CosProperty_Hash_Key (void)
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (void)
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (const CORBA::Any &any,
                                                const CosPropertyService::PropertyModeType &mode)
  : pvalue_ (new CORBA::Any (any)),
    pmode_ (mode)
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (const CosProperty_Hash_Value &src)
  : pvalue_ (src.pvalue_),
    pmode_ (src.pmode_)
{
}

CosProperty_Hash_Value::~CosProperty_Hash_Value (void)
{
}

// = The actual implementation methods.

// Constructor.
TAO_PropertySetFactory::TAO_PropertySetFactory (void)
{
}

// Destructor.
TAO_PropertySetFactory::~TAO_PropertySetFactory (void)
{
}

// Returns a new TAO_PropertySet object. "The property set returned
// will *not* have any initial properties."  Keep sequence of things
// new'ed and at the destructor of the factory delete all these New'ed
// things.

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_propertyset (CORBA::Environment &TAO_IN_ENV)
{
  // New a TAO_PropertySet.
  TAO_PropertySet<POA_CosPropertyService::PropertySet> *new_set;
  ACE_NEW_RETURN (new_set, TAO_PropertySet<POA_CosPropertyService::PropertySet>, 0);

  // Successful, store this in the products sequence and return.
  size_t cur_len = this->propertyset_products_.length ();
  this->propertyset_products_.length (cur_len + 1);
  this->propertyset_products_[cur_len] = new_set;
  return new_set->_this (TAO_IN_ENV);
}

// Allows a client to create a new TAO_PropertySet with specific
// constraints. "All the properties will have *fixed-normal* modes".

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_constrained_propertyset (const CosPropertyService::PropertyTypes &allowed_property_types,
                                                        const CosPropertyService::Properties &allowed_properties,
                                                        CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "create_constrained_propertyset\n"));
  TAO_PropertySet<POA_CosPropertyService::PropertySet> *new_set = 0;

  TAO_TRY
    {
      // New a TAO_PropertySet using these constraints.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySet<POA_CosPropertyService::PropertySet> (allowed_property_types,
									    allowed_properties,
									    TAO_TRY_ENV),
                      0);
      TAO_CHECK_ENV;

      // Successful, store this in the products sequence.
      size_t products_len = this->propertyset_products_.length ();
      this->propertyset_products_.length (products_len + 1);
      this->propertyset_products_[products_len] = new_set;

      // All done. Return the pointer.
      return new_set->_this (TAO_IN_ENV);
    }
  TAO_CATCH (CORBA::UserException, ex)
    {
      // Should be the Multiple exception raised by the PropertySet
      // constructor, which got that from define_properties method.
      // Throw ConstraintNotSupported exception for this, because that
      // is the only exception that this method can throw.
      TAO_TRY_ENV.print_exception ("User exception");

      // Release the memory.
      delete new_set;

      // Throw the exception.
      TAO_THROW_RETURN (CosPropertyService::ConstraintNotSupported(),
                        0);
    }
  TAO_CATCH (CORBA::SystemException, ex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");

      // Release memory.
      delete new_set;

      // Throw the exception.
      TAO_RETHROW_RETURN (0);
    }
  TAO_ENDTRY_RETURN (0);
}

// Allows a client to create a new TAO_PropertySet with specific
// constraints. "All the properties will have *fixed-normal* modes".

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_initial_propertyset (const CosPropertyService::Properties &initial_properties,
                                                    CORBA::Environment &TAO_IN_ENV)
{
  TAO_PropertySet<POA_CosPropertyService::PropertySet> *new_set = 0;

  TAO_TRY
    {
      // New a TAO_PropertySet.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySet<POA_CosPropertyService::PropertySet> (initial_properties,
									    TAO_TRY_ENV),
                      0);
      TAO_CHECK_ENV;

      // Successful, store this in the products sequence.
      size_t products_len = this->propertyset_products_.length ();
      this->propertyset_products_.length (products_len + 1);
      this->propertyset_products_[products_len] = new_set;

      // All done. Return the pointer.
      return new_set->_this (TAO_IN_ENV);
    }
  TAO_CATCH (CosPropertyService::MultipleExceptions, ex)
    {
      // This is the only user exception the constructor can give, and
      // this is the only thing I can throw.
      TAO_TRY_ENV.print_exception ("MultipleExceptions");

      // Release memory.
      delete new_set;

      // Throw the exception.
      TAO_RETHROW_RETURN (0);
    }
  TAO_CATCH (CORBA::SystemException, ex)
    {
      // Should be some system exceptions.
      TAO_TRY_ENV.print_exception ("System Exception");

      // Release the memory.
      delete new_set;

      // Throw the exception.
      TAO_RETHROW_RETURN (0);
    }
  TAO_ENDTRY_RETURN (0);
}

// Destructor.
TAO_PropertySetDefFactory::~TAO_PropertySetDefFactory (void)
{
  // Release all the new'ed products.
  for (size_t pi = 0;
       pi < this->propertysetdef_products_.length ();
       pi++)
    delete this->propertysetdef_products_[pi];
}

// Constrctor.
TAO_PropertySetDefFactory::TAO_PropertySetDefFactory (void)
{
}

// Returns a  new TAO_PropertySetDef object. "The property setdef
// returned  will *not* have any initial properties."
// Keep sequence of things new'ed and at the destructor of the factory
// delete all these New'ed things.

CosPropertyService::PropertySetDef_ptr
TAO_PropertySetDefFactory::create_propertysetdef (CORBA::Environment &TAO_IN_ENV)
{
  // New a TAO_PropertySetDef.
  TAO_PropertySetDef *new_set;
  ACE_NEW_RETURN (new_set, TAO_PropertySetDef, 0);

  // Successful, store this in the products sequence and return.
  size_t cur_len = this->propertysetdef_products_.length ();
  this->propertysetdef_products_.length (cur_len + 1);
  this->propertysetdef_products_[cur_len] = new_set;
  return new_set->_this (TAO_IN_ENV);
}

CosPropertyService::PropertySetDef_ptr
TAO_PropertySetDefFactory::create_constrained_propertysetdef (const CosPropertyService::PropertyTypes &allowed_property_types,
                                                              const CosPropertyService::PropertyDefs &allowed_property_defs,
                                                              CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "create_constrained_propertysetdef\n"));

  TAO_PropertySetDef *new_set = 0;

  TAO_TRY
    {
      // New a TAO_PropertySetDef using these constraints.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySetDef (allowed_property_types,
                                          allowed_property_defs,
                                          TAO_TRY_ENV),
                      0);
      TAO_CHECK_ENV;

      // Successful, store this in the products sequence.
      size_t products_len = this->propertysetdef_products_.length ();
      this->propertysetdef_products_.length (products_len + 1);
      this->propertysetdef_products_[products_len] = new_set;

      // All done. Return the pointer.
      return new_set->_this (TAO_IN_ENV);
    }
  TAO_CATCH (CORBA::UserException, ex)
    {
      // Should be the Multiple exception raised by the PropertySetDef
      // constructor, which got that from define_properties_with_modes
      // method.  Throw ConstraintNotSupported exception for this,
      // because that is the only exception that this method can
      // throw.
      TAO_TRY_ENV.print_exception ("User exception");

      // Release the memory.
      delete new_set;

      // Throw the exception.
      TAO_THROW_RETURN (CosPropertyService::ConstraintNotSupported(),
                        0);
    }
  TAO_CATCH (CORBA::SystemException, ex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");

      // Release memory.
      delete new_set;

      // Throw the exception.
      TAO_RETHROW_RETURN (0);
    }
  TAO_ENDTRY_RETURN (0);
}

// Allows the client to create a new TAO_PropertySetDef with specific
// initital constraints.

CosPropertyService::PropertySetDef_ptr
TAO_PropertySetDefFactory::create_initial_propertysetdef (const CosPropertyService::PropertyDefs &initial_property_defs,
                                                          CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "create_initial_propertysetdef\n"));

  TAO_PropertySetDef *new_set = 0;

  TAO_TRY
    {
      // New a TAO_PropertySet using these lengths.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySetDef (initial_property_defs,
                                          TAO_TRY_ENV),
                      0);
      TAO_CHECK_ENV;

      // Successful, store this in the products sequence.
      size_t products_len = this->propertysetdef_products_.length ();
      this->propertysetdef_products_.length (products_len + 1);
      this->propertysetdef_products_[products_len] = new_set;

      // All done. Return the pointer.
      return new_set->_this (TAO_IN_ENV);
    }
  TAO_CATCH (CosPropertyService::MultipleExceptions, ex)
    {
      // This is the only user exception the constructor can give, and
      // this is the only thing I can throw.
      TAO_TRY_ENV.print_exception ("MultipleExceptions");

      // Release memory.
      delete new_set;

      // Throw the exception.
      TAO_RETHROW_RETURN (0);
    }
  TAO_CATCH (CORBA::SystemException, ex)
    {
      // Should be some system exceptions.
      TAO_TRY_ENV.print_exception ("System Exception");

      // Release the memory.
      delete new_set;

      // Throw the exception.
      TAO_RETHROW_RETURN (0);
    }
  TAO_ENDTRY_RETURN (0);
}
// Makes default sized hash_table_.

TAO_PropertySetDef::TAO_PropertySetDef (void)
{
}

// Constructor that the factory uses.

TAO_PropertySetDef::TAO_PropertySetDef (const CosPropertyService::PropertyTypes allowed_property_types,
                                        const CosPropertyService::PropertyDefs allowed_property_defs,
                                        CORBA::Environment &TAO_IN_ENV)
  : TAO_PropertySet<POA_CosPropertyService::PropertySetDef> (allowed_property_types,
							     allowed_property_defs.length (),
							     TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySetDef Hash_Table:-current_size %d\ntotal_size %d\n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));

  // Set the length of the allowed property names.
  this->allowed_property_names_.length (allowed_property_defs.length ());

  ACE_DEBUG ((LM_DEBUG,
              "Allowed property types size %d\nAllowed propertydefs' names size%d\n",
              this->allowed_property_types_.length (),
              this->allowed_property_names_.length ()));

  // Copy the allowed properties' names to the sequence.
  for (size_t ni = 0; ni < allowed_property_defs.length (); ni++)
    this->allowed_property_names_[ni] =
      allowed_property_defs[ni].property_name;

  // Define the allowed properties in the hash table.
  TAO_TRY
    {
      this->define_properties_with_modes (allowed_property_defs,
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_PropertySetDef");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

// Constructor that the factory uses.

TAO_PropertySetDef::TAO_PropertySetDef (const CosPropertyService::PropertyDefs initial_property_defs,
                                        CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySetDef Hash_Table:-current_size %d\ntotal_size %d\nInitialProps %d\n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size (),
              initial_property_defs.length ()));

  // Define the initial properties in this property set def.
  TAO_TRY
    {
      this->define_properties_with_modes (initial_property_defs,
                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_PropertySetDef-Constructor");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

// Destructor.

TAO_PropertySetDef::~TAO_PropertySetDef (void)
{
}

// Return the sequence that is there in side.
void
TAO_PropertySetDef::get_allowed_property_types (CosPropertyService::PropertyTypes_out property_types,
                                                CORBA::Environment &)
{
  // Copy contents of the sequence.
  ACE_NEW (property_types,
           CosPropertyService::PropertyTypes (this->allowed_property_types_));
}

void
TAO_PropertySetDef::get_allowed_properties (CosPropertyService::PropertyDefs_out property_defs,
                                            CORBA::Environment &)
{
  // We have all the names, get the values and the modes from the Hash
  // Table and return.

  // Allocate memory.
  ACE_NEW (property_defs,
           CosPropertyService::PropertyDefs (this->allowed_property_names_.length ()));

  // Get the modes and property values for all these property
  // names. Some may not be there in the Hash Table, probably got
  // deleted because of their modes were not safe.

  // @@ TO DO.
}

// Check for name's validity. If name not there define it. If it is
// there and if type is equal and if mode allows define it else raise
// exception.
void
TAO_PropertySetDef::define_property_with_mode (const char *property_name,
                                               const CORBA::Any &property_value,
                                               CosPropertyService::PropertyModeType property_mode,
                                               CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "define_property with mode : property_name %s \n",
              property_name));

  // Check the names validity.
  if (property_name == 0)
    TAO_THROW (CosPropertyService::InvalidPropertyName());

  // Is this type allowed?
  if (is_type_allowed (property_value.type ()) != 1)
    TAO_THROW (CosPropertyService::UnsupportedTypeCode());

  // Is this property allowed?
  if (is_property_allowed (property_name) != 1)
    TAO_THROW (CosPropertyService::UnsupportedProperty());

  // Is this a valid mode.
  if (property_mode == CosPropertyService::undefined)
    TAO_THROW (CosPropertyService::UnsupportedMode());

  // Try to bind the Property.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value (property_value,
                                     property_mode);
  CosProperty_Hash_Entry_ptr entry_ptr;


  int ret = this->hash_table_.bind (hash_key, hash_value, entry_ptr);

  CosProperty_Hash_Value old_value;
  CosProperty_Hash_Key old_key;

  switch (ret)
    {
    case 0:
      // Bind successful.
      ACE_DEBUG ((LM_DEBUG,
                  "define_property_with_mode successful\n"));
      break;
    case 1:
      // Property name exists.

      // Is the pointer valid.
      if (entry_ptr == 0)
        TAO_THROW (CORBA::UNKNOWN ());

      // If type is not the same, raise exception.
      if (entry_ptr->int_id_.pvalue_->type () != property_value.type ())
        TAO_THROW (CosPropertyService::ConflictingProperty());

      // If mode is read only, raise exception.
      if ((entry_ptr->int_id_.pmode_ == CosPropertyService::read_only) ||
          (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
        TAO_THROW (CosPropertyService::ReadOnlyProperty());

      // If current mode is fixed_normal, but the new mode is not
      // fixed, reject it.
      if ((entry_ptr->int_id_.pmode_ ==
           CosPropertyService::fixed_normal) &&
          (property_mode < CosPropertyService::fixed_normal))
        TAO_THROW (CosPropertyService::UnsupportedMode());

      // Everything is fine. Overwrite the value.
      if (this->hash_table_.rebind (hash_key,
                                    hash_value,
                                    old_key,
                                    old_value) > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Property Defined\n"));
          break;
        }
    default:
      // Error. ret is -1 or rebind returned other than 1.
      TAO_THROW (CORBA::UNKNOWN ());
    }

  return;
}

// Define one by one. If any excceptions raised, build
// MultipleExceptions sequence and raise that.
void
TAO_PropertySetDef::define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs,
                                                  CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "define_properties_with_modes\n"));

  // Get the length.
  size_t sequence_length = property_defs.length ();

  // Define multiple exceptions object.
  CosPropertyService::MultipleExceptions multi_ex;

  // Try defining the propdefs one by one.
  for (size_t i = 0; i < sequence_length; i++)
    {
      TAO_TRY
        {
          // Define the property.
          this->define_property_with_mode (property_defs[i].property_name,
                                           property_defs[i].property_value,
                                           property_defs[i].property_mode,
                                           TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      TAO_CATCH (CosPropertyService::InvalidPropertyName, ex)
        {
          TAO_TRY_ENV.print_exception ("InvalidPropertyName");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      TAO_CATCH (CosPropertyService::ConflictingProperty, ex)
        {
          TAO_TRY_ENV.print_exception ("ConflictingProperty");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::conflicting_property;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      TAO_CATCH (CosPropertyService::ReadOnlyProperty, ex)
        {
          TAO_TRY_ENV.print_exception ("ReadOnlyProperty");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::read_only_property;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      TAO_CATCH (CosPropertyService::UnsupportedTypeCode, ex)
        {
          TAO_TRY_ENV.print_exception ("UnsupportedTypeCode");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_type_code;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
       TAO_CATCH (CosPropertyService::UnsupportedProperty, ex)
         {
           TAO_TRY_ENV.print_exception ("UnsupportedProperty");
           size_t len = multi_ex.exceptions.length ();
           multi_ex.exceptions.length (len + 1);
           multi_ex.exceptions[len].reason =
             CosPropertyService::unsupported_property;
           multi_ex.exceptions[len].failing_property_name =
             property_defs[i].property_name;
         }
        TAO_CATCH (CosPropertyService::UnsupportedMode, ex)
          {
            TAO_TRY_ENV.print_exception ("UnsupportedMode");
            size_t len = multi_ex.exceptions.length ();
            multi_ex.exceptions.length (len + 1);
            multi_ex.exceptions[len].reason =
              CosPropertyService::unsupported_mode;
            multi_ex.exceptions[len].failing_property_name =
              property_defs[i].property_name;
          }
        TAO_CATCH (CORBA::SystemException, sysex)
          {
            // Print it and throw it again.
            TAO_TRY_ENV.print_exception ("SystemException");
            TAO_RETHROW;
          }
        TAO_ENDTRY;
    }

  // Raise the multi exception if needed.
  if (multi_ex.exceptions.length () > 0)
    TAO_THROW (CosPropertyService::MultipleExceptions (multi_ex));
}

// Get the mode of a property. Raises InvalidpropertyName,
// PropertyNotFound exceptions.
CosPropertyService::PropertyModeType
TAO_PropertySetDef::get_property_mode (const char *property_name,
                                       CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "get_property_mode\n"));

  // Check for the name's validity.
  if (property_name == 0)
    TAO_THROW_RETURN (CosPropertyService::InvalidPropertyName(),
                      CosPropertyService::undefined);

  // Find the property in the hash table.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value;

  int ret = this->hash_table_.find (hash_key, hash_value);

  switch (ret)
    {
    case 0:
      // Property found.
      return hash_value.pmode_;
    default:
      // Error or property is not found.
      TAO_THROW_RETURN (CosPropertyService::PropertyNotFound(),
                        CosPropertyService::undefined);
    }
}

// Batch operation for getting the property. Invoke get_property_mode
// for each name.
// Return value False indicates that properties with *undefined* modes
// have failed due to PropertyNotFound or InvalidPropertyName exception.
// Returning False in case of *Nothing to retun* or New is
// failing. The caller has  to check the out parameter whether it is
// Nil or no, before doing something with it.
CORBA::Boolean
TAO_PropertySetDef::get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                        CosPropertyService::PropertyModes_out property_modes,
                                        CORBA::Environment &)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (property_modes,
                  CosPropertyService::PropertyModes,
                  1);

  // Validate the length of names sequence.
  size_t sequence_length = property_names.length ();

  if (sequence_length == 0)
    return 1;

  // Set the length of the sequence.
  property_modes->length (sequence_length);

  // Intialize thre return value.
  CORBA::Boolean ret = 1;

  // Invoking get_property_mode for each name.
  CosPropertyService::PropertyModeType mode;
  for (size_t i = 0; i < sequence_length; i++)
    {
      TAO_TRY
        {
          // Invoke get_property_mode for this name.
          mode = this->get_property_mode (property_names[i],
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Store the mode in the out sequence.
          property_modes[i].property_name = property_names[i];
          property_modes[i].property_mode = mode;
        }
      TAO_CATCHANY
        {
          // DEBUG.
          TAO_TRY_ENV.print_exception ("Some exception");

          // Return value becomes false.
          ret = 1;

          // Assign this property to the out parameter with undefined
          // mode.
          property_modes[i].property_name = property_names[i];
          property_modes[i].property_mode = CosPropertyService::undefined;
        }
      TAO_ENDTRY;
    }

  return ret;
}

// Changing the mode of the property.
// "Normal" to anything is possible.
// "Readonly" mode to "Fixed-Readonly" is possible. Others not possible.
// "Fixed-Normal" to "Fixed-Readonly" is possible. Other things are impossible.
// "Fixed-Readonly" to anything is *not* possible.
void
TAO_PropertySetDef::set_property_mode (const char *property_name,
                                       CosPropertyService::PropertyModeType property_mode,
                                       CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "set-property_mode"));

  // Check the names validity.
  if (property_name == 0)
    TAO_THROW (CosPropertyService::InvalidPropertyName());

  // Trying to set to undefined mode is not allowed.
  if (property_mode == CosPropertyService::undefined)
    TAO_THROW (CosPropertyService::UnsupportedMode());

  // Find the property from the Hash Table.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value;

  int ret = this->hash_table_.find (hash_key, hash_value);

  CosProperty_Hash_Value old_value;
  CosProperty_Hash_Key old_key;

  // Act acc to the ret value.
  switch (ret)
    {
    case 0:
      // Property found.

      // If the new mode  is the same as the old one, nothing to do.
      if (hash_value.pmode_ == property_mode)
        return;

      // Check for legality of the mode change.
      switch (hash_value.pmode_)
        {
        case CosPropertyService::normal:
          // Set the new mode and update the hash table.
          hash_value.pmode_ = property_mode;
          if (this->hash_table_.rebind (hash_key,
                                        hash_value,
                                        old_key,
                                        old_value) != 1)
            // Return values 0 and -1 are not possible.
            TAO_THROW (CORBA::UNKNOWN ());
          else
            ACE_DEBUG ((LM_DEBUG,
                        "Mode set succesful\n"));
          break;

        case CosPropertyService::read_only:
          // Read_only to fixed read only alone is possible.
          if (property_mode != CosPropertyService::fixed_readonly)
            TAO_THROW (CosPropertyService::UnsupportedMode());
          else
            {
              // Change the mode and update hash table.
              hash_value.pmode_ = property_mode;
              if (this->hash_table_.rebind (hash_key,
                                            hash_value,
                                            old_key,
                                            old_value) != 1)
                // Return values 0 and -1 are not possible.
                TAO_THROW (CORBA::UNKNOWN ());
              else
                ACE_DEBUG ((LM_DEBUG, "Mode set successful\n"));
            }
          break;

        case CosPropertyService::fixed_normal:
          // Fixed_normal to fixed_readonly alone is possible.
          if (property_mode != CosPropertyService::fixed_readonly)
            TAO_THROW (CosPropertyService::UnsupportedMode());
          else
            {
              // Change the mode and update the hash table.
              hash_value.pmode_ = property_mode;
              if (this->hash_table_.rebind (hash_key,
                                            hash_value,
                                            hash_key,
                                            hash_value) != 1)
                // Return values 0 and -1 are not possible.
                TAO_THROW (CORBA::UNKNOWN ());
              else
                ACE_DEBUG ((LM_DEBUG, "Mode set successful\n"));
            }
          break;

        default:
          // Fixed_readonly to any mode is not possible.
          TAO_THROW (CosPropertyService::UnsupportedMode());
        }
      break;
    case -1:
    default:
      // Error or property not found in the Hash Table.
      TAO_THROW (CosPropertyService::PropertyNotFound());
    }
}

// Batch operation for setting the property. Raises
// MultipleExceptions.  Set the properties one by one, catch
// exceptions if any and keep them as in the multiple exceptions
// sequence and return.

void
TAO_PropertySetDef::set_property_modes (const CosPropertyService::PropertyModes &property_modes,
                                        CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "set_property_modes\n"));

  // Get the length of the sequence.
  size_t sequence_length = property_modes.length ();

  // Multiple exception variable to keep track of exceptions.
  CosPropertyService::MultipleExceptions multi_ex;

  // Set  modes one by one.
  for (size_t i = 0; i < sequence_length; i++)
    {
      TAO_TRY
        {
          this->set_property_mode (property_modes[i].property_name,
                                   property_modes[i].property_mode,
                                   TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      TAO_CATCH (CosPropertyService::PropertyNotFound, ex)
        {
          // DEBUG.
          TAO_TRY_ENV.print_exception ("PropertyNotFound");

          // Put this exception in the multiple exception.
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::property_not_found;
          multi_ex.exceptions[len].failing_property_name =
            property_modes[i].property_name;
        }
      TAO_CATCH (CosPropertyService::InvalidPropertyName, ex)
        {
          // DEBUG.
          TAO_TRY_ENV.print_exception ("InvalidPropertyName");

          // Put this exception in the multiple exception.
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            property_modes[i].property_name;
        }
      TAO_CATCH (CosPropertyService::UnsupportedMode, ex)
        {
          // DEBUG.
          TAO_TRY_ENV.print_exception ("UnsupportedMode");

          // Put this exception in the multiple exception.
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_mode;
          multi_ex.exceptions[len].failing_property_name =
            property_modes[i].property_name;
        }
      TAO_CATCH (CORBA::SystemException, systex)
        {
          // DEBUG.
          TAO_TRY_ENV.print_exception ("Unknown");

          // We cant afford to get this. Throw this.
          TAO_RETHROW;
        }
      TAO_ENDTRY;
    }

  // Raise the multi_ex, if needed.
  if (multi_ex.exceptions.length () > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "set_property_modes:- Raising multi_ex\n"));
      TAO_THROW (CosPropertyService::MultipleExceptions (multi_ex));
    }
}

// Constructor. Construct the iterator from the PropertySet object.

TAO_PropertyNamesIterator::TAO_PropertyNamesIterator (TAO_PropertySet<POA_CosPropertyService::PropertySet> &property_set)
  : iterator_ (property_set.hash_table_)
{
}

// Destructor.

TAO_PropertyNamesIterator::~TAO_PropertyNamesIterator (void)
{
}

// Resets the position in an iterator to the first property name, if
// one exists.

void
TAO_PropertyNamesIterator::reset (CORBA::Environment &)
{
  this->iterator_ = this->iterator_.map ().begin ();
}

// The next_one operation returns true if an item exists at the
// current position in the iterator with an output parameter of a
// property name. A return of false signifies no more items in the
// iterator.

CORBA::Boolean
TAO_PropertyNamesIterator::next_one (CORBA::String_out property_name,
                                     CORBA::Environment &)
{
  CosProperty_Hash_Entry_ptr entry_ptr;

  if (this->iterator_.next (entry_ptr) != 0)
    {
      property_name =
        CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());
      this->iterator_.advance ();
      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
TAO_PropertyNamesIterator::next_n (CORBA::ULong how_many,
                                   CosPropertyService::PropertyNames_out property_names,
                                   CORBA::Environment &)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (property_names,
                  CosPropertyService::PropertyNames,
                  0);

  CosProperty_Hash_Entry_ptr entry_ptr = 0;

  if (this->iterator_.next (entry_ptr) == 0 || how_many == 0)
    return 0;

  size_t size = this->iterator_.map ().current_size ();

  property_names->length (how_many <= size ? how_many : size);

  for (size_t ni = 0;
       ni < property_names->length ();
       ni++, this->iterator_.advance ())
    if (this->iterator_.next (entry_ptr) != 0)
      property_names [ni] =
        CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());

  return 1;
}

void
TAO_PropertyNamesIterator::destroy (CORBA::Environment &)
{
}

TAO_PropertiesIterator::TAO_PropertiesIterator (TAO_PropertySet<POA_CosPropertyService::PropertySet> &property_set)
  : iterator_ (property_set.hash_table_)
{
}

TAO_PropertiesIterator::~TAO_PropertiesIterator (void)
{
}

void
TAO_PropertiesIterator::reset (CORBA::Environment &)
{
  this->iterator_ = this->iterator_.map ().begin ();
}

CORBA::Boolean
TAO_PropertiesIterator::next_one (CosPropertyService::Property_out aproperty,
                                  CORBA::Environment &)
{
  CosProperty_Hash_Entry_ptr entry_ptr;

  if (this->iterator_.next (entry_ptr) != 0)
    {
      aproperty = new CosPropertyService::Property;
      aproperty->property_name = entry_ptr->ext_id_.pname_;
      aproperty->property_value = entry_ptr->int_id_.pvalue_.in ();
      this->iterator_.advance ();
      return 1;
    }
  else
    {
      aproperty = new CosPropertyService::Property;
      return 0;
    }
}

CORBA::Boolean
TAO_PropertiesIterator::next_n (CORBA::ULong how_many,
                                CosPropertyService::Properties_out nproperties,
                                CORBA::Environment &)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (nproperties,
                  CosPropertyService::Properties,
                  0);

  CosProperty_Hash_Entry_ptr entry_ptr = 0;

  if (this->iterator_.next (entry_ptr) == 0 || how_many == 0)
    return 0;

  size_t size = this->iterator_.map ().current_size ();

  nproperties->length (how_many <= size ? how_many : size);

  for (size_t ni = 0;
       ni < nproperties->length ();
       ni++,
         this->iterator_.advance ())
    if (this->iterator_.next (entry_ptr) != 0)
      {
        nproperties [ni].property_name = entry_ptr->ext_id_.pname_;
        nproperties [ni].property_value =
          entry_ptr->int_id_.pvalue_.in ();
      }
    else
      break;
  return 1;
}

void
TAO_PropertiesIterator::destroy (CORBA::Environment &)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>;
template class ACE_Hash<CosProperty_Hash_Key>;
template class ACE_Equal_To<CosProperty_Hash_Key>;
template class ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>;
template class TAO_PropertySet<POA_CosPropertyService::PropertySet>;
template class TAO_PropertySet<POA_CosPropertyService::PropertySetDef>;
template class TAO_Unbounded_Sequence<TAO_PropertySet<POA_CosPropertyService::PropertySet>*>;
template class TAO_Unbounded_Sequence<TAO_PropertySetDef*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>
#pragma instantiate ACE_Hash<CosProperty_Hash_Key>
#pragma instantiate ACE_Equal_To<CosProperty_Hash_Key>
#pragma instantiate ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Hash<CosProperty_Hash_Key>, ACE_Equal_To<CosProperty_Hash_Key>, ACE_Null_Mutex>
#pragma instantiate TAO_PropertySet<POA_CosPropertyService::PropertySet>
#pragma instantiate TAO_PropertySet<POA_CosPropertyService::PropertySetDef>
#pragma instantiate TAO_Unbounded_Sequence<TAO_PropertySet<POA_CosPropertyService::PropertySet>*>
#pragma instantiate TAO_Unbounded_Sequence<TAO_PropertySetDef*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
