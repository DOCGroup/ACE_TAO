/* -*- C++ -*- */
// $Id$

/* Stores information about a single friend's status. */

#ifndef _FRIEND_RECORD_H
#define _FRIEND_RECORD_H

#include "Drwho_Node.h"

class Protocol_Record
{
public:
  static Drwho_Node drwho_node;
  char	           *key_name1;
  char		   *key_name2;
  Drwho_Node	   *drwho_list;
  Protocol_Record	   *next;
  int		   is_active;

                   Protocol_Record (void);
		   Protocol_Record (int use_dummy);
		   Protocol_Record (char *key_name1, Protocol_Record *next = 0);
		   ~Protocol_Record (void);
  char		   *get_host (void);
  char		   *set_host (char *str);
  char		   *get_login (void);
  char		   *set_login (char *str);
  char		   *get_real (void);
  char		   *set_real (char *str);
  Drwho_Node	   *get_drwho_list (void);
};

#ifdef __OPTIMIZE__
inline
Protocol_Record::Protocol_Record (void)
     : is_active (0), drwho_list (0), key_name1 (0), key_name2 (0), next (0)
{}

inline
Protocol_Record::Protocol_Record (int use_dummy)
     : is_active (0), drwho_list (&Protocol_Record::drwho_node), key_name1 (0), key_name2 (0), next (0)
{}

inline
Protocol_Record::Protocol_Record (char *key_name1, Protocol_Record *next)
     : is_active (0), drwho_list (0), key_name2 (0)
{
  this->key_name1 = key_name1;
  this->next	  = next;
}

inline char *
Protocol_Record::get_login (void) 
{ 
  return this->key_name1; 
}

inline char *
Protocol_Record::set_login (char *str)
{ 
  return this->key_name1 = str; 
}

inline char *
Protocol_Record::get_real (void) 
{ 
  return this->key_name2; 
}

inline char *
Protocol_Record::get_host (void)
{
  return this->key_name1;
}

inline char *
Protocol_Record::set_host (char *str)
{ 
  return this->key_name1 = str; 
}

inline char *
Protocol_Record::set_real (char *str)
{
  return this->key_name2 = str;
}

inline Drwho_Node *
Protocol_Record::get_drwho_list (void)
{
  return this->drwho_list;
}
#endif /* __OPTIMIZE__ */
#endif /* _FRIEND_RECORD_H */
