// $Id$
// 
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
TAO_PropertySetDefFactory::create_propertysetdef ( CORBA::Environment &env)
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
              "TAO_PropertySet Constructor-Hash_Map_Manager created::  CurntSize : %d , TotalSize : %d \n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));
}

// Destructor.
TAO_PropertySet::~TAO_PropertySet (void)
{
}

// Function to modify or add a property to the PropertySet  
// alex: Just adds the name value to the set... Doesn't check for
// Typecode overwriting, duplicate names, void names etc, yet.
// @@ Using  Normal mode as the default mode of properties, I need to
// change this based on the Initial set of allowed modes, if there is
// anything like that set by the client.
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

// Defining a sequence of properties ( Need to check for overwriting,
// duplicate names, void names etc.
void 
TAO_PropertySet::define_properties (const CosPropertyService::Properties &nproperties,
                                    CORBA::Environment &env)
{
  for (int pi = 0; pi < nproperties.length (); pi++) 
    {
      this->define_property (nproperties [pi].property_name.in (),
                             nproperties [pi].property_value,
                             env);
    }
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
  size_t num_of_properties = get_number_of_properties (env);
  if (num_of_properties == 0)
    // Nothing to do.
    return;
  // Set the length of the property_names appropriately.
  CORBA::ULong sequence_length = 0;
  if (how_many > 0)
    {
      property_names = new CosPropertyService::PropertyNames;
      sequence_length = (how_many >= num_of_properties) ? num_of_properties : how_many;
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
      TAO_PropertySet *property_set = new TAO_PropertySet;
      for (size_t i = how_many;
           i < num_of_properties;
           i++, iterator.advance ())
        if (iterator.next (entry_ptr) != 0) 
          if (property_set->hash_table_.bind (entry_ptr->ext_id_,
                                              entry_ptr->int_id_) < 0)
            ACE_DEBUG ((LM_DEBUG, "Err: get_all_property_names\n"));
      // Make the NamesIterator out of this TAO_PropertySet.
      TAO_PropertyNamesIterator *names_iterator = new TAO_PropertyNamesIterator (*property_set);
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
  
  ACE_DEBUG ((LM_DEBUG, "get_prop_value: hash_key.pname_.in () : %s \n",
              hash_key.pname_.in ()));
  if (this->hash_table_.find (hash_key, hash_value) != 0) 
    ACE_ERROR ((LM_ERROR, "Find failed: \n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Find succeeded \n"));
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
  nproperties = new CosPropertyService::Properties;
  nproperties->length (n);
  CORBA::Boolean ret_val = CORBA::B_TRUE;
  for (size_t i = 0;  i < n; i++)
    if ((any_ptr = get_property_value (property_names [i], env)) != 0)
      {
        // Property name is valid.
        nproperties [i].property_name = CORBA::string_dup (property_names [i]);
        nproperties [i].property_value = *any_ptr;
      }
    else 
      { 
        // Invalid name. Ret value is False. 
        ret_val = CORBA::B_FALSE;
        nproperties [i].property_name = CORBA::string_dup (property_names [i]);
        // @@ Make any value with tk_void type. Using replace method,
        // <<= operator doesnot exist yet for this.
        nproperties [i].property_value.replace (CORBA::_tc_void,
                                                0,
                                                CORBA::B_FALSE,
                                                env);
      }
  TAO_CHECK_ENV_RETURN (env, 1);
  return ret_val;
}

void 
TAO_PropertySet::get_all_properties (CORBA::ULong how_many,
                                     CosPropertyService::Properties_out nproperties, 
                                     CosPropertyService::PropertiesIterator_out rest,
                                     CORBA::Environment &env)
{
  size_t num_of_properties = hash_table_.current_size ();
  if (num_of_properties == 0)
    return;
  // Alloc memory for nproperties if how_many > 0.
  CORBA::ULong sequence_length = 0;
  if (how_many > 0)
    {
      nproperties= new CosPropertyService::Properties;
      sequence_length = (how_many >= num_of_properties) ? num_of_properties : how_many;
      nproperties->length (sequence_length);
    }
  // Prepare an iterator and iterate thru the PropertySet. Retrive the
  // values.
  CosProperty_Hash_Iterator iterator (this->hash_table_);
  CosProperty_Hash_Entry_ptr entry_ptr = 0;
  for (CORBA::ULong i = 0;
       i < sequence_length;
       i++, iterator.advance ()) 
    if (iterator.next (entry_ptr) != 0)
      {
        nproperties[i].property_name = CORBA::string_dup (entry_ptr->ext_id_.pname_);
        nproperties[i].property_value = entry_ptr->int_id_.pvalue_.in ();
      }
  
  // If there are more properties, put them in the PropertiesIterator.
  // Make a new TAO_PropertySet and use that to create an Properties
  // iterator.  put that in a iterator and assign that to the out
  // paramerter. 
  if (num_of_properties > how_many) 
    {
      TAO_PropertySet *prop_set = new TAO_PropertySet;
      for (size_t i = how_many; i < num_of_properties; i++, iterator.advance ())
        if (iterator.next (entry_ptr) != 0) 
          if (prop_set->hash_table_.bind (entry_ptr->ext_id_,
                                          entry_ptr->int_id_) < 0)
            ACE_DEBUG ((LM_DEBUG, "Err: get_all_properties\n"));
      // Make the iterator out of the new TAO_Propset.
      TAO_PropertiesIterator *prop_iterator = new TAO_PropertiesIterator (*prop_set);
      rest = CosPropertyService::PropertiesIterator::_duplicate (prop_iterator->_this (env));
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
  // back... Problem if dynamic allocation is done.
  // @@ Is memory deallocation done?
  if (this->hash_table_.unbind (hash_key) != 0)
    ACE_ERROR ((LM_ERROR,
                "Unbind failed \n"));
}

// Deletes the properties defined in the property_names
// parameter. This  is a batch operation that returns the
// MultipleExceptions  exception if any delete failed. 
void 
TAO_PropertySet::delete_properties (const CosPropertyService::PropertyNames &property_names,
                                    CORBA::Environment &env)
{
  for (size_t pi = 0; pi < property_names.length (); pi++)
    delete_property (property_names[pi], env);
}

// Delete all the properties in the current ProperySet : Do it one by
// one. close () and then open ().
CORBA::Boolean
TAO_PropertySet::delete_all_properties (CORBA::Environment &env)
{
  if (hash_table_.close () != 0) 
    {
      ACE_DEBUG ((LM_DEBUG, "delete_all_properties:Error Closing the Hash_Map_Manager"));
      return CORBA::B_FALSE;
    }
  if (hash_table_.open () != 0) 
    {
      ACE_DEBUG ((LM_DEBUG, "Could not open Hash_Map_Manager"));
      return CORBA::B_FALSE;
    }
  return CORBA::B_TRUE;
}  

// Returns TRUE if the property is defined in the PropertySet.
CORBA::Boolean
TAO_PropertySet::is_property_defined (const char *property_name,  CORBA::Environment &env)
{
  CosProperty_Hash_Key hash_key (property_name);
  
  if (this->hash_table_.find (hash_key) == 0) 
    {
      ACE_DEBUG ((LM_DEBUG, "is_property_defined :find successfull \n"));
      return CORBA::B_TRUE;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "is_property_defined :find failed \n"));
      return CORBA::B_FALSE;
    }
}  

