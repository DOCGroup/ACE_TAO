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

      // We create 10 transports and have a max of 10 tran

      size_t const transport_max = 10;
      int const cache_maximum = 10;
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

      // Now purge the cache and see which one get removed. Only transport
      // 0 and 1 should be purged in that order, 0 has the lowest purging
      // count, 1 has the second lowest value.
      my_cache.purge ();

      for (size_t i = 2; i < transport_max; ++i)
        {
          if (mytransport[i].purged_count () != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR Incorrect purged count %d for transport %B\n",
                          mytransport[i].purged_count (), i));
              ++result;
            }
        }

      if (mytransport[0].purged_count () != 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR Incorrect purged count for transport 0: %d\n",
                      mytransport[0].purged_count ()));
          ++result;
        }

      if (mytransport[1].purged_count () != 2)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR Incorrect purged count for transport 1: %d\n",
                      mytransport[1].purged_count ()));
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
