// -*- C++ -*-

//=============================================================================
/**
 *  @file    Load_Balancing_Service.h
 *
 *  $Id$
 *
 *  Driver for Load Balancing service.
 *
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef LOAD_BALANCING_SERVICE_H_
#define LOAD_BALANCING_SERVICE_H_

#include "tao/Utils/ORB_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Load_Balancing_Service
 *
 * Server, which creates and initializes a
 * <Load_Balancer::Object_Group_Factory>
 * object, and runs the orb loop.
 */
class Load_Balancing_Service
{
public:

  /// Default constructor.
  Load_Balancing_Service (void);

  /// Destructor.
  ~Load_Balancing_Service (void);

  /// Initialize the <Load_Balancing_Service>: initializes the ORB, parses
  /// arguments, creates a servant ...
  int init (int argc, ACE_TCHAR *argv[]);

  /// Run the server.
  int run (void);

private:
  /// Parses the commandline arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// File to output the <Object_Group_Factory> IOR.
  FILE *ior_output_file_;
};

#endif /* LOAD_BALANCING_SERVICE_H_ */
