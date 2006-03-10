#include "tao/Connect_Strategy.h"

ACE_RCSID (tao,
           Connect_Strategy,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Connect_Strategy::TAO_Connect_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}


TAO_Connect_Strategy::~TAO_Connect_Strategy (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
