/* -*- C++ -*- */
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "IFR_Service.h"
#include "tao/Environment.h"
#include "orbsvcs/Shutdown_Utilities.h"

class IFR_Service_Shutdown_Functor : public Shutdown_Functor
{
public:
  IFR_Service_Shutdown_Functor (IFR_Service& ifr);

  void operator() (int which_signal);
private:
  IFR_Service& ifr_;
};

IFR_Service_Shutdown_Functor::IFR_Service_Shutdown_Functor (IFR_Service &ifr)
  : ifr_(ifr)
{
}

void
IFR_Service_Shutdown_Functor::operator() (int which_signal)
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "shutting down on signal %d\n", which_signal));
  (void) this->ifr_.shutdown ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  IFR_Service server;

  IFR_Service_Shutdown_Functor killer (server);
  Service_Shutdown kill_contractor (killer);

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
