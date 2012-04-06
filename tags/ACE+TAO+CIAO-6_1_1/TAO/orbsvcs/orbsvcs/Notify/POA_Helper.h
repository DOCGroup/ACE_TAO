// -*- C++ -*-

/**
 *  @file POA_Helper.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_POA_Helper_H
#define TAO_Notify_POA_Helper_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/ID_Factory.h"

#include "tao/PortableServer/PortableServer.h"

#include "ace/Copy_Disabled.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_POA_Helper
 *
 * @brief POA Abstraction.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_POA_Helper : private ACE_Copy_Disabled
{
public:
  /// Default Constructor
  TAO_Notify_POA_Helper (void);

  /// Create a new PortableServer::POA.
  void init (PortableServer::POA_ptr parent_poa, const char* poa_name);

#if !defined (CORBA_E_MICRO)
  /// Create a new persistent PortableServer::POA.
  void init_persistent (PortableServer::POA_ptr parent_poa,
                        const char* poa_name);
#endif /* !CORBA_E_MICRO */

  /// Create a new PortableServer::POA. The name is chosen at random.
  void init (PortableServer::POA_ptr parent_poa);

  /// Destructor
  virtual ~TAO_Notify_POA_Helper ();

  /// Get underlying POA
  PortableServer::POA_ptr poa (void);

  /// Destroy underlying POA.
  void destroy (void);

  /// Activate Object, the POA will assign an ID and return its value.
  CORBA::Object_ptr activate (PortableServer::Servant servant,
                              CORBA::Long& id);

  /// Activate Object, using existing ID
  CORBA::Object_ptr activate_with_id (PortableServer::Servant servant, CORBA::Long id);

  /// Deactivate Object with ID
  void deactivate (CORBA::Long id) const;

  /// Convert ID to reference.
  CORBA::Object_ptr id_to_reference (CORBA::Long id) const;

  /// Convert reference to pointer to servant
  PortableServer::ServantBase * reference_to_servant (CORBA::Object_ptr ptr) const;

  CORBA::Object_ptr servant_to_reference (PortableServer::ServantBase * servant) const;

  /// Generate a unique id for each POA created.
  ACE_CString get_unique_id (void);

protected:
  /// Set default POA policies.
  virtual void set_policy (PortableServer::POA_ptr parent_poa,
                           CORBA::PolicyList &policy_list);

#if !defined (CORBA_E_MICRO)
  /// Set persistent POA policies.
  virtual void set_persistent_policy (PortableServer::POA_ptr parent_poa,
                                      CORBA::PolicyList &policy_list);
#endif /* !CORBA_E_MICRO */

  /// Apply the polices and create child POA.
  void create_i (PortableServer::POA_ptr parent_poa,
                 const char* poa_name,
                 CORBA::PolicyList &policy_list);

  /// Convert id to ObjectID
  PortableServer::ObjectId* long_to_ObjectId (CORBA::Long id) const;

protected:
  /// POA
  PortableServer::POA_var poa_;

  /// ID Factory for objects.
  TAO_Notify_ID_Factory id_factory_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/POA_Helper.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_POA_Helper_H */
