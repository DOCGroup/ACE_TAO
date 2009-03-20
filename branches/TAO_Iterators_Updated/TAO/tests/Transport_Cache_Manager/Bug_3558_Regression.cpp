// $Id$

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/SString.h"
#include "ace/Manual_Event.h"

#include "tao/Transport_Cache_Manager_T.h"
#include "tao/ORB.h"

class mock_transport;
class mock_tdi;
class mock_ps;

static int global_purged_count = 0;

typedef TAO::Transport_Cache_Manager_T<mock_transport, mock_tdi, mock_ps> TCM;

#include "mock_tdi.h"
#include "mock_transport.h"
#include "mock_ps.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  try
    {
      // We need an ORB to get an ORB core
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // We create 10 transports and have a max of 5 tran

      size_t const transport_max = 10;
      size_t const cache_maximum = 5;
      int purging_percentage = 20;
      size_t i = 0;
      mock_transport mytransport[transport_max];
      mock_tdi mytdi[transport_max];
      mock_ps* myps = new mock_ps(10);
      TCM my_cache (purging_percentage, myps, cache_maximum, false, 0);

      // Cache all transports in the cache
      for (i = 0; i < transport_max; i++)
        {
          my_cache.cache_transport (&mytdi[i], &mytransport[i]);
          mytransport[i].purging_order (i);
        }

     if (my_cache.current_size () != cache_maximum)
       {
         ACE_ERROR ((LM_ERROR, "ERROR Incorrect cache size %d\n", my_cache.current_size ()));
         ++result;
       }

      orb->destroy ();

    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions..
    }
  return result;
}
