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
//    Alexander Babu Arulanthu
//
// ============================================================================

#include "CosPropertyService_i.h"

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

// Makes default sized hash_table_ 
TAO_PropertySet::TAO_PropertySet (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Hash_Map_Manager created::  CurntSize : %d , TotalSize : %d \n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));
}

// Destructor
TAO_PropertySet::~TAO_PropertySet (void)
{
}

// Function to modify or add a property to the PropertySet  
// alex: Just adds the name value to the set... Doesn't check for
// Typecode overwriting, duplicate names, void names etc, yet.
void 
TAO_PropertySet::define_property (const char *property_name,
                                  const CORBA::Any &property_value,
                                  CORBA::Environment &env)
{
  CosProperty_Hash_Key CosProperty_Hash_Key ((CosPropertyService::PropertyName) property_name);
  CosProperty_Hash_Value CosProperty_Hash_Value (property_value);
  
  ACE_DEBUG ((LM_DEBUG,
              "define_property : property_name %s \n",
              property_name));
  
  int ret = this->hash_table_.bind (CosProperty_Hash_Key, CosProperty_Hash_Value);
  
  if (ret == 0)
    ACE_DEBUG ((LM_DEBUG,
                "define_property: retval : %d \n",
                ret));
}

// Defining a sequence of properties ( Need to check for overwriting,
// duplicate names, void names etc
void 
TAO_PropertySet::define_properties (const CosPropertyService::Properties &nproperties,
                                    CORBA::Environment &env)
{
  for (int pi = 0; pi < nproperties.length (); pi++) 
    {
      this->define_property (nproperties [pi].property_name,
                             nproperties [pi].property_value.in (),
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
//  PropertyNamesIterator.  ??Interator not done
void
TAO_PropertySet::get_all_property_names (CORBA::ULong how_many, 
                                         CosPropertyService::PropertyNames_out property_names, 
                                         CosPropertyService::PropertyNamesIterator_out rest,
                                         CORBA::Environment &env)
{
  // Set the Sequence length appropriately
  int num_of_properties = get_number_of_properties (env);
  property_names->length (num_of_properties);
}

// Returns the value of a property in the PropertySet.
CORBA::Any * 
TAO_PropertySet::get_property_value (const char *property_name,
                                     CORBA::Environment &env)
{
  CosProperty_Hash_Key CosProperty_Hash_Key ((CosPropertyService::PropertyName) property_name);
  CosProperty_Hash_Value CosProperty_Hash_Value;
  
  ACE_DEBUG ((LM_DEBUG, " get_prop_value:  Input str : %s, CosProperty_Hash_Key.pname_.in () : %s \n",
              property_name,
              CosProperty_Hash_Key.pname_.in ()));

  if (this->hash_table_.find (CosProperty_Hash_Key, CosProperty_Hash_Value) != 0) 
    ACE_ERROR ((LM_ERROR, "Find failed: \n"));
  
  ACE_DEBUG ( (LM_DEBUG, "find fn over \n") );
  
  return &CosProperty_Hash_Value.pvalue_;
}

// Returns the values of the properties listed in property_names. When
// the boolean flag is true, the Properties parameter contains valid
// values for all requested property names. If false, then all
// properties with a value of type tk_void may have failed due to
// PropertyNotFound or InvalidPropertyName.
// A separate invocation of get_property for each such property name
// is necessary to determine the specific exception or to verify that
// tk_void is the correct any TypeCode for that property name. This
// approach was taken to avoid a complex, hard to program structure to
// carry mixed results. 
CORBA::Boolean
TAO_PropertySet::get_properties (const CosPropertyService::PropertyNames &property_names,
                                 CosPropertyService::Properties_out nproperties,
                                 CORBA::Environment &env)
{
  return CORBA::B_FALSE;
  
}

// Returns all of the property names currently defined in the
// PropertySet. If the PropertySet contains more than how_many
// property names, then the remaining property names are put into the PropertyNamesIterator.
void 
TAO_PropertySet::get_all_properties (CORBA::ULong how_many,
                                     CosPropertyService::Properties_out nproperties, 
                                     CosPropertyService::PropertiesIterator_out rest,
                                     CORBA::Environment &env)
{
  
}

// Deletes the specified property if it exists from a PropertySet.
void 
TAO_PropertySet::delete_property (const char *property_name,
                                  CORBA::Environment &env)
{
  CosProperty_Hash_Key CosProperty_Hash_Key ((CosPropertyService::PropertyName) property_name);
  
  ACE_DEBUG ((LM_DEBUG, "delete_property : property_name %s, CosProperty_Hash_Key.pname_.in() %s \n",
              property_name,
              CosProperty_Hash_Key.pname_.in()));
  // Doing unbinding.. Not getting CosProperty_Hash_Value back... Problem if dynamic allocation is done
  if (this->hash_table_.unbind (CosProperty_Hash_Key) != 0)
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
// one. close () and then open ()
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
      return CORBA::B_TRUE;
    }
  return CORBA::B_TRUE;
}  

// Returns TRUE if the property is defined in the PropertySet.
CORBA::Boolean
TAO_PropertySet::is_property_defined (const char *property_name,  CORBA::Environment &env)
{
  CosProperty_Hash_Key CosProperty_Hash_Key ((CosPropertyService::PropertyName) property_name);
  
  if (this->hash_table_.find (CosProperty_Hash_Key) == 0) 
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

TAO_PropertyNamesIterator::TAO_PropertyNamesIterator (void)
{
}  

TAO_PropertyNamesIterator::~TAO_PropertyNamesIterator (void)
{
}

void 
TAO_PropertyNamesIterator::reset (CORBA::Environment &env)
{
}

CORBA::Boolean
TAO_PropertyNamesIterator::next_one (CORBA::String_out property_name,
                                     CORBA::Environment &env)
{
  CORBA::Boolean return_val = CORBA::B_FALSE;

  return return_val;
}

CORBA::Boolean
TAO_PropertyNamesIterator::next_n (CORBA::ULong how_many,
                                   CosPropertyService::PropertyNames_out property_names, 
                                   CORBA::Environment &env)
{
  CORBA::Boolean return_val = CORBA::B_FALSE;

  return return_val;

}

void 
TAO_PropertyNamesIterator::destroy (CORBA::Environment &env)
{    
}

TAO_PropertiesIterator::TAO_PropertiesIterator (void)
{
}

TAO_PropertiesIterator::~TAO_PropertiesIterator (void) 
{
}

void 
TAO_PropertiesIterator::reset (CORBA::Environment &env)
{
}

CORBA::Boolean
TAO_PropertiesIterator::next_one (CosPropertyService::Property_out aproperty,
                                  CORBA::Environment &env)
{    
  CORBA::Boolean return_val = CORBA::B_FALSE;

  return return_val;
}  

CORBA::Boolean
TAO_PropertiesIterator::next_n (CORBA::ULong how_many,
                                CosPropertyService::Properties_out nproperties, 
                                CORBA::Environment &env)
{
  CORBA::Boolean return_val = CORBA::B_FALSE;
  
  return return_val;
}

void
TAO_PropertiesIterator::destroy (CORBA::Environment &env)
{
}


