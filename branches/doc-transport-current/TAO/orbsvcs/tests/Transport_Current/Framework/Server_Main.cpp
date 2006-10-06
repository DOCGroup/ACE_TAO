// -*- C++ -*-

// $Id$

#include "Server_Request_Interceptor.h"

/// Prototype
int server_main (int argc,
                 ACE_TCHAR *argv[],
                 Test::Server_Request_Interceptor *cri);

int
test_transport_current (CORBA::ORB_ptr orb
                        ACE_ENV_ARG_DECL)  ACE_THROW_SPEC ((CORBA::SystemException,
                                                            CORBA::UserException));

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  Test::Server_Request_Interceptor cri ("test_orb", test_transport_current);
  return server_main (argc, argv, &cri);
}
