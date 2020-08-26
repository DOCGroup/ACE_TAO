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
  virtual ~Echo_i (void);

  /// Return the result sequences to the cllient.
  virtual Echo::List *return_list ();
  virtual Echo::WList *return_wlist ();

  /// Shutdown the server.
  virtual void shutdown ();

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  void operator= (const Echo_i&);
};

#endif /* ECHO_I_H */
