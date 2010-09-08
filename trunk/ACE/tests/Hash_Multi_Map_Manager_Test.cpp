// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Hash_Multi_Map_Manager_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of <ACE_Hash_Multi_Map_Manager> to
//      maintain a hash table using strings.
//
// = AUTHOR
//    Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Hash_Multi_Map_Manager_T.h"
#include "ace/Null_Mutex.h"



typedef ACE_Hash_Multi_Map_Manager<const ACE_TCHAR *,
                                   const ACE_TCHAR *,
                                   ACE_Hash<const ACE_TCHAR *>,
                                   ACE_Equal_To<const ACE_TCHAR *>,
                                   ACE_Null_Mutex> HASH_STRING_MAP;

typedef HASH_STRING_MAP::ENTRY HASH_STRING_ENTRY;
typedef HASH_STRING_ENTRY::VALUE_SET HASH_STRING_VALUE_SET;
typedef HASH_STRING_ENTRY::VALUE_SET_ITERATOR HASH_STRING_VALUE_SET_ITERATOR;
typedef HASH_STRING_MAP::ITERATOR HASH_STRING_ITERATOR;
typedef HASH_STRING_MAP::CONST_ITERATOR HASH_STRING_CONST_ITERATOR;
typedef HASH_STRING_MAP::REVERSE_ITERATOR HASH_STRING_REVERSE_ITERATOR;

static const ACE_TCHAR *color_sorts[] = {ACE_TEXT ("warm"), ACE_TEXT ("cold"), ACE_TEXT ("neutral"), 0};
static const ACE_TCHAR *warm_colors[] = {ACE_TEXT ("red"), ACE_TEXT ("yellow"), ACE_TEXT ("orange"), 0};
static const ACE_TCHAR *cold_colors[] = {ACE_TEXT ("blue"), ACE_TEXT ("cyan"), 0};
static const ACE_TCHAR *neutral_colors[] = {ACE_TEXT ("green"), ACE_TEXT ("purple"), 0};

