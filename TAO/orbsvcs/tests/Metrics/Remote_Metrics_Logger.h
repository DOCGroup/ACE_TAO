// $Id$
#ifndef REMOTE_METRICS_LOGGER_H
#define REMOTE_METRICS_LOGGER_H

#include "orbsvcs/Metrics/Metrics_Logger.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Remote_Metrics_Logger_Driver
{
  // = TITLE
  //   Defines a class that implements a remote driver for
  //   the TAO_Metrics_Logger class.
  //
  // = DESCRIPTION
  //   This class makes use of the <TAO_Metrics_Logger> class to
  //   implement a Metrics::QoSLogger servant.  It can optionally log
  //   data to a file or to stdout, and/or generate events for a DOVE
  //   visualizer.

public:
  TAO_Remote_Metrics_Logger_Driver (void);
  // Default Constructor.

  int init ();
  // Initialize the Scheduling Service with the arguments.

  int run (CORBA_Environment& ACE_TRY_ENV);
  // Run the TAO_Remote_Metrics_Logger_Driver.

  ~TAO_Remote_Metrics_Logger_Driver (void);
  // Destructor.

  int parse_args (int argc, char *argv[]);
  // parses the arguments.

private:
  static void * run_orb (void *);
  // Run the orb event loop (can be spawned as a separate thread).

  void generate_orb_args (char *& orb_init_str);
  // Generate the ORB args using the parsed logger host name

  // ORB reference.
  CORBA::ORB_ptr orb_;

  // Root POA reference.
  PortableServer::POA_var root_poa_;

  // Child (persistent) POA reference.
  PortableServer::POA_var persistent_poa_;

  // POA manager.
  PortableServer::POAManager_var poa_manager_;

  TAO_Metrics_Logger * logger_impl_;
  // Logger instance.

  char * logger_ior_file_name_;
  // Name of the logger IOR output file.

  char * log_file_name_;
  // Name of the process id output file.

  char * logger_host_name_;

  char * logger_host_ip_;

  int log_output_;
  // Whether or not to log output.

  int generate_tabbed_output_;
  // Whether or not to generate a tab delimited output file for excel import.

  int visualize_;
  // Whether or not visualization is activated.
};

#endif /* REMOTE_METRICS_LOGGER_H */










