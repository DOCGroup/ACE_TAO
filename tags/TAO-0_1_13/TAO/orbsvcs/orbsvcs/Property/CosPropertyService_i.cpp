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

TAO_PropertySetFactory::TAO_PropertySetFactory (void)
{
}

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_propertyset (CORBA::Environment &env) 
{
  CosPropertyService::PropertySet_ptr return_val = 0;

  return return_val;
}

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_constrained_propertyset (const CosPropertyService::PropertyTypes &allowed_property_types, 
                                                        const CosPropertyService::Properties &allowed_properties, 
                                                        CORBA::Environment &env)
{
  CosPropertyService::PropertySet_ptr return_val = 0;

  return return_val;
}  

CosPropertyService::PropertySet_ptr 
TAO_PropertySetFactory::create_initial_propertyset (const CosPropertyService::Properties &initial_properties, 
                                                    CORBA::Environment &env) 
{
  CosPropertyService::PropertySet_ptr return_val = 0;

  return return_val;
}

TAO_PropertySetFactory::~TAO_PropertySetFactory (void)
{
}

TAO_PropertySetDefFactory::TAO_PropertySetDefFactory (void)
{
}

TAO_PropertySetDefFactory::~TAO_PropertySetDefFactory (void)
{
}
  
CosPropertyService::PropertySetDef_ptr 
TAO_PropertySetDefFactory::create_propertysetdef (CORBA::Environment &env)
{
  CosPropertyService::PropertySetDef_ptr return_val = 0;
  
  return return_val;
}  

CosPropertyService::PropertySetDef_ptr 
TAO_PropertySetDefFactory::create_constrained_propertysetdef (const CosPropertyService::PropertyTypes &allowed_property_types, 
                                                              const CosPropertyService::PropertyDefs &allowed_property_defs, 
                                                              CORBA::Environment &env)
{
  CosPropertyService::PropertySetDef_ptr return_val = 0;
  
  return return_val;
}

CosPropertyService::PropertySetDef_ptr  
TAO_PropertySetDefFactory::create_initial_propertysetdef (const CosPropertyService::PropertyDefs &initial_property_defs,
                                                          CORBA::Environment &env)
{
  CosPropertyService::PropertySetDef_ptr return_val = 0;

  return return_val;
}

// Makes default sized hash_table_.

TAO_PropertySet::TAO_PropertySet (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySet Constructor-Hash_Map_Manager created, current_size %d, total_size %d \n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));
}

// Destructor.

TAO_PropertySet::~TAO_PropertySet (void)
{
}

// Function to modify or add a property to the PropertySet alex: Just
// adds the name value to the set... Doesn't check for Typecode
// overwriting, duplicate names, void names etc, yet.  @@ Using Normal
// mode as the default mode of properties, I need to change this based
// on the Initial set of allowed modes, if there is anything like that
// set by the client.

void 
TAO_PropertySet::define_property (const char *property_name,
                                  const CORBA::Any &property_value,
                                  CORBA::Environment &env)
{
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value (property_value,
                                     CosPropertyService::normal); 
  
  ACE_DEBUG ((LM_DEBUG,
              "define_property : property_name %s \n",
              property_name));
  
  int ret = this->hash_table_.bind (hash_key, hash_value);
  
  if (ret == 0)
    ACE_DEBUG ((LM_DEBUG,
                "define_property: retval : %d \n",
                ret));
}

// Defining a sequence of properties 
//
// @@ Need to check for overwriting, duplicate names, void names etc.

void 
TAO_PropertySet::define_properties (const CosPropertyService::Properties &nproperties,
                                    CORBA::Environment &env)
{
  for (int pi = 0; pi < nproperties.length (); pi++) 
    this->define_property (nproperties [pi].property_name.in (),
                           nproperties [pi].property_value,
                           env);
}

// Returns the current number of properties associated with this
// PropertySet.

CORBA::ULong 
TAO_PropertySet::get_number_of_properties ( CORBA::Environment &env)
{
  return this->hash_table_.current_size ();
}

//  Returns all of the property names currently defined in the
//  PropertySet. If the PropertySet contains more than how_many
//  property names, then the remaining property names are put into the
//  PropertyNamesIterator.

