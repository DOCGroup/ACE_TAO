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

#include "ace/ACE.h"
#include "Hash_Table.h"

#define NIL(TYPE) (TYPE *)0

// The size of the hash table is always the smallest power of 2 >= the
// size indicated by the user.  This allows several optimizations,
// including the use of double hashing and elimination of the mod
// instruction.  Note that the size had better be larger than the
// number of items in the hash table, else there's trouble!!!  Note
// that the memory for the hash table is allocated *outside* the
// intialization routine.  This compromises information hiding
// somewhat, but greatly reduces memory fragmentation, since we can
// now use alloca!

Hash_Table::Hash_Table (List_Node **table_ptr, int s)
  : table (table_ptr),
    size (s),
    collisions (0)
{
  memset ((char *) table, 0, size * sizeof *table);
}

Hash_Table::~Hash_Table (void)
{
  if (option[DEBUG])
    {
      int field_width = option.get_max_keysig_size ();

      fprintf (stderr, "\ndumping the hash table\ntotal available table slots = %d, total bytes = %d, total collisions = %d\n"
               "location, %*s, keyword\n", size, size * (int) sizeof *table, collisions, field_width, "keysig");

      for (int i = size - 1; i >= 0; i--)
        if (table[i])
          fprintf (stderr, "%8d, %*s, %s\n",
                   i, field_width, table[i]->char_set, table[i]->key);

      fprintf (stderr, "\nend dumping hash table\n\n");
    }
}

// If the ITEM is already in the hash table return the item found in
// the table.  Otherwise inserts the ITEM, and returns FALSE.  Uses
// double hashing.

List_Node *
Hash_Table::operator() (List_Node *item, int ignore_length)
{
  unsigned hash_val  = ACE::hash_pjw (item->char_set);
  int      probe     = hash_val & size - 1;
  int      increment = (hash_val ^ item->length | 1) & size - 1;

  while (table[probe]
         && (strcmp (table[probe]->char_set, item->char_set)
             || (!ignore_length && table[probe]->length != item->length)))
    {
      collisions++;
      probe = probe + increment & size - 1;
    }

  return table[probe] ? table[probe] : (table[probe] = item, NIL (List_Node));
}