TAO_PropertySetDef::TAO_PropertySetDef (void)
{
}

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

void 
TAO_PropertySetDef::define_property_with_mode (const char *property_name,
                                               const CORBA::Any &property_value,
                                               CosPropertyService::PropertyModeType property_mode,
                                               CORBA::Environment &env)
{
}  

void 
TAO_PropertySetDef::define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs,
                                                  CORBA::Environment &env)
{
}

CosPropertyService::PropertyModeType 
TAO_PropertySetDef::get_property_mode (const char *property_name,
                                       CORBA::Environment &env)
{
  CosPropertyService::PropertyModeType return_val = CosPropertyService::undefined;

  return return_val;
}

CORBA::Boolean 
TAO_PropertySetDef::get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                        CosPropertyService::PropertyModes_out property_modes,
                                        CORBA::Environment &env)
{
  CORBA::Boolean return_val = CORBA::B_FALSE;

  return return_val;
}  

void 
TAO_PropertySetDef::set_property_mode (const char *property_name,
                                       CosPropertyService::PropertyModeType property_mode, 
                                       CORBA::Environment &env)
{ 
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
TAO_PropertyNamesIterator::~TAO_PropertyNamesIterator ()
{
}

// Resets the position in an iterator to the first property name, if
// one exists.
void 
TAO_PropertyNamesIterator::reset (CORBA::Environment &env)
{
  this->iterator_.reset ();
}

// The next_one operation returns true if an item exists at the
// current position in the iterator with an output parameter of a
// property name. A return of false signifies no more items in the iterator.
CORBA::Boolean
TAO_PropertyNamesIterator::next_one (CORBA::String_out property_name,
                                     CORBA::Environment &env)
{
  CosProperty_Hash_Entry_ptr entry_ptr;
  if (this->iterator_.next (entry_ptr) != 0)
    {
      property_name = CORBA::string_copy (entry_ptr->ext_id_.pname_);
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
  if ((this->iterator_.next (entry_ptr) == 0) || (how_many == 0))
    return CORBA::B_FALSE;
  property_names = new CosPropertyService::PropertyNames;
  size_t size = this->iterator_.map().current_size ();
  property_names->length ((how_many <= size) ? how_many : size);
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

TAO_PropertiesIterator::~TAO_PropertiesIterator ()
{
}

void 
TAO_PropertiesIterator::reset (CORBA::Environment &env)
{
  this->iterator_.reset ();
}

CORBA::Boolean
TAO_PropertiesIterator::next_one (CosPropertyService::Property_out aproperty,
                                  CORBA::Environment &env)
{   
  CosProperty_Hash_Entry_ptr entry_ptr;
  if (this->iterator_.next (entry_ptr) != 0)
    {
      aproperty = new CosPropertyService::Property ;
      aproperty->property_name = entry_ptr->ext_id_.pname_;
      aproperty->property_value = entry_ptr->int_id_.pvalue_.in ();
      this->iterator_.advance ();
      return CORBA::B_TRUE;
    }
  else
    CORBA::B_FALSE;
}  

CORBA::Boolean
TAO_PropertiesIterator::next_n (CORBA::ULong how_many,
                                CosPropertyService::Properties_out nproperties, 
                                CORBA::Environment &env)
{
  CosProperty_Hash_Entry_ptr entry_ptr = 0;
  if ((this->iterator_.next (entry_ptr) == 0) || (how_many == 0))
    return CORBA::B_FALSE;
  nproperties = new CosPropertyService::Properties;
  size_t size = this->iterator_.map ().current_size ();
  nproperties->length ((how_many <= size) ? how_many : size);
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

