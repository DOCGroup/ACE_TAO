// $Id$
#include "global.h"
#include "new.h"
#include "Options.h"
#include "HT_Client.h"
#include "PMC_Ruser.h"

/* This function is pretty much a no-op that just sets up the appropriate
   lookup function to use. */

int
PMC_Ruser::encode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "in PMC_Ruser::encode\n");

  this->ss = new (PRIVATE_POOL) HT_Client;

  SET_PACKET_TYPE (packet, Options::PROTO_RUSER);

  char *buf_ptr = SKIP_PACKET_TYPE (packet);

  *buf_ptr++ = char (Options::get_opt (Options::PRINT_LOGIN_NAME));

  packet_length = buf_ptr - packet;
  return 1;
}

/* This method is responsible for transforming the msg from the server
   back into a form usable by the client.  Note that it reads the REAL_NAME
   from the packet (since the server placed it there)... */
   
int
PMC_Ruser::decode (char *packet, int &packet_length)
{
  if (Options::get_opt (Options::DEBUG) != 0)
    {
      fprintf (stderr, "in PMC_Ruser::decode, packet_length = %d\n", packet_length);
      write (2, packet, packet_length);
      putc ('\n', stderr);
    }
  char 	*cp		= packet;
  int 	remote_users	= 0;

  sscanf (cp, "Users   %d", &remote_users);

  this->increment_total_users (remote_users);

  for (cp = ACE::strend (cp); *cp != '\n'; cp++)
    {
      char *host_name = cp;

      for (cp = ACE::strend (cp); *(cp = this->handle_protocol_entries (cp, host_name)) != '\t'; )
	;
    }

  return 1;
}

Protocol_Record *
PMC_Ruser::insert_protocol_info (Protocol_Record &protocol_record)
{
  Protocol_Record *frp	      = this->ss->insert (protocol_record.get_host (), MAXHOSTNAMELEN);
  Drwho_Node	*current_node = protocol_record.get_drwho_list ();
  Drwho_Node	*np	      = this->get_drwho_node (ACE::strnnew (current_node->get_login_name (), MAXUSERIDNAMELEN), frp->drwho_list);
  int		length	      = strlen (frp->get_host ());

  np->set_real_name (ACE::strnew (current_node->get_real_name ()));

  if (np->get_active_count () < current_node->get_active_count ())
    np->set_active_count (current_node->get_active_count ());
  if (np->get_inactive_count () < current_node->get_inactive_count())
    np->set_inactive_count (current_node->get_inactive_count ());

  if (length > this->max_key_length)
    this->max_key_length = length;

  return frp;
}
  
char *
PMC_Ruser::handle_protocol_entries (char *cp, char *host_name, char *)
{
  static Protocol_Record protocol_record (1);
  auto	 Drwho_Node    *current_node = protocol_record.get_drwho_list ();
  
  protocol_record.set_host (host_name);
  current_node->set_inactive_count (atoi (cp));
  current_node->set_active_count (atoi (cp = strchr (cp, ' ') + 1));
  current_node->set_login_name (cp = strchr (cp, ' ') + 1);
  current_node->set_real_name (cp = strchr (cp, '\0') + 1);

  this->insert_protocol_info (protocol_record);

  return ACE::strend (cp);
}

void
PMC_Ruser::process (void)
{
  auto char *(Drwho_Node::*get_name)(void) = 
    Options::get_opt (Options::PRINT_LOGIN_NAME) ? &Drwho_Node::get_login_name : &Drwho_Node::get_real_name;

  for (Protocol_Record *frp; (frp = this->Protocol_Manager::get_each_friend ()) != 0; )
    {
      printf ("%-*s ", this->max_key_length, frp->get_host ());

      for (Drwho_Node *np = frp->get_drwho_list (); ;)
	{
	  fputs ((np->*get_name) (), stdout);
	  
	  if (np->get_inactive_count () != 0)
	    {
	      if (np->get_active_count () != 0)
		printf ("*(%d)", np->get_active_count ());
	    }
	  else if (np->get_active_count () > 1)
	    printf ("*(%d)", np->get_active_count ());
	  else if (np->get_active_count () == 1)
	    putchar ('*');

	  if ((np = np->next) == 0)
	    break;
	  else if (Options::get_opt (Options::PRINT_LOGIN_NAME))
	    putchar (' ');
	  else
	    printf (", ");
	}
      
      putchar ('\n');
    }
}

#ifndef __OPTIMIZE__
PMC_Ruser::PMC_Ruser (void)
{}
#endif /* __OPTIMIZE__ */
