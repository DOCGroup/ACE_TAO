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

#include "tao/PortableServer/PortableServerC.h"
#include "orbsvcs/orbsvcs/IFRService/IFR_Service_Utils.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class IFR_Service
{
  // = TITLE
  //    IFR_Service
  //
  // = DESCRIPTION
  //    A class that initializes, runs and shuts down
  //    the Interface Repository service.
public:

  IFR_Service (void);
  // Default constructor.

  ~IFR_Service (void);
  // Destructor

  int init (int argc,
            char *argv[]
            ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Initialize the IFR service.

  int fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Shut down the IFR Service.
  
  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Run the IFR service.

protected:
  
  CORBA::ORB_var orb_;
  // Reference to our ORB.

  PortableServer::POA_var root_poa_;
  // Root POA reference.

  TAO_IFR_Server my_ifr_server_;
  // IFR Server instance.
};

#endif /* IFR_SERVICE_H */
