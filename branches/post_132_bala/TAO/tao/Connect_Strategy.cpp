#include "Connect_Strategy.h"
#include "Connection_Handler.h"


ACE_RCSID(tao,
          Connect_Strategy,
          "$Id$")

TAO_Connect_Strategy::TAO_Connect_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}


TAO_Connect_Strategy::~TAO_Connect_Strategy (void)
{
}

int
TAO_Connect_Strategy::post_failed_connect (TAO_Connection_Handler *ch,
                                           int is_finalized)
{
  int ref = 0;
  if (!is_finalized)
    {
      // We need to do this here else we will leak memory. We cannot call
      // close_connection () for the following reasons
      // . the event_handlers get_handle () will return an
      //   ACE_INVALID_HANDLE as things havent been shutdown
      //   properly.
      // . We cannot relax the conditions since the conditions are right
      //   for other natural cases ie. LF
      // Under these odd circumstances we need to take care of things
      // ourseleves
      ch->transport (0);
      ref =  ch->decr_refcount ();
    }

  return ref;
}