static int
run_test (void)
{
  HASH_STRING_MAP colors;

  size_t i;

  // Check the <bind> operation.
  for (i = 0; warm_colors[i] != 0; i++)
    {
      if (colors.bind (color_sorts[0],
                       warm_colors[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("bind failed for %s in %s\n"),
                          warm_colors[i],
                          color_sorts[0]),
                          -1);
    }

  HASH_STRING_ENTRY *entry = 0;
  for (i = 0; cold_colors[i] != 0; i++)
    {
      if (colors.bind (color_sorts[1],
                       cold_colors[i],
                       entry) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("bind failed for %s in %s\n"),
                          cold_colors[i],
                          color_sorts[1]),
                          -1);
    }

  HASH_STRING_VALUE_SET value_set;
  entry = 0;
  for (i = 0; neutral_colors[i] != 0; i++)
    value_set.insert (neutral_colors[i]);
  if (colors.bind (color_sorts[2],
                   value_set,
                   entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  // Check the <trybind> operation.
  value_set.reset ();
  if (colors.trybind (color_sorts[2],
                      value_set) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  entry = 0;
  if (colors.trybind (color_sorts[2],
                      value_set,
                      entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  entry = 0;
  if (colors.trybind (color_sorts[2],
                      value_set,
                      entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  // Check the <rebind> operation.
  value_set.reset ();
  value_set.insert (neutral_colors[0]);
  if (colors.rebind (color_sorts[2],
                     value_set) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  value_set.insert (neutral_colors[1]);
  entry = 0;
  if (colors.rebind (color_sorts[2],
                     value_set,
                     entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  value_set.insert (neutral_colors[0]);
  HASH_STRING_VALUE_SET old_value_set;
  if (colors.rebind (color_sorts[2],
                     value_set,
                     old_value_set) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  value_set.insert (neutral_colors[1]);
  old_value_set.reset ();
  entry = 0;
  if (colors.rebind (color_sorts[2],
                     value_set,
                     old_value_set,
                     entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  value_set.insert (neutral_colors[0]);
  const ACE_TCHAR *old_key;
  old_value_set.reset ();
  if (colors.rebind (color_sorts[2],
                     value_set,
                     old_key,
                     old_value_set) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  value_set.reset ();
  for (i = 0; neutral_colors[i] != 0; i++)
    value_set.insert (neutral_colors[i]);
  old_key = 0;
  old_value_set.reset ();
  entry = 0;
  if (colors.rebind (color_sorts[2],
                     value_set,
                     old_key,
                     old_value_set,
                     entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("bind failed for %s\n"),
                       color_sorts[2]),
                       -1);

  // Check the <find> operation.
  for (i = 0; color_sorts[i] != 0; i++)
    if (colors.find (color_sorts[i]) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s found\n"),
                  color_sorts[i]));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%s not found\n"),
                         color_sorts[i]),
                        -1);

  entry = 0;
  for (i = 0; color_sorts[i] != 0; i++)
    if (colors.find (color_sorts[i],
                     entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s found\n"),
                  color_sorts[i]));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%s not found\n"),
                         color_sorts[i]),
                        -1);

  value_set.reset ();
  for (i = 0; color_sorts[i] != 0; i++)
    if (colors.find (color_sorts[i],
                     value_set) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s found\n"),
                  color_sorts[i]));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%s not found\n"),
                         color_sorts[i]),
                        -1);

  for (i = 0; warm_colors[i] != 0; i++)
    if (colors.find (color_sorts[0],
                     warm_colors[i]) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s color %s found\n"),
                  color_sorts[0],
                  warm_colors[i]));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%s color %s not found\n"),
                         color_sorts[0],
                         warm_colors[i]),
                        -1);

  // Check the iterator.
  i = 0;
  entry = 0;
  for (HASH_STRING_ITERATOR hash_iter (colors);
       hash_iter.next (entry) != 0;
       hash_iter.advance ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("iterating (%d): %s:\n"),
                  i,
                  entry->ext_id_));
      i++;

      for (HASH_STRING_VALUE_SET_ITERATOR iter (entry->int_id_set_);
           iter != entry->int_id_set_.end ();
           iter++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%s \n"),
                      *iter));
        }
    }

  // Check the unbind operation.
  if (colors.unbind (color_sorts[0]) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("unbind failed for %s\n"),
                       color_sorts[1]),
                      -1);
  for (i = 0; warm_colors[i] != 0; i++)
    {
      if (colors.bind (color_sorts[0],
                       warm_colors[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("bind failed for %s in %s\n"),
                          warm_colors[i],
                          color_sorts[0]),
                          -1);
    }

  value_set.reset ();
  if (colors.unbind (color_sorts[0], value_set) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("unbind failed for %s\n"),
                       color_sorts[1]),
                      -1);
  for (i = 0; warm_colors[i] != 0; i++)
    {
      if (colors.bind (color_sorts[0],
                       warm_colors[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("bind failed for %s in %s\n"),
                          warm_colors[i],
                          color_sorts[0]),
                          -1);
    }

  for (i = 0; warm_colors[i] != 0; i++)
    {
      if (colors.unbind (color_sorts[0],
                         warm_colors[i]) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("unbind failed for %s in %s\n"),
                           warm_colors[i],
                           color_sorts[0]),
                          -1);
    }

  // Check the reverse iterator.
  i = 0;
  entry = 0;
  for (HASH_STRING_REVERSE_ITERATOR hash_iter (colors);
       hash_iter.next (entry) != 0;
       hash_iter.advance ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("reverse iterating (%d): %s\n"),
                  i,
                  entry->ext_id_));
      i++;

      for (HASH_STRING_VALUE_SET_ITERATOR iter (entry->int_id_set_);
           iter != entry->int_id_set_.end ();
           iter++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%s \n"),
                      *iter));
        }
    }

  entry = 0;
  colors.find (color_sorts[1], entry);
  if (colors.unbind (entry) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("unbind failed for %s\n"),
                       color_sorts[1]),
                      -1);

  // Check the const iterator.
  i = 0;
  entry = 0;
  for (HASH_STRING_CONST_ITERATOR hash_iter (colors);
       hash_iter.next (entry) != 0;
       hash_iter.advance ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("const iterating (%d): %s\n"),
                  i,
                  entry->ext_id_));
      i++;

      for (HASH_STRING_VALUE_SET_ITERATOR iter (entry->int_id_set_);
           iter != entry->int_id_set_.end ();
           iter++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%s \n"),
                      *iter));
        }
    }

  // Check the unbind_all operation.
  if (colors.unbind_all () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("unbind_all failed\n")),
                       -1);

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Hash_Multi_Map_Manager_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}