void
TAO_PropertySet::get_all_property_names (CORBA::ULong how_many, 
                                         CosPropertyService::PropertyNames_out property_names, 
                                         CosPropertyService::PropertyNamesIterator_out rest,
                                         CORBA::Environment &env)
{
  size_t num_of_properties =
    this->get_number_of_properties (env);
  
  if (num_of_properties == 0)
    // Nothing to do.
    return;

  // Set the length of the property_names appropriately.
  CORBA::ULong sequence_length = 0;

  if (how_many > 0)
    {
      ACE_NEW (property_names,
               CosPropertyService::PropertyNames);
      if (how_many >= num_of_properties)
        sequence_length = num_of_properties;
      else
        sequence_length = how_many;
      property_names->length (sequence_length);
    }

  // Iterate thru names and put them in the property_names.

  CosProperty_Hash_Entry_ptr entry_ptr;
  CosProperty_Hash_Iterator iterator (this->hash_table_);

  for (int ni = 0;
       ni < sequence_length;
       ni++, iterator.advance ())
    if (iterator.next (entry_ptr) != 0)
      property_names [ni] = CORBA::string_dup (entry_ptr->ext_id_.pname_);
  
  // If there are some more properties, put them in the
  // iterator. How?? Make a new PropertSet and use that to create
  // propertyNames Iterator.

  if (num_of_properties > how_many)
    {
      TAO_PropertySet *property_set;

      ACE_NEW (property_set, TAO_PropertySet);

      for (size_t i = how_many;
           i < num_of_properties;
           i++, iterator.advance ())
        if (iterator.next (entry_ptr) != 0) 
          if (property_set->hash_table_.bind (entry_ptr->ext_id_,
                                              entry_ptr->int_id_) < 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Err: get_all_property_names\n"));

      // Make the NamesIterator out of this TAO_PropertySet.
      TAO_PropertyNamesIterator *names_iterator;
      ACE_NEW (names_iterator, TAO_PropertyNamesIterator (*property_set));

      rest = CosPropertyService::PropertyNamesIterator::_duplicate (names_iterator->_this (env));
    }
}      

// Returns the value of a property in the PropertySet.

CORBA::Any * 
TAO_PropertySet::get_property_value (const char *property_name,
                                     CORBA::Environment &env)
{
  CosProperty_Hash_Key hash_key (CORBA::string_copy (property_name));
  CosProperty_Hash_Value hash_value;
  
  ACE_DEBUG ((LM_DEBUG,
              "get_prop_value: hash_key.pname_.in () : %s \n",
              hash_key.pname_.in ()));
  if (this->hash_table_.find (hash_key, hash_value) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Find failed\n"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Find succeeded \n"));
  return hash_value.pvalue_._retn ();
}

// Returns the values of the properties listed in property_names. When
// the boolean flag is true, the Properties parameter contains valid
// values for all requested property names. If false, then all
// properties with a value of type tk_void may have failed due to
// PropertyNotFound or InvalidPropertyName.

CORBA::Boolean
TAO_PropertySet::get_properties (const CosPropertyService::PropertyNames &property_names,
                                 CosPropertyService::Properties_out nproperties,
                                 CORBA::Environment &env)
{
  size_t n = property_names.length ();
  
  if (n == 0)
    return CORBA::B_FALSE;

  CORBA::Any_ptr any_ptr = 0;
  
  ACE_NEW_RETURN (nproperties,
                  CosPropertyService::Properties,
                  CORBA::B_FALSE);
  
  nproperties->length (n);

  CORBA::Boolean ret_val = CORBA::B_TRUE;

  for (size_t i = 0;  i < n; i++)
    {
      any_ptr = get_property_value (property_names [i], env);

      if (any_ptr != 0)
        {
          // Property name is valid.
          nproperties [i].property_name =
            CORBA::string_dup (property_names [i]);
          nproperties [i].property_value = *any_ptr;
        }
      else 
        { 
          // Invalid name. Ret value is False. 
          ret_val = CORBA::B_FALSE;
          nproperties [i].property_name =
            CORBA::string_dup (property_names [i]);
          
          // @@ Make any value with tk_void type. Using replace
          // method, <<= operator doesnot exist yet for this.
          nproperties [i].property_value.replace (CORBA::_tc_void,
                                                  0,
                                                  CORBA::B_FALSE,                                   
                                                  env);
        }
    }
  TAO_CHECK_ENV_RETURN (env, CORBA::B_FALSE);
  return ret_val;
}

