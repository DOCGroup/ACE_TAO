/* -*- C++ -*- */
/**
 *  @file Container.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_CONTAINER_H
#define TAO_NS_CONTAINER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/NotifyExtC.h"
#include "Object.h"

class TAO_NS_POA_Helper;
class TAO_NS_QoSAdmin;

/**
 * @class TAO_NS_Container
 *
 * @brief A Base class for implementing the Notify participants that behave as Obejct Containers.
 *
 */
class TAO_Notify_Export TAO_NS_Container : public virtual TAO_NS_Object
{
public:
  /// Constuctor
  TAO_NS_Container (void);

  /// Destructor
  virtual ~TAO_NS_Container ();

  /// Init this object.
  void init (TAO_NS_POA_Helper* poa, TAO_NS_Worker_Task* worker_task, TAO_NS_POA_Helper* object_poa, TAO_NS_POA_Helper* proxy_poa);

  /// Cleanup
  void cleanup (ACE_ENV_SINGLE_ARG_DECL);

  /// Accessor for the object_poa_
  /// Setting the object_poa_ gives ownership to this class.
  void object_poa (TAO_NS_POA_Helper* object_poa);
  TAO_NS_POA_Helper* object_poa (void);

protected:
  /// = Protected Methods

  /// = Data Members

  /// The POA in which the object's are activated.
  TAO_NS_POA_Helper* object_poa_;

  /// Flag that indicates if we own <object_poa_>
  int delete_object_poa_;
};

#if defined (__ACE_INLINE__)
#include "Container.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_CONTAINER_H */
