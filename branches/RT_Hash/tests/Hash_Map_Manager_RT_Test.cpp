
//=============================================================================
/**
 *  @file    Hash_Map_Manager_RT_Test.cpp
 *
 *  $Id$
 *
 *  @author Jeremy M. Nolle <jmn3@cs.wustl.edu>
 */
//=============================================================================

#include "test_config.h"
#include "ace/Hash_Map_Manager_RT.h"
#include "ace/Malloc_T.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Hash_Map_Manager_Test, "$Id$")

static const size_t STRING_TABLE_ENTRIES = 18;
static const size_t MAX_HASH =  18;

typedef ACE_Hash_Map_RT_Table<const ACE_TCHAR *,
                              const ACE_TCHAR *,
                              const ACE_TCHAR *> HASH_STRING_TABLE;

typedef ACE_Hash_Map_RT_Bucket<const ACE_TCHAR *,
                               const ACE_TCHAR *,
                               const ACE_TCHAR *> HASH_STRING_BUCKET;

// This allows us enough for 4 tables and 8 buckets in each table
// regardless of the number of entries
static const size_t STRING_TABLE_SIZE =
  (6 * (sizeof (HASH_STRING_TABLE))) + 48 * (sizeof(HASH_STRING_BUCKET)) ;


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_RT_Entry<const ACE_TCHAR *, const ACE_TCHAR *>;
template class ACE_Hash_Map_RT_Manager_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Static_Allocator<STRING_TABLE_SIZE>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_RT_Entry<const ACE_TCHAR *, const ACE_TCHAR *>
#pragma instantiate ACE_Hash_Map_RT_Manager_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Static_Allocator<STRING_TABLE_SIZE>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

static ACE_Static_Allocator<STRING_TABLE_SIZE> ace_test_allocator;

typedef ACE_Hash_Map_RT_Manager_Ex<const ACE_TCHAR *,
                                   const ACE_TCHAR *,
                                   ACE_Hash<const ACE_TCHAR *>,
                                   ACE_Equal_To<const ACE_TCHAR *>,
                                   ACE_Null_Mutex> HASH_STRING_MAP;

struct String_Table
{
  const ACE_TCHAR *key_;
  const ACE_TCHAR *value_;
};

