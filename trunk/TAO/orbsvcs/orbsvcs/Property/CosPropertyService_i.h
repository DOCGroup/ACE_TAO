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
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef COSPROPERTYSERVICE_I_H
#define COSPROPERTYSERVICE_I_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs_export.h"
#include "orbsvcs/CosPropertyServiceS.h"

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// =  Classes to deal with the ACE_Hash_Map_Manager.

class TAO_ORBSVCS_Export CosProperty_Hash_Key
{
  // = TITLE
  //     Key for the Hash Table. The EXT_ID of the
  //     ACE_Hash_Map_Manager.
public:
  // = Initialization and termination methods.
  CosProperty_Hash_Key (void);
  // Default constructor.

  CosProperty_Hash_Key (const char * &name);
  // Constructor from a const string.

  CosProperty_Hash_Key (const CosPropertyService::PropertyName &name);
  // Costructor that takes PropertyName.

  CosProperty_Hash_Key (const CosProperty_Hash_Key &hash_key);
  // Copy constructor.

  virtual ~CosProperty_Hash_Key (void);
  // Destructor.

  virtual int operator == (const CosProperty_Hash_Key &hash_key) const;
  // The operator for hash binding and "find"ing.

  virtual u_long hash (void) const;
  // The function that computes a hash value.

  CosPropertyService::PropertyName_var pname_;
  // This is still the public variable.
};

class TAO_ORBSVCS_Export CosProperty_Hash_Value
{
  // = TITLE
  //     This will be the value part in the Hash_Table. The INT_ID of
  //     the ACE_Hash_Map_Manager.
public:
  // = Initialization and termination methods.

  CosProperty_Hash_Value (void);
  // Default constructor.

  CosProperty_Hash_Value (const CORBA::Any &any,
                          const CosPropertyService::PropertyModeType &mode);
  // Constructor with the property_value and the property_mode.

  CosProperty_Hash_Value (const CosProperty_Hash_Value &hash_value);
  // Copy constructor.

  virtual ~CosProperty_Hash_Value (void);
  // Destructor.

  CORBA::Any_var pvalue_;
  // property value.

  CosPropertyService::PropertyModeType pmode_;
  // Property Mode.
};


// Include templates here.
#include "CosPropertyService_i_T.h"

class TAO_ORBSVCS_Export TAO_PropertySetDef : public virtual POA_CosPropertyService::PropertySetDef,
                                              public virtual TAO_PropertySet<POA_CosPropertyService::PropertySetDef>
{
  // = TITLE
  //     This class implements PropertySetDef interface, which is
  //     specialization (subclass) of PropertySet interface. This
  //     class provides operations to PropertySet constraints, define
  //     and modify with modes, and to get or set property modes.
  //
  // = DESCRIPTION
  //     It should be noted that a TAO_PropertySetDef is still considered
  //     a PropertySet. The specialization operations are simply to
  //     provide more client access and control of the characteristics
  //     (metadata) of a PropertySet.
public:
  // = Initialization and termination methods.
  TAO_PropertySetDef (void);
  // Constructor.

  TAO_PropertySetDef (const CosPropertyService::PropertyTypes allowed_property_types,
                      const CosPropertyService::PropertyDefs allowed_property,
                      CORBA::Environment &_env);
  // The factory uses this constructor.

  TAO_PropertySetDef (const CosPropertyService::PropertyDefs initial_property_defs,
                      CORBA::Environment &_env);
  // This is also for the factory.

  virtual ~TAO_PropertySetDef (void);
  // Destructor.

  virtual void get_allowed_property_types (CosPropertyService::PropertyTypes_out property_types,
                                           CORBA::Environment &env);
  // Indicates which types of properties are supported by this
  // PropertySet. If the output sequence is empty, then there is no
  // restrictions on the any TypeCode portion of the property_value
  // field of a Property in this PropertySet, unless the
  // get_allowed_properties output sequence is not empty.

  virtual void get_allowed_properties (CosPropertyService::PropertyDefs_out property_defs,
                                       CORBA::Environment &env);
  // Indicates which properties are supported by this PropertySet. If
  // the output sequence is empty, then there is no restrictions on
  // the properties that can be in this PropertySet, unless the
  // get_allowed_property_types output sequence is not empty.

  virtual void define_property_with_mode (const char *property_name,
                                          const CORBA::Any &property_value,
                                          CosPropertyService::PropertyModeType property_mode,
                                          CORBA::Environment &env);
  // This operation will modify or add a property to the
  // PropertySet. If the property already exists, then the property
  // type is checked before the value is overwritten. The property
  // mode is also checked to be sure a new value may be written. If
  // the property does not exist, then the property is added to the
  // PropertySet. If type or mode is violated, ConflictingProperty
  // exception is thrown.

  virtual void define_properties_with_modes (const CosPropertyService::PropertyDefs &property_defs,
                                             CORBA::Environment &env);
  // This operation will modify or add each of the properties in the
  // Properties parameter to the PropertySet.

  virtual CosPropertyService::PropertyModeType get_property_mode (const char *property_name,
                                                                  CORBA::Environment &env);
  // Get the mode of a property. Raises InvalidpropertyName,
  // PropertyNotFound exceptions.

  virtual CORBA::Boolean get_property_modes (const CosPropertyService::PropertyNames &property_names,
                                             CosPropertyService::PropertyModes_out property_modes,
                                             CORBA::Environment &env);
  // Batch operation for getting the property. Invoke
  // get_property_mode for each name.  Return value False indicates
  // that properties with *undefined* modes have failed due to
  // PropertyNotFound or InvalidPropertyName exception.  Returning
  // False in case of *Nothing to retun* or New is failing. The caller
  // has to check the out parameter whether it is Nil or no, before
  // doing something with it.

  virtual void set_property_mode (const char *property_name,
                                  CosPropertyService::PropertyModeType property_mode,
                                  CORBA::Environment &env);
  // Set the mode of a property. Watch the following. The change of
  // mode is allowed introduce more constraints, but it should not
  // relax the constraints. The following decisions have been made, in
  // TAO's implementation. The Property Spec has left this to the
  // implenters.  "Normal" to anything is possible.  "Readonly" mode
  // to "Fixed-Readonly" is possible. Others not possible.
  // "Fixed-Normal" to "Fixed-Readonly" is possible. Other things are
  // impossible.  "Fixed-Readonly" to anything is *not* possible.  For
  // all illegal set_mode attempts, UnsupportedMode exception is
  // raised.

  virtual void set_property_modes (const CosPropertyService::PropertyModes &property_modes,
                                   CORBA::Environment &env);
  // Batch operation for setting the property. Raises
  // MultipleExceptions.
};

