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

#if !defined (HASH_TABLE_H)
#define HASH_TABLE_H

#include "Options.h"
#include "List_Node.h"

// @@ This class should be replaced by something in ACE, e.g.,
// ACE_Hash_Map_Manager.  Perhaps we should implement a new
// ACE_Hash_Map that uses double hashing, however!
class Hash_Table 
{
  // = TITLE
  //   Hash table used to check for duplicate keyword entries.
  // 
  // = DESCRIPTION
  //   This implementation uses "double hashing."
public:
  Hash_Table (List_Node **t,
              int s);

  ~Hash_Table (void);

  List_Node *operator () (List_Node *item, int ignore_length);

private:
  List_Node **table;
  // Vector of pointers to linked lists of List_Node's. 

  int size;
  // Size of the vector. 

  int collisions; 
  // Find out how well our double hashing is working!
};

#endif /* HASH_TABLE_H */