void 
TAO_PropertySet::get_all_properties (CORBA::ULong how_many,
                                     CosPropertyService::Properties_out nproperties, 
                                     CosPropertyService::PropertiesIterator_out rest,
                                     CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "\nDB:TAO_PropertySet::get_all_properties\n"));
  size_t num_of_properties =
    hash_table_.current_size ();
  
  if (num_of_properties == 0)
    return;

  // Alloc memory for nproperties if how_many > 0.
  CORBA::ULong sequence_length = 0;

  if (how_many > 0)
    {
      ACE_NEW (nproperties,
               CosPropertyService::Properties);
      if (how_many >= num_of_properties)
        sequence_length = num_of_properties;
      else
        sequence_length = how_many;
      nproperties->length (sequence_length);
    }
  ACE_DEBUG ((LM_DEBUG,
              "PropertySet::get_all_properties -seq-length :%d\n",
              sequence_length));

  // Prepare an iterator and iterate thru the PropertySet. Retrive the
  // values.
  CosProperty_Hash_Iterator iterator (this->hash_table_);
  CosProperty_Hash_Entry_ptr entry_ptr = 0;
  
  for (CORBA::ULong i = 0;
       i < sequence_length;
       i++, iterator.advance ()) 
    if (iterator.next (entry_ptr) != 0)
      {
        nproperties[i].property_name =
          CORBA::string_dup (entry_ptr->ext_id_.pname_);
        nproperties[i].property_value =
          entry_ptr->int_id_.pvalue_.in ();
      }
  
  // If there are more properties, put them in the PropertiesIterator.
  // Make a new TAO_PropertySet and use that to create an Properties
  // iterator.  put that in a iterator and assign that to the out
  // paramerter.

  if (num_of_properties > how_many) 
    {
      ACE_DEBUG ((LM_DEBUG,
                  "DB:TAO_PropertySet-Creating PropertySet iterator\n"));
      TAO_PropertySet *prop_set;
      
      ACE_NEW (prop_set, TAO_PropertySet);
      
      for (size_t i = sequence_length;
           i < num_of_properties;
           i++, iterator.advance ())
        {
          if (iterator.next (entry_ptr) != 0) 
            if (prop_set->hash_table_.bind (entry_ptr->ext_id_,
                                            entry_ptr->int_id_) < 0)
              ACE_DEBUG ((LM_DEBUG, "Err: get_all_properties\n"));
        }
      ACE_DEBUG ((LM_DEBUG,
                  "DB:New PropertySet created--size %d\n",
                  prop_set->get_number_of_properties (env)));
      
      // Make the iterator out of the new TAO_Propset.
      TAO_PropertiesIterator *iterator = 0;
      ACE_NEW_RETURN (iterator,
                      TAO_PropertiesIterator (*prop_set),
                      );
      
      rest = CosPropertyService::PropertiesIterator::_duplicate (iterator->_this (env));
      TAO_CHECK_ENV_RETURN (env, );

      ACE_DEBUG ((LM_DEBUG, "DB:TAO_PropSet::get_all_properties-Done\n"));
    }
}
  
// Deletes the specified property if it exists from a PropertySet.

void 
TAO_PropertySet::delete_property (const char *property_name,
                                  CORBA::Environment &env)
{
  CosProperty_Hash_Key hash_key (CORBA::string_copy (property_name));
  
  ACE_DEBUG ((LM_DEBUG, "delete_property : hash_key.pname_.in() %s \n",
              hash_key.pname_.in()));
  // Doing unbinding.. Not getting CosProperty_Hash_Value
  // back... Problem if dynamic allocation is done.  @@ Is memory
  // deallocation done?
  if (this->hash_table_.unbind (hash_key) != 0)
    ACE_ERROR ((LM_ERROR,
                "Unbind failed \n"));
}

