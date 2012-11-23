// -*- C++ -*-

//=============================================================================
/**
 * @file client.cpp
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  // Process a Service Configurator directive that will cause the test
  // client module to be dynamically loaded.
  //
  // This is done to prevent the client binary from being forced to
  // explicitly link to the test client module library.  Hence, the
  // client binary is completely decoupled from the test client
  // module (and the ORB!).
  //
  // In the process of doing this, the Test method provided by target
  // CORBA object will be invoked.
  if (ACE_Service_Config::process_directive (
        ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
          "Client_Module",
          "Test_Client_Module",
          TAO_VERSION,
          "_make_Test_Client_Module",
          "-k file://test.ior")) != 0)
    {

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ERROR: Client unable to process the "
                         "Service Configurator directive"),
                        -1);
    }

  ACE_DEBUG ((LM_INFO,
              "CLIENT: CORBA-portion of the test dynamically loaded.\n"));

  // Wait for all threads to complete.
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_INFO,
              "CLIENT:  Terminated successfully.\n"));

  return 0;
}
