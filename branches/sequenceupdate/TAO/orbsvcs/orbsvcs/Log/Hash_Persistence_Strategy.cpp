#include "orbsvcs/Log/Hash_Persistence_Strategy.h"
#include "orbsvcs/Log/Hash_LogStore.h"

ACE_RCSID (Log,
           Hash_Persistence_Strategy,
           "$Id$")

TAO_Hash_Persistence_Strategy::TAO_Hash_Persistence_Strategy()
{
}


TAO_Hash_Persistence_Strategy::~TAO_Hash_Persistence_Strategy()
{
}

TAO_LogStore *
TAO_Hash_Persistence_Strategy::create_log_store(CORBA::ORB_ptr orb,
                                                TAO_LogMgr_i *mgr)
{
  return new TAO_Hash_LogStore (orb, mgr);
}
