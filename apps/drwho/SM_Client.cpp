// $Id$
#include "Options.h"
#include "new.h"
#include "PMC_All.h"
#include "PMC_Flo.h"
#include "PMC_Usr.h"
#include "PMC_Ruser.h"
#include "SM_Client.h"

/* Call-back function that invokes the appropriate decode function. */

int 
SM_Client::demux (char *packet, int &packet_length)
{
  if (this->pm_client->decode (packet, packet_length) < 0)
    return -1;
  return 1;
}

/* Call-back function that invokes the appropriate encode function. */

int 
SM_Client::mux (char *packet, int &packet_length)
{
  switch (Options::protocol_type)
    {
    case Options::PROTO_USR:
      this->pm_client = new (PRIVATE_POOL) PMC_Usr (Options::user_name);
      break;
    case Options::PROTO_ALL:
      this->pm_client = new (PRIVATE_POOL) PMC_All;      
      break;
    case Options::PROTO_FLO:
      this->pm_client = new (PRIVATE_POOL) PMC_Flo;
      break;
    case Options::PROTO_RUSER:
      this->pm_client = new (PRIVATE_POOL) PMC_Ruser;
      break;
    default:
      fprintf (stderr, "%s: bad protocol\n", Options::program_name);
      return -1;
    }
  
  if (this->pm_client->encode (packet, packet_length) < 0)
    return -1;
  return 1;
}

#ifndef __OPTIMIZE__
SM_Client::SM_Client (void)
{}

SM_Client::~SM_Client (void)
{}

void
SM_Client::process (void)
{
  this->pm_client->process ();
}
#endif /* __OPTIMIZE__ */
