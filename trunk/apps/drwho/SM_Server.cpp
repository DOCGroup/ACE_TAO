// $Id$
#include "new.h"
#include "Options.h"
#include "PMS_All.h"
#include "PMS_Flo.h"
#include "PMS_Usr.h"
#include "PMS_Ruser.h"
#include "SM_Server.h"

int 
SM_Server::demux (char *packet, int &packet_length)
{
  /* SUPPRESS 112 */
  switch (GET_PACKET_TYPE (packet))
    {
    case Options::PROTO_USR:
      this->pm_server = new (PRIVATE_POOL) PMS_Usr;
      break;
    case Options::PROTO_ALL:
      this->pm_server = new (PRIVATE_POOL) PMS_All;
      break;
    case Options::PROTO_FLO:
      this->pm_server = new (PRIVATE_POOL) PMS_Flo;
      break;
    case Options::PROTO_RUSER:
      this->pm_server = new (PRIVATE_POOL) PMS_Ruser;
      break;
    default:
      return -1;
    }

  if (pm_server->decode (SKIP_PACKET_TYPE (packet), SUBTRACT_PACKET_TYPE (packet_length)) < 0)
    return -1;

  if (pm_server->process () < 0)
    return -1;

  return 1;
}

int 
SM_Server::mux (char *packet, int &packet_length)
{
  int status = pm_server->encode (packet, packet_length);

  /* Hum... should this really go here? */
  if (Options::get_opt (Options::STAND_ALONE_SERVER))
    release_memory ();
  return status;
}

#ifndef __OPTIMIZE__
SM_Server::SM_Server (void)
{}

SM_Server::~SM_Server (void)
{}
#endif /* __OPTIMIZE__ */

