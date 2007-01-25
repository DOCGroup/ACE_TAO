/* -*- C++ -*- */
// $Id$

#include "IFR_Service.h"
#include "tao/Environment.h"

ACE_RCSID (IFR_Service,
           IFR_Server,
           "$Id$")

int
main (int argc, char *argv[])
{
  IFR_Service server;

  try
    {
      int status = server.init (argc, argv);

      if (status != 0)
        {
          return 1;
        }
      else
        {
          server.run ();

          status = server.fini ();

          if (status == -1)
            {
              return 1;
            }
        }
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown Exception");
      return -1;
    }
  return 0;
}
