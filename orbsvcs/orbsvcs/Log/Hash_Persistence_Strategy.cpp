// $Id$

#include "orbsvcs/Log/Hash_Persistence_Strategy.h"
#include "orbsvcs/Log/Hash_LogStore.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Hash_Persistence_Strategy::TAO_Hash_Persistence_Strategy()
{
}


TAO_Hash_Persistence_Strategy::~TAO_Hash_Persistence_Strategy()
{
}

TAO_LogStore *
TAO_Hash_Persistence_Strategy::create_log_store(TAO_LogMgr_i *logmgr_i)
{
  return new TAO_Hash_LogStore (logmgr_i);
}

TAO_END_VERSIONED_NAMESPACE_DECL
