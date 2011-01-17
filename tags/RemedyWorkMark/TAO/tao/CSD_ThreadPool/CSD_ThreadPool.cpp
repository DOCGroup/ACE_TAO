// $Id$

#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CSD_ThreadPool::init (void)
{
  static int initialized = 0;
  if (initialized == 1)
    return 0;
  initialized = 1;

  TAO_CSD_Framework_Loader::static_init();
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_TP_Strategy_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
