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

#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Options.h"

#if defined (ACE_HAS_GPERF)
class List_Node
{
  // = TITLE
  //   Data and function members for defining values and operations of
  //   a list node.
public:
  // = Initialization and termination methods.
  List_Node (char *key, int len);
  // Constructor.

  ~List_Node (void);
  // Destructor.

  static void sort (char *base, int len);

  List_Node *link;
  // TRUE if key has an identical KEY_SET as another key.

  List_Node *next;
  // Points to next element on the list.

  char *key;
  // Each keyword string stored here.

  char *rest;
  // Additional information for building hash function.

  char *keysig;
  // Set of characters to hash, specified by user.

  int length;
  // Length of the key.

  int hash_value;
  // Hash value for the key.

  int occurrence;
  // A metric for frequency of key set occurrences.

  int slot;
  // Position of this node relative to other nodes.
};

#endif /* ACE_HAS_GPERF */
#endif /* LIST_NODE_H */
