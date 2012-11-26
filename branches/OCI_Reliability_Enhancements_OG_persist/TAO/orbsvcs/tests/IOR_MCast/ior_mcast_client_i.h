// -*- C++ -*-
// $Id$

#if !defined (IOR_MCAST_CLIENT_I_H)
#define IOR_MCAST_CLIENT_I_H

#include "MCastC.h"
#include "ace/SString.h"

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

  int run (void);
  // Execute the client example code.

  int init (int& argc, ACE_TCHAR *argv[]);
  // Initialize the client communication endpoint with the server.

 private:
  // mcast_ior
  ACE_CString mcast_url_;

};

#endif /* IOR_MCAST_CLIENT_I_H */
