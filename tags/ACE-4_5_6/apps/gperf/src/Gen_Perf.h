/* -*- C++ -*- */
// $Id$

/* Copyright (C) 1989 Free Software Foundation, Inc.
   written by Douglas C. Schmidt (schmidt@ics.uci.edu)

This file is part of GNU GPERF.

GNU GPERF is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU GPERF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU GPERF; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111, USA. */

#if !defined (GEN_PERF_H)
#define GEN_PERF_H 

#include "Options.h"
#include "Key_List.h"
#include "Bool_Array.h"

class Gen_Perf
{
  // = TITLE
  //   Provides high-level routines to manipulate the keyword list
  //   structures the code generation output.  
public:
  Gen_Perf (void);
  ~Gen_Perf (void);
  int generate (void);

private:
  void change (List_Node *prior, List_Node *curr);
  int affects_prev (char c, List_Node *curr);
  static int hash (List_Node *key_node);
  static int compute_disjoint_union (char *set_1, char *set_2, char *set_3);
  static void sort_set (char *union_set, int len);
  
  int max_hash_value;    
  // Maximum possible hash value.

  int fewest_collisions; 
  // Records fewest # of collisions for asso value.
  
  int num_done;          
  // Number of keywords processed without a collision.

  Bool_Array char_search;
  // Table that keeps track of key collisions.
  
  Key_List key_list;
  // List of the keys we're trying to map into a perfect hash
  // function.
};

#endif /* GEN_PERF_H */
