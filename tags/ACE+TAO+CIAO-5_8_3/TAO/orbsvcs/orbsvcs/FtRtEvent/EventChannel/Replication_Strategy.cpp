// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/Replication_Strategy.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Replication_Strategy::Replication_Strategy()
{
}


Replication_Strategy::~Replication_Strategy()
{
}


void
Replication_Strategy::check_validity(void)
{
}


Replication_Strategy*
Replication_Strategy::make_primary_strategy()
{
  return this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
