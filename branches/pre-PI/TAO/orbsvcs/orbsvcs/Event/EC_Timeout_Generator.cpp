// $Id$

#include "EC_Timeout_Generator.h"
#include "EC_Timeout_Filter.h"
#include "orbsvcs/Event_Service_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Timeout_Generator, "$Id$")

TAO_EC_Timeout_Generator::~TAO_EC_Timeout_Generator (void)
{
}

// ****************************************************************

int
TAO_EC_Timeout_Adapter::handle_timeout (const ACE_Time_Value & /* tv */,
                                        const void *vp)
{
  //  ACE_DEBUG ((LM_DEBUG,
  //              "TAO_EC_Timeout_Adapter::handle_timeout -"
  //              " expire at %d:%d\n",
  //              tv.sec (), tv.usec ()));

  TAO_EC_Timeout_Filter *filter =
    ACE_static_cast(TAO_EC_Timeout_Filter*, ACE_const_cast(void*,vp));

  if (filter == 0)
    return 0;

  ACE_TRY_NEW_ENV
    {
      RtecEventComm::Event e;
      e.header.type = filter->type ();
      e.header.source = 0;

      RtecEventComm::EventSet single_event (1, 1, &e, 0);

      TAO_EC_QOS_Info qos_info = filter->qos_info ();
      filter->push_to_proxy (single_event,
                             qos_info,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
