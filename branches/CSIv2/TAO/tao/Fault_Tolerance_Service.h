//-*- C++ -*-
//=============================================================================
/**
 *  @file    Fault_Tolerance_Service.h
 *
 *  $Id$
 *
 *  A collection of the ORB and ORB core related properties that are
 *  specific to FT service. The TAO_ORB_Core holds an instance of this
 *  class.
 *
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FAULT_TOLERANCE_SERVICE_H
#define TAO_FAULT_TOLERANCE_SERVICE_H
#include /**/ "ace/pre.h"

#include "ace/SString.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/corbafwd.h"
#include "ace/Synch.h"


class TAO_Service_Callbacks;
class TAO_ORB_Core;

/**
 * @class TAO_Fault_Tolerance_Service
 *
 * @brief TAO_Fault_Tolerant_Service
 *
 * A collection of ORB & ORB_Core related stuff that is needed at
 * the ORB level. The ORB Core would carry an instance of this
 * class and invoke methods on this.
 * Note: This collection would be really useful when we have
 * logging in  place. The contents of this class can be logged at
 * regular check point intervals.
 */
class TAO_Export TAO_Fault_Tolerance_Service
{

public:
  /// Ctor
  TAO_Fault_Tolerance_Service (void);

  /// Dtor
  ~TAO_Fault_Tolerance_Service (void);

  /// Initialise the internal data structures
  void init (TAO_ORB_Core *orb_core);

  /// Return the underlying callback object
  TAO_Service_Callbacks *service_callback (void);

  /// Return the underlying <ft_object_id>
  const ACE_CString &client_id (void);

  /// Set the client id
  void client_id (const char *id);

  /// Generate and return a new retention id
  CORBA::Long retention_id (void);

private:

  /// hook to callback on to the service
  TAO_Service_Callbacks *ft_service_callback_;

  /// The object id that would be used if the ft service is loaded.
  ACE_CString ft_object_id_;

  /**
   * This and the <ft_object_id_> act as unique identifiers for the
   * request sent from the source Object. Modification of this value
   * is done by the loaded FT
   */
  CORBA::Long ft_object_retention_id_;

  /// Lock for the retention id
  ACE_Lock *ft_object_retention_id_lock_;

  // NOTE: At a glance this retention id can be easily mistaken for a
  // request id in a GIOP request. But the purpose served are a lot
  // more than what a RequestId does for GIOP. So, we have a unique
  // generator with a lock to protect from different threads accessing
  // this.
};


#if defined (__ACE_INLINE__)
# include "tao/Fault_Tolerance_Service.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_FAULT_TOLERANCE_SERVICE_H*/
