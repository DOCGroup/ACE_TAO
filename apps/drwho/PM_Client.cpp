// $Id$
#include "Options.h"
#include "PM_Server.h"
#include "PM_Client.h"

// This function is used to merge the LOGIN_NAME from server HOST_NAME
// into the userids kept on the client's side.  Note that we must
// allocate memory for HOST_NAME...

Protocol_Record *
PM_Client::insert_protocol_info (Protocol_Record &protocol_record)
{
  Protocol_Record *frp = this->ss->insert (protocol_record.get_login ());
  Drwho_Node *current_node = protocol_record.get_drwho_list ();
  Drwho_Node *np = this->get_drwho_node (ACE::strnew (current_node->get_host_name ()),
                                         frp->drwho_list);

  // Update the active and inactive counts.
  
  if (np->get_active_count () < current_node->get_active_count ())
    {
      np->set_active_count (current_node->get_active_count ());
      frp->is_active 	= 1;
    }

  if (np->get_inactive_count () < current_node->get_inactive_count())
    np->set_inactive_count (current_node->get_inactive_count ());

  return frp;
}

// This routine does all the dirty work, and actually prints out the
// friends info in a nicely formatted manner.

void
PM_Client::process (void)
{
  char *(Protocol_Record::*get_name)(void);
  
  if (Options::get_opt (Options::PRINT_LOGIN_NAME))
    get_name = &Protocol_Record::get_login;
  else
    get_name = &Protocol_Record::get_real;

  int active_friends = 0;
  int users = this->Protocol_Manager::get_total_users ();

  ACE_DEBUG ((LM_DEBUG,
              "------------------------\n"));
  
  if (Options::get_opt (Options::PRINT_LOGIN_NAME))
    this->max_key_length = MAXUSERIDNAMELEN;

  // Goes through the queue of all the logged in friends and prints
  // out the associated information.
  
  for (Protocol_Record *frp = this->Protocol_Manager::get_each_friend ()
       frp != 0;
       frp = this->Protocol_Manager::get_each_friend ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%c%-*s [", (frp->is_active != 0 ? '*' : ' '), 
                  this->max_key_length,
                  (frp->*get_name) ()));
      
      for (Drwho_Node *np = frp->get_drwho_list (); ;)
	{
	  ACE_DEBUG ((LM_DEBUG,
                      np->get_host_name (),
                      stdout));
	  
	  active_friends += np->get_active_count ();
	  
	  if (np->get_inactive_count () != 0)
	    {
	      if (np->get_active_count () != 0)
		ACE_DEBUG ((LM_DEBUG,
                            "*(%d)",
                            np->get_active_count ()));
	    }
	  else if (np->get_active_count () > 1)
	    ACE_DEBUG ((LM_DEBUG,
                        "*(%d)",
                        np->get_active_count ()));
	  else if (np->get_active_count () == 1)
	    ACE_DEBUG ((LM_DEBUG,
                        "*"));
	  
	  if ((np = np->next) == 0)
	    break;
	  else
            ACE_DEBUG ((LM_DEBUG
                        " "));
	}
      
      ACE_DEBUG ((LM_DEBUG
                  "]\n"));
    }
  
  ACE_DEBUG ((LM_DEBUG,
              "------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, 
              "friends: %d\tusers: %d\n",
              active_friends,
              users));
}

char *
PM_Client::handle_protocol_entries (const char *cp,
                                    const char *login_name,
                                    const char *real_name)
{
  static Protocol_Record protocol_record (1);
  Drwho_Node *current_node = protocol_record.get_drwho_list ();
  
  protocol_record.set_login (login_name);
  protocol_record.set_real  (real_name);
  current_node->set_inactive_count (atoi (cp));
  current_node->set_active_count (atoi (cp = strchr (cp, ' ') + 1));
  current_node->set_host_name (cp = strchr (cp, ' ') + 1);
  
  this->insert_protocol_info (protocol_record);
  
  return ACE::strend (cp);
}

PM_Client::PM_Client (void)
  : max_key_length (0)
{
}

PM_Client::~PM_Client (void)
{
}