// Deletes the properties defined in the property_names
// parameter. This is a batch operation that returns the
// MultipleExceptions exception if any delete failed.

void 
TAO_PropertySet::delete_properties (const CosPropertyService::PropertyNames &property_names,
                                    CORBA::Environment &env)
{
  for (size_t pi = 0; pi < property_names.length (); pi++)
    this->delete_property (property_names[pi], env);
}

// Delete all the properties in the current ProperySet : Do it one by
// one. close () and then open ().

CORBA::Boolean
TAO_PropertySet::delete_all_properties (CORBA::Environment &env)
{
  if (hash_table_.close () != 0) 
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "delete_all_properties:Error Closing the Hash_Map_Manager"),
                      CORBA::B_FALSE);
  else if (hash_table_.open () != 0) 
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "Could not open Hash_Map_Manager"),
                       CORBA::B_FALSE);
  else
    return CORBA::B_TRUE;
}  

// Returns TRUE if the property is defined in the PropertySet.

CORBA::Boolean
TAO_PropertySet::is_property_defined (const char *property_name,  CORBA::Environment &env)
{
  CosProperty_Hash_Key hash_key (property_name);
  
  if (this->hash_table_.find (hash_key) == 0) 
    ACE_DEBUG ((LM_DEBUG,
                "is_property_defined :find successfull \n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "is_property_defined :find failed \n"),
                      CORBA::B_FALSE);

  return CORBA::B_TRUE;
} 

// Makes default sized hash_table_.

TAO_PropertySetDef::TAO_PropertySetDef (void)
{
}

// Destructor.

TAO_PropertySetDef::~TAO_PropertySetDef (void)
{
}

void 
TAO_PropertySetDef::get_allowed_property_types (CosPropertyService::PropertyTypes_out property_types,
                                                CORBA::Environment &env)
{
}

void 
TAO_PropertySetDef::get_allowed_properties (CosPropertyService::PropertyDefs_out property_defs,
                                            CORBA::Environment &env)
{
}

// Check for name's validity. If name not there define it. If there
// and if type is equal and if mode allows define it else raise exception.
void 
TAO_PropertySetDef::define_property_with_mode (const char *property_name,
                                               const CORBA::Any &property_value,
                                               CosPropertyService::PropertyModeType property_mode,
                                               CORBA::Environment &env)
{
  // Check the names validity.
  if (property_name == 0)
    {
      env.clear ();
      env.exception (new CosPropertyService::InvalidPropertyName);
      return;
    }
  
  // Try to bind the Property. 
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value (property_value,
                                     property_mode);
  CosProperty_Hash_Entry_ptr entry_ptr;

  ACE_DEBUG ((LM_DEBUG,
              "define_property with mode : property_name %s \n",
              property_name));
  
  int ret = this->hash_table_.bind (hash_key, hash_value, entry_ptr);
  CosProperty_Hash_Value old_value;
  CosProperty_Hash_Key old_key;
  switch (ret)
    {
    case 0: // Bind successful.
      ACE_DEBUG ((LM_DEBUG,
                  "define_property with mode: retval : %d\n",
                  ret));
      return;
    case 1: // Property name exists.
      // Is the pointer valid.
      if (entry_ptr == 0)
        {
          env.clear ();
          env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
          return;
        }
      // If type is not the same, raise exception.
      if (entry_ptr->int_id_.pvalue_->type () != property_value.type ())
        {
          env.clear ();
          env.exception (new CosPropertyService::ConflictingProperty);
          return;
        }
      // If mode is read only, raise exception.
      if ((entry_ptr->int_id_.pmode_ == CosPropertyService::read_only) ||
          (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
        {
          env.clear ();
          env.exception (new CosPropertyService::ReadOnlyProperty);
          return;
        }
      // Everything is fine. Overwrite the value.
      if (this->hash_table_.rebind (hash_key,
                                    hash_value,
                                    old_key,
                                    old_value) > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Property Defined\n"));
          return;
        }
    default:
      {
        env.clear ();
        env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
        return;
      }
    }
}  

// Define one by one. If any excceptions raised, build
// MultipleExceptions sequence and raise that.
void 
TAO_PropertySetDef::define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs,
                                                  CORBA::Environment &env)
{
  //  Get the length, init env.
  size_t sequence_length = property_defs.length ();
  env.clear ();
  
  // Try defining the propdefs one by one.
  for (size_t i = 0; i < sequence_length; i++)
    {
      this->define_property_with_mode (property_defs[i].property_name,
                                       property_defs[i].property_value,
                                       property_defs[i].property_mode,
                                       env);
    }
}

// Get the mode of a property. Raises InvalidpropertyName,
// PropertyNotFound exceptions.
CosPropertyService::PropertyModeType 
TAO_PropertySetDef::get_property_mode (const char *property_name,
                                       CORBA::Environment &env)
{
  // Check for the name's validity.
  if (property_name == 0)
    {
      env.clear ();
      env.exception (new CosPropertyService::InvalidPropertyName);
      return CosPropertyService::undefined;
    }
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
      // Raise exception.
      env.clear ();
      env.exception (new CosPropertyService::PropertyNotFound);
      return CosPropertyService::undefined; 
    }
}

CORBA::Boolean 
TAO_PropertySetDef::get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                        CosPropertyService::PropertyModes_out property_modes,
                                        CORBA::Environment &env)
{
  CORBA::Boolean return_val = CORBA::B_FALSE;

  return return_val;
}  

