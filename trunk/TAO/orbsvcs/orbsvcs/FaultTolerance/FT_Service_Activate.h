// $Id$
//
// ============================================================================
//
// = LIBRARY
//    FT
//
// = FILENAME
//    FT_Callbacks.h
//
// = DESCRIPTION
//   A concrete implementation of a service callback
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef TAO_FT_ACTIVATE_H
#define TAO_FT_ACTIVATE_H
#include "ace/pre.h"

#include "fault_tol_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Services_Activate.h"

// Forward declarations
class TAO_Service_Callbacks;
class TAO_ORB_Core;
class TAO_FT_Service_Callbacks;

class TAO_FT_Export TAO_FT_Service_Activate : public TAO_Services_Activate
{
  // = TITLE
  //   A class to dynamically load the FT callback implementations in
  //   to the ORB.
  //
  // = DESCRIPTION
  //   An implementation of the service callbacks that allows the
  //   loading of the FT ORB level library in to the ORB

public:
  TAO_FT_Service_Activate (void);
  // Constructor

  virtual ~TAO_FT_Service_Activate (void);
  // The destructor

  virtual TAO_Service_Callbacks *activate_services (TAO_ORB_Core *orb)
    ACE_THROW_SPEC (());
  // Create and activate the service callbacks into the orb.
  // This method cannot throw any exception, but it can return a nil
  // object to indicate an error condition.


private:
  TAO_ORB_Core *orb_core_;
  // Our copy of the ORB Core pointer

  TAO_FT_Service_Callbacks *ft_service_callback_;
  // A pointer to the callback that we will create for use and hand it
  // over to the ORB.
};

ACE_FACTORY_DECLARE (TAO_FT, TAO_FT_Service_Activate)

#include "ace/post.h"
#endif /*TAO_FT_ACTIVATE_H*/
