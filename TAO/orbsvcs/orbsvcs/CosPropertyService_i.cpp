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
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
//
// ============================================================================

#include "CosPropertyService_i.h"

TAO_PropertySetFactory::TAO_PropertySetFactory (void)
{
}

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_propertyset (CORBA::Environment &env) 
{
  CosPropertyService::PropertySet_ptr return_val;
  
  return return_val;
}

CosPropertyService::PropertySet_ptr
TAO_PropertySetFactory::create_constrained_propertyset (const CosPropertyService::PropertyTypes &allowed_property_types, 
                                                        const CosPropertyService::Properties &allowed_properties, 
                                                        CORBA::Environment &env)
{
  CosPropertyService::PropertySet_ptr return_val;

  return return_val;

}  

CosPropertyService::PropertySet_ptr 
TAO_PropertySetFactory::create_initial_propertyset (const CosPropertyService::Properties &initial_properties, 
                                                    CORBA::Environment &env) 
{
  CosPropertyService::PropertySet_ptr return_val;

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
  CosPropertyService::PropertySetDef_ptr return_val;
  
  return return_val;
}  

CosPropertyService::PropertySetDef_ptr 
TAO_PropertySetDefFactory::create_constrained_propertysetdef (const CosPropertyService::PropertyTypes &allowed_property_types, 
                                                             const CosPropertyService::PropertyDefs &allowed_property_defs, 
                                                             CORBA::Environment &env)
{
  CosPropertyService::PropertySetDef_ptr return_val;
  
  return return_val;
}

CosPropertyService::PropertySetDef_ptr  
TAO_PropertySetDefFactory::create_initial_propertysetdef (const CosPropertyService::PropertyDefs &initial_property_defs,
                                                          CORBA::Environment &env)
{
  CosPropertyService::PropertySetDef_ptr return_val;

  return return_val;
}

// Constructor
// Init the hash_table_ member variable.
// alex : Is it correct to have this const hash Table size ????

TAO_PropertySet::TAO_PropertySet (void)
  : hash_table_ (HASH_TABLE_SIZE)
{
  ACE_DEBUG ((LM_DEBUG,
              "Hash_Map_Manager created::  CurntSize : %d , TotalSize : %d \n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));
}

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
  EXT_ID ext_id ((CosPropertyService::PropertyName) property_name);
  INT_ID int_id (property_value);

  ACE_DEBUG ((LM_DEBUG,
              "define_property : property_name %s \n",
              property_name));

  int ret = this->hash_table_.bind (ext_id, int_id);

  if (ret == 0)
    ACE_DEBUG ((LM_DEBUG,
                "define_property: retval : %d \n",
                ret));
}

void 
TAO_PropertySet::define_properties (const CosPropertyService::Properties &nproperties,
                                    CORBA::Environment &env)
{
}
  
// Returns the current number of properties associated with this
// PropertySet.
CORBA::ULong 
TAO_PropertySet::get_number_of_properties ( CORBA::Environment &env)
{
  return this->hash_table_.current_size ();
}

void 
TAO_PropertySet::get_all_property_names (CORBA::ULong how_many, 
                                         CosPropertyService::PropertyNames_out property_names, 
                                         CosPropertyService::PropertyNamesIterator_out rest,
                                         CORBA::Environment &env)
{  
}

// Returns the value of a property in the PropertySet.

CORBA::Any * 
TAO_PropertySet::get_property_value (const char *property_name,
                                     CORBA::Environment &env)
{
  EXT_ID ext_id ((CosPropertyService::PropertyName) property_name);
  INT_ID int_id;
  
  ACE_DEBUG ((LM_DEBUG, " get_prop_value:  Input str : %s, ext_id.pname_.in () : %s \n",
              property_name,
              ext_id.pname_.in ()));

  if (this->hash_table_.find (ext_id, int_id) != 0)
    ACE_ERROR ((LM_ERROR, "Find failed: \n"));

  ACE_DEBUG ( (LM_DEBUG, "find fn over \n") );
  
  return &int_id.pvalue_;
}
  
CORBA::Boolean
TAO_PropertySet::get_properties (const CosPropertyService::PropertyNames &property_names,
                                 CosPropertyService::Properties_out nproperties,
                                 CORBA::Environment &env)
{
  CORBA::Boolean return_val;

  return return_val;
}
 
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
  EXT_ID ext_id ((CosPropertyService::PropertyName) property_name);
  
  ACE_DEBUG ((LM_DEBUG, "delete_property : property_name %s, ext_id.pname_.in() %s \n", property_name,
              ext_id.pname_.in()));
  
  // alex: Doing unbinding.. Not getting INT_ID back... Problem if dynamic allocation is done
  if (this->hash_table_.unbind (ext_id) != 0)
    ACE_ERROR ((LM_ERROR,
                "Unbind failed \n"));
}
  
void 
TAO_PropertySet::delete_properties (const CosPropertyService::PropertyNames &property_names,
                                    CORBA::Environment &env)
{
}

CORBA::Boolean
TAO_PropertySet::delete_all_properties (CORBA::Environment &env)
{
  CORBA::Boolean return_val;

  return return_val;
}  

// Returns TRUE if the property is defined in the PropertySet.

CORBA::Boolean
TAO_PropertySet::is_property_defined (const char *property_name,  CORBA::Environment &env)
{
  EXT_ID ext_id ((CosPropertyService::PropertyName) property_name);
 
  if (this->hash_table_.find (ext_id) == 0)
    return CORBA::B_TRUE;
  else
    return CORBA::B_FALSE;
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
  CosPropertyService::PropertyModeType return_val;

  return return_val;
}

CORBA::Boolean 
TAO_PropertySetDef::get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                        CosPropertyService::PropertyModes_out property_modes,
                                        CORBA::Environment &env)
{
  CORBA::Boolean return_val;

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
  CORBA::Boolean return_val;

  return return_val;
}

CORBA::Boolean
TAO_PropertyNamesIterator::next_n (CORBA::ULong how_many,
                                   CosPropertyService::PropertyNames_out property_names, 
                                   CORBA::Environment &env)
{
  CORBA::Boolean return_val;

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
  CORBA::Boolean return_val;

  return return_val;
}  

CORBA::Boolean
TAO_PropertiesIterator::next_n (CORBA::ULong how_many,
                                CosPropertyService::Properties_out nproperties, 
                                CORBA::Environment &env)
{
  CORBA::Boolean return_val;
  
  return return_val;
}

void
TAO_PropertiesIterator::destroy (CORBA::Environment &env)
{
}


