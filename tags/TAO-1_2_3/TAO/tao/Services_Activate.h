//=============================================================================
/**
 *  @file    Services_Activate.h
 *
 *  $Id$
 *
 * This is a generic interface that would be used to activate
 * the services that are loaded through the svc.conf file
 *
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVICES_ACTIVATE_H
#define TAO_SERVICES_ACTIVATE_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "tao/Exception.h"


class TAO_ORB_Core;
class TAO_Service_Callbacks;

/**
 * @class TAO_Services_Activate
 *
 * @brief A class to dynamically load callback implementations in to an
 * ORB.
 *
 * Many services and components of the ORB whose default behaviour
 * needs to be changed can use this class to activate the Callback
 * hooks. These hooks can then be called by the ORB at the right
 * points.
 * @@ TODO
 */
class TAO_Export TAO_Services_Activate : public ACE_Service_Object
{
public:
  /// The destructor
  virtual ~TAO_Services_Activate (void);

  /**
   * Create and activate the service callbacks into the orb.
   * This method cannot throw any exception, but it can return a nil
   * object to indicate an error condition.
   */
  virtual TAO_Service_Callbacks* activate_services (TAO_ORB_Core *orb)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

};


#include "ace/post.h"
#endif /*TAO_SERVICES_ACTIVATE_H*/
