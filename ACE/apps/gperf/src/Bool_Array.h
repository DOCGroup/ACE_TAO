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

#ifndef BOOL_ARRAY_H
#define BOOL_ARRAY_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Options.h"

#include <memory>

/**
 * Efficient lookup table abstraction implemented as a "Generation
 * Number" Array.
 *
 * Uses a "Generation Numbering" implementation to minimize
 * initialization time.
 */
class Bool_Array
{
public:
  Bool_Array () = default;
  Bool_Array (Bool_Array const &) = delete;
  Bool_Array &operator= (Bool_Array const &) = delete;
  Bool_Array (Bool_Array &&) = delete;
  Bool_Array &operator= (Bool_Array &&) = delete;

  /// Initialize the array (requires O(n) time).
  int open (size_t);

  ~Bool_Array ();

  /// Locate the @a value in the array (requires O(1) time).
  int find (size_t value);

  /// Reinitializes the array (requires O(1) time).
  void reset ();

private:
  using generation_type = unsigned long;

  /// Initialization of the index space.
  std::unique_ptr<generation_type[]> storage_array_;

  /// Keep track of the current Generation.
  generation_type generation_number_ {};

  /// Keep track of array size.
  size_t size_ {};
};

#endif /* BOOL_ARRAY_H */
