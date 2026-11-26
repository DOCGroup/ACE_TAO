/**
 *  @file    Drwho_Node.h
 *
 *  @author Douglas C. Schmidt
 */

#ifndef _DRWHO_NODE_H
#define _DRWHO_NODE_H

#include "global.h"

/**
 * @class Drwho_Node
 *
 * @brief Stores information about a host for a specific friend.
 */
class Drwho_Node
{
public:
  Drwho_Node (char const *host, Drwho_Node *next);
  Drwho_Node ();

  int get_active_count ();
  int get_inactive_count ();
  int set_active_count (int count);
  int set_inactive_count (int count);
  int set_idle_time (int idle_time);
  int get_idle_time ();
  char const *get_host_name ();
  char const *set_host_name (char const *str);
  char const *get_login_name ();
  char const *set_login_name (char const *);
  char const *get_real_name ();
  char const *set_real_name (char const *);

  char const *key_name1_ {};
  char const *key_name2_ {};
  char const *tty_name_ {};
  int idle_time_ {};
  int active_count_ {};
  int inactive_count_ {};
  Drwho_Node *next_ {};
};

#endif /* _DRWHO_NODE_H */
