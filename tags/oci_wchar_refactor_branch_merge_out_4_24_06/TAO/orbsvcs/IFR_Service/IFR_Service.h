// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs/IFR_Service
//
// = FILENAME
//    IFR_Service.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =======================================================================

#ifndef IFR_SERVICE_H
#define IFR_SERVICE_H

#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class IFR_Service
 *
 * A class that initializes, runs and shuts down
 * the Interface Repository service.
 */
class IFR_Service
{
public:

  /// Default constructor.
  IFR_Service (void);

  /// Destructor
  ~IFR_Service (void);

  /// Initialize the IFR service.
  int init (int argc,
            char *argv[]
            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Shut down the IFR Service.
  int fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Run the IFR service.
  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:

  /// Reference to our ORB.
  CORBA::ORB_var orb_;

  /// Root POA reference.
  PortableServer::POA_var root_poa_;

  /// IFR Server instance.
  TAO_IFR_Server my_ifr_server_;
};

#endif /* IFR_SERVICE_H */