// Changing the mode of the property.
// "Normal" to anything is possible.
// "Readonly" mode to "Fixed-Readonly" is possible. Others not possible.
// "Fixed-Normal" to "Fixed-Readonly" is possible. Other things are impossible.
// "Fixed-Readonly" to anything is *not* possible.
void 
TAO_PropertySetDef::set_property_mode (const char *property_name,
                                       CosPropertyService::PropertyModeType property_mode, 
                                       CORBA::Environment &env)
{
  // Check the names validity.
  if (property_name == 0)
    {
      env.clear ();
      env.exception (new CosPropertyService::InvalidPropertyName);
      return;
    }
  
  // Trying to set to undefined mode is not allowed.
  if (property_mode == CosPropertyService::undefined) 
    {
      env.clear ();
      env.exception (new CosPropertyService::UnsupportedMode);
      return;
    }
  
  // Find the property from the Hash Table.
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value;
  
  int ret = this->hash_table_.find (hash_key, hash_value);

  CosProperty_Hash_Value old_value;
  CosProperty_Hash_Key old_key;
  switch (ret)
    {
    case 0:
      // Property found.
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
            {
              // Return values 0 and -1 are not possible.
              env.clear ();
              env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
              return;
            }
          ACE_DEBUG ((LM_DEBUG, "Mode set succesful\n"));
          return;
          
        case CosPropertyService::read_only:
          // Read_only to fixed read only alone is possible.
          if (property_mode != CosPropertyService::fixed_readonly) 
            {
              env.clear ();
              env.exception (new CosPropertyService::UnsupportedMode);
              return;
            }
          else 
            {
              // Change the mode and update hash table.
              hash_value.pmode_ = property_mode;
              if (this->hash_table_.rebind (hash_key,
                                            hash_value,
                                            old_key,
                                            old_value) != 1)
                {
                  // Return values 0 and -1 are not possible.
                  env.clear ();
                  env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
                  return;
                }
              ACE_DEBUG ((LM_DEBUG, "Mode set successful\n"));
              return;
            }

        case CosPropertyService::fixed_normal:
          // Fixed_normal to fixed_readonly alone is possible.
          if (property_mode != CosPropertyService::fixed_readonly)
            {
              env.clear ();
              env.exception (new CosPropertyService::UnsupportedMode);
              return;
            }
          else 
            {
              // Change the mode and update the hash table.
              hash_value.pmode_ = property_mode;
              if (this->hash_table_.rebind (hash_key,
                                            hash_value,
                                            hash_key,
                                            hash_value) != 1)
                {
                  // Return values 0 and -1 are not possible.
                  env.clear ();
                  env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
                  return;
                }
              ACE_DEBUG ((LM_DEBUG, "Mode set successful\n"));
              return;
            } 

        default:
          // Fixed_readonly to any mode is not possible.
          env.clear ();
          env.exception (new CosPropertyService::UnsupportedMode);
          return;
        }
    case -1:
    default:
      // Property not found in the Hash Table.
      env.clear ();
      env.exception (new CosPropertyService::PropertyNotFound);
      return;
    }
}

