//-*- C++ -*-
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Fault_Tolerance_Service.h
//
// = DESCRIPTION
//    A collection of the ORB and ORB core related properties that are
//    specific to FT service. The TAO_ORB_Core holds an instance of this
//    class.
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================

#ifndef TAO_FAULT_TOLERANCE_SERVICE_H
#define TAO_FAULT_TOLERANCE_SERVICE_H
#include "ace/pre.h"

#include "ace/SString.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/corbafwd.h"


#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Service_Callbacks;
class TAO_ORB_Core;

class TAO_Export TAO_Fault_Tolerance_Service
{
  // = TITLE
  //    TAO_Fault_Tolerant_Service
  //
  // = DESCRIPTION
  //    A collection of ORB & ORB_Core related stuff that is needed at
  //    the ORB level. The ORB Core would carry an instance of this
  //    class and invoke methods on this.
  //    Note: This collection would be really useful when we have
  //    logging in  place. The contents of this class can be logged at
  //    regular check point intervals.

public:
  TAO_Fault_Tolerance_Service (void);
  // Ctor

  ~TAO_Fault_Tolerance_Service (void);
  // Dtor

  void init (TAO_ORB_Core *orb_core);
  // Initialise the internal data structures

  TAO_Service_Callbacks *service_callback (void);
  // Return the underlying callback object

  const ACE_CString &client_id (void);
  // Return the underlying <ft_object_id>

  void client_id (const char *id);
  // Set the client id

  CORBA::Long retention_id (void);
  // Generate and return a new retention id

private:

  TAO_Service_Callbacks *ft_service_callback_;
  // hook to callback on to the service

  ACE_CString ft_object_id_;
  // The object id that would be used if the ft service is loaded.

  CORBA::Long ft_object_retention_id_;
  // This and the <ft_object_id_> act as unique identifiers for the
  // request sent from the source Object. Modification of this value
  // is done by the loaded FT

  ACE_Lock *ft_object_retention_id_lock_;
  // Lock for the retention id

  // NOTE: At a glance this retention id can be easily mistaken for a
  // request id in a GIOP request. But the purpose served are a lot
  // more than what a RequestId does for GIOP. So, we have a unique
  // generator with a lock to protect from different threads accessing
  // this.
};


#if defined (__ACE_INLINE__)
# include "tao/Fault_Tolerance_Service.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_FAULT_TOLERANCE_SERVICE_H*/
