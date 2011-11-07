
//=============================================================================
/**
 *  @file    IFR_Service.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


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
            ACE_TCHAR *argv[]);

  /// Shut down the IFR Service.
  int fini (void);

  /// Run the IFR service.
  int run (void);

  /// Shutdown the Service.
  void shutdown (void);

protected:

  /// Reference to our ORB.
  CORBA::ORB_var orb_;

  /// Root POA reference.
  PortableServer::POA_var root_poa_;

  /// IFR Server instance.
  TAO_IFR_Server my_ifr_server_;
};

#endif /* IFR_SERVICE_H */
