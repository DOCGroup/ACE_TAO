// $Id$
#include "Options.h"
#include "new.h"
#include "SL_Server.h"
#include "PMS_Usr.h"

/* This function "encodes" a list of friends by putting the userid's in
   a contiguous block.  This block can then be transmitted over to the
   network to servers on other subnets.  Several things are added to
   make decoding easier on the other end:

   * A count of the number of friends is prepended (assumption: there
     are no more than 9999999 friends... ;-))
   * The login userids are separated by a single space. */

int
PMS_Usr::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMS_Usr::encode");

  char *buf_ptr = packet;

  /* We only send back info on friend that is actually logged in. */

  Protocol_Record *frp = this->get_next_friend ();

  if (frp)
    {
      buf_ptr    = this->handle_protocol_entries (ACE::strecpy (buf_ptr, frp->get_login ()), frp->get_drwho_list ());
      *buf_ptr++ = '\t';
    }

  *buf_ptr++ = '\n';
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
PMS_Usr::decode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, "in PMS_Usr::decode, packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  this->ss = new (PRIVATE_POOL) SL_Server (packet);
  return 1;
}

#ifndef __OPTIMIZE__
PMS_Usr::PMS_Usr (void)
{}
#endif /* __OPTIMIZE__ */
