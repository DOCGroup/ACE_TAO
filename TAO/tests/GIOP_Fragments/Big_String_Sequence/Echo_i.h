// -*- C++ -*-

//=============================================================================
/**
 *  @file    Echo_i.h
 *
 *  This class implements the Echo IDL interface.
 *
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
 */
class Echo_i : public POA_Echo
{
public:
  /// Constructor.
  Echo_i (CORBA::ORB_ptr o);

  /// Destructor.
  ~Echo_i () override = default;

  /// Return the result sequences to the cllient.
  Echo::List *return_list () override;
  Echo::WList *return_wlist () override;

  /// Shutdown the server.
  void shutdown () override;

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  void operator= (const Echo_i&) = delete;
};

#endif /* ECHO_I_H */
