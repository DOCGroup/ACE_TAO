// -*- C++ -*-
// $Id$

#if !defined (IOR_MCAST_CLIENT_I_H)
#define IOR_MCAST_CLIENT_I_H

#include "MCastC.h"

class ior_mcast_Client_i
{
  // = TITLE
  //     ior_mcast_client_i.h
  //
  // = DESCRIPTION
  //      Helper class for the client which uses the mcast ior type to
  //      get reference to the server.

 public:
  // = COnstructor and destructor.
  ior_mcast_Client_i (void);
  ~ior_mcast_Client_i (void);

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Execute the client example code.

  int init (int& argc, char *argv[] ACE_ENV_ARG_DECL);
  // Initialize the client communication endpoint with the server.

 private:
  // mcast_ior
  ACE_CString mcast_url_;

};

#endif /* IOR_MCAST_CLIENT_I_H */
