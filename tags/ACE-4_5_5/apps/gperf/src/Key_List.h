/* -*- C++ -*- */
// $Id$

/* Copyright (C) 1989 Free Software Foundation, Inc.
   written by Douglas C. Schmidt (schmidt@ics.uci.edu)

This file is part of GNU GPERF.

GNU GPERF is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 1, or (at your option) any
later version.

GNU GPERF is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU GPERF; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111,
USA.  */

#if !defined (KEY_LIST_H)
#define KEY_LIST_H

#include "Options.h"
#include "List_Node.h"

class Key_List 
{
  // = TITLE
  //   Data and function member declarations for the keyword list class.
  //
  // = DESCRIPTION
  //   The key word list is a useful abstraction that keeps track of
  //   various pieces of information that enable that fast generation of
  //   the Gen_Perf.hash function.  A Key_List is a singly-linked list
  //   of List_Nodes.
public:
  Key_List   (void);
  ~Key_List  (void);
  int keyword_list_length (void);
  int max_key_length (void);
  void reorder (void);
  void sort (void);
  void read_keys (void);
  void output (void);

  List_Node *head;                                  
  // Points to the head of the linked list. 

  int total_duplicates; 
  // Total number of duplicate hash values. 

private:
  static int  get_occurrence (List_Node *ptr);
  static int  already_determined (List_Node *ptr);
  static void set_determined (List_Node *ptr);

  // @@ All of the following methods should be factored out and
  // replaced by the use of the Strategy/Bridge pattern so that we can
  // easily add new languages.
  void output_min_max (void);
  void output_switch (void);
  void output_keyword_table (void);
  void output_keylength_table (void);
  void output_hash_function (void);
  void output_lookup_function (void);
  void output_lookup_array (void);
  void output_strcasecmp (void);  
  void set_output_types (void);
  void dump (void); 
  char *get_array_type (void);
  char *save_include_src (void);
  char *get_special_input (char delimiter);
  List_Node *merge (List_Node *list1, List_Node *list2);
  List_Node *merge_sort (List_Node *head);

  char *array_type;		
  // Pointer to the type for word list. 

  char *return_type;		
  // Pointer to return type for lookup function. 

  char *struct_tag;		
  // Shorthand for user-defined struct tag type. 

  char *include_src;		
  // C source code to be included verbatim. 

  int max_key_len;		
  // Maximum length of the longest keyword. 

  int min_key_len;		
  // Minimum length of the shortest keyword. 

  int min_hash_value;		
  // Minimum hash value for all keywords. 

  int max_hash_value;
  // Maximum hash value for all keywords. 

  int occurrence_sort;		
  // True if sorting by occurrence. 

  int hash_sort;		
  // True if sorting by hash value. 

  int additional_code;		
  // True if any additional C code is included. 

  int list_len;			
  // Length of head's Key_List, not counting duplicates. 

  int total_keys;		
  // Total number of keys, counting duplicates.
};

#endif /* KEY_LIST_H */
