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

#ifndef VECTORS_H
#define VECTORS_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_GPERF)

class Vectors
{
  // = TITLE
  //   Static class data members that are shared between several
  //   classes via inheritance.
public:
  enum
  {
    ALPHA_SIZE = 128
  };

  static int occurrences[ALPHA_SIZE];
  // Counts occurrences of each key set character.

  static int asso_values[ALPHA_SIZE];
  // Value associated with each character.
};

#endif /* ACE_HAS_GPERF */
#endif /* VECTORS_H */
