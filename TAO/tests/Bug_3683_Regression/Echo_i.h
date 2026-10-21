// -*- C++ -*-

//=============================================================================
/**
 *  @file    Echo_i.h
 *
 *  This class implements the Echo IDL interface.
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ECHO_I_H
#define ECHO_I_H

#include "EchoS.h"

/**
 * @class Echo_i
 *
 * @brief Echo Object Implementation
 *
 * The object implementation  performs teh following functions:
 * -- To return the string which needs to be displayed
 * from the server.
 * -- shuts down the server
 */
class Echo_i : public POA_Echo
{
public:
  /// Constructor.
  Echo_i () = default;

  /// Destructor.
  ~Echo_i () override = default;
  Echo_i (Echo_i const &) = delete;
  Echo_i (Echo_i &&) = delete;
  Echo_i &operator= (Echo_i const &) = delete;
  Echo_i &operator= (Echo_i &&) = delete;

  /// Return the mesg string back from the server.
  Echo::List *echo_list (char const *mesg) override;

  /// Return the mesg string back from the server.
  char *echo_string (char const *mesg) override;

  /// Shutdown the server.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

};

#endif /* ECHO_I_H */
