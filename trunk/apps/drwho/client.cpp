// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    Client driver program for drwho.
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#include "Options.h"
#include "SML_Client.h"
#include "SMR_Client.h"

// Factory function.

static SM_Client *
make_client (void)
{
  SM_Client *client = 0;

  if (Options::get_opt (Options::REMOTE_USAGE) == 0)
    ACE_NEW_RETURN (client,
                    SML_Client,
                    0);
  else
    ACE_NEW_RETURN (client,
                    SMR_Client (Options::port_number),
                    0);
  return client;
}
 
int
main (int argc, char *argv[])
{
  Options::set_options (argc, argv);

  SM_Client *sm_client = make_client ();

  if (sm_client->send () < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       Options::program_name),
                      -1);

  if (sm_client->receive (Options::max_server_timeout) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       Options::program_name),
                      -1);
    
  sm_client->process ();

  return 0;
}