class TAO_ORBSVCS_Export TAO_PropertySetFactory :  public virtual POA_CosPropertyService::PropertySetFactory
{
  // = TITLE
  //     Factory class for the TAO_PropertySet class objects.
  //
  // = DESCRIPTION
  //     The object may be created with some predfined properties.
public:
  // = Initialization and termination methods.

  TAO_PropertySetFactory (void);
  // Constructor.

  virtual ~TAO_PropertySetFactory (void);
  // Destructor.

  virtual CosPropertyService::PropertySet_ptr
  create_propertyset (CORBA::Environment &env);
  // Returns a  new TAO_PropertySet object. "The property set returned
  // will *not* have any initial properties."

  virtual CosPropertyService::PropertySet_ptr
  create_constrained_propertyset (const CosPropertyService::PropertyTypes &allowed_property_types,
                                  const CosPropertyService::Properties &allowed_properties,
                                  CORBA::Environment &env);
  // Allows a client to create a new TAO_PropertySet with specific
  // constraints. "All the properties will have *fixed-normal* modes".

  virtual CosPropertyService::PropertySet_ptr
  create_initial_propertyset (const CosPropertyService::Properties &initial_properties,
                              CORBA::Environment &env);
  // Allows a client to create a new TAO_PropertySet with specific
  // initial properties."All the properties will have *fixed-normal"
  // modes".

private:
  TAO_Unbounded_Sequence<TAO_PropertySet<POA_CosPropertyService::PropertySet>*> propertyset_products_;
  // The PropertySet objects new'ed and given to the client. Let us
  // keep track all of them so that we can delete them at the end.
};

class TAO_ORBSVCS_Export TAO_PropertySetDefFactory : public virtual POA_CosPropertyService::PropertySetDefFactory
{
  // = TITLE
  //     Factory class for the TAO_PropertySetDef objects.
  //
  // = DESCRIPTION
  //     The object creation may be done with some predefined properties.
public:
  // = Initialization and termination methods.

  TAO_PropertySetDefFactory(void);
  // Constructor.

  virtual ~TAO_PropertySetDefFactory (void);
  // Destructor.

  virtual CosPropertyService::PropertySetDef_ptr create_propertysetdef (CORBA::Environment &env);
  // Returns a new TAO_PropertySetDef object.

  virtual CosPropertyService::PropertySetDef_ptr
  create_constrained_propertysetdef (const CosPropertyService::PropertyTypes &allowed_property_types,
                                     const CosPropertyService::PropertyDefs &allowed_property_defs,
                                     CORBA::Environment &env);
  // Allows a client to create a new TAO_PropertySetDef with specific
  // constraints.

