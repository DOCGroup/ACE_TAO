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
  Protocol_Record (const char *key_name1,
                   Protocol_Record *next = 0);
  ~Protocol_Record (void);
  char *get_host (void);
  char *set_host (const char *str);
  char *get_login (void);
  char *set_login (const char *str);
  char *get_real (void);
  char *set_real (const char *str);
  Drwho_Node *get_drwho_list (void);

  static Drwho_Node drwho_node_;
  char *key_name1_;
  char *key_name2_;
  Drwho_Node *drwho_list_;
  Protocol_Record *next_;
  int is_active_;
};

#endif /* _PROTOCOL_RECORD_H */
