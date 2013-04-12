
//=============================================================================
/**
 *  @file   CosPropertyService_i.cpp
 *
 *  $Id$
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

bool
CosProperty_Hash_Key::operator == (const CosProperty_Hash_Key &hash_key) const
{
  return (ACE_OS::strcmp (this->pname_,
                          hash_key.pname_) == 0);
}

u_long
CosProperty_Hash_Key::hash (void) const
{
  u_long ret = ACE::hash_pjw (this->pname_);

  return ret;
}

CosProperty_Hash_Key::~CosProperty_Hash_Key (void)
{
}

//======================================================================

CosProperty_Hash_Value::CosProperty_Hash_Value (void)
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (const CORBA::Any &any,
                                                const CosPropertyService::PropertyModeType &mode)
  : pvalue_ (any),
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

//======================================================================

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
TAO_PropertySetFactory::create_propertyset (void)
{
  // New a TAO_PropertySet.
  TAO_PropertySet *new_set;
  ACE_NEW_RETURN (new_set, TAO_PropertySet, 0);

  // Successful, store this in the products sequence and return.
  CORBA::ULong cur_len = this->propertyset_products_.length ();
  this->propertyset_products_.length (cur_len + 1);
  this->propertyset_products_[cur_len] = new_set;
  return new_set->_this ();
}

// Allows a client to create a new TAO_PropertySet with specific
// constraints. "All the properties will have *fixed-normal* modes".

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_constrained_propertyset (const CosPropertyService::PropertyTypes &allowed_property_types,
                                                        const CosPropertyService::Properties &allowed_properties)
{
  TAO_PropertySet *new_set = 0;
  CosPropertyService::PropertySet_ptr propset_ptr = 0;

  try
    {
      // New a TAO_PropertySet using these constraints.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySet (allowed_property_types,
                                       allowed_properties), 0);

      // Successful, store this in the products sequence.
      CORBA::ULong products_len = this->propertyset_products_.length ();
      this->propertyset_products_.length (products_len + 1);
      this->propertyset_products_[products_len] = new_set;

      // All done.
      propset_ptr = new_set->_this ();
    }
  catch (const CORBA::UserException& )
    {
      // Release the memory.
      delete new_set;

      // Throw the exception.
      throw CosPropertyService::ConstraintNotSupported();
    }
  catch (const CORBA::SystemException& )
    {
      // Release memory.
      delete new_set;

      // Throw the exception.
      throw;
    }

  return propset_ptr;
}

// Allows a client to create a new TAO_PropertySet with specific
// constraints. "All the properties will have *fixed-normal* modes".

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_initial_propertyset (const CosPropertyService::Properties &initial_properties)
{
  TAO_PropertySet *new_set = 0;
  CosPropertyService::PropertySet_ptr propset_ptr = 0;

  try
    {
      // New a TAO_PropertySet.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySet (initial_properties), 0);

      // Successful, store this in the products sequence.
      CORBA::ULong products_len = this->propertyset_products_.length ();
      this->propertyset_products_.length (products_len + 1);
      this->propertyset_products_[products_len] = new_set;

      // All done.
      propset_ptr = new_set->_this ();
    }
  catch (const CosPropertyService::MultipleExceptions& )
    {
      // Release memory.
      delete new_set;

      // Throw the exception.
      throw;
    }
  catch (const CORBA::SystemException& )
    {
      // Release the memory.
      delete new_set;

      // Throw the exception.
      throw;
    }

  return propset_ptr;
}

// Destructor.
TAO_PropertySetDefFactory::~TAO_PropertySetDefFactory (void)
{
  // Release all the new'ed products.
  for (CORBA::ULong pi = 0;
       pi < this->propertysetdef_products_.length ();
       pi++)
    delete this->propertysetdef_products_[pi];
}

//======================================================================

// Constrctor.
TAO_PropertySetDefFactory::TAO_PropertySetDefFactory (void)
{
}

// Returns a  new TAO_PropertySetDef object. "The property setdef
// returned  will *not* have any initial properties."
// Keep sequence of things new'ed and at the destructor of the factory
// delete all these New'ed things.

CosPropertyService::PropertySetDef_ptr
TAO_PropertySetDefFactory::create_propertysetdef (void)
{
  // New a TAO_PropertySetDef.
  TAO_PropertySetDef *new_set;
  ACE_NEW_RETURN (new_set, TAO_PropertySetDef, 0);

  // Successful, store this in the products sequence and return.
  CORBA::ULong cur_len = this->propertysetdef_products_.length ();
  this->propertysetdef_products_.length (cur_len + 1);
  this->propertysetdef_products_[cur_len] = new_set;

  CosPropertyService::PropertySetDef_ptr propsetdef_ptr =
    new_set->_this ();

  return propsetdef_ptr;
}

CosPropertyService::PropertySetDef_ptr
TAO_PropertySetDefFactory::create_constrained_propertysetdef (const CosPropertyService::PropertyTypes &allowed_property_types,
                                                              const CosPropertyService::PropertyDefs &allowed_property_defs)
{
  TAO_PropertySetDef *new_set = 0;
  CosPropertyService::PropertySetDef_ptr propsetdef_ptr = 0;

  try
    {
      // New a TAO_PropertySetDef using these constraints.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySetDef (allowed_property_types,
                                          allowed_property_defs), 0);

      // Successful, store this in the products sequence.
      CORBA::ULong products_len = this->propertysetdef_products_.length ();
      this->propertysetdef_products_.length (products_len + 1);
      this->propertysetdef_products_[products_len] = new_set;

      // All done. Return the pointer.
      propsetdef_ptr = new_set->_this ();
    }
  catch (const CORBA::UserException& )
    {
      // Release the memory.
      delete new_set;

      // Throw the exception.
      throw CosPropertyService::ConstraintNotSupported();
    }
  catch (const CORBA::SystemException& )
    {
      // Release memory.
      delete new_set;

      // Throw the exception.
      throw;
    }

  return propsetdef_ptr;
}

// Allows the client to create a new TAO_PropertySetDef with specific
// initital constraints.

CosPropertyService::PropertySetDef_ptr
TAO_PropertySetDefFactory::create_initial_propertysetdef (const CosPropertyService::PropertyDefs &initial_property_defs)
{
  TAO_PropertySetDef *new_set = 0;
  CosPropertyService::PropertySetDef_ptr propsetdef_ptr = 0;

  try
    {
      // New a TAO_PropertySet using these lengths.
      ACE_NEW_RETURN (new_set,
                      TAO_PropertySetDef (initial_property_defs), 0);

      // Successful, store this in the products sequence.
      CORBA::ULong products_len = this->propertysetdef_products_.length ();
      this->propertysetdef_products_.length (products_len + 1);
      this->propertysetdef_products_[products_len] = new_set;

      // All done.
      propsetdef_ptr = new_set->_this ();
    }
  catch (const CosPropertyService::MultipleExceptions& )
    {
      // Release memory.
      delete new_set;

      // Throw the exception.
      throw;
    }
  catch (const CORBA::SystemException& )
    {
      // Release the memory.
      delete new_set;

      // Throw the exception.
      throw;
    }

  return propsetdef_ptr;
}

//======================================================================

// Makes default sized hash_table_. All the sequences are set the max
// length as 0.

TAO_PropertySet::TAO_PropertySet (void)
{
}

// Init values that the PropertySetFactory will want to specify. Write
// the allowed properties in the Hash Table with *fixed-normal* mode.
// @@ Need to check for duplicate properties and raise exceptions.


TAO_PropertySet::TAO_PropertySet (const CosPropertyService::PropertyTypes allowed_property_types,
                                      const CosPropertyService::Properties allowed_properties)
  : allowed_property_types_ (allowed_property_types),
    allowed_property_names_ (allowed_properties.length ())
{
  // Set the length for the sequence, just to make sure.
  this->allowed_property_names_.length (allowed_properties.length ());

  // Keep the allowed property names in the sequence..
  for (CORBA::ULong ni = 0;
       ni < allowed_properties.length ();
       ni++)
    this->allowed_property_names_[ni] =
      allowed_properties[ni].property_name;

  // Define the allowed properties in the hash table.
  try
    {
      this->define_properties (allowed_properties);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_PropertySet-Constructor");
      throw;
    }
}

// TAO_PropertySetDef's constructor needs this, for initializing the
// allowed properties' sequence withe just the maximum length.

TAO_PropertySet::TAO_PropertySet (const CosPropertyService::PropertyTypes allowed_property_types,
                                      const CORBA::ULong number_of_allowed_properties)
  : allowed_property_types_ (allowed_property_types),
    allowed_property_names_ (number_of_allowed_properties)
{
}

// PropertySetFactory needs this constructor. Store all the initial
// properies with *normal* modes.


TAO_PropertySet::TAO_PropertySet (const CosPropertyService::Properties initial_properties)
{
  // Define all the initial properties in the Property Set. All take
  // *normal* modes.
  try
    {
      this->define_properties (initial_properties);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_PropertySet-Constructor");
      throw;
    }
}

// Destructor. All sequences will be deleted.

TAO_PropertySet::~TAO_PropertySet (void)
{
}

// Function to modify or add a property to the PropertySet:
// Adds the name and the value to the set... Doesn't check for Typecode
// overwriting, duplicate names, void names etc, yet.
// @@ Uses Normal mode as the default mode of properties, We can
// change this behavior based on the Initial set of allowed modes, if
// there is anything like that set by the client.


void
TAO_PropertySet::define_property (const char *property_name,
                                  const CORBA::Any &property_value)
{
  // Check the name's validity.
  if (property_name == 0)
    throw CosPropertyService::InvalidPropertyName();

  CORBA::TypeCode_var arg_tc = property_value.type ();

  // Is this type allowed?
  if (is_type_allowed (arg_tc.in ()) != 1)
    throw CosPropertyService::UnsupportedTypeCode();

  // Is this property allowed?
  if (is_property_allowed (property_name) != 1)
    throw CosPropertyService::UnsupportedProperty();

  // Try to bind the property. Use normal mode.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value (property_value,
                                     CosPropertyService::normal);
  COSPROPERTY_HASH_ENTRY *entry_ptr = 0;

  int ret = this->hash_table_.bind (hash_key,
                                    hash_value,
                                    entry_ptr);

  CORBA::TypeCode_var mapped_tc;

  switch (ret)
    {
    case 0:
      break;
    case 1:
      // Property already exists.

      // Is the pointer valid?
      if (entry_ptr == 0)
        throw CORBA::UNKNOWN ();

      mapped_tc = entry_ptr->int_id_.pvalue_.type ();

      {
        CORBA::Boolean const equal_tc =
          mapped_tc.in ()->equal (arg_tc.in ());

        // If type is not the same, raise exception.
        if (!equal_tc)
          throw CosPropertyService::ConflictingProperty();
      }

      // If mode is read only, raise exception.
      if ((entry_ptr->int_id_.pmode_ == CosPropertyService::read_only) ||
          (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
        throw CosPropertyService::ReadOnlyProperty();

      // Use the mode that is already there.
      hash_value.pmode_ = entry_ptr->int_id_.pmode_;

      // Everything is fine. Overwrite the value.
      if (this->hash_table_.rebind (hash_key,
                                    hash_value) != 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "TAO_PropertySet::Define Property failed\n"));
          throw CORBA::UNKNOWN ();
        }
      break;
    default:
      // Error. ret is -1.
      throw CORBA::UNKNOWN ();
    }

  return;
}

// Tells whether this type is allowed in this property set or no.

CORBA::Boolean
TAO_PropertySet::is_type_allowed (CORBA::TypeCode_ptr type)
{

  // If the sequence is empty, no constraints.
  if (this->allowed_property_types_.length () == 0)
    return 1;

  // Check in the allowed_property_types sequence.
  CORBA::Boolean ret_val = 0;
  for (CORBA::ULong ti = 0;
       ti < this->allowed_property_types_.length ();
       ti++)
    {
      try
        {
          ret_val = this->allowed_property_types_[ti]->equal (type);

          if (ret_val == 1)
            return 1;
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("TAO_PropertySet::is_type_allowed failed");
          return ret_val;
        }
    }

  // Type not found.
  return ret_val;
}

// Tells whether this property is allowed in this property or no.

CORBA::Boolean
TAO_PropertySet::is_property_allowed (const char* property_name)
{
  // If  the sequence is empty, no constraints.
  if (this->allowed_property_names_.length() == 0)
    return 1;

  // Check in the allowed_property_names.
  for (CORBA::ULong ni = 0;
       ni < this->allowed_property_names_.length ();
       ni++)
    if (ACE_OS::strcmp ((const char *) this->allowed_property_names_[ni],
                        property_name) == 0)
      return 1;

  // Name not found in the sequence.
  return 0;
}

// Defining a sequence of properties
//
// Check for overwriting, duplicate names, void names etc and raise
// appropriate exceptions.


void
TAO_PropertySet::define_properties (const CosPropertyService::Properties &nproperties)
{
  // Get the length.
  CORBA::ULong sequence_length = nproperties.length ();

  // Define multiple exceptions object.
  CosPropertyService::MultipleExceptions multi_ex;

  for (CORBA::ULong pi = 0; pi < sequence_length; pi++)
    {
      try
        {
          // Define this property.
          this->define_property (nproperties [pi].property_name.in (),
                                 nproperties [pi].property_value);
        }
      catch (const CosPropertyService::InvalidPropertyName& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      catch (const CosPropertyService::ConflictingProperty& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::conflicting_property;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      catch (const CosPropertyService::ReadOnlyProperty& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::read_only_property;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      catch (const CosPropertyService::UnsupportedTypeCode& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_type_code;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      catch (const CosPropertyService::UnsupportedProperty& )
         {
           CORBA::ULong len = multi_ex.exceptions.length ();
           multi_ex.exceptions.length (len + 1);
           multi_ex.exceptions[len].reason =
             CosPropertyService::unsupported_property;
           multi_ex.exceptions[len].failing_property_name =
             nproperties[pi].property_name;
         }
       catch (const CORBA::SystemException& )
         {
           throw;
         }
    }

  // Raise the multi exception if needed.
  if (multi_ex.exceptions.length () > 0)
    throw CosPropertyService::MultipleExceptions (multi_ex);
}

// Returns the current number of properties associated with this
// PropertySet.


CORBA::ULong
TAO_PropertySet::get_number_of_properties (void)
{
  return static_cast<CORBA::ULong> (this->hash_table_.current_size ());
}

//  Returns all of the property names currently defined in the
//  PropertySet. If the PropertySet contains more than <how_many>
//  property names, then the remaining property names are put into the
//  PropertyNamesIterator.


void
TAO_PropertySet::get_all_property_names (CORBA::ULong how_many,
                                             CosPropertyService::PropertyNames_out property_names,
                                             CosPropertyService::PropertyNamesIterator_out rest)
{
  // Allocating storage is a must.
  ACE_NEW (property_names,
           CosPropertyService::PropertyNames);

  CORBA::ULong num_of_properties =
    this->get_number_of_properties ();

  if (num_of_properties == 0)
    // Nothing to do.
    return;

  // Set the length of the property_names appropriately.
  CORBA::ULong sequence_length = 0;

  if (how_many > 0)
    {
      if (how_many >= num_of_properties)
        sequence_length = num_of_properties;
      else
        sequence_length = how_many;
      property_names->length (sequence_length);
    }

  // Iterate thru names and put them in the property_names.

  COSPROPERTY_HASH_ENTRY *entry_ptr;
  COSPROPERTY_HASH_ITERATOR iterator (this->hash_table_);

  for (CORBA::ULong ni = 0;
       ni < sequence_length;
       ni++, iterator.advance ())
    if (iterator.next (entry_ptr) != 0)
      property_names [ni] =
        CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());

  // If there are some more properties, put them in the
  // iterator. How?? Make a new PropertySet and use that to create
  // propertyNames Iterator.

  if (num_of_properties > how_many)
    {
      TAO_PropertySet *property_set;

      ACE_NEW (property_set, TAO_PropertySet);

      for (CORBA::ULong i = how_many;
           i < num_of_properties;
           i++, iterator.advance ())
        if (iterator.next (entry_ptr) != 0)
          if (property_set->hash_table_.bind (entry_ptr->ext_id_,
                                              entry_ptr->int_id_) < 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "Error:TAO_PropertySet::get_all_property_names\n"));

      // Make the NamesIterator out of this TAO_PropertySet.
      TAO_PropertyNamesIterator *names_iterator;
      ACE_NEW (names_iterator, TAO_PropertyNamesIterator (*property_set));

      // Init the out parameter.

      // Get the Interface ptr.
      CosPropertyService::PropertyNamesIterator_ptr iterator_ptr =
        names_iterator->_this ();

      // POA stuff todo here, since we have <destroy> method in the
      // <NamesIterator> interface.
      // Give ownership of this servant to the POA.
      names_iterator->_remove_ref ();

      // Init the out parameter.
      rest = iterator_ptr;
    }
}

// Returns the value of a property in the PropertySet.


CORBA::Any *
TAO_PropertySet::get_property_value (const char *property_name)
{
  // Check the name's validity.
  if (property_name == 0)
    throw CosPropertyService::InvalidPropertyName();

  // Get the value out of the hash table.

  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value;

  if (this->hash_table_.find (hash_key, hash_value) != 0)
    throw CosPropertyService::PropertyNotFound();

  // Return the any value got.
  CORBA::Any *any_ptr =0;
  ACE_NEW_RETURN (any_ptr,
                  CORBA::Any (hash_value.pvalue_),
                  0);
  return any_ptr;
}

// Returns the values of the properties listed in property_names. When
// the boolean return value is true, the Properties parameter contains
// valid values for all requested property names. If false, then all
// properties with a value of type tk_void may have failed due to
// <PropertyNotFound> or <InvalidPropertyName>.


CORBA::Boolean
TAO_PropertySet::get_properties (const CosPropertyService::PropertyNames &property_names,
                                 CosPropertyService::Properties_out nproperties)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (nproperties,
                  CosPropertyService::Properties,
                  0);

  // Validate the length.
  CORBA::ULong n = property_names.length ();
  if (n == 0)
    return 0;

  // Set the length for the out parameter.
  nproperties->length (n);

  // Get values for all the names.

  CORBA::Any_ptr any_ptr = 0;
  CORBA::Boolean ret_val = 1;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      any_ptr = get_property_value (property_names [i]);

      if (any_ptr != 0)
        {
          // Property is found.
          nproperties [i].property_name = property_names [i];
          nproperties [i].property_value = *any_ptr;
        }
      else
        {
          // Invalid name. Ret value is False.
          ret_val = 0;

          // Assign void type to this name in the out parameter.
          nproperties [i].property_name =
            property_names [i];

          // Make an any value with tk_void type.
          CORBA::Any any;
          any._tao_set_typecode (CORBA::_tc_void);
          nproperties [i].property_value = any;
        }
    }

  return ret_val;
}


void
TAO_PropertySet::get_all_properties (CORBA::ULong how_many,
                                     CosPropertyService::Properties_out nproperties,
                                     CosPropertyService::PropertiesIterator_out rest)
{
  // Allocate memory for the out parameter.
  ACE_NEW (nproperties,
           CosPropertyService::Properties);

  // Validate the length.
  CORBA::ULong num_of_properties =
          this->get_number_of_properties ();

  if (num_of_properties == 0)
    return;

  // Set the length for the nproperties if how_many > 0.
  CORBA::ULong sequence_length = 0;

  if (how_many > 0)
    {
      if (how_many >= num_of_properties)
        sequence_length = num_of_properties;
      else
        sequence_length = how_many;

      nproperties->length (sequence_length);
    }

  // Prepare an iterator and iterate through the PropertySet. Retrive
  // the values.

  COSPROPERTY_HASH_ITERATOR iterator (this->hash_table_);
  COSPROPERTY_HASH_ENTRY *entry_ptr = 0;

  for (CORBA::ULong i = 0;
       i < sequence_length;
       i++, iterator.advance ())
    if (iterator.next (entry_ptr) != 0)
      {
        nproperties[i].property_name =
          CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());
        nproperties[i].property_value =
          entry_ptr->int_id_.pvalue_;
      }

  // If there are more properties, put them in the <PropertiesIterator>.
  // Make a new <TAO_PropertySet> and use that to create an Properties
  // iterator.  put that in a iterator and assign that to the out
  // paramerter.

  if (num_of_properties > how_many)
    {
      TAO_PropertySet *prop_set;

      ACE_NEW (prop_set, TAO_PropertySet);

      for (CORBA::ULong i = sequence_length;
           i < num_of_properties;
           i++, iterator.advance ())
        {
          if (iterator.next (entry_ptr) != 0
              && prop_set->hash_table_.bind (entry_ptr->ext_id_,
                                             entry_ptr->int_id_) < 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "Error:TAO_PropertySet::get_all_properties\n"));
        }

      // Make the iterator out of the new TAO_Propset.
      TAO_PropertiesIterator *iterator = 0;
      ACE_NEW (iterator,
               TAO_PropertiesIterator (*prop_set));

      // Init the out parameter.

      // Get the interface ptr.
      CosPropertyService::PropertiesIterator_ptr iterator_ptr =
        iterator->_this ();

      // POA stuff todo here, since we have <destroy> method in the
      // <NamesIterator> interface.
      // Give ownership of this servant to the POA.
      iterator->_remove_ref ();

      // Init the out parameter.
      rest = iterator_ptr;
    }
}

// Deletes the specified property if it exists from a PropertySet.


void
TAO_PropertySet::delete_property (const char *property_name)
{
  // Check the name's validity.
  if (property_name == 0)
    throw CosPropertyService::InvalidPropertyName();

  // Get the entry from the Hash Table.

  CosProperty_Hash_Key hash_key (property_name);
  COSPROPERTY_HASH_ENTRY *entry_ptr = 0;

  if (this->hash_table_.find (hash_key,
                              entry_ptr) == -1)
    throw CosPropertyService::PropertyNotFound();

  // If property is fixed, then raise exception.
  if ((entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_normal)
      || (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
    throw CosPropertyService::FixedProperty();

  // Unbind this property.
  if (this->hash_table_.unbind (entry_ptr) != 0)
    {
      throw CORBA::UNKNOWN ();
    }

  return;
}

// Deletes the properties defined in the property_names
// parameter. This is a batch operation that returns the
// MultipleExceptions exception if any delete failed.


void
TAO_PropertySet::delete_properties (const CosPropertyService::PropertyNames &property_names)
{
  // Get the length.
  CORBA::ULong sequence_length = property_names.length ();

  // Declare multiple exceptions' object.
  CosPropertyService::MultipleExceptions multi_ex;

  for (CORBA::ULong pi = 0; pi < sequence_length; pi++)
    {
      try
        {
          // Delete this property.
          this->delete_property (property_names[pi]);
        }
      catch (const CosPropertyService::InvalidPropertyName& )
        {
          // Put this exception in the multiple exception.
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            property_names[pi];
        }
      catch (const CosPropertyService::PropertyNotFound& )
        {
          // Put this exception in the multiple exception.
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::property_not_found;
          multi_ex.exceptions[len].failing_property_name =
            property_names[pi];
        }
      catch (const CosPropertyService::FixedProperty& )
        {
          // Put this exception in the multiple exception.
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::fixed_property;
          multi_ex.exceptions[len].failing_property_name =
            property_names[pi];
        }
      catch (const CORBA::SystemException& )
        {
          // We cant afford to get this. Throw this.
          throw;
        }
    }

  // Raise the multiple exceptions if there are any.
  if (multi_ex.exceptions.length () > 0)
    throw CosPropertyService::MultipleExceptions (multi_ex);
}

// Delete all the properties in the current ProperySet : Delete the
// properties one by one.


CORBA::Boolean
TAO_PropertySet::delete_all_properties (void)
{
  // Get all the property names in a names' sequence.
  CosPropertyService::PropertyNames *names_ptr = 0;
  CosPropertyService::PropertyNames_out names_out (names_ptr);
  CosPropertyService::PropertyNames_var names;

  CosPropertyService::PropertyNamesIterator *iter_ptr = 0;
  CosPropertyService::PropertyNamesIterator_out iter_out (iter_ptr);
  CosPropertyService::PropertyNamesIterator_var iter;

  CORBA::ULong size = this->get_number_of_properties ();

  this->get_all_property_names (size,
                                names_out,
                                iter_out);

  // Get the out values on to the var varibles.
  names = names_out.ptr ();
  iter = iter_out.ptr ();

  // Delete all these properties.
  this->delete_properties (names.in ());

  // All properties deleted.
  return 1;
}

// Returns TRUE if the property is defined in the PropertySet.


CORBA::Boolean
TAO_PropertySet::is_property_defined (const char *property_name)
{
  CosProperty_Hash_Key hash_key (property_name);

  if (this->hash_table_.find (hash_key) == 0)
    return 1;
  else
    return 0;
}


void
TAO_PropertySet::operator= (const TAO_PropertySet &)
{
  // Empty.
}

//======================================================================

// Makes default sized hash_table_.

TAO_PropertySetDef::TAO_PropertySetDef (void)
{
}

// Constructor that the factory uses.

TAO_PropertySetDef::TAO_PropertySetDef (const CosPropertyService::PropertyTypes allowed_property_types,
                                        const CosPropertyService::PropertyDefs allowed_property_defs)
  : TAO_PropertySet(allowed_property_types,
                    allowed_property_defs.length ())
{
  // Set the length of the allowed property names.
  this->allowed_property_names_.length (allowed_property_defs.length ());

  // Copy the allowed properties' names to the sequence.
  for (CORBA::ULong ni = 0; ni < allowed_property_defs.length (); ni++)
    this->allowed_property_names_[ni] =
      allowed_property_defs[ni].property_name;

  // Define the allowed properties in the hash table.
  try
    {
      this->define_properties_with_modes (allowed_property_defs);
    }
  catch (const CORBA::Exception&)
    {
      throw;
    }
}

// Constructor that the factory uses.

TAO_PropertySetDef::TAO_PropertySetDef (const CosPropertyService::PropertyDefs initial_property_defs)
{
  this->define_properties_with_modes (initial_property_defs);
}

// Destructor.

TAO_PropertySetDef::~TAO_PropertySetDef (void)
{
}

// Return the sequence that is there in side.
void
TAO_PropertySetDef::get_allowed_property_types (CosPropertyService::PropertyTypes_out property_types)
{
  // Copy contents of the sequence.
  ACE_NEW (property_types,
           CosPropertyService::PropertyTypes (this->allowed_property_types_));
}

void
TAO_PropertySetDef::get_allowed_properties (CosPropertyService::PropertyDefs_out property_defs)
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
                                               CosPropertyService::PropertyModeType property_mode)
{
  // Check the names validity.
  if (property_name == 0)
    throw CosPropertyService::InvalidPropertyName();

  // Is this type allowed?
  if (is_type_allowed (property_value.type ()) != 1)
    throw CosPropertyService::UnsupportedTypeCode();

  // Is this property allowed?
  if (is_property_allowed (property_name) != 1)
    throw CosPropertyService::UnsupportedProperty();

  // Is this a valid mode.
  if (property_mode == CosPropertyService::undefined)
    throw CosPropertyService::UnsupportedMode();

  // Try to bind the Property.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value (property_value,
                                     property_mode);
  COSPROPERTY_HASH_ENTRY *entry_ptr;


  int ret = this->hash_table_.bind (hash_key, hash_value, entry_ptr);

  //CosProperty_Hash_Value old_value;
  //CosProperty_Hash_Key old_key;

  switch (ret)
    {
    case 0:
      break;
    case 1:
      // Property name exists.

      // Is the pointer valid.
      if (entry_ptr == 0)
        throw CORBA::UNKNOWN ();

      // If type is not the same, raise exception.
      if (entry_ptr->int_id_.pvalue_.type () != property_value.type ())
        throw CosPropertyService::ConflictingProperty();

      // If mode is read only, raise exception.
      if ((entry_ptr->int_id_.pmode_ == CosPropertyService::read_only) ||
          (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
        throw CosPropertyService::ReadOnlyProperty();

      // If current mode is fixed_normal, but the new mode is not
      // fixed, reject it.
      if ((entry_ptr->int_id_.pmode_ ==
           CosPropertyService::fixed_normal) &&
          (property_mode < CosPropertyService::fixed_normal))
        throw CosPropertyService::UnsupportedMode();

      // Everything is fine. Overwrite the value.
      if (this->hash_table_.rebind (hash_key,
                                    hash_value) > 0)
        {
          break;
        }
    default:
      // Error. ret is -1 or rebind returned other than 1.
      throw CORBA::UNKNOWN ();
    }

  return;
}

// Define one by one. If any excceptions raised, build
// MultipleExceptions sequence and raise that.
void
TAO_PropertySetDef::define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs)
{
  // Get the length.
  CORBA::ULong sequence_length = property_defs.length ();

  // Define multiple exceptions object.
  CosPropertyService::MultipleExceptions multi_ex;

  // Try defining the propdefs one by one.
  for (CORBA::ULong i = 0; i < sequence_length; i++)
    {
      try
        {
          // Define the property.
          this->define_property_with_mode (property_defs[i].property_name,
                                           property_defs[i].property_value,
                                           property_defs[i].property_mode);
        }
      catch (const CosPropertyService::InvalidPropertyName& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      catch (const CosPropertyService::ConflictingProperty& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::conflicting_property;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      catch (const CosPropertyService::ReadOnlyProperty& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::read_only_property;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      catch (const CosPropertyService::UnsupportedTypeCode& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_type_code;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      catch (const CosPropertyService::UnsupportedProperty& )
         {
           CORBA::ULong len = multi_ex.exceptions.length ();
           multi_ex.exceptions.length (len + 1);
           multi_ex.exceptions[len].reason =
             CosPropertyService::unsupported_property;
           multi_ex.exceptions[len].failing_property_name =
             property_defs[i].property_name;
         }
      catch (const CosPropertyService::UnsupportedMode& )
        {
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_mode;
          multi_ex.exceptions[len].failing_property_name =
            property_defs[i].property_name;
        }
      catch (const CORBA::SystemException& )
        {
          throw;
        }
    }

  // Raise the multi exception if needed.
  if (multi_ex.exceptions.length () > 0)
    throw CosPropertyService::MultipleExceptions (multi_ex);
}

// Get the mode of a property. Raises InvalidpropertyName,
// PropertyNotFound exceptions.
CosPropertyService::PropertyModeType
TAO_PropertySetDef::get_property_mode (const char *property_name)
{
  // Check for the name's validity.
  if (property_name == 0)
    throw CosPropertyService::InvalidPropertyName();

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
      throw CosPropertyService::PropertyNotFound();
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
                                        CosPropertyService::PropertyModes_out property_modes)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (property_modes,
                  CosPropertyService::PropertyModes,
                  1);

  // Validate the length of names sequence.
  CORBA::ULong sequence_length = property_names.length ();

  if (sequence_length == 0)
    return 1;

  // Set the length of the sequence.
  property_modes->length (sequence_length);

  // Intialize thre return value.
  CORBA::Boolean ret = 1;

  // Invoking get_property_mode for each name.
  CosPropertyService::PropertyModeType mode;
  for (CORBA::ULong i = 0; i < sequence_length; i++)
    {
      try
        {
          // Invoke get_property_mode for this name.
          mode = this->get_property_mode (property_names[i]);

          // Store the mode in the out sequence.
          property_modes[i].property_name = property_names[i];
          property_modes[i].property_mode = mode;
        }
      catch (const CORBA::Exception&)
        {
          // Return value becomes false.
          ret = 1;

          // Assign this property to the out parameter with undefined
          // mode.
          property_modes[i].property_name = property_names[i];
          property_modes[i].property_mode = CosPropertyService::undefined;
        }
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
                                       CosPropertyService::PropertyModeType property_mode)
{
  // Check the names validity.
  if (property_name == 0)
    throw CosPropertyService::InvalidPropertyName();

  // Trying to set to undefined mode is not allowed.
  if (property_mode == CosPropertyService::undefined)
    throw CosPropertyService::UnsupportedMode();

  // Find the property from the Hash Table.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value;

  int ret = this->hash_table_.find (hash_key, hash_value);

  //CosProperty_Hash_Value old_value;
  //CosProperty_Hash_Key old_key;

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
                                        hash_value) != 1)
            // Return values 0 and -1 are not possible.
            throw CORBA::UNKNOWN ();
          break;

        case CosPropertyService::read_only:
          // Read_only to fixed read only alone is possible.
          if (property_mode != CosPropertyService::fixed_readonly)
            throw CosPropertyService::UnsupportedMode();
          else
            {
              // Change the mode and update hash table.
              hash_value.pmode_ = property_mode;
              if (this->hash_table_.rebind (hash_key,
                                            hash_value) != 1)
                // Return values 0 and -1 are not possible.
                throw CORBA::UNKNOWN ();
            }
          break;

        case CosPropertyService::fixed_normal:
          // Fixed_normal to fixed_readonly alone is possible.
          if (property_mode != CosPropertyService::fixed_readonly)
            throw CosPropertyService::UnsupportedMode();
          else
            {
              // Change the mode and update the hash table.
              hash_value.pmode_ = property_mode;
              if (this->hash_table_.rebind (hash_key,
                                            hash_value) != 1)
                // Return values 0 and -1 are not possible.
                throw CORBA::UNKNOWN ();
            }
          break;

        default:
          // Fixed_readonly to any mode is not possible.
          throw CosPropertyService::UnsupportedMode();
        }
      break;
    case -1:
    default:
      // Error or property not found in the Hash Table.
      throw CosPropertyService::PropertyNotFound();
    }
}

// Batch operation for setting the property. Raises
// MultipleExceptions.  Set the properties one by one, catch
// exceptions if any and keep them as in the multiple exceptions
// sequence and return.

void
TAO_PropertySetDef::set_property_modes (const CosPropertyService::PropertyModes &property_modes)
{
  // Get the length of the sequence.
  CORBA::ULong sequence_length = property_modes.length ();

  // Multiple exception variable to keep track of exceptions.
  CosPropertyService::MultipleExceptions multi_ex;

  // Set  modes one by one.
  for (CORBA::ULong i = 0; i < sequence_length; i++)
    {
      try
        {
          this->set_property_mode (property_modes[i].property_name,
                                   property_modes[i].property_mode);
        }
      catch (const CosPropertyService::PropertyNotFound& )
        {
          // Put this exception in the multiple exception.
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::property_not_found;
          multi_ex.exceptions[len].failing_property_name =
            property_modes[i].property_name;
        }
      catch (const CosPropertyService::InvalidPropertyName& )
        {
          // Put this exception in the multiple exception.
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            property_modes[i].property_name;
        }
      catch (const CosPropertyService::UnsupportedMode& )
        {
          // Put this exception in the multiple exception.
          CORBA::ULong len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_mode;
          multi_ex.exceptions[len].failing_property_name =
            property_modes[i].property_name;
        }
      catch (const CORBA::SystemException& )
        {
          throw;
        }
    }

  // Raise the multi_ex, if needed.
  if (multi_ex.exceptions.length () > 0)
    throw CosPropertyService::MultipleExceptions (multi_ex);
}

//======================================================================

// Constructor. Construct the iterator from the PropertySet object.

TAO_PropertyNamesIterator::TAO_PropertyNamesIterator (TAO_PropertySet &property_set)
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
TAO_PropertyNamesIterator::reset (void)
{
  this->iterator_ = this->iterator_.map ().begin ();
}

// The next_one operation returns true if an item exists at the
// current position in the iterator with an output parameter of a
// property name. A return of false signifies no more items in the
// iterator.

CORBA::Boolean
TAO_PropertyNamesIterator::next_one (CORBA::String_out property_name)
{
  COSPROPERTY_HASH_ENTRY *entry_ptr;

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
                                   CosPropertyService::PropertyNames_out property_names)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (property_names,
                  CosPropertyService::PropertyNames,
                  0);

  COSPROPERTY_HASH_ENTRY *entry_ptr = 0;

  if (this->iterator_.next (entry_ptr) == 0 || how_many == 0)
    return 0;

  CORBA::ULong size = static_cast<CORBA::ULong> (this->iterator_.map ().current_size ());

  CORBA::ULong len = 0;
  if (how_many <= size)
    len = how_many;
  else
    len = size;
  property_names->length (len);

  for (CORBA::ULong ni = 0;
       ni < property_names->length ();
       ni++, this->iterator_.advance ())
    if (this->iterator_.next (entry_ptr) != 0)
      property_names [ni] =
        CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());

  return 1;
}

void
TAO_PropertyNamesIterator::destroy (void)
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.

  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id = poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

//======================================================================

TAO_PropertiesIterator::TAO_PropertiesIterator (TAO_PropertySet &property_set)
  : iterator_ (property_set.hash_table_)
{
}

TAO_PropertiesIterator::~TAO_PropertiesIterator (void)
{
}

void
TAO_PropertiesIterator::reset (void)
{
  this->iterator_ = this->iterator_.map ().begin ();
}

CORBA::Boolean
TAO_PropertiesIterator::next_one (CosPropertyService::Property_out aproperty)
{
  COSPROPERTY_HASH_ENTRY *entry_ptr;

  if (this->iterator_.next (entry_ptr) != 0)
    {
      aproperty = new CosPropertyService::Property;
      aproperty->property_name = entry_ptr->ext_id_.pname_;
      aproperty->property_value = entry_ptr->int_id_.pvalue_;
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
                                CosPropertyService::Properties_out nproperties)
{
  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (nproperties,
                  CosPropertyService::Properties,
                  0);

  COSPROPERTY_HASH_ENTRY *entry_ptr = 0;

  if (this->iterator_.next (entry_ptr) == 0 || how_many == 0)
    return 0;

  CORBA::ULong size = static_cast<CORBA::ULong> (this->iterator_.map ().current_size ());

  CORBA::ULong len;
  if (how_many <= size)
    len = how_many;
  else
    len = size;
  nproperties->length (len);

  for (CORBA::ULong ni = 0;
       ni < nproperties->length ();
       ni++,
         this->iterator_.advance ())
    {
      if (this->iterator_.next (entry_ptr) != 0)
        {
          nproperties [ni].property_name = entry_ptr->ext_id_.pname_;
          nproperties [ni].property_value =
            entry_ptr->int_id_.pvalue_;
        }
      else
        break;
    }

  return 1;
}

void
TAO_PropertiesIterator::destroy (void)
{
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.

  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id = poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
