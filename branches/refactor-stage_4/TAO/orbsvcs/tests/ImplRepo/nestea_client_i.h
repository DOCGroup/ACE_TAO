// -*- C++ -*-

//=============================================================================
/**
 *  @file    nestea_client_i.h
 *
 *  $Id$
 *
 *  This class implements a simple CORBA client which controls a Nestea
 *  can database
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#if !defined (NESTEA_CLIENT_I_H)
#define NESTEA_CLIENT_I_H

#include "tao/corba.h"
#include "NesteaC.h"

/**
 * @class Nestea_Client_i
 *
 * @brief Nestea Bookshelf Client Implementation
 *
 * Class wrapper for a client which gets the server IOR and then makes
 * a couple of calls to the server.
 */
class Nestea_Client_i
{
public:
  // = Constructor and destructor.
  Nestea_Client_i (void);
  ~Nestea_Client_i (void);

  /// Execute client example code.
  int run ();

  /// Initialize the client communication endpoint with server.
  int init (int argc, char **argv);

private:
  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  char **argv_;

  /// Key of the obj ref of the server.
  char *server_key_;

  /// Server object ptr.
  Nestea_Bookshelf_ptr server_;

  /// Remember our orb.
  CORBA::ORB_var orb_;
};

#endif /* NESTEA_CLIENT_I_H */
