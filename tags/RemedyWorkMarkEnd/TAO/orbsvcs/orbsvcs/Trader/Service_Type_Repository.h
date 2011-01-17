// -*- C++ -*-

//=============================================================================
/**
 *  @file   Service_Type_Repository.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SERVICE_TYPE_REPOSITORY_H
#define TAO_SERVICE_TYPE_REPOSITORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Trader.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Service_Type_Repository
 *
 * @brief This class implements CosTradingRepos::ServiceTypeRepository
 * IDL interface.
 */
class TAO_Trading_Serv_Export TAO_Service_Type_Repository
  : public POA_CosTradingRepos::ServiceTypeRepository
{
public:
  /**
   * Parameterize the Service_Type_Repository with a lock to serialize
   * access to the type repository map. A reader/writer lock is
   * probably best. The Service_Type_Repository assumes control of the
   * lock.
   */
  TAO_Service_Type_Repository (ACE_Lock *lock = 0);

  ~TAO_Service_Type_Repository (void);

  virtual CosTradingRepos::ServiceTypeRepository::IncarnationNumber
    incarnation (void);

  /**
   * BEGIN SPEC
   * The add_type operation enables the creation of new service types
   * in the service type repository. The caller supplies the "name"
   * for the new type, the identifier for the interface associated
   * with instances of this service type, the properties definitions
   * for this service type, and the service type names of the
   * immediate super-types to this service type.
   */
  virtual CosTradingRepos::ServiceTypeRepository::IncarnationNumber
  add_type (const char *name,
            const char *if_name,
            const CosTradingRepos::ServiceTypeRepository::PropStructSeq &props,
            const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types);

  // If the type creation is successful, an incarnation number is
  // returned as the value of the operation. Incarnation numbers are
  // opaque values that are assigned to each modification to the
  // repository's state. An incarnation number can be quoted when
  // invoking the list_types operation to retrieve all changes to the
  // service repository since a particular logical time. (Note:
  // IncarnationNumber is currently declared as a struct consisting of
  // two unsigned longs; what we really want here is an unsigned hyper
  // [64-bit integer]. A future revision task force should modify this
  // when CORBA systems support IDL 64-bit integers.)

  // If the "name" parameter is malformed, then the
  // CosTrading::IllegalServiceType exception is raised.  If the type
  // already exists, then the ServiceTypeExists exception is raised.
  // If the "if_name" parameter is not a sub-type of the interface
  // associated with a service type from which this service type is
  // derived, such that substitutability would be violated, then the
  // InterfaceTypeMismatch exception is raised.  If a property name
  // supplied in the "props" parameter is malformed, the
  // CosTrading::IllegalPropertyName exception is raised.  If the
  // same property name appears two or more times in the "props"
  // parameter, the CosTrading::DuplicatePropertyName exception is
  // raised.  If a property value type associated with this service
  // type illegally modifies the value type of a super-type's
  // property, or if two super-types incompatibly declare value types
  // for the same property name, then the ValueTypeRedefinition
  // exception is raised.  If one of the ServiceTypeNames in
  // "super_types" is malformed, then the
  // CosTrading::IllegalServiceType exception is raised. If one of the
  // ServiceTypeNames in "super_types" does not exist, then the
  // CosTrading::UnknownServiceType exception is raised.  If the same
  // service type name is included two or more times in this
  // parameter, the DuplicateServiceTypeName exception is raised.
  // END SPEC

  /**
   * BEGIN SPEC
   * The remove_type operation removes the named type from the service
   * type repository. If "name" is malformed, then the
   * CosTrading::IllegalServiceType exception is raised.  If
   * "name" does not exist within the repository, then the
   * CosTrading::UnknownServiceType exception is raised.  If
   * "name" has a service type which has been derived from it, then
   * the HasSubTypes exception is raised.  END SPEC
   */
  virtual void remove_type (const char  *name);

  virtual CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq *
  list_types (const CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes &which_types);

  // BEGIN SPEC
  // The list_types operation permits a client to obtain the names of
  // service types which are in the repository. The "which_types"
  // parameter permits the client to specify one of two possible
  // values:  all types known to the repository  all types
  // added/modified since a particular incarnation number The names of
  // the requested types are returned by the operation for subsequent
  // querying via the describe_type or the fully_describe_type
  // operation.
  // END SPEC

  /**
   * BEGIN SPEC
   * The describe_type operation permits a client to obtain the
   * details for a particular service type. If "name" is malformed,
   * then the CosTrading::IllegalServiceType exception is raised.  If
   * "name" does not exist within the repository, then the
   * CosTrading::UnknownServiceType exception is raised.
   * END SPEC
   */
  virtual CosTradingRepos::ServiceTypeRepository::TypeStruct *
  describe_type (const char *name);

  /**
   * BEGIN SPEC
   * The fully_describe_type operation permits a client to obtain the
   * details for a particular service type. The property sequence
   * returned in the TypeStruct includes all properties inherited from
   * the transitive closure of its super types; the sequence of super
   * types in the TypeStruct contains the names of the types in the
   * transitive closure of the super type relation.  If "name" is
   * malformed, then the CosTrading::IllegalServiceType exception is
   * raised.  If "name" does not exist within the repository, then
   * the CosTrading::UnknownServiceType exception is raised.
   * END SPEC
   */
  virtual CosTradingRepos::ServiceTypeRepository::TypeStruct *
  fully_describe_type (const char *name);

  /**
   * BEGIN SPEC
   * The mask_type operation permits the deprecation of a particular
   * type (i.e., after being masked, exporters will no longer be able
   * to advertise offers of that particular type). The type continues
   * to exist in the service repository due to other service types
   * being derived from it.  If "name" is malformed, then the
   * CosTrading::IllegalServiceType exception is raised.  If "name"
   * does not exist within the repository, then the
   * CosTrading::UnknownServiceType exception is raised.  If the type
   * is currently in the masked state, then the AlreadyMasked
   * exception is raised.
   * END SPEC
   */
  virtual void mask_type (const char *name);

  /**
   * BEGIN SPEC
   * The unmask_type undeprecates a type (i.e., after being unmasked,
   * exporters will be able to resume advertisement of offers of that
   * particular type).  If "name" is malformed, then the
   * CosTrading::IllegalServiceType exception is raised.  If "name"
   * does not exist within the repository, then the
   * CosTrading::UnknownServiceType exception is raised.  If the type
   * is not currently in the masked state, then the NotMasked
   * exception is raised.
   * END SPEC
   */
  virtual void unmask_type (const char  *name);

private:

  /**
   * @class Type_Info
   *
   * @brief Storage structure for information pertinent to the type.
   */
  class Type_Info
  {
  public:
    /// Standard type info.
    CosTradingRepos::ServiceTypeRepository::TypeStruct type_struct_;

    /// Names of subtypes.
    CORBA::Boolean has_subtypes_;
  };

  typedef ACE_Hash_Map_Manager_Ex <CORBA::String_var,
                                  CosTradingRepos::ServiceTypeRepository::PropStruct *,
                                  ACE_Hash<CORBA::String_var>,
                                  ACE_Equal_To<CORBA::String_var>,
                                  ACE_Null_Mutex>
                                  Prop_Map;

  typedef ACE_Hash_Map_Manager_Ex <CORBA::String_var,
                                   Type_Info *,
                                   ACE_Hash<CORBA::String_var>,
                                   ACE_Equal_To<CORBA::String_var>,
                                   ACE_Null_Mutex>
                                   Service_Type_Map;

  typedef ACE_Hash_Map_Iterator_Ex<CORBA::String_var,
                                   Type_Info *,
                                   ACE_Hash<CORBA::String_var>,
                                   ACE_Equal_To<CORBA::String_var>,
                                   ACE_Null_Mutex>
                                   Service_Type_Map_Iterator;

  /**
   * Build a sequence aggregating the property names from all
   * supertypes of the type, and a sequence representing the
   * transitive closure of the super type relation.
   */
  void fully_describe_type_i (const CosTradingRepos::ServiceTypeRepository::TypeStruct &type_struct,
                              CosTradingRepos::ServiceTypeRepository::PropStructSeq &props,
                              CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types);

  void collect_inheritance_hierarchy (const CosTradingRepos::ServiceTypeRepository::TypeStruct &type_struct,
                                      TAO_String_Queue &target);

  /// Confirm that the properties in props have valid names, and aren't
  /// duplicated. Cram those properties into the prop_map.
  void validate_properties (Prop_Map &prop_map,
                            const CosTradingRepos::ServiceTypeRepository::PropStructSeq &props);

  /// Confirm that the each super type exists, and cram them into super_map.
  void validate_supertypes (Service_Type_Map &super_map,
                            const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types);

  /// Ensure that properties of a super_type aren't having their types
  /// or retstrictions redefined.
  void validate_inheritance (Prop_Map &prop_map,
                             const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types);

  /// Update the type map with the information contained in the
  /// TypeStruct, prop_map, and super_map.
  void update_type_map (const char *name,
                        const char  *if_name,
                        const CosTradingRepos::ServiceTypeRepository::PropStructSeq &props,
                        const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq &super_types,
                        Prop_Map &prop_map,
                        Service_Type_Map &super_map);

  TAO_Service_Type_Repository (const TAO_Service_Type_Repository &);
  TAO_Service_Type_Repository &operator= (const TAO_Service_Type_Repository &);

  /// Lock with which to serialize access to the service type map.
  ACE_Lock *lock_;

  /**
   * Stores information for each service type in the repository.  This
   * is a mapping from service type name to a Type_Info struct which
   * serves as a storage for various information for the given type.
   */
  Service_Type_Map type_map_;

  /// Incarnation number to be used for the next modification to the
  /// repository.
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber incarnation_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_SERVICE_TYPE_REPOSITORY_H */
