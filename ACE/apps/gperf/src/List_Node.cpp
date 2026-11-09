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

#include "List_Node.h"
#include "Vectors.h"
#include "ace/OS_NS_ctype.h"
#include "ace/Truncate.h"

/// Sorts the key set alphabetically to speed up subsequent operation
/// Uses insertion sort since the set is probably quite small.
inline void
List_Node::sort (char *base, int len)
{
  int i, j;

  for (i = 0, j = len - 1; i < j; ++i)
    {
      char tmp;
      int curr;

      for (curr = i + 1, tmp = base[curr];
           curr > 0 && tmp < base[curr - 1];
           --curr)
        {
          base[curr] = base[curr - 1];
        }

      base[curr] = tmp;
    }
}

/// Initializes a List_Node, copying the input keyword and fields and
/// building its key signature using the information stored in the
/// KEY_POSITIONS array in Options, and checking for simple errors.
/// The data member REST stores any additional fields of the input file
/// (it is empty if Option[TYPE] is not enabled). This is useful if the
/// user wishes to incorporate a lookup structure, rather than just an
/// array of keys.
/// Finally, KEY_NUMBER contains a count of the total number of keys
/// seen so far.  This is used to initialize the INDEX field to some
/// useful value.
List_Node::List_Node (char const *input, int len)
  : link {},
    next {},
    key (input, static_cast<size_t> (len)),
    rest (option[TYPE] ? input + len + 1 : ""),
    keysig {},
    length (len),
    slot (0)
{
  // Lower case if STRCASECMP option is enabled.
  if (option[STRCASECMP])
    for (char& ch : this->key)
      if (ACE_OS::ace_isupper (ch))
        ch = static_cast<char> (ACE_OS::ace_tolower (ch));

  if (option[ALLCHARS])         // Use all the character position in the KEY.
    for (char const ch : this->key)
      {
        this->keysig.push_back (ch);
        int const i = static_cast<int> (ch);
        ++Vectors::occurrences[i];
      }
  else
    {
      // Only use those character positions specified by the user.
      option.reset ();

      // Iterate thru the list of key_positions, initializing
      // occurrences table and keysig (via char * pointer ptr).
      for (int i; (i = option.get ()) != EOS; )
        {
          char ch;
          if (i == WORD_END) // Special notation for last KEY position, i.e. '$'.
            ch = this->key[len - 1];
          else if (i <= len) // Within range of KEY length, so we'll keep it.
            ch = this->key[i - 1];
          else // Out of range of KEY length, so we'll just skip it.
            continue;
          this->keysig.push_back (ch);
          ++Vectors::occurrences[static_cast<int> (ch)];
        }

      // Didn't get any hits and user doesn't want to consider the
      // keylength, so there are essentially no usable hash positions!
      if (this->keysig.empty () && option[NOLENGTH])
        ACE_ERROR ((LM_ERROR,
                    "Can't hash keyword %s with chosen key positions.\n%a",
                    this->key.c_str (),
                    1));
    }

  // Sort the KEYSIG items alphabetically.
  sort (this->keysig.data (), ACE_Utils::truncate_cast<int> (this->keysig.size ()));
}

