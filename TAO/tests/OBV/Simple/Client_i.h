// -*- C++ -*-

//=============================================================================
/**
 *  @file    Client_i.h
 *
 *  This class is a template for a CORBA client.
 *  run () sends some valuetypes to a server and
 *  gets back a list of valuetypes.
 *
 *  @author Torsten Kuepperbased on the echo examplefrom code from Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef ECHO_CLIENT_I_H
#define ECHO_CLIENT_I_H

#include "OBVC.h"
#include "Simple_util.h"

/**
 * @class Checkpoint_Client_i
 *
 * @brief Checkpoint_Client interface subclass.
 *
 * This class implements the interface between the interface
 * objects and the client .
 */
class Checkpoint_Client_i
{
public:
  /// Constructor
  Checkpoint_Client_i () = default;

  /// Destructor
  ~Checkpoint_Client_i () = default;

  Checkpoint_Client_i (Checkpoint_Client_i const&) = delete;
  Checkpoint_Client_i (Checkpoint_Client_i&&) = delete;
  Checkpoint_Client_i& operator= (Checkpoint_Client_i const&) = delete;
  Checkpoint_Client_i& operator= (Checkpoint_Client_i&&) = delete;

  /// Execute the methods
  int run (char const *name, int argc, ACE_TCHAR *argv[]);

private:
  /// Instantiate the checkpoint object.
  Client<Checkpoint, Checkpoint_var> checkpoint;
};


#endif /* TIME_CLIENT_I_H */
