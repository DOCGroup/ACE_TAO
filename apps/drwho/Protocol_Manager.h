/* -*- C++ -*- */
// $Id$

/* A base class that consolidates friend management functionality
   shared by both clients and servers. */ 

#ifndef _FRIEND_MANAGER_H
#define _FRIEND_MANAGER_H

#include "ace/OS.h"
#include "Options.h"
#include "Search_Struct.h"
#include "Protocol_Record.h"

class Protocol_Manager
{
protected:
  int                   	total_users;
  Search_Struct			*ss;

  int				friend_count (void);

  Drwho_Node			*get_drwho_node (char *host_name, Drwho_Node *&head);
  int				get_total_users (void);
  void				increment_total_users (int remote_users = 1);

  Protocol_Record		*get_next_friend (void);
  Protocol_Record		*get_each_friend (void);

  virtual Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record) = 0;

public:
	                	Protocol_Manager (void);
  virtual			~Protocol_Manager (void);

  virtual int			encode (char *packet, int &total_bytes) = 0;
  virtual int			decode (char *packet, int &total_bytes) = 0;
};

#ifdef __OPTIMIZE__
inline 
Protocol_Manager::Protocol_Manager (void): total_users (0)
{}

inline
Protocol_Manager::~Protocol_Manager (void)
{
  if (Options::DEBUG)
    fprintf (stderr, "disposing Protocol_Manager\n");
}

/* Returns the next friend in the sequence of sorted friends. */

inline Protocol_Record *
Protocol_Manager::get_next_friend (void)
{
  return this->ss->get_next_entry ();
}

inline Protocol_Record *
Protocol_Manager::get_each_friend (void)
{
  return this->ss->get_each_entry ();
}

/* Returns the number of friends. */

inline int
Protocol_Manager::friend_count (void)
{
  return this->ss->n_elems ();
}

inline int 
Protocol_Manager::get_total_users (void)
{
  return this->total_users;
}

inline void			
Protocol_Manager::increment_total_users (int remote_users)
{
  this->total_users += remote_users;
}
#endif /* __OPTIMIZE__ */
#endif /* _FRIEND_MANAGER_H */
