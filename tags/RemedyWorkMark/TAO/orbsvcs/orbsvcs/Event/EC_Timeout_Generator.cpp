// $Id$

#include "orbsvcs/Event/EC_Timeout_Generator.h"
#include "orbsvcs/Event/EC_Timeout_Filter.h"
#include "orbsvcs/Event_Service_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Timeout_Generator.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    static_cast<TAO_EC_Timeout_Filter*> (const_cast<void*> (vp));

  if (filter == 0)
    return 0;

  try
    {
      RtecEventComm::Event e;
      e.header.type = filter->type ();
      e.header.source = 0;

      RtecEventComm::EventSet single_event (1, 1, &e, 0);

      TAO_EC_QOS_Info qos_info = filter->qos_info ();
      filter->push_to_proxy (single_event,
                             qos_info);
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
