/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   CosPropertyService_i.h
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//     
// ============================================================================

#if !defined (COSPROPERTYSERVICE_I_H)
#define	COSPROPERTYSERVICE_I_H

#include "ace/OS.h"
#include "orbsvcs/CosPropertyServiceS.h"
#include "CosProperty_Hash.h"

class TAO_ORBSVCS_Export TAO_PropertySetFactory :  public virtual POA_CosPropertyService::PropertySetFactory
{
// @@ Alex, can you please add the normal = TITLE and = DESCRIPTION
// comments here?  In addition, can you please comment the various methods.
public:
  TAO_PropertySetFactory (void);
 
  virtual CosPropertyService::PropertySet_ptr 
  create_propertyset (CORBA::Environment &env); 

  virtual CosPropertyService::PropertySet_ptr  
  create_constrained_propertyset (const CosPropertyService::PropertyTypes &allowed_property_types, 
                                  const CosPropertyService::Properties &allowed_properties,
                                  CORBA::Environment &env);

  virtual CosPropertyService::PropertySet_ptr 
  create_initial_propertyset (const CosPropertyService::Properties &initial_properties,
                              CORBA::Environment &env) ;

  virtual ~TAO_PropertySetFactory (void);

};

class TAO_ORBSVCS_Export TAO_PropertySetDefFactory : public virtual POA_CosPropertyService::PropertySetDefFactory  
{
// @@ Alex, can you please add the normal = TITLE and = DESCRIPTION
// comments here?  In addition, can you please comment the various methods.
public:
  TAO_PropertySetDefFactory(void); 

  virtual ~TAO_PropertySetDefFactory (void);

  virtual CosPropertyService::PropertySetDef_ptr create_propertysetdef (CORBA::Environment &env); 

  virtual CosPropertyService::PropertySetDef_ptr 
  create_constrained_propertysetdef (const CosPropertyService::PropertyTypes &allowed_property_types, 
                                     const CosPropertyService::PropertyDefs &allowed_property_defs, 
                                     CORBA::Environment &env);

  virtual CosPropertyService::PropertySetDef_ptr 
  create_initial_propertysetdef (const CosPropertyService::PropertyDefs &initial_property_defs,
                                 CORBA::Environment &env);
};

class TAO_ORBSVCS_Export TAO_PropertySet :  public virtual POA_CosPropertyService::PropertySet  
{    
  // = TITLE
  //    Gives operations for defining, deleting, enumerating and
  //    checking of properties.
  //
  // = DESCRIPTION
  //     Uses a HashTable to manage the properties. 
public:
  typedef ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> CosProperty_Hash_Map;
  
  TAO_PropertySet (void);
  // Default constructor 

  virtual ~TAO_PropertySet (void);
  // Destructor function
  
  virtual void define_property (const char *property_name,
                                const CORBA::Any &property_value,
                                CORBA::Environment &env);
  // Store the property in the hash after checking for validity of the
  // property name, duplicate name, type code over writing etc.

  virtual void define_properties (const CosPropertyService::Properties &nproperties,
                                  CORBA::Environment &env);
  // Define a sequence of properties at a time
    
  virtual CORBA::ULong get_number_of_properties ( CORBA::Environment &env);
  // Get the number of properties that are currently defined in the PropertySet

  virtual void get_all_property_names (CORBA::ULong how_many, 
                                       CosPropertyService::PropertyNames_out property_names, 
                                       CosPropertyService::PropertyNamesIterator_out rest,  CORBA::Environment &env);
  // Get the names of all the properties that are currently defined
  // in the property set.
  
  virtual CORBA::Any *get_property_value (const char *property_name,
                                          CORBA::Environment &env);
  // Get the value of the property, given the name

  virtual CORBA::Boolean get_properties (const CosPropertyService::PropertyNames &property_names,
                                         CosPropertyService::Properties_out nproperties,
                                         CORBA::Environment &env);
  // Get all names and their property values

