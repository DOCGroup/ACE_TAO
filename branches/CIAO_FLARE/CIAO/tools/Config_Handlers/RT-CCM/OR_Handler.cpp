// $Id$

#include "OR_Handler.h"
#include "TP_Handler.h"
#include "TPL_Handler.h"
#include "CB_Handler.h"
#include "CIAOServerResources.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool OR_Handler::orb_resources (const ORBResources &src,
                                    ::CIAO::DAnCE::ORBResources &dest)
    {
      // We know that there should be only one.
      dest.length (1);

      // Set the length of the threadpools
      CORBA::ULong pos (dest[0].threadpool_list.length ());
      dest[0].threadpool_list.length (pos + src.count_threadpool ());

      for (ORBResources::threadpool_const_iterator i = src.begin_threadpool ();
           i != src.end_threadpool ();
           ++i)
        {
          TP_Handler::thread_pool (*i,
                                   dest[0].threadpool_list[pos++]);
        }

      pos = dest[0].threadpool_with_lanes_list.length ();
      dest[0].threadpool_with_lanes_list.length (pos + src.count_threadpoolWithLanes ());

      for (ORBResources::threadpoolWithLanes_const_iterator j = src.begin_threadpoolWithLanes();
           j != src.end_threadpoolWithLanes();
           j++)
        {
          TPL_Handler::threadpool_with_lanes (*j,
                                              dest[0].threadpool_with_lanes_list [pos++]);
        }

      pos = dest[0].connection_bands_list.length ();
      dest[0].connection_bands_list.length (pos + src.count_connectionBands ());

      for (ORBResources::connectionBands_const_iterator k = src.begin_connectionBands();
           k != src.end_connectionBands();
           k++)
        {
          CB_Handler::connection_band (*k,
                                       dest[0].connection_bands_list[pos++]);
        }
      return true;
    }

    ORBResources OR_Handler::orb_resources (const ::CIAO::DAnCE::ORBResources &src)
      throw (OR_Handler::No_Resource)
    {
      if (src.length () == 0 ||
          src[0].threadpool_list.length () == 0 ||
          src[0].threadpool_with_lanes_list.length () == 0 ||
          src[0].connection_bands_list.length () == 0)
        {
          throw No_Resource ();
        }

      size_t len; //For checking the length of sequences

      ORBResources ores;

      len = src[0].threadpool_list.length();
      for(size_t i = 0; i < len; ++i)
        {
          ores.add_threadpool (TP_Handler::thread_pool (src[0].threadpool_list[i]));
        }

      len = src[0].threadpool_with_lanes_list.length();
      for(size_t j = 0; j < len; ++j)
        {
          ores.add_threadpoolWithLanes(TPL_Handler::threadpool_with_lanes (src[0].threadpool_with_lanes_list[j]));
        }

      len = src[0].connection_bands_list.length();
      for(size_t k = 0; k < len; k++)
        {
          ores.add_connectionBands (CB_Handler::connection_band (src[0].connection_bands_list[k]));
        }

      return ores;
    }
  }
}
