#include "Connect_Strategy.h"


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
TAO_Connect_Strategy::post_failed_connect (TAO_Connection_Handler * /*ch*/)
{
  return 0;
}
