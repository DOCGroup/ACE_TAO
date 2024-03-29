/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Protocol_Manager.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PROTOCOL_MANAGER_H
#define _PROTOCOL_MANAGER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Options.h"
#include "Search_Struct.h"
#include "Protocol_Record.h"

/**
 * @class Protocol_Manager
 *
 * @brief A base class that consolidates friend management functionality
 * shared by both clients and servers.
 */
class Protocol_Manager
{
public:
  Protocol_Manager ();
  virtual ~Protocol_Manager ();

  virtual int encode (char *packet, int &total_bytes) = 0;
  virtual int decode (char *packet, int &total_bytes) = 0;

protected:
  int total_users;
  Search_Struct *ss;

  int friend_count ();

  Drwho_Node *get_drwho_node (char *host_name, Drwho_Node *&head);
  int get_total_users ();
  void increment_total_users (int remote_users = 1);

  Protocol_Record *get_next_friend ();
  Protocol_Record *get_each_friend ();

  virtual Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record) = 0;
};

#endif /* _PROTOCOL_MANAGER_H */
