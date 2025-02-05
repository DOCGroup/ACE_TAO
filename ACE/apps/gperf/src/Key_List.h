// -*- C++ -*-

/**
 * Copyright (C) 1989 Free Software Foundation, Inc.
 * written by Douglas C. Schmidt (d.schmidt@vanderbilt.edu)
 *
 * This file is part of GNU GPERF.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef KEY_LIST_H
#define KEY_LIST_H

#include "Options.h"

#include "List_Node.h"
#include "Vectors.h"

/**
 * Describes a duplicate entry.
 *
 * This is used for generating code by the <Key_List>.
 */
class Duplicate_Entry
{
public:
  Duplicate_Entry () = default;
  Duplicate_Entry (const Duplicate_Entry &) = delete;
  Duplicate_Entry (Duplicate_Entry &&) = delete;
  Duplicate_Entry &operator= (const Duplicate_Entry &) = delete;
  Duplicate_Entry &operator= (Duplicate_Entry &&) = delete;
  ~Duplicate_Entry () = default;

  /// Hash value for this particular duplicate set.
  int hash_value;

  /// Slot into the main keyword storage array.
  int slot;

  /// Number of consecutive duplicates at this slot.
  int count;
};

/**
 * Data and function member declarations for the keyword list class.
 *
 * The key word list is a useful abstraction that keeps track of
 * various pieces of information that enable that fast generation of
 * the Gen_Perf.hash function.  A Key_List is a singly-linked list
 * of List_Nodes.
 */
class Key_List
{
public:
  Key_List ();
  Key_List (const Key_List &) = delete;
  Key_List (Key_List &&) = delete;
  Key_List &operator= (const Key_List &) = delete;
  Key_List &operator= (Key_List &&) = delete;
  ~Key_List ();
  int keyword_list_length ();
  int max_key_length ();
  void reorder ();
  void sort ();
  void string_sort ();
  int read_keys ();
  int output ();

  /// Points to the head of the linked list.
  List_Node *head;

  /// Total number of duplicate hash values.
  int total_duplicates;

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
  void output_min_max ();
  void output_switch (int use_keyword_table = 0);
  void output_keyword_table ();
  void output_keylength_table ();
  void output_hash_function ();
  void output_lookup_function ();
  int output_binary_search_function();
  int output_linear_search_function ();
  int output_lookup_array ();
  void output_strcasecmp ();
  int output_types ();
  void dump ();
  char *array_type ();
  char *save_include_src ();
  char *special_input (char delimiter);
  List_Node *merge (List_Node *list1, List_Node *list2);
  List_Node *merge_sort (List_Node *head);
  int count_duplicates (List_Node *link, const char *type);
  void update_lookup_array (int lookup_array[],
                            int i1,
                            int i2,
                            Duplicate_Entry *dup_ptr,
                            int value);
  /// Pointer to the type for word list.
  char *array_type_;

  // Pointer to return type for lookup function.
  char *return_type;

  /// Shorthand for user-defined struct tag type.
  char *struct_tag;

  /// C source code to be included verbatim.
  char *include_src;

  /// Maximum length of the longest keyword.
  int max_key_len;

  /// Minimum length of the shortest keyword.
  int min_key_len;

  /// Minimum hash value for all keywords.
  int min_hash_value;

  /// Maximum hash value for all keywords.
  int max_hash_value;

  /// True if sorting by occurrence.
  int occurrence_sort;

  /// True if sorting by hash value.
  int hash_sort;

  /// True if sorting by key value.
  int key_sort;

  /// True if any additional C code is included.
  int additional_code;

  /// Length of head's Key_List, not counting duplicates.
  int list_len;

  /// Total number of keys, counting duplicates.
  int total_keys;

  /// Default type for generated code.
  static const char *const default_array_type;

  /// in_word_set return type, by default.
  static const char *const default_return_type;

  /// How wide the printed field width must be to contain the maximum
  /// hash value.
  static int field_width;

  /// Sets the slot location for all keysig characters that are now
  /// determined.
  static int determined_[ACE_STANDARD_CHARACTER_SET_SIZE];
};

#endif /* KEY_LIST_H */