void 
TAO_PropertySetDef::set_property_modes (const CosPropertyService::PropertyModes &property_modes,
                                        CORBA::Environment &env)
{  
}

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
TAO_PropertyNamesIterator::reset (CORBA::Environment &env)
{
  this->iterator_ = this->iterator_.map ().begin ();
}

// The next_one operation returns true if an item exists at the
// current position in the iterator with an output parameter of a
// property name. A return of false signifies no more items in the
// iterator.

CORBA::Boolean
TAO_PropertyNamesIterator::next_one (CORBA::String_out property_name,
                                     CORBA::Environment &env)
{
  CosProperty_Hash_Entry_ptr entry_ptr;

  if (this->iterator_.next (entry_ptr) != 0)
    {
      property_name =
        CORBA::string_copy (entry_ptr->ext_id_.pname_);
      this->iterator_.advance ();
      return CORBA::B_TRUE;
    }
  else
    {
      property_name = 0;
      return CORBA::B_FALSE;
    }
}

CORBA::Boolean
TAO_PropertyNamesIterator::next_n (CORBA::ULong how_many,
                                   CosPropertyService::PropertyNames_out property_names, 
                                   CORBA::Environment &env)
{
  CosProperty_Hash_Entry_ptr entry_ptr = 0;

  if (this->iterator_.next (entry_ptr) == 0 || how_many == 0)
    return CORBA::B_FALSE;

  ACE_NEW_RETURN (property_names,
                  CosPropertyService::PropertyNames,
                  CORBA::B_FALSE);

  size_t size = this->iterator_.map ().current_size ();

  property_names->length (how_many <= size ? how_many : size);

  for (size_t ni = 0;
       ni < property_names->length ();
       ni++, this->iterator_.advance ())
    if (this->iterator_.next (entry_ptr) != 0)
      property_names [ni] = CORBA::string_dup (entry_ptr->ext_id_.pname_);

  return CORBA::B_TRUE;
}

void 
TAO_PropertyNamesIterator::destroy (CORBA::Environment &env)
{    
}

TAO_PropertiesIterator::TAO_PropertiesIterator (TAO_PropertySet &property_set)
  : iterator_ (property_set.hash_table_)
{
}

TAO_PropertiesIterator::~TAO_PropertiesIterator (void)
{
}

void 
TAO_PropertiesIterator::reset (CORBA::Environment &env)
{
  this->iterator_ = this->iterator_.map ().begin ();
}

CORBA::Boolean
TAO_PropertiesIterator::next_one (CosPropertyService::Property_out aproperty,
                                  CORBA::Environment &env)
{   
  CosProperty_Hash_Entry_ptr entry_ptr;
  
  if (this->iterator_.next (entry_ptr) != 0)
    {
      aproperty = new CosPropertyService::Property;
      aproperty->property_name = entry_ptr->ext_id_.pname_;
      aproperty->property_value = entry_ptr->int_id_.pvalue_.in ();
      this->iterator_.advance ();
      return CORBA::B_TRUE;
    }
  else
    {
      aproperty = new CosPropertyService::Property;
      return CORBA::B_FALSE;
    }
}  

CORBA::Boolean
TAO_PropertiesIterator::next_n (CORBA::ULong how_many,
                                CosPropertyService::Properties_out nproperties, 
                                CORBA::Environment &env)
{
  CosProperty_Hash_Entry_ptr entry_ptr = 0;

  if (this->iterator_.next (entry_ptr) == 0 || how_many == 0)
    return CORBA::B_FALSE;

  ACE_NEW_RETURN (nproperties,
                  CosPropertyService::Properties,
                  CORBA::B_FALSE);

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
      // @@ Should be an exception.
      break;
  return CORBA::B_TRUE;
}  

void
TAO_PropertiesIterator::destroy (CORBA::Environment &env)
{
}

