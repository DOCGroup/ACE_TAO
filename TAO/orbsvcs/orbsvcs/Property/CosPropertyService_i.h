// -*- C++ -*-

/**
 *  @file    CosPropertyService_i.h
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */

#ifndef TAO_COSPROPERTYSERVICE_I_H
#define TAO_COSPROPERTYSERVICE_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Property/property_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosPropertyS.h"

#include "tao/Sequence_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// @name ACE_Hash_Map_Manager support classes
/// @{

/// Hash key used as the `EXT_ID` for `ACE_Hash_Map_Manager`.
class TAO_Property_Serv_Export CosProperty_Hash_Key
{
public:
  CosProperty_Hash_Key () = default;

  /// Constructs a key from a string.
  explicit CosProperty_Hash_Key (char const * name);

  /// Constructs a key from a property name.
  explicit CosProperty_Hash_Key (CosPropertyService::PropertyName const & name);

  CosProperty_Hash_Key (CosProperty_Hash_Key const & hash_key);

  CosProperty_Hash_Key &operator= (CosProperty_Hash_Key const &) = default;

  virtual ~CosProperty_Hash_Key () = default;

  /// Compares keys for hash-table lookup.
  virtual bool operator == (CosProperty_Hash_Key const & hash_key) const;

  /// Computes the hash value for this key.
  virtual u_long hash () const;

  /// The property name stored by this key.
  CosPropertyService::PropertyName_var pname_;
};

/// Hash value used as the `INT_ID` for `ACE_Hash_Map_Manager`.
class TAO_Property_Serv_Export CosProperty_Hash_Value
{
public:
  CosProperty_Hash_Value () = default;

  /// Constructs a value from a property value and mode.
  CosProperty_Hash_Value (CORBA::Any const & any,
                          CosPropertyService::PropertyModeType mode);

  CosProperty_Hash_Value (CosProperty_Hash_Value const & hash_value);

  CosProperty_Hash_Value &operator= (CosProperty_Hash_Value const &) = default;

  virtual ~CosProperty_Hash_Value ();

  /// The property value.
  CORBA::Any pvalue_;

  /// The property mode.
  CosPropertyService::PropertyModeType pmode_ {CosPropertyService::undefined};
};

/// @}

class TAO_PropertySet;

/// Factory for `TAO_PropertySet` objects, optionally initialized with
/// predefined properties.
class TAO_Property_Serv_Export TAO_PropertySetFactory :  public virtual POA_CosPropertyService::PropertySetFactory
{
public:
  TAO_PropertySetFactory () = default;

  ~TAO_PropertySetFactory () override;

  TAO_PropertySetFactory (TAO_PropertySetFactory const &) = delete;
  TAO_PropertySetFactory (TAO_PropertySetFactory &&) = delete;
  TAO_PropertySetFactory &operator= (TAO_PropertySetFactory const &) = delete;
  TAO_PropertySetFactory &operator= (TAO_PropertySetFactory &&) = delete;

  /// Creates a property set without initial properties.
  CosPropertyService::PropertySet_ptr create_propertyset () override;

  /// Creates a property set constrained by allowed property types and values.
  /// Its properties use fixed-normal modes.
  CosPropertyService::PropertySet_ptr
  create_constrained_propertyset (CosPropertyService::PropertyTypes const & allowed_property_types,
                                  CosPropertyService::Properties const & allowed_properties) override;

  /// Creates a property set with initial properties using fixed-normal modes.
  CosPropertyService::PropertySet_ptr
  create_initial_propertyset (CosPropertyService::Properties const & initial_properties) override;

private:
  /// Property sets created for clients and owned by this factory.
  TAO::unbounded_value_sequence<TAO_PropertySet*> propertyset_products_;
};

class TAO_PropertySetDef;

/// Factory for `TAO_PropertySetDef` objects, optionally initialized with
/// predefined properties.
class TAO_Property_Serv_Export TAO_PropertySetDefFactory : public virtual POA_CosPropertyService::PropertySetDefFactory
{
public:
  TAO_PropertySetDefFactory () = default;

  ~TAO_PropertySetDefFactory () override;

  TAO_PropertySetDefFactory (TAO_PropertySetDefFactory const &) = delete;
  TAO_PropertySetDefFactory (TAO_PropertySetDefFactory &&) = delete;
  TAO_PropertySetDefFactory &operator= (TAO_PropertySetDefFactory const &) = delete;
  TAO_PropertySetDefFactory &operator= (TAO_PropertySetDefFactory &&) = delete;

  /// Creates a property-set definition without initial properties.
  CosPropertyService::PropertySetDef_ptr create_propertysetdef () override;

  /// Creates a property-set definition with constraints.
  CosPropertyService::PropertySetDef_ptr
  create_constrained_propertysetdef (CosPropertyService::PropertyTypes const & allowed_property_types,
                                     CosPropertyService::PropertyDefs const & allowed_property_defs) override;

  /// Creates a property-set definition with initial properties.
  CosPropertyService::PropertySetDef_ptr
  create_initial_propertysetdef (CosPropertyService::PropertyDefs const & initial_property_defs) override;

private:
  /// Property-set definitions created for clients and owned by this factory.
  TAO::unbounded_value_sequence<TAO_PropertySetDef*> propertysetdef_products_;
};

