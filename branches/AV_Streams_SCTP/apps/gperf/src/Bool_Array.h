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

#ifndef BOOL_ARRAY_H
#define BOOL_ARRAY_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Options.h"

#if defined (ACE_HAS_GPERF)

class Bool_Array
{
  // = TITLE
  //   Efficient lookup table abstraction implemented as a "Generation
  //   Number" Array.
  //
  // = DESCRIPTION
  //   Uses a "Generation Numbering" implementation to minimize
  //   initialization time.
public:
  // = Initialization and termination methods.
  Bool_Array (void);
  // Constructor

  int open (u_long);
  // Initialize the array (requires O(n) time).

  ~Bool_Array (void);
  // Destructor.

  int find (u_long value);
  // Locate the <value> in the array (requires O(1) time).

  void reset (void);
  // Reinitializes the array (requires O(1) time).

private:
  unsigned long *storage_array_;
  // Initialization of the index space.

  unsigned long generation_number_;
  // Keep track of the current Generation.

  unsigned long size_;
  // Keep track of array size.
};

#endif /* ACE_HAS_GPERF */
#endif /* BOOL_ARRAY_H */
