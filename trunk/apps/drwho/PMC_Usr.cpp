// $Id$
#include "Options.h"
#include "new.h"
#include "SL_Client.h"
#include "PMC_Usr.h"

int
PMC_Usr::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMC_Usr::encode");

  this->ss = new (PRIVATE_POOL) SL_Client (this->usr_name);

  SET_PACKET_TYPE (packet, Options::PROTO_USR);

  char *buf_ptr = SKIP_PACKET_TYPE (packet);

  buf_ptr = ACE::strecpy (buf_ptr, this->get_next_friend ()->get_login ());

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
PMC_Usr::decode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, "in PMC_Usr::decode, packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  
  char *cp = packet;

  if (*cp != '\n')
    {
      char *login_name = cp;

      for (cp = ACE::strend (cp); *(cp = this->handle_protocol_entries (cp, login_name)) != '\t'; )
	;
    }
  return 1;
}

void
PMC_Usr::process (void)
{
  Protocol_Record *frp = this->get_each_friend ();
  Drwho_Node	*np  = frp->get_drwho_list ();
  
  if (np == 0)
    fputs ("<unknown>", stdout);
  else
    {

      /* First try to get a login session that is active... */
      
      for (; np != 0; np = np->next)
	if (np->active_count > 0)
	  {
	    printf ("%s ", np->get_host_name ());

	    if (Options::get_opt (Options::USE_VERBOSE_FORMAT) == 0)
	      return;
	  }
      
      for (np = frp->get_drwho_list (); np != 0; np = np->next)
	if (np->active_count == 0)
	  {
	    printf ("%s ", np->get_host_name ());
	    
	    if (Options::get_opt (Options::USE_VERBOSE_FORMAT) == 0)
	      return;
	  }
    }
}

#ifndef __OPTIMIZE__
PMC_Usr::PMC_Usr (char *u_name): usr_name (u_name)
{}
#endif				/* __OPTIMIZE__ */
