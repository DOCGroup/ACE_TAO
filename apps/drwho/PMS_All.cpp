// $Id$
#include "Options.h"
#include "new.h"
#include "HT_Server.h"
#include "PMS_All.h"

/* This function packs the located friends userids, plus the machines
   they are logged into (along with the inactive and active counts on
   each machine) into a buffer that is subsequently transmitted back
   to the client across the network.  Note that this function encodes
   the REAL_NAME of the user in the packet. */

int
PMS_All::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMS_All::encode");

  Protocol_Record *frp;
  char          *buf_ptr = packet;

  sprintf (buf_ptr, "Users   %d", this->get_total_users ());
  buf_ptr += strlen (buf_ptr) + 1;

  /* We only send back info on friends that we actually see logged in. */

  for (; (frp = this->get_next_friend ()) != 0; *buf_ptr++ = '\t')
    buf_ptr = this->handle_protocol_entries (ACE::strecpy (ACE::strecpy (buf_ptr, frp->get_login ()), frp->get_real ()), frp->get_drwho_list ());
  
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
PMS_All::decode (char *, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMS_All::decode, packet_length = %d\n", packet_length);

  this->ss = new (PRIVATE_POOL) HT_Server;
  return 1;
}

/* We only want the user's real name, not the gecos junk after the first leading ','. 
   However, if the real-name is not in the password file, just return the login name 
   instead. */

Protocol_Record *
PMS_All::insert_protocol_info (Protocol_Record &protocol_record)
{
  Protocol_Record *frp   = PM_Server::insert_protocol_info (protocol_record);
  passwd	*pwent = getpwnam (frp->get_login ());
  char		*cp;

  if ((cp = strchr (frp->set_real (pwent == 0 ? frp->get_login () : ACE::strnew (pwent->pw_gecos)), ',')) != 0)
    *cp = '\0';

  return frp;
}

#ifndef __OPTIMIZE__
PMS_All::PMS_All (void)
{}
#endif /* __OPTIMIZE__ */
