// $Id$
#include "global.h"
#include "new.h"
#include "Options.h"
#include "HT_Client.h"
#include "PMC_All.h"

/* This function is pretty much a no-op that just sets up the appropriate
   lookup function to use. */

int
PMC_All::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMC_All::encode\n");

  this->ss = new (PRIVATE_POOL) HT_Client;

  SET_PACKET_TYPE (packet, Options::PROTO_ALL);

  char *buf_ptr = SKIP_PACKET_TYPE (packet);

  packet_length = buf_ptr - packet;
  return 1;
}

/* This method is responsible for transforming the msg from the server
   back into a form usable by the client.  Note that it reads the REAL_NAME
   from the packet (since the server placed it there)... */
   
int
PMC_All::decode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, "in PMC_All::decode, packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  char 	*cp		= packet;
  int 	remote_users	= 0;

  sscanf (cp, "Users   %d", &remote_users);

  this->increment_total_users (remote_users);

  for (cp = ACE::strend (cp); *cp != '\n'; cp++)
    {
      /* Skip over the LOGIN_NAME. */

      char *login_name = cp;
      char *real_name  = cp = ACE::strend (cp);

      for (cp = ACE::strend (cp); 
	   *(cp = this->handle_protocol_entries (cp, login_name, real_name)) != '\t'
	   ;)
	;
    }

  return 1;
}

Protocol_Record *
PMC_All::insert_protocol_info (Protocol_Record &protocol_record)
{
  Protocol_Record *frp   = PM_Client::insert_protocol_info (protocol_record);
  int		length = strlen (frp->set_real (ACE::strnew (protocol_record.get_real ())));
	
  if (length > this->max_key_length)
    this->max_key_length = length;

  return frp;
}
  
void
PMC_All::process (void)
{
  printf ("remote users logged on\n");
  PM_Client::process ();
}

#ifndef __OPTIMIZE__
PMC_All::PMC_All (void)
{}
#endif /* __OPTIMIZE__ */
