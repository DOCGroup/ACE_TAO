// -*- C++ -*-

// $Id$

// Copyright (C) 1989 Free Software Foundation, Inc.
// written by Douglas C. Schmidt (schmidt@cs.wustl.edu)

// This file is part of GNU GPERF.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef KEY_LIST_H
#define KEY_LIST_H

#include "Options.h"
#include "List_Node.h"
#include "Vectors.h"

#if defined (ACE_HAS_GPERF)

class Duplicate_Entry
{
  // = TITLE
  //     Describes a duplicate entry.
  //
  // = DESCRIPTION
  //     This is used for generating code by the <Key_List>.
public:
  int hash_value;
  // Hash value for this particular duplicate set.

  int index;
  // Index into the main keyword storage array.

  int count;
  // Number of consecutive duplicates at this index.
};

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
  Key_List (void);
  ~Key_List (void);
  int keyword_list_length (void);
  int max_key_length (void);
  void reorder (void);
  void sort (void);
  void string_sort (void);
  int read_keys (void);
  int output (void);

  List_Node *head;
  // Points to the head of the linked list.

  int total_duplicates;
  // Total number of duplicate hash values.

private:
  // = Make hash table 10 times larger than # of keyword entries.
  enum
  {
    TABLE_MULTIPLE = 10
  };

  static int occurrence (List_Node *ptr);
  static int already_determined (List_Node *ptr);
  static void determined (List_Node *ptr);

  // @@ All of the following methods should be factored out and
  // replaced by the use of the Strategy/Bridge pattern so that we can
  // easily add new languages.
  void output_min_max (void);
  void output_switch (int use_keyword_table = 0);
  void output_keyword_table (void);
  void output_keylength_table (void);
  void output_hash_function (void);
  void output_lookup_function (void);
  int output_binary_search_function(void);
  int output_linear_search_function (void);
  int output_lookup_array (void);
  void output_strcasecmp (void);
  int output_types (void);
  void dump (void);
  char *array_type (void);
  char *save_include_src (void);
  char *special_input (char delimiter);
  List_Node *merge (List_Node *list1, List_Node *list2);
  List_Node *merge_sort (List_Node *head);
  int count_duplicates (List_Node *link, const char *type);
  void update_lookup_array (int lookup_array[],
                            int i1,
                            int i2,
                            Duplicate_Entry *dup_ptr,
                            int value);
  char *array_type_;
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

  int key_sort;
  // True if sorting by key value.

  int additional_code;
  // True if any additional C code is included.

  int list_len;
  // Length of head's Key_List, not counting duplicates.

  int total_keys;
  // Total number of keys, counting duplicates.

  static char *const default_array_type;
  // Default type for generated code.

  static char *const default_return_type;
  // in_word_set return type, by default.

  static int field_width;
  // How wide the printed field width must be to contain the maximum
  // hash value.

  static int determined_[Vectors::ALPHA_SIZE];
  // Sets the index location for all keysig characters that are now
  // determined.
};

#endif /* ACE_HAS_GPERF */
#endif /* KEY_LIST_H */
