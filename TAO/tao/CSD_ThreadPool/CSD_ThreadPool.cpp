// $Id$

#include "CSD_ThreadPool.h"
#include "CSD_TP_Strategy_Factory.h"
#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"

int
TAO_CSD_ThreadPool::init (void)
{
  static int initialized = 0;
  if (initialized == 1)
    return 0;
  initialized = 1;

  TAO_CSD_Framework_Loader::init();
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_TP_Strategy_Factory);
}