/// Implements operations for defining, deleting, enumerating, and checking
/// properties. A hash table stores the properties.
class TAO_Property_Serv_Export TAO_PropertySet :  public virtual POA_CosPropertyService::PropertySet
{
public:
  friend class TAO_PropertyNamesIterator;
  friend class TAO_PropertiesIterator;

  TAO_PropertySet () = default;

  /// Initializes the property set with factory-specified values.
  TAO_PropertySet (CosPropertyService::PropertyTypes const &allowed_property_types,
                   CosPropertyService::Properties const &allowed_properties);

  /// Constructs a property set with initial properties.
  explicit TAO_PropertySet (CosPropertyService::Properties const &initial_properties);

  /// Constructs a definition property set with the specified number of allowed definitions.
  TAO_PropertySet (CosPropertyService::PropertyTypes const &allowed_property_types,
                   CORBA::ULong number_of_allowed_propertydefs);

  ~TAO_PropertySet () override = default;

  TAO_PropertySet (TAO_PropertySet const &) = delete;
  TAO_PropertySet (TAO_PropertySet &&) = delete;
  TAO_PropertySet &operator= (TAO_PropertySet const &) = delete;
  TAO_PropertySet &operator= (TAO_PropertySet &&) = delete;

  /// Stores a property after validating its name, uniqueness, and value type.
  void define_property (char const *property_name,
                        CORBA::Any const &property_value) override;

  /// Defines a sequence of properties.
  void define_properties (CosPropertyService::Properties const &nproperties) override;

  /// Returns the number of properties currently defined.
  CORBA::ULong get_number_of_properties () override;

  /// Gets property names, returning any remaining names through an iterator.
  void get_all_property_names (CORBA::ULong how_many,
                               CosPropertyService::PropertyNames_out property_names,
                               CosPropertyService::PropertyNamesIterator_out rest) override;

  /// Gets the value of a property by name.
  CORBA::Any *get_property_value (char const *property_name) override;

  /// Gets the values for a sequence of property names.
  CORBA::Boolean get_properties (CosPropertyService::PropertyNames const &property_names,
                                 CosPropertyService::Properties_out nproperties) override;

  /// Gets properties, returning any remaining properties through an iterator.
  void get_all_properties (CORBA::ULong how_many,
                           CosPropertyService::Properties_out nproperties,
                           CosPropertyService::PropertiesIterator_out rest) override;

  /// Deletes the named property.
  void delete_property (char const *property_name) override;

  /// Deletes the named properties.
  void delete_properties (CosPropertyService::PropertyNames const &property_names) override;

  /// Deletes all properties.
  CORBA::Boolean delete_all_properties () override;

  /// Checks whether the named property is defined.
  CORBA::Boolean is_property_defined (char const *property_name) override;

protected:
  using COSPROPERTY_HASH_MAP =
    ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
  using COSPROPERTY_HASH_ENTRY = ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>;
  using COSPROPERTY_HASH_ITERATOR =
    ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
  /// Checks whether a value type is allowed in this property set.
  CORBA::Boolean is_type_allowed (CORBA::TypeCode_ptr type) const;

  /// Checks whether the named property is allowed in this property set.
  CORBA::Boolean is_property_allowed (char const *name) const;

public: // @todo make private
  /// Hash table storing the properties.
  COSPROPERTY_HASH_MAP hash_table_;

protected:
  /// Property types allowed in this set.
  CosPropertyService::PropertyTypes allowed_property_types_;

  /// Allowed property names. When nonempty, no other properties may be
  /// defined; these properties default to fixed-normal mode.
  CosPropertyService::PropertyNames allowed_property_names_;
};

