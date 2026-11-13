#include "Options.h"
#include "PMC_All.h"
#include "PMC_Flo.h"
#include "PMC_Usr.h"
#include "PMC_Ruser.h"
#include "SM_Client.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"

// Call-back function that invokes the appropriate decode function.

int
SM_Client::demux (char *packet,
                  int &packet_length)
{
  if (this->pm_client_->decode (packet, packet_length) < 0)
    return -1;
  else
    return 1;
}

// Call-back function that invokes the appropriate encode function.

int
SM_Client::mux (char *packet, int &packet_length)
{
  PM_Client *manager {};

  switch (Options::protocol_type)
    {
    case Options::PROTO_USR:
      ACE_NEW_RETURN (manager, PMC_Usr (Options::user_name), -1);
      break;
    case Options::PROTO_ALL:
      ACE_NEW_RETURN (manager, PMC_All, -1);
      break;
    case Options::PROTO_FLO:
      ACE_NEW_RETURN (manager, PMC_Flo, -1);
      break;
    case Options::PROTO_RUSER:
      ACE_NEW_RETURN (manager, PMC_Ruser, -1);
      break;
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "%s: bad protocol\n",
                  Options::program_name));
      return -1;
    }

  this->pm_client_.reset (manager);

  if (this->pm_client_->encode (packet, packet_length) < 0)
    return -1;
  return 1;
}

void
SM_Client::process ()
{
  this->pm_client_->process ();
}
