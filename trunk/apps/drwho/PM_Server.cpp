// $Id$

#include "Options.h"
#include "Rwho_DB_Manager.h"
#include "PM_Server.h"

// This is the main method for the server side of things.  It reads
// the RWHO file on the local machine and inserts HOST_NAME
// information for each LOGIN_NAME that is a friend into the
// DRWHO_LIST.  This function is also responsible for determining
// whether a given LOGIN_NAME is currently idle or not.

int
PM_Server::process (void)
{
  RWho_DB_Manager  ru;
  Protocol_Record protocol_record (1);

  while (ru.get_next_user (protocol_record) > 0)
    this->insert_protocol_info (protocol_record);
  
  return 1;
}

// Insert the HOST_NAME into the server's lookup table on behalf of
// user LOGIN_NAME.  Note that we need to allocate memory for
// HOST_NAME...

Protocol_Record *
PM_Server::insert_protocol_info (Protocol_Record &protocol_record)
{
  Protocol_Record *frp = this->ss->insert (protocol_record.get_login ());

  Drwho_Node *current_node = protocol_record.get_drwho_list ();

  if (current_node->get_idle_time () < MAX_USER_TIMEOUT)
        this->increment_total_users ();

  if (frp)
    {
      Drwho_Node *np =
        this->get_drwho_node (ACE::strnew (current_node->get_host_name ()),
                              frp->drwho_list);

      if (current_node->get_idle_time () >= MAX_USER_TIMEOUT)
	np->inactive_count++;
      else
	np->active_count++;
    }

  return frp;
}

// Put the inactive and active counts, plus the hostname into the
// packet.

char *
PM_Server::handle_protocol_entries (char *buf_ptr, Drwho_Node *np)
{

  for (; np != 0; np = np->next)
    {
      sprintf (buf_ptr,
               "%d %d %s",
               np->get_inactive_count (),
               np->get_active_count (),
               np->get_host_name ());
      buf_ptr += strlen (buf_ptr) + 1;
    }

  return buf_ptr;
}     

PM_Server::PM_Server (void)
{
}

PM_Server::~PM_Server (void)
{
}
