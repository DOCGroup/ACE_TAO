// -*- C++ -*-

// Fast lookup table abstraction implemented as an Iteration Number Array

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

#include "Bool_Array.h"
#include "ace/OS_Memory.h"
#include <algorithm>

// Prints out debugging diagnostics.

Bool_Array::~Bool_Array ()
{
  if (option[DEBUGGING])
    ACE_DEBUG ((LM_DEBUG,
                "\ndumping boolean array information\n"
                "size = %u\niteration number = %u\nend of array dump\n",
                size_,
                generation_number_));
}

Bool_Array::Bool_Array ()
  : generation_number_ (0),
    size_ (0)
{
}

int
Bool_Array::open (u_long s)
{
  this->generation_number_ = 1;
  this->size_ = s;

  u_long *storage_array {};
  ACE_NEW_RETURN (storage_array,
                  u_long[s],
                  -1);
  this->storage_array_.reset (storage_array);

  std::fill_n (this->storage_array_.get (), s, 0UL);

  if (option[DEBUGGING])
    ACE_DEBUG ((LM_DEBUG,
                "\nbool array size = %u, total bytes = %u\n",
                size_,
                size_ * (int) sizeof (unsigned long)));
  return 0;
}

int
Bool_Array::find (u_long slot)
{
  if (this->storage_array_[slot] == generation_number_)
    return 1;
  else
    {
      this->storage_array_[slot] = generation_number_;
      return 0;
    }
}

void
Bool_Array::reset ()
{
  if (++generation_number_ == 0)
    {
      if (option[DEBUGGING])
        ACE_DEBUG ((LM_DEBUG,
                    "(re-initializing bool_array)..."));

      this->generation_number_ = 1;
      std::fill_n (this->storage_array_.get (), size_, 0UL);
      if (option[DEBUGGING])
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));
    }
}
