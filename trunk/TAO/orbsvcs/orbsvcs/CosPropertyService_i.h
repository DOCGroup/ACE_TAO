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
// 
// ============================================================================

#if !defined (COSPROPERTYSERVICE_I_H)
#define	COSPROPERTYSERVICE_I_H

#include "orbsvcs/CosPropertyServiceS.h"










class TAO_PropertySetFactory : public POA_CosPropertyService::PropertySetFactory 
{  
 
public:  
  TAO_PropertySetFactory(void);
  
  virtual CosPropertyService::PropertySet_ptr create_propertyset (CORBA::Environment &env);
  
  virtual CosPropertyService::PropertySet_ptr 
    create_constrained_propertyset (CosPropertyService::PropertyTypes &allowed_property_types,
                                    CosPropertyService::Properties  &allowed_properties,
                                    CORBA::Environment &env);
                                   
  virtual CosPropertyService::PropertySet_ptr
    create_initial_propertyset (CosPropertyService::Properties &initial_properties,
                                CORBA::Environment &env);

  ~TAO_PropertySetFactory(void);
};

class TAO_PropertySetDefFactory : public POA_CosPropertyService::PropertySetDefFactory
{
  
public:
  TAO_PropertySetDefFactory(void);    
  
  virtual CosPropertyService::PropertySetDef_ptr create_propertysetdef (CORBA::Environment &env); 

  virtual CosPropertyService::PropertySetDef_ptr 
    create_constrained_propertysetdef (CosPropertyService::PropertyTypes &allowed_property_types,
                                       CosPropertyService::PropertyDefs  &allowed_property_defs,
                                       CORBA::Environment &env);
  
  virtual CosPropertyService::PropertySetDef_ptr 
    create_initial_propertysetdef (CosPropertyService::PropertyDefs &initial_property_defs,
                                   CORBA::Environment &env);

  ~TAO_PropertySetDefFactory(void);
};  
 
class TAO_PropertySet : POA_CosPropertyService::PropertySet  
{
  /* Support for defining and modifying properties */
  void define_property (const CosPropertyService::PropertyName &property_name,
                        const any &property_value,
                        CORBA::Environment &env);

  void define_properties (const CosPropertyService::Properties  &nproperties,
                          CORBA::Environment &env);
    
  /* Support for Getting Properties and their Names */
  CORBA::ULong get_number_of_properties(CORBA::Environment &env);

  void get_all_property_names (CORBA::ULong how_many,
                              CosPropertyService::PropertyNames *&property_names,
                              CosPropertyService::PropertyNamesIterator *&rest,
                              CORBA::Environment &env);

  any *get_property_value (CosPropertyService::PropertyName &property_name,
                           CORBA::Environment &env);

  CORBA::Boolean get_properties (CosPropertyService::PropertyNames *&property_names,
                                 CosPropertyService::Properties *&nproperties,
                                 CORBA::Environment &env);

  void get_all_properties(CORBA::ULong how_many,
                          CosPropertyService::Properties *&nproperties,
                          CosPropertyService::PropertiesIterator *&rest,
                          CORBA::Environment &env);

  /* Support for Deleting Properties              */
  void delete_property (CosPropertyService::PropertyName  &property_name, 
                        CORBA::Environment &env);

  void delete_properties (CosPropertyService::PropertyNames &property_names, 
                          CORBA::Environment &env);

  CORBA::Boolean delete_all_properties (CORBA::Environment &env);

  /* Support for Existence Check             */
  CORBA::Boolean is_property_defined (const CosPropertyService::PropertyName *property_name,
                                      CORBA::Environment &env);
};

class TAO_PropertySetDef : public TAO_PropertySet
{
  /* Support for retrieval of PropertySet constraints*/
  void get_allowed_property_types (CosPropertyService::PropertyTypes *&property_types,
                                   CORBA::Environment &env);

  void get_allowed_properties (CosPropertyService::PropertyDefs *&property_defs,
                               CORBA::Environment &env);

  /* Support for defining and modifying properties */
  void define_property_with_mode (const CosPropertyService::PropertyName &property_name,
                                  const any &property_value,
                                  const CosPropertyService::PropertyModeType &property_mode,
                                  CORBA::Environment &env);

  void define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs,
                                     CORBA::Environment &env);

  /* Support for Getting and Setting Property Modes */
  PropertyModeType get_property_mode (const CosPropertyService::PropertyName &property_name,
                                      CORBA::Environment &env);

  CORBA::Boolean get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                     CosPropertyService::PropertyModes *&property_modes,
                                     CORBA::Environment &env);

  void set_property_mode (CosPropertyService::PropertyName &property_name,
                          CosPropertyService::PropertyModeType &property_mode, 
                          CORBA::Environment &env);

  void set_property_modes (const CosPropertyService::PropertyModes &property_modes, 
                           CORBA::Environment &env);
};

class TAO_PropertyNamesIterator : POA_CosPropertyService::PropertyNamesIterator

{
  void reset (CORBA::Environment &env);
  
  CORBA::Boolean next_one (CosPropertyService::PropertyName *&property_name, 
                           CORBA::Environment &env);
  
  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosPropertyService::PropertyNames  *&property_names,
                         CORBA::Environment &env);
  void destroy(CORBA::Environment &env);
};
 
class TAO_PropertiesIterator : POA_CosPropertyService::PropertiesIterator 
{
  void    reset (CORBA::Environment &env);
  CORBA::Boolean next_one (CosPropertyService::Property *&aproperty,
                           CORBA::Environment &env);
  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosPropertyService::Properties *&nproperties,
                         CORBA::Environment &env);
  void destroy(CORBA::Environment &env);
};

#endif /* COSPROPERTYSERVICE_I_H */








