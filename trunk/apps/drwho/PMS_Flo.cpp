// $Id$
#include "Options.h"
#include "new.h"
#include "BS_Server.h"
#include "PMS_Flo.h"

/* This function packs the located friends userids, plus the machines
   they are logged into (along with the inactive and active counts on
   each machine) into a buffer that is subsequently transmitted back
   to the client across the network. */

int
PMS_Flo::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMS_Flo::encode");

  Protocol_Record *frp;
  char          *buf_ptr = packet;

  sprintf (buf_ptr, "Users   %d", this->get_total_users ());
  buf_ptr += strlen (buf_ptr) + 1;

  /* We only send back info on friends that we actually see logged in. */

  for (; (frp = this->get_next_friend ()) != 0; *buf_ptr++ = '\t')
    buf_ptr = this->handle_protocol_entries (ACE::strecpy (buf_ptr, frp->get_login ()), frp->get_drwho_list ());
  
  *buf_ptr++	= '\n';
  packet_length = buf_ptr - packet;

  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, ", packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  return 1;
}

/* This function takes a packet received from the client and calls
   the appropriate Protocol_Manager routine to build the local table of
   friends. */

int
PMS_Flo::decode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMS_Flo::decode, packet_length = %d\n", packet_length);

  this->ss = new (PRIVATE_POOL) BS_Server (packet);
  return 1;
}

#ifndef __OPTIMIZE__
PMS_Flo::PMS_Flo (void)
{}
#endif /* __OPTIMIZE__ */
