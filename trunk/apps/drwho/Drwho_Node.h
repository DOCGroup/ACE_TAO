/* -*- C++ -*- */
// $Id$

/* Stores information about a host for a specific friend. */

#ifndef _DRWHO_NODE_H
#define _DRWHO_NODE_H

#include "ace/OS.h"
#include "global.h"

struct Drwho_Node
{
  char 		*key_name1;
  char		*key_name2;
  char		*tty_name;
  int		idle_time;
  int 		active_count;
  int 		inactive_count;
  Drwho_Node 	*next;

                Drwho_Node (char *host, Drwho_Node *next);
		Drwho_Node (void);
  int		get_active_count (void);
  int		get_inactive_count (void);
  int		set_active_count (int count);
  int		set_inactive_count (int count);
  int		set_idle_time (int idle_time);
  int		get_idle_time (void);
  char		*get_host_name (void);
  char		*set_host_name (char *str);
  char		*get_login_name (void);
  char		*set_login_name (char *);
  char		*get_real_name (void);
  char		*set_real_name (char *);
};
  
#ifdef __OPTIMIZE__
inline 
Drwho_Node::Drwho_Node (char *h_name, Drwho_Node *n)
     : next (n), inactive_count (0), active_count (0), key_name1 (h_name), key_name2 (0)
{}

inline
Drwho_Node::Drwho_Node (void)
     : next (0), inactive_count (0), active_count (0), key_name1 (0), tty_name (0), idle_time (0), key_name2 (0)
{}

inline char *
Drwho_Node::get_login_name (void)
{
  return this->key_name1;
}

inline char *
Drwho_Node::set_login_name (char *str)
{
  return this->key_name1 = str;
}

inline char *
Drwho_Node::get_real_name (void)
{
  return this->key_name2;
}

inline char *
Drwho_Node::set_real_name (char *str)
{
  return this->key_name2 = str;
}

inline char *
Drwho_Node::get_host_name (void)
{
  return this->key_name1;
}

inline char *
Drwho_Node::set_host_name (char *str)
{
  return this->key_name1 = str;
}

inline int
Drwho_Node::get_active_count (void)
{
  return this->active_count;
}

inline int
Drwho_Node::get_inactive_count (void)
{
  return this->inactive_count;
}

inline int
Drwho_Node::set_active_count (int count)
{
  return this->active_count = count;
}

inline int
Drwho_Node::set_inactive_count (int count)
{
  return this->inactive_count = count;
}

inline int
Drwho_Node::set_idle_time (int idle_time)
{
  return this->idle_time = idle_time;
}

inline int
Drwho_Node::get_idle_time (void)
{
  return this->idle_time;
}
#endif /* __OPTIMIZE__ */
#endif /* _DRWHO_NODE_H */
