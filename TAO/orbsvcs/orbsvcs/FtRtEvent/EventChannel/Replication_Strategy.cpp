#include "orbsvcs/FtRtEvent/EventChannel/Replication_Strategy.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
Replication_Strategy::check_validity()
{
}


Replication_Strategy*
Replication_Strategy::make_primary_strategy()
{
  return this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