  virtual void get_all_properties (CORBA::ULong how_many,
                                   CosPropertyService::Properties_out nproperties, 
                                   CosPropertyService::PropertiesIterator_out rest,
                                   CORBA::Environment &env);
  // Get all the property values alone

  virtual void delete_property (const char *property_name,
                                CORBA::Environment &env);
  // Delete a property given a name

  virtual void delete_properties (const CosPropertyService::PropertyNames &property_names,
                                  CORBA::Environment &env);
  // Delete all the these properties from this property set

  virtual CORBA::Boolean delete_all_properties (CORBA::Environment &env);
  // Delete everything from this property set

  virtual CORBA::Boolean is_property_defined (const char *property_name,
                                              CORBA::Environment &env);
  // Tell whether this property is defined or no. Forget about the value

private:
  CosProperty_Hash_Map hash_table_;
  // This Hash_Table is the storage for our properties 
};

class TAO_ORBSVCS_Export TAO_PropertySetDef : public virtual TAO_PropertySet
{
// @@ Alex, can you please add the normal = TITLE and = DESCRIPTION
// comments here?  In addition, can you please comment the various methods.
public:
  TAO_PropertySetDef (void);
  
  virtual ~TAO_PropertySetDef (void);
  
  virtual void get_allowed_property_types (CosPropertyService::PropertyTypes_out property_types,
                                           CORBA::Environment &env);

  virtual void get_allowed_properties (CosPropertyService::PropertyDefs_out property_defs,
                                       CORBA::Environment &env);
  
  virtual void define_property_with_mode (const char *property_name,
                                          const CORBA::Any &property_value,
                                          CosPropertyService::PropertyModeType property_mode,
                                          CORBA::Environment &env);

  virtual void define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs,
                                             CORBA::Environment &env);

  virtual CosPropertyService::PropertyModeType get_property_mode (const char *property_name,
                                                                  CORBA::Environment &env);

  virtual CORBA::Boolean get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                             CosPropertyService::PropertyModes_out property_modes,
                                             CORBA::Environment &env);
  
  virtual void set_property_mode (const char *property_name,
                                  CosPropertyService::PropertyModeType property_mode, 
                                  CORBA::Environment &env);
  
  
  virtual void set_property_modes (const CosPropertyService::PropertyModes &property_modes,
                                   CORBA::Environment &env);
};

class TAO_ORBSVCS_Export TAO_PropertyNamesIterator :  public virtual POA_CosPropertyService::PropertyNamesIterator
{
// @@ Alex, can you please add the normal = TITLE and = DESCRIPTION
// comments here?  In addition, can you please comment the various methods.
public:
  TAO_PropertyNamesIterator (void);

  virtual ~TAO_PropertyNamesIterator (void);

  virtual void reset ( CORBA::Environment &env);
    
  virtual CORBA::Boolean next_one (CORBA::String_out property_name,
                                   CORBA::Environment &env);
    
  virtual CORBA::Boolean next_n (CORBA::ULong how_many,
                                 CosPropertyService::PropertyNames_out property_names, 
                                 CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);
    
};

class TAO_ORBSVCS_Export TAO_PropertiesIterator :  public virtual POA_CosPropertyService::PropertiesIterator
{
// @@ Alex, can you please add the normal = TITLE and = DESCRIPTION
// comments here?  In addition, can you please comment the various methods.
public:
  TAO_PropertiesIterator (void);
  
  virtual ~TAO_PropertiesIterator (void);
    
  virtual void reset (CORBA::Environment &env);

  virtual CORBA::Boolean next_one (CosPropertyService::Property_out aproperty,
                                   CORBA::Environment &env);
    
  virtual CORBA::Boolean next_n (CORBA::ULong how_many,
                                 CosPropertyService::Properties_out nproperties, 
                                 CORBA::Environment &env);

  virtual void destroy (CORBA::Environment &env);
};

#endif /* COSPROPERTYSERVICE_I_H */








