// $Id$

#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "orbsvcs/orbsvcs/IOR_Multicast.h"

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

  int init (int &argc, char **&argv TAO_ENV_ARG_DECL_NOT_USED);
  // Initialize the server multicast.

 private:

  int enable_multicast (const char *ior
                        TAO_ENV_ARG_DECL);
  // Sets the IOR_Multicast class to listen for multicast requests
  // for this server.

  int parse_args (int argc, char *argv[]);
  // Parse the command line arguments.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
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
