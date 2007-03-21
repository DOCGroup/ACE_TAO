// -*- C++ -*-

//=============================================================================
/**
 * @file FT_ClientService_Activate.h
 *
 * $Id$
 *
 * A concrete implementation of a service callback
 *
 * @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FT_CLIENTACTIVATE_H
#define TAO_FT_CLIENTACTIVATE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/FaultTolerance/FT_ClientORB_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Services_Activate.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Service_Callbacks;

// Forward declarations
/**
 * @class TAO_FT_ClientService_Activate
 *
 * @brief A class to dynamically load the FT callback implementations in
 * to the ORB.
 *
 * An implementation of the service callbacks that allows the
 * loading of the FT ORB level library in to the ORB
 */
class TAO_FT_ClientORB_Export TAO_FT_ClientService_Activate : public TAO_Services_Activate
{

public:
  /// Constructor
  TAO_FT_ClientService_Activate (void);

  /// The destructor
  virtual ~TAO_FT_ClientService_Activate (void);

  /**
   * Create and activate the service callbacks into the orb.
   * This method cannot throw any exception, but it can return a nil
   * object to indicate an error condition.
   */
  virtual TAO_Service_Callbacks *activate_services (TAO_ORB_Core *orb);

  /// Used to force the initialization.
  static int Initializer (void);
};

static int
TAO_FT_Requires_ClientService_Activate = TAO_FT_ClientService_Activate::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_FT_ClientService_Activate)
ACE_FACTORY_DECLARE (TAO_FT_ClientORB, TAO_FT_ClientService_Activate)

#include /**/ "ace/post.h"
#endif /*TAO_FT_CLIENTACTIVATE_H*/
