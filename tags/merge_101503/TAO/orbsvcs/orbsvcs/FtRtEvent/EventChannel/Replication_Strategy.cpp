// $Id$

#include "Replication_Strategy.h"
#include "FTEC_Event_Channel.h"

ACE_RCSID (EventChannel,
           Replication_Strategy,
           "$Id$")


Replication_Strategy::Replication_Strategy()
{
}


Replication_Strategy::~Replication_Strategy()
{
}


void
Replication_Strategy::check_validity(ACE_ENV_SINGLE_ARG_DECL)
{
}


Replication_Strategy*
Replication_Strategy::make_primary_strategy()
{
  return this;
}
