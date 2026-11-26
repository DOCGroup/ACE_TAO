/**
 *  @file    Protocol_Record.h
 *
 *  @author Douglas C. Schmidt
 */

#ifndef _PROTOCOL_RECORD_H
#define _PROTOCOL_RECORD_H

#include "Drwho_Node.h"

/**
 * @class Protocol_Record
 *
 * @brief Stores information about a single friend's status.
 */
class Protocol_Record
{
public:
  Protocol_Record ();
  explicit Protocol_Record (int use_dummy);
  explicit Protocol_Record (char const *key_name1,
                            Protocol_Record *next = nullptr);
  ~Protocol_Record ();
  Protocol_Record (Protocol_Record const &) = delete;
  Protocol_Record (Protocol_Record &&) = delete;
  Protocol_Record &operator= (Protocol_Record const &) = delete;
  Protocol_Record &operator= (Protocol_Record &&) = delete;
  char const *get_host ();
  char const *set_host (char const *str);
  char const *get_login ();
  char const *set_login (char const *str);
  char const *get_real ();
  char const *set_real (char const *str);
  Drwho_Node *get_drwho_list ();

  static Drwho_Node drwho_node_;
  char const *key_name1_ {};
  char const *key_name2_ {};
  Drwho_Node *drwho_list_ {};
  Protocol_Record *next_ {};
  bool is_active_ {};
};

#endif /* _PROTOCOL_RECORD_H */
