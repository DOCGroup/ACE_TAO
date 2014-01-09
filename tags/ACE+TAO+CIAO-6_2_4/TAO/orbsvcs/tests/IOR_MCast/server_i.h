// -*- C++ -*-
//
// $Id$

#ifndef SERVER_I_H
#define SERVER_I_H

#include "orbsvcs/IOR_Multicast.h"

class Server_i
{
  // = TITLE
  //     Server_i
  //
  // = DESCRIPTION
  //     Helper class for the server implementation.

public:
  // = Constructor and destructor.
  Server_i (void);
  ~Server_i (void);

  int init (int &argc, ACE_TCHAR **argv);
  // Initialize the server multicast.

private:

  /// Sets the IOR_Multicast class to listen for multicast requests
  // for this server.
  int enable_multicast (const char *ior);

  /// Parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

private:

  int argc_;
  // # of arguments on the command line.

  ACE_TCHAR **argv_;
  // arguments from command line.

  CORBA::ORB_var orb_;
  // ORB

  TAO_IOR_Multicast *ior_multicast_;
  // The ior_multicast event handler.

  CORBA::String_var service_ior_;
  // The IOR string of the service.

  CORBA::String_var mcast_address_;
  // Address of the multicast address where to listen for requests for */
  // the server. */


};

#endif /* SERVER_I_H */
