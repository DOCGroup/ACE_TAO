// $Id$

#ifndef TAO_COSPROPERTYSERVICE_I_T_H
#define TAO_COSPROPERTYSERVICE_I_T_H

// = Implementation classes for the PropertyService interfaces.
template <class IF>
class TAO_ORBSVCS_Export TAO_PropertySet :  public virtual IF
{
  // = TITLE
  //    Gives operations for defining, deleting, enumerating and
  //    checking of properties.
  //
  // = DESCRIPTION
  //     Uses a HashTable to manage the properties.
public:
  friend class TAO_PropertyNamesIterator;
  friend class TAO_PropertiesIterator;

  // = Initialization and termination methods.

  TAO_PropertySet (void);
  // Default constructor.

  TAO_PropertySet (const CosPropertyService::PropertyTypes allowed_property_types,
                   const CosPropertyService::Properties allowed_properties,
                   CORBA::Environment &_env);
  // Init values that the PropertySetFactory will want to specify.

  TAO_PropertySet (const CosPropertyService::Properties initial_properties,
                   CORBA::Environment &_env);
  // PropertySetFactory needs this constructor.

  TAO_PropertySet (const CosPropertyService::PropertyTypes allowed_property_types,
                   const CORBA::ULong number_of_allowed_propertydefs,
                   CORBA::Environment &_env);
  // PropertySetDef's construction needs this.

  virtual ~TAO_PropertySet (void);
  // Destructor function.

  virtual void define_property (const char *property_name,
                                const CORBA::Any &property_value,
                                CORBA::Environment &env);
  // Store the property in the hash after checking for validity of the
  // property name, duplicate name, type code over writing etc.

  virtual void define_properties (const CosPropertyService::Properties &nproperties,
                                  CORBA::Environment &env);
  // Define a sequence of properties at a time.

  virtual CORBA::ULong get_number_of_properties (CORBA::Environment &env);
  // Get the number of properties that are currently defined in the
  // PropertySet.

  virtual void get_all_property_names (CORBA::ULong how_many,
                                       CosPropertyService::PropertyNames_out property_names,
                                       CosPropertyService::PropertyNamesIterator_out rest,
                                       CORBA::Environment &env);
  // Get the names of all the properties that are currently defined in
  // the property set.

  virtual CORBA::Any *get_property_value (const char *property_name,
                                          CORBA::Environment &env);
  // Get the value of the property, given the name.

  virtual CORBA::Boolean get_properties (const CosPropertyService::PropertyNames &property_names,
                                         CosPropertyService::Properties_out nproperties,
                                         CORBA::Environment &env);
  // Get all names and their property values.

  virtual void get_all_properties (CORBA::ULong how_many,
                                   CosPropertyService::Properties_out nproperties,
                                   CosPropertyService::PropertiesIterator_out rest,
                                   CORBA::Environment &env);
  // Returns all of the property names currently defined in the
  // PropertySet. If the PropertySet contains more than how_many
  // property names, then the remaining property names are put into the PropertyNamesIterator.

  virtual void delete_property (const char *property_name,
                                CORBA::Environment &env);
  // Delete a property given a name.

  virtual void delete_properties (const CosPropertyService::PropertyNames &property_names,
                                  CORBA::Environment &env);
  // Delete all the these properties from this property set.

  virtual CORBA::Boolean delete_all_properties (CORBA::Environment &env);
  // Delete everything from this property set.

  virtual CORBA::Boolean is_property_defined (const char *property_name,
                                              CORBA::Environment &env);
  // Tell whether this property is defined or no. Forget about the
  // value.

protected:
  typedef ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
          COSPROPERTY_HASH_MAP;
  typedef ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>
          COSPROPERTY_HASH_ENTRY;
  typedef ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
          COSPROPERTY_HASH_ITERATOR;
  // Typedefs are useful.

  CORBA::Boolean is_type_allowed (CORBA::TypeCode_ptr type);
  // Tells whether this type is allowed in this property set or no.

  CORBA::Boolean is_property_allowed (const char *name);
  // Tells  whether this property is allowed in this property set or
  // no.

  void operator= (const TAO_PropertySet<IF> &);
  // hack to avoid strange undefined symbol errors for operator= for PropertyTypes.

  // XXX:This public: is a hack to keep the compiler complain about access violation.
public:
  COSPROPERTY_HASH_MAP hash_table_;
  // This Hash_Table manages storage for our properties.

protected:
  CosPropertyService::PropertyTypes allowed_property_types_;
  // Stores the property types that can be allowed in this property
  // set.

  CosPropertyService::PropertyNames allowed_property_names_;
  // Stores the property names that are allowed in this property
  // set. These properties will be defined with *fixed-normal* modes, by default, at
  // creation.
  // If this is *not* empty, these properties will be the only
  // properties that will exist in this property set. Nothing more
  // can be defined.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "CosPropertyService_i_T.cpp"
#endif /*ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CosPropertyService_i_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_COSPROPERTYSERVICE_I_T_H */
