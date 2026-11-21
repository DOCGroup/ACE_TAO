#include "Options.h"
#include "PMS_All.h"
#include "PMS_Flo.h"
#include "PMS_Usr.h"
#include "PMS_Ruser.h"
#include "SM_Server.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"

int
SM_Server::demux (char *packet, int &packet_length)
{
  PM_Server *manager {};

  switch (drwho_get_packet_type (packet))
    {
    case Options::PROTO_USR:
      ACE_NEW_RETURN (manager, PMS_Usr, -1);
      break;
    case Options::PROTO_ALL:
      ACE_NEW_RETURN (manager, PMS_All, -1);
      break;
    case Options::PROTO_FLO:
      ACE_NEW_RETURN (manager, PMS_Flo, -1);
      break;
    case Options::PROTO_RUSER:
      ACE_NEW_RETURN (manager, PMS_Ruser, -1);
      break;
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "%s: bad protocol\n",
                  Options::program_name));
      return -1;
    }

  this->pm_server_.reset (manager);
  packet_length = drwho_subtract_packet_type (packet_length);

  if (pm_server_->decode (drwho_skip_packet_type (packet), packet_length) < 0)
  {
    return -1;
  }

  if (pm_server_->process () < 0)
  {
    return -1;
  }

  return 1;
}

int
SM_Server::mux (char *packet,
                int &packet_length)
{
  return pm_server_->encode (packet, packet_length);
}