static String_Table string_table[] =
{
  {
    ACE_TEXT ("hello   "),
    ACE_TEXT ("guten Tag")
  },
  {
    ACE_TEXT ("one     "),
    ACE_TEXT ("111111")
  },
  {
    ACE_TEXT ("two     "),
    ACE_TEXT ("222222")
  },
  {
    ACE_TEXT ("three   "),
    ACE_TEXT ("333333")
  },
  {
    ACE_TEXT ("four    "),
    ACE_TEXT ("444444")
  },
  {
    ACE_TEXT ("five    "),
    ACE_TEXT ("555555")
  },
  {
    ACE_TEXT ("six     "),
    ACE_TEXT ("666666")
  },
  {
    ACE_TEXT ("seven   "),
    ACE_TEXT ("777777")
  },
  {
    ACE_TEXT ("eight   "),
    ACE_TEXT ("888888")
  },
  {
    ACE_TEXT ("nine    "),
    ACE_TEXT ("999999")
  },
  {
    ACE_TEXT ("ten     "),
    ACE_TEXT ("101010")
  },
  {
    ACE_TEXT ("eleven  "),
    ACE_TEXT ("111111")
  },
  {
    ACE_TEXT ("twelve  "),
    ACE_TEXT ("121212")
  },
  {
    ACE_TEXT ("thirteen"),
    ACE_TEXT ("131313")
  },
  {
    ACE_TEXT ("fourteen"),
    ACE_TEXT ("141414")
  },
  {
    ACE_TEXT ("fifteen "),
    ACE_TEXT ("151515")
  },
  {
    ACE_TEXT ("sixteen "),
    ACE_TEXT ("161616")
  },
  {
    ACE_TEXT ("seventen"),
    ACE_TEXT ("171717")
  },
  {
    ACE_TEXT ("eighteen"),
    ACE_TEXT ("181818")
  },
  {
    ACE_TEXT ("nineteen"),
    ACE_TEXT ("191919")
  },
  {
    ACE_TEXT ("twenty  "),
    ACE_TEXT ("202020")
  },
  {
    ACE_TEXT ("twentyon"),
    ACE_TEXT ("212121")
  },
  {
    ACE_TEXT ("twentytw"),
    ACE_TEXT ("222222")
  },
  {
    ACE_TEXT ("twenthre"),
    ACE_TEXT ("232323")
  },
  {
    ACE_TEXT ("twenfour"),
    ACE_TEXT ("242424")
  },
  {
    ACE_TEXT ("twenfive"),
    ACE_TEXT ("252525")
  },
  {
    ACE_TEXT ("twnsix  "),
    ACE_TEXT ("262626")
  },
  {
    ACE_TEXT ("twensevn"),
    ACE_TEXT ("272727")
  },
  {
    ACE_TEXT ("tweneigt"),
    ACE_TEXT ("282828")
  },
  {
    ACE_TEXT ("twennine"),
    ACE_TEXT ("292929")
  },
  {
    ACE_TEXT ("thirty  "),
    ACE_TEXT ("303030")
  },
  {
    ACE_TEXT ("thirtyon"),
    ACE_TEXT ("313131")
  },
  {
    ACE_TEXT ("thirtytw"),
    ACE_TEXT ("323232")
  },
  {
    ACE_TEXT ("thirthre"),
    ACE_TEXT ("333333")
  },
  {
    ACE_TEXT ("thirfour"),
    ACE_TEXT ("343434")
  },  
  {
    ACE_TEXT ("thirfive"),
    ACE_TEXT ("353535")
  },
  {
    ACE_TEXT ("thirsix "),
    ACE_TEXT ("363636")
  },
  {
    ACE_TEXT ("thirsevn"),
    ACE_TEXT ("373737")
  },
  {
    ACE_TEXT ("colours "),
    ACE_TEXT ("Farben")
  },
  {
    ACE_TEXT ("hello   "),
    ACE_TEXT ("not so, hello")
  },
  {
    ACE_TEXT ("ja whol "),
    ACE_TEXT ("sure thing")
  },
  {
    ACE_TEXT ("no idea "),
    ACE_TEXT ("keine Idee")
  },
  {
    ACE_TEXT ("cheese  "),
    ACE_TEXT ("Kase")
  },
  {
    ACE_TEXT ("mensch  "),
    ACE_TEXT ("bummer")
  },
  {
    ACE_TEXT ("mustard "),
    ACE_TEXT ("yellow")
  },
  {
    ACE_TEXT ("ketchup "),
    ACE_TEXT ("red")
  },
  {
    ACE_TEXT ("grapes  "),
    ACE_TEXT ("purpurot")
  },
  {
    ACE_TEXT ("kartofel"),
    ACE_TEXT ("potato")
  },
  {
    ACE_TEXT ("brot    "),
    ACE_TEXT ("bread")
  },
  {
    ACE_TEXT ("0"),
    ACE_TEXT ("zilch")
  },
  {
    ACE_TEXT ("goodbye "),
    ACE_TEXT ("auf wiedersehen")
  },
	{
    0,
    0
  }
};


static int
run_test (void)
{
  HASH_STRING_MAP hash (2,
                        3,
                        4,
                        &ace_test_allocator);

  int i;
  // Check the <bind> operation.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.bind (string_table[i].key_,
                   string_table[i].value_) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("bind failed for %s \n"),
                         string_table[i].key_), -1);
    else
      ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("just put %s\n"),
                 string_table[i]));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n just put a bunch of stuff\n\n")));


  const ACE_TCHAR *id;

  // Check the <find> operation.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   id) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' found `%s'\n"),
                  string_table[i].key_,
                  id));
    else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' _________NOT_________ found \n"),
                  string_table[i].key_));

    id = NULL;

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\n\n")));

  // now remove the 6th element
  hash.unbind (string_table[5].key_);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("removing `%s'\n"),
                         string_table[5].key_));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n")));
  
  // remove the first element
  hash.unbind (string_table[0].key_);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("removing `%s'\n"),
                         string_table[0].key_));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\n")));

  // Check the <find> operation again with missing elements
  for (i = 0; string_table[i].key_ != 0; i++)
  {
    if (hash.find (string_table[i].key_,
                   id) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' found `%s'\n"),
                  string_table[i].key_,
                  id));
    else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' _________NOT_________ found \n"),
                  string_table[i].key_));
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\n")));

  // now put the 6th element back in
  if (hash.bind (string_table[5].key_,
                 string_table[5].value_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p failed for %s \n"),
                       ACE_TEXT ("bind"),
                       string_table[5].key_), -1);
  
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("putting five back \n\n")));

  // Check the <find> operation. now only the first element is missing (twice)
  for (i = 0; string_table[i].key_ != 0; i++)
  {
    if (hash.find (string_table[i].key_,
                   id) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' found `%s'\n"),
                  string_table[i].key_,
                  id));
    else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' _________NOT_________ found \n"),
                  string_table[i].key_));
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("dump\n")));
  ace_test_allocator.dump ();
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Hash_Map_Manager_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}
