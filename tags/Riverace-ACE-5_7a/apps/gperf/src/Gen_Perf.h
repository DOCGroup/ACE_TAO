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

#ifndef GEN_PERF_H
#define GEN_PERF_H

#include "Options.h"
#include "Key_List.h"
#include "Bool_Array.h"

#if defined (ACE_HAS_GPERF)

class Gen_Perf
{
  // = TITLE
  //   Provides high-level routines to manipulate the keyword list
  //   structures the code generation output.
public:
  // = Initialization and termination methods.
  Gen_Perf (void);
  // Constructor.

  ~Gen_Perf (void);
  // Destructor

  int run (void);
  // Attempt to generate a perfect hash function.

private:
  int open (void);
  int change (List_Node *prior, List_Node *curr);
  int affects_prev (char c, List_Node *curr);
  int compute_perfect_hash (void);
  int compute_binary_search (void);
  int compute_linear_search (void);
  static int hash (List_Node *key_node);
  static int compute_disjoint_union (char *s1, char *s2, char *s3);
  static void sort_set (char *union_set, int len);

  int max_hash_value;
  // Maximum possible hash value.

  int fewest_collisions;
  // Records fewest # of collisions for asso value.

  int num_done;
  // Number of keywords processed without a collision.

  char *union_set;
  // Disjoint union.

  Key_List key_list;
  // List of the keys we're trying to map into a perfect hash
  // function.

  Bool_Array char_search;
  // Table that keeps track of key collisions.
};

#endif /* ACE_HAS_GPERF */
#endif /* GEN_PERF_H */
