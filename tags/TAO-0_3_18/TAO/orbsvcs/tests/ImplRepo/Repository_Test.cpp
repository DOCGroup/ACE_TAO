/* -*- C++ -*- */
// $Id$

#include "../../ImplRepo_Service/Repository.h"

const char *NAME_VALUE = "server";
const char *COMM_LINE_VALUE = "server -i";
const char *ENV_VALUE = "null environment";
const char *HOST_VALUE = "maxixe.cs.wustl.edu";
const int PORT_VALUE = 20000;
const char *PING_IOR_VALUE = "iiop://maxixe/ping";
const char *WDIR_VALUE = ".";

// Just tests the functionality of the Repository

int main (int argc, char *argv[])
{
  int test_success = 1;
  Repository rep;
  Repository::Record new_rec, rec;

  new_rec.comm_line = ACE::strnew (COMM_LINE_VALUE);
  new_rec.env = ACE::strnew (ENV_VALUE);
  new_rec.host = ACE::strnew (HOST_VALUE);
  new_rec.port = PORT_VALUE;
  new_rec.ping_ior = ACE::strnew (PING_IOR_VALUE);
  new_rec.wdir = ACE::strnew (WDIR_VALUE);

  ACE_DEBUG ((LM_DEBUG, "=Adding Record to Repository\n"));

  // Add our one record
  int result = rep.add (NAME_VALUE, new_rec);

  if (result == 1) // We need to get rid of it
    {
      ACE_DEBUG ((LM_DEBUG, "New Record already registered, updating\n"));
      rep.update (NAME_VALUE, new_rec);
    }

  ACE_DEBUG ((LM_DEBUG, "=Performing Resolve Test\n"));

  // Bring it back!
  if (rep.resolve (NAME_VALUE, rec) == 0)
    {
      if (ACE_OS::strcmp (rec.comm_line, COMM_LINE_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Error: comm_line compare failed: \"%s\" \"%s\"\n",
                     rec.comm_line,
                     COMM_LINE_VALUE));
          test_success = 0;
        }

      if (ACE_OS::strcmp (rec.env, ENV_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Error: env compare failed: \"%s\" \"%s\"\n",
                     rec.env,
                     ENV_VALUE));
          test_success = 0;
        }

      if (ACE_OS::strcmp (rec.wdir, WDIR_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Error: wdir compare failed: \"%s\" \"%s\"\n",
                     rec.wdir,
                     WDIR_VALUE));
          test_success = 0;
        }

      if (ACE_OS::strcmp (rec.host, HOST_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Error: host compare failed: \"%s\" \"%s\"\n",
                     rec.host,
                     HOST_VALUE));
          test_success = 0;
        }

      if (rec.port != PORT_VALUE)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Error: port compare failed: \"%hu\" \"%hu\"\n",
                     rec.port,
                     PORT_VALUE));
          test_success = 0;
        }

      if (ACE_OS::strcmp (rec.ping_ior, PING_IOR_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Error: ping_ior compare failed: \"%s\" \"%s\"\n",
                     rec.ping_ior,
                     PING_IOR_VALUE));
          test_success = 0;
        }
    }

  // Try some of the single accessors

  char *cl, *env, *ping_ior;

  ACE_DEBUG ((LM_DEBUG, "=Performing Single Accessor Test\n"));

  if (rep.get_comm_line (NAME_VALUE, cl) == 0
      && rep.get_env (NAME_VALUE, env) == 0
      && rep.get_ping_ior (NAME_VALUE, ping_ior) == 0)
    {
      if (ACE_OS::strcmp (cl, COMM_LINE_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error: comm_line compare failed: \"%s\" \"%s\"\n",
                      cl,
                      COMM_LINE_VALUE));
          test_success = 0;
        }

      if (ACE_OS::strcmp (env, ENV_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error: env compare failed: \"%s\" \"%s\"\n",
                      env,
                      ENV_VALUE));
          test_success = 0;
        }

      if (ACE_OS::strcmp (ping_ior, PING_IOR_VALUE) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error: ping_ior compare failed: \"%s\" \"%s\"\n",
                      ping_ior,
                      PING_IOR_VALUE));
          test_success = 0;
        }

      delete [] cl;
      delete [] env;
      delete [] ping_ior;
    }

  ACE_DEBUG ((LM_DEBUG, "=Performing Reregister Test\n"));

  if (rep.add (NAME_VALUE, new_rec) != 1)
    {
      ACE_DEBUG ((LM_DEBUG, "Error: Reregistration not detected\n"));
      test_success = 0;
    }

  ACE_DEBUG ((LM_DEBUG, "=Cleaning up\n"));

  if (rep.remove (NAME_VALUE) != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Error: Could not remove record\n"));
      test_success = 0;
    }

  if (test_success == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Tests were successful\n"));
      return 0;
    }
  else
    return -1;
}
