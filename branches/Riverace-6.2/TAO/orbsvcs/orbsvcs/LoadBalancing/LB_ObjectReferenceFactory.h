// -*- C++ -*-

//=============================================================================
/**
 * @file LB_ObjectReferenceFactory.h
 *
 * $Id$

 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LB_OBJECT_REFERENCE_FACTORY_H
#define TAO_LB_OBJECT_REFERENCE_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LB_ORTC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "orbsvcs/CosLoadBalancingC.h"

#include "ace/Array_Base.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_ObjectReferenceFactory
 *
 * @brief Implementation of the PortableInterceptor::ObjectReferenceFactory
 *        interface.
 *
 * This ObjectReferenceFactory creates an object group for a member of
 * the given repository ID (if instructed to do so), creates an
 * "unpublished" reference for that member, and adds it to the object
 * group via the LoadManager.
 */
class TAO_LB_ObjectReferenceFactory
  : public virtual CORBA::DefaultValueRefCountBase
  , public virtual OBV_TAO_LB::ObjectReferenceFactory
{
 public:
  /// Constructor
  TAO_LB_ObjectReferenceFactory (
    PortableInterceptor::ObjectReferenceFactory * old_orf,
    const CORBA::StringSeq & object_groups,
    const CORBA::StringSeq & repository_ids,
    const char * location,
    CORBA::ORB_ptr orb,
    CosLoadBalancing::LoadManager_ptr lm);

  virtual ::CORBA::ValueBase *_copy_value (void);

  /**
   * @name PortableInterceptor::ObjectReferenceFactory Methods
   *
   * Methods required by the
   * PortableInterceptor::ObjectReferenceFactory interface.
   */
  //@{
  virtual CORBA::Object_ptr make_object (
      const char * repository_id,
      const PortableInterceptor::ObjectId & id);
  //@}

  typedef ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    PortableGroup::ObjectGroup_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> Table;

  typedef ACE_Array_Base<
    PortableGroup::GenericFactory::FactoryCreationId_var> fcid_list;

protected:
  /// Destructor
  /**
   * Protected destructor to enforce proper memory management via
   * reference counting.
   */
  ~TAO_LB_ObjectReferenceFactory (void);

  /// Retrieve the object group reference for objects with the given
  /// RepositoryId.
  CORBA::Boolean find_object_group (const char * repository_id,
                                    CORBA::ULong & index,
                                    PortableGroup::ObjectGroup_out object_group);

  /// Determine if object with given RepositoryId is load managed.
  CORBA::Boolean load_managed_object (const char * repository_id,
                                      CORBA::ULong & i);
private:
  /// The old ObjectReferenceFactory used to create object references.
  /**
   * This ObjectReferenceFactory will be used when creating object
   * references for non-load balanced objects.
   */
  PortableInterceptor::ObjectReferenceFactory_var old_orf_;

  /// List of stringified object group references.
  /**
   * All stringified object groups in this sequence have a one-to-one
   * correspondence to the repository IDs found in the repository_ids_
   * member below.
   *
   * @par
   *
   * The special string "CREATE" denotes that creation of a new object
   * group should be performed.
   */
  const CORBA::StringSeq object_groups_;

  /// List of RepositoryIds for object that will be load
  /// managed/balanced.
  /**
   * All RepositoryIds in this sequence have a one-to-one
   * correspondence to the stringified object references found in the
   * object_groups_ member above.
   */
  const CORBA::StringSeq repository_ids_;

  /// The configured location for the server within which this
  /// ObjectReferenceFactory resides.
  PortableGroup::Location location_;

  /// Table that maps repository ID to (non-stringified) object group
  /// reference.
  Table table_;

  /// List of FactoryCreationIds that will later be used to destroy
  /// object groups.
  fcid_list fcids_;

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

  /// Reference to the LoadManager object.
  CosLoadBalancing::LoadManager_var lm_;

  /// Array of flags that denotes whether or not an object group
  /// member of a given RepositoryId has been registered with the
  /// LoadManager.
  /**
   * The values are cached here to avoid querying the LoadManager,
   * which can be costly.
   */
  CORBA::Boolean * registered_members_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_LB_OBJECT_REFERENCE_FACTORY_H */
