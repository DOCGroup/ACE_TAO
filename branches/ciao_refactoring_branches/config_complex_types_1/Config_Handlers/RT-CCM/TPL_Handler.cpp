// $Id$

#include "TPL_Handler.h"
#include "CIAOServerResources.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    TPL_Handler::threadpool_with_lanes (
           const ThreadpoolWithLanesDef &src,
           ::CIAO::DAnCE::ORS_ThreadpoolWithLanes &dest)
    {
      if (src.id_p ())
        {
          dest.Id = CORBA::string_dup (src.id().c_str ());
        }

      for (ThreadpoolWithLanesDef::threadpoolLane_const_iterator loc =
             src.begin_threadpoolLane ();
           loc != src.end_threadpoolLane ();
           loc++)
        {
          size_t len = dest.threadpool_lanes.length ();

          dest.threadpool_lanes.length (len + 1);

          dest.threadpool_lanes[len].lane_priority =
            static_cast <ACE_INT16> (loc->priority ());

          dest.threadpool_lanes[len].static_threads =
            static_cast <ACE_UINT32> (loc->static_threads ());

          dest.threadpool_lanes[len].dynamic_threads =
            static_cast <ACE_UINT32> (loc->dynamic_threads ());
        }

      dest.stacksize = static_cast <ACE_INT16> (src.stacksize ());

      dest.allow_borrowing = src.allow_borrowing ();

      dest.allow_request_buffering = src.allow_request_buffering ();

      dest.max_buffered_requests =
        static_cast <ACE_UINT32> (src.max_buffered_requests ());

      dest.max_request_buffer_size =
        static_cast <ACE_UINT32> (src.max_request_buffered_size ());

      return true;
    }

    ThreadpoolWithLanesDef TPL_Handler::threadpool_with_lanes (
           const ::CIAO::DAnCE::ORS_ThreadpoolWithLanes &src)
    {
      ThreadpoolWithLanesDef tpl (XMLSchema::unsignedLong (src.stacksize),
                                  XMLSchema::boolean ((src.allow_borrowing)),
                                  XMLSchema::boolean ((src.allow_request_buffering)),
                                  XMLSchema::unsignedLong ((src.max_buffered_requests)),
                                  XMLSchema::unsignedLong ((src.max_request_buffer_size))
                                  );

      //XMLSchema::ID <ACE_TCHAR> id = ((src.Id));
      tpl.id (src.Id.in ());

      size_t len = src.threadpool_lanes.length ();
      for (size_t i = 0; i < len; i++)
        {
          ThreadpoolLaneDef new_tplane (
               XMLSchema::unsignedLong (src.threadpool_lanes[i].static_threads),
               XMLSchema::unsignedLong (src.threadpool_lanes[i].dynamic_threads),
               XMLSchema::int_ (src.threadpool_lanes[i].lane_priority));

          tpl.add_threadpoolLane (new_tplane);
        }

      return tpl;
    }
  }
}