/// Implements the `PropertySetDef` specialization of `PropertySet`, adding
/// operations to constrain properties and get or set their modes.
/// The object remains a `PropertySet`; these operations expose and control
/// its metadata.
class TAO_Property_Serv_Export TAO_PropertySetDef : public virtual POA_CosPropertyService::PropertySetDef,
                                              public virtual TAO_PropertySet
{
public:
  TAO_PropertySetDef () = default;

  /// Constructs a definition set with the factory's allowed property types
  /// and definitions.
  TAO_PropertySetDef (CosPropertyService::PropertyTypes const &allowed_property_types,
                      CosPropertyService::PropertyDefs const &allowed_property);

  /// Constructs a definition set with initial property definitions.
  explicit TAO_PropertySetDef (CosPropertyService::PropertyDefs const &initial_property_defs);

  ~TAO_PropertySetDef () override = default;

  TAO_PropertySetDef (TAO_PropertySetDef const &) = delete;
  TAO_PropertySetDef (TAO_PropertySetDef &&) = delete;
  TAO_PropertySetDef &operator= (TAO_PropertySetDef const &) = delete;
  TAO_PropertySetDef &operator= (TAO_PropertySetDef &&) = delete;

  /// Returns the property types supported by this set. An empty sequence
  /// means unrestricted types unless allowed properties are specified.
  void get_allowed_property_types (CosPropertyService::PropertyTypes_out property_types) override;

  /// Returns the properties supported by this set. An empty sequence means
  /// unrestricted properties unless allowed property types are specified.
  void get_allowed_properties (CosPropertyService::PropertyDefs_out property_defs) override;

  /// Adds or modifies a property, checking its type and mode. Raises
  /// `ConflictingProperty` when either constraint is violated.
  void define_property_with_mode (char const *property_name,
                                  CORBA::Any const &property_value,
                                  CosPropertyService::PropertyModeType property_mode) override;

  /// Adds or modifies each property in the supplied sequence.
  void define_properties_with_modes (CosPropertyService::PropertyDefs const &property_defs) override;

  /// Gets a property's mode. Raises `InvalidPropertyName` or
  /// `PropertyNotFound` if the property cannot be found.
  CosPropertyService::PropertyModeType get_property_mode (char const *property_name) override;

  /// Gets the modes for a sequence of property names. Failed lookups are
  /// represented by an undefined mode in the output sequence.
  CORBA::Boolean get_property_modes (CosPropertyService::PropertyNames const &property_names,
                                     CosPropertyService::PropertyModes_out property_modes) override;

  /// Changes a property's mode without relaxing its constraints. Illegal
  /// changes raise `UnsupportedMode`. In TAO, Normal may change to any mode;
  /// Readonly and Fixed-Normal may change to Fixed-Readonly; Fixed-Readonly
  /// cannot change to another mode.
  void set_property_mode (char const *property_name,
                          CosPropertyService::PropertyModeType property_mode) override;

  /// Sets multiple property modes. Raises `MultipleExceptions` for failures.
  void set_property_modes (CosPropertyService::PropertyModes const &property_modes) override;

};

/// Provides iteration over property names returned by `get_all_property_names`.
/// When more names remain than were returned directly, the operation supplies
/// an iterator for the remaining names.
class TAO_Property_Serv_Export TAO_PropertyNamesIterator
  : public virtual POA_CosPropertyService::PropertyNamesIterator
{
public:
  explicit TAO_PropertyNamesIterator (TAO_PropertySet &property_set);

  ~TAO_PropertyNamesIterator () override = default;

  TAO_PropertyNamesIterator (TAO_PropertyNamesIterator const &) = delete;
  TAO_PropertyNamesIterator (TAO_PropertyNamesIterator &&) = delete;
  TAO_PropertyNamesIterator &operator= (TAO_PropertyNamesIterator const &) = delete;
  TAO_PropertyNamesIterator &operator= (TAO_PropertyNamesIterator &&) = delete;

  /// Resets the iterator to the first property name, if one exists.
  void reset () override;

  /// Returns the next property name, or false when no names remain.
  CORBA::Boolean next_one (CORBA::String_out property_name) override;

  /// Returns up to `how_many` property names, or false when no names remain.
  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosPropertyService::PropertyNames_out property_names) override;

  /// Destroys the iterator.
  void destroy () override;

private:
  using COSPROPERTY_HASH_MAP =
    ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
  using COSPROPERTY_HASH_ITERATOR =
    ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
  using COSPROPERTY_HASH_ENTRY = ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>;

  /// Iterator over the property-name hash table.
  COSPROPERTY_HASH_ITERATOR iterator_;
};

/// Provides iteration over the properties returned by `get_all_properties`.
/// When more properties remain than were returned directly, the operation
/// supplies an iterator for the remaining properties.
class TAO_Property_Serv_Export TAO_PropertiesIterator
  : public virtual POA_CosPropertyService::PropertiesIterator
{
public:
  explicit TAO_PropertiesIterator (TAO_PropertySet &property_set);

  ~TAO_PropertiesIterator () override = default;

  TAO_PropertiesIterator (TAO_PropertiesIterator const &) = delete;
  TAO_PropertiesIterator (TAO_PropertiesIterator &&) = delete;
  TAO_PropertiesIterator &operator= (TAO_PropertiesIterator const &) = delete;
  TAO_PropertiesIterator &operator= (TAO_PropertiesIterator &&) = delete;

  /// Resets the iterator to the first property, if one exists.
  void reset () override;

  /// Returns the next property, or false when no properties remain.
  CORBA::Boolean next_one (CosPropertyService::Property_out aproperty) override;

  /// Returns up to `how_many` properties, or false when no properties remain.
  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosPropertyService::Properties_out nproperties) override;

  /// Destroys the iterator.
  void destroy () override;

private:
  using COSPROPERTY_HASH_MAP =
    ACE_Hash_Map_Manager<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
  using COSPROPERTY_HASH_ITERATOR =
    ACE_Hash_Map_Iterator<CosProperty_Hash_Key, CosProperty_Hash_Value, ACE_Null_Mutex>;
  using COSPROPERTY_HASH_ENTRY = ACE_Hash_Map_Entry<CosProperty_Hash_Key, CosProperty_Hash_Value>;

  /// Iterator over the property hash table.
  COSPROPERTY_HASH_ITERATOR iterator_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_COSPROPERTYSERVICE_I_H */
