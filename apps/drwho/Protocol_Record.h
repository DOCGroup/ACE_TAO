/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    Protocol_Record.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#if !defined (_PROTOCOL_RECORD_H)
#define _PROTOCOL_RECORD_H

#include "Drwho_Node.h"

class Protocol_Record
{
  // = TITLE
  //   Stores information about a single friend's status. 

public:
  Protocol_Record (void);
  Protocol_Record (int use_dummy);
  Protocol_Record (char *key_name1,
                   Protocol_Record *next = 0);
  ~Protocol_Record (void);
  char *get_host (void);
  char *set_host (char *str);
  char *get_login (void);
  char *set_login (char *str);
  char *get_real (void);
  char *set_real (char *str);
  Drwho_Node *get_drwho_list (void);

  static Drwho_Node drwho_node;
  char *key_name1;
  char *key_name2;
  Drwho_Node *drwho_list;
  Protocol_Record *next;
  int is_active;
};

#endif /* _PROTOCOL_RECORD_H */