  virtual CosPropertyService::PropertySetDef_ptr
  create_initial_propertysetdef (const CosPropertyService::PropertyDefs &initial_property_defs,
                                 CORBA::Environment &env);
  // Allows a client to create a new TAO_PropertySetDef with specific
  // initial properties.

private:
  TAO_Unbounded_Sequence<TAO_PropertySetDef*> propertysetdef_products_;
  // The PropertySet objects new'ed and given to the client. Let us
  // keep track all of them so that we can delete them at the end.
};

class TAO_ORBSVCS_Export TAO_PropertyNamesIterator :  public virtual POA_CosPropertyService::PropertyNamesIterator
{
  // = TITLE
  //     The PropertyNamesIterator interface allows a client to
  //     iterate through the names using the next_one or next_n operations.
  //
  // = DESCRIPTION
  //     A PropertySet maintains a set of name-value pairs. The
  //     get_all_property_names operation returns a sequence of names
  //     (PropertyNames). If there are additional names, the
  //     get_all_property_names operation returns an object supporting
  //     the PropertyNamesIterator interface with the additional names.
public:
  // = Initialization and termination methods.
  TAO_PropertyNamesIterator (TAO_PropertySet<POA_CosPropertyService::PropertySet> &property_set);
  // Constructor.

  virtual ~TAO_PropertyNamesIterator (void);
  // Destructor.

  virtual void reset (CORBA::Environment &env);
  // The reset operation resets the position in an iterator to the
  // first property name, if one exists.

  virtual CORBA::Boolean next_one (CORBA::String_out property_name,
                                   CORBA::Environment &env);
  // The next_one operation returns true if an item exists at the
  // current position in the iterator with an output parameter of a
  // property name. A return of false signifies no more items in the iterator.


  virtual CORBA::Boolean next_n (CORBA::ULong how_many,
                                 CosPropertyService::PropertyNames_out property_names,
                                 CORBA::Environment &env);
  // The next_n operation returns true if an item exists at the
  // current position in the iterator and the how_many parameter was
  // set greater than zero. The output is a PropertyNames sequence
  // with at most the how_many number of names. A return of false
  // signifies no more items in the iterator.

  virtual void destroy (CORBA::Environment &env);
  // Destroys the iterator.
private:
  typedef ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
          CosProperty_Hash_Map;
  typedef ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
          CosProperty_Hash_Iterator;
  typedef ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>
          CosProperty_Hash_Entry;
  typedef CosProperty_Hash_Entry * CosProperty_Hash_Entry_ptr;

  CosProperty_Hash_Iterator iterator_;
  // The Iterator object.
};

class TAO_ORBSVCS_Export TAO_PropertiesIterator :  public virtual POA_CosPropertyService::PropertiesIterator
{
  // = TITLE
  //     Thid class implements PropertiesIterator interface allows a client to
  //     iterate through the name-value pairs using the next_one or
  //     next_n operations.
  //
  // = DESCRIPTION
  //     A PropertySet maintains a set of name-value pairs. The
  //     get_all_properties operation of the PropertySet interface
  //     returns a sequence of Property structures (Properties). If
  //     there are additional properties, the get_all_properties
  //     operation returns an object supporting the PropertiesIterator
  //     interface with the additional properties.
public:
  // = Initialization and termination methods.
  TAO_PropertiesIterator (TAO_PropertySet<POA_CosPropertyService::PropertySet> &property_set);
  // Constructor.

  virtual ~TAO_PropertiesIterator (void);
  // Destructor.

  virtual void reset (CORBA::Environment &env);
  // Resets the position in an iterator to the first property, if one exists.

  virtual CORBA::Boolean next_one (CosPropertyService::Property_out aproperty,
                                   CORBA::Environment &env);
  // The next_one operation returns true if an item exists at the
  // current position in the iterator with an output parameter of a
  // property. A return of false signifies no more items in the
  // iterator.

  virtual CORBA::Boolean next_n (CORBA::ULong how_many,
                                 CosPropertyService::Properties_out nproperties,
                                 CORBA::Environment &env);
  // The next_n operation returns true if an item exists at the
  // current position in the iterator and the how_many parameter was
  // set greater than zero. The output is a properties sequence with
  // at most the how_many number of properties. A return of false
  // signifies no more items in the iterator.

  virtual void destroy (CORBA::Environment &env);
  // Destroys the iterator.

private:
  typedef ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
          CosProperty_Hash_Map;
  typedef ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>
          CosProperty_Hash_Iterator;
  typedef ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>
          CosProperty_Hash_Entry;
  typedef CosProperty_Hash_Entry * CosProperty_Hash_Entry_ptr;

  CosProperty_Hash_Iterator iterator_;
  // The iterator object.
};

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

#endif /* COSPROPERTYSERVICE_I_H */
