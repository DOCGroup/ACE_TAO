// $Id$

#include "EC_Timeout_Generator.h"
#include "EC_Timeout_Filter.h"
#include "orbsvcs/Event_Service_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

#if ! defined (ACE_WIN32) && defined (ACE_HAS_DSUI)
#include "ec_dsui_config.h"
#include "ec_dsui_families.h"
#include <ace/Counter.h>
#include <dsui.h>
#endif /* ! ACE_WIN32 && ACE_HAS_DSUI */

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
      RtecEventComm::Event evnt;
      evnt.header.type = filter->type ();
      evnt.header.source = 0;

      ACE_Object_Counter::object_id oid = ACE_OBJECT_COUNTER->increment();
      evnt.header.eid.id = oid.id;
      evnt.header.eid.tid = oid.tid;
      DSUI_EVENT_LOG (EC2_GROUP_FAM, ENTER_TIMEOUT_GENERATOR, 0, sizeof(ACE_Object_Counter::object_id), (char*)&oid);

      RtecEventComm::EventSet single_event (1, 1, &evnt, 0);

      TAO_EC_QOS_Info qos_info = filter->qos_info ();
      filter->push_to_proxy (single_event,
                             qos_info
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
