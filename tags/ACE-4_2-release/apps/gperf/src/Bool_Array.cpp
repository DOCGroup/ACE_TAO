/* Fast lookup table abstraction implemented as an Iteration Number Array
// $Id$

   Copyright (C) 1989 Free Software Foundation, Inc.
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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA.  */

#include "Bool_Array.h"

// Prints out debugging diagnostics.

Bool_Array::~Bool_Array (void) 
{
  if (option[DEBUG])
    fprintf (stderr, "\ndumping boolean array information\n"
             "size = %d\niteration number = %d\nend of array dump\n",
             size, generation_number);
}

Bool_Array::Bool_Array (void)
  : storage_array (0), 
    generation_number (0),
    size (0)
{
}

void
Bool_Array::init (STORAGE_TYPE *buffer, STORAGE_TYPE s)
{
  size = s;
  generation_number = 1;
  storage_array    = buffer;

  memset (storage_array, 0, s * sizeof *storage_array);

  if (option[DEBUG])
    fprintf (stderr, "\nbool array size = %d, total bytes = %d\n",
             size, size * sizeof *storage_array);
}

int  
Bool_Array::find (int index) 
{
  if (storage_array[index] == generation_number)
    return 1;
  else
    {
      storage_array[index] = generation_number;
      return 0;
    }
}

void 
Bool_Array::reset (void) 
{ 
  if (++generation_number == 0)
    {
      if (option[DEBUG])
        {
          fprintf (stderr, "(re-initializing bool_array)...");
          fflush (stderr);
        }

      generation_number = 1;
      memset (storage_array, 0, size * sizeof *storage_array);

      if (option[DEBUG])
        {
          fprintf (stderr, "done\n");
          fflush (stderr);
        }
    }
}

