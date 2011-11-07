/**
 * @file Echo_Caller.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Echo_Caller.h"
#include "tao/ORB_Core.h"
#include "Server_Thread_Pool.h"

Echo_Caller::Echo_Caller(Thread_Pool *pool)
  : pool_(pool)
{
}

void
Echo_Caller::start_task(Test::Echo_ptr client)
{
  pool_->put(client);
}

void
Echo_Caller::shutdown(void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server received shutdown message\n"));
  pool_->shutdown();
}
