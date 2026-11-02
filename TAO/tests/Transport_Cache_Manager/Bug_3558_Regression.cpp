#include "tao/Transport_Cache_Manager_T.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"

#include <memory>

class mock_transport;
class mock_tdi;
class mock_ps;

static int global_purged_count = 0;

using TCM = TAO::Transport_Cache_Manager_T<mock_transport, mock_tdi, mock_ps>;

#include "mock_tdi.h"
#include "mock_transport.h"
#include "mock_ps.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  int result = 0;

  try
    {
      // We need an ORB to get an ORB core
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // We create 10 transports and have a max of 5 tran

      size_t const transport_max = 10;
      size_t const cache_maximum = 5;
      int const purging_percentage = 20;
      mock_transport mytransport[transport_max];
      mock_tdi mytdi[transport_max];
      std::unique_ptr<mock_ps> purging_strategy = std::make_unique<mock_ps> (10);
      TCM my_cache (*orb->orb_core ()->reactor (), purging_percentage,
                    purging_strategy.get (), cache_maximum, false,
                    nullptr, 0, 30);
      // The cache manager takes ownership of the purge strategy.
      purging_strategy.release ();

      // Cache all transports in the cache
      for (size_t i = 0; i < transport_max; ++i)
        {
          my_cache.cache_transport (&mytdi[i], &mytransport[i]);
          mytransport[i].purging_order (i);
        }

      if (my_cache.current_size () != cache_maximum)
        {
          ACE_ERROR ((LM_ERROR, "ERROR Incorrect cache size %B\n",
                      my_cache.current_size ()));
          ++result;
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      // Ignore exceptions..
    }
  return result;
}
