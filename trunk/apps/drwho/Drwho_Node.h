/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    Drwho_Node.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#if !defined (_DRWHO_NODE_H)
#define _DRWHO_NODE_H

#include "global.h"

class Drwho_Node
{
  // = TITLE
  //   Stores information about a host for a specific friend.
public:
  Drwho_Node (char *host, Drwho_Node *next);
  Drwho_Node (void);

  int get_active_count (void);
  int get_inactive_count (void);
  int set_active_count (int count);
  int set_inactive_count (int count);
  int set_idle_time (int idle_time);
  int get_idle_time (void);
  char *get_host_name (void);
  char *set_host_name (char *str);
  char *get_login_name (void);
  char *set_login_name (char *);
  char *get_real_name (void);
  char *set_real_name (char *);

  char *key_name1;
  char *key_name2;
  char *tty_name;
  int idle_time;
  int active_count;
  int inactive_count;
  Drwho_Node *next;
};
  
#endif /* _DRWHO_NODE_H */
