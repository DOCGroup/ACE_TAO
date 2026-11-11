
//=============================================================================
/**
 *  @file    LifeCycle_Service.h
 *
 *  A creation service for objects using the CosLifeCycle GenericFactory.
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#ifndef LIFECYCLE_SERVICE_H
#define LIFECYCLE_SERVICE_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Get_Opt.h"
#include "tao/Utils/ORB_Manager.h"
#include "LifeCycle_Service_i.h"

/**
 * @class Life_Cycle_Service_Server
 = TILE
 * Server object for the LifeCycle Service
 */
class Life_Cycle_Service_Server
{
public:
  Life_Cycle_Service_Server ();

  Life_Cycle_Service_Server (Life_Cycle_Service_Server const &) = delete;
  Life_Cycle_Service_Server (Life_Cycle_Service_Server &&) = delete;
  Life_Cycle_Service_Server &operator= (Life_Cycle_Service_Server const &) = delete;
  Life_Cycle_Service_Server &operator= (Life_Cycle_Service_Server &&) = delete;

  ~Life_Cycle_Service_Server ();

  /// Initialize the Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR * argv[]);

  /// Run the orb.
  int run ();

  /// Parse the passed parameters.
  u_int parse_args (int argc,
                    ACE_TCHAR * argv[]);

private:
  /// instance of the ORB Manager
  TAO_ORB_Manager orb_manager_;

  /// reference to the naming service
  CosNaming::NamingContext_var namingContext_var_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_ {1};
};

#endif /* LIFECYCLE_SERVICE_H */
