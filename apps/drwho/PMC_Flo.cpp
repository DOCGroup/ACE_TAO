// $Id$
#include "Options.h"
#include "new.h"
#include "BS_Client.h"
#include "PMC_Flo.h"

/* This function "encodes" a list of friends by putting the userid's in
   a contiguous block.  This block can then be transmitted over to the
   network to servers on other subnets.  Several things are added to
   make decoding easier on the other end:

   * A count of the number of friends is prepended (assumption: there
     are no more than 9999999 friends... ;-))
   * The login userids are separated by a single space. */

int
PMC_Flo::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMC_Flo::encode");

  this->ss = new (PRIVATE_POOL) BS_Client;

  SET_PACKET_TYPE (packet, Options::PROTO_FLO);
  char	*buf_ptr = SKIP_PACKET_TYPE (packet);

  sprintf (buf_ptr, "%d", this->friend_count ());

  buf_ptr += MAXUSERIDNAMELEN;

  /* Loop through all the friends, copying them into the packet buffer. */

  for (Protocol_Record *frp; (frp = this->get_next_friend ()) != 0; )
    buf_ptr = ACE::strecpy (buf_ptr, frp->get_login ());

  packet_length = buf_ptr - packet;

  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, ", packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  return 1;
}

/* This method is responsible for transforming the msg from the server
   back into a form usable by the client. */
   
int
PMC_Flo::decode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, "in PMC_Flo::decode, packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  char 	*cp		= packet;
  int 	remote_users	= 0;

  sscanf (cp, "Users   %d", &remote_users);

  this->increment_total_users (remote_users);

  for (cp = ACE::strend (cp); *cp != '\n'; cp++)
    {
      char *login_name = cp;

      for (cp = ACE::strend (cp); *(cp = this->handle_protocol_entries (cp, login_name)) != '\t'; )
	;
    }

  return 1;
}

Protocol_Record *
PMC_Flo::insert_protocol_info (Protocol_Record &protocol_record)
{
  Protocol_Record *frp	= PM_Client::insert_protocol_info (protocol_record);
  int		length	= strlen (frp->get_real ());

  if (length > this->max_key_length)
    this->max_key_length = length;

  return frp;
}

void
PMC_Flo::process (void)
{
  printf ("remote friends logged on\n");
  PM_Client::process ();
}

#ifndef __OPTIMIZE__
PMC_Flo::PMC_Flo (void)
{}
#endif /* __OPTIMIZE__ */
