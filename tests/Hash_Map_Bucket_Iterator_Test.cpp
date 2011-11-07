
//=============================================================================
/**
 *  @file    Hash_Map_Bucket_Iterator_Test.cpp
 *
 *  $Id$
 *
 *   This is a simple test of the <Hash_Map_Bucket_Iterator>.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/Get_Opt.h"
#include "ace/Functor.h"

static ACE_UINT32 iterations = ACE_MAX_ITERATIONS * 10;
static ACE_UINT32 table_size = ACE_MAX_ITERATIONS;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("t:i:"));

  int cc;

  while ((cc = get_opt ()) != -1)
    switch (cc)
      {
      case 't':
        table_size = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case '?':
      case 'h':
      default:
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("usage: %s ")
                    ACE_TEXT ("[-i (iterations)] ")
                    ACE_TEXT ("[-t (table size)] "),
                    ACE_TEXT ("Hash_Map_Bucket_Iterator_Test")));
        return -1;
      }

  return 0;
}

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT32,
                                ACE_UINT32,
                                ACE_Hash<ACE_UINT32>,
                                ACE_Equal_To<ACE_UINT32>,
                                ACE_SYNCH_NULL_MUTEX> HASH_MAP;

typedef ACE_Hash_Map_Bucket_Iterator<ACE_UINT32,
                                     ACE_UINT32,
                                     ACE_Hash<ACE_UINT32>,
                                     ACE_Equal_To<ACE_UINT32>,
                                     ACE_SYNCH_NULL_MUTEX> HASH_MAP_BUCKET_ITERATOR;

int
run_main (int argc, ACE_TCHAR *argv[])
{
  // Validate options.
  int result = parse_args (argc, argv);
  if (result != 0)
    return result;

  // Start the test only if options are valid.
  ACE_START_TEST (ACE_TEXT ("Hash_Map_Bucket_Iterator_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  ACE_UINT32 i = 0;
  HASH_MAP map (table_size);

  for (i = 0; i < iterations; ++i)
    {
      int result = map.bind (i, i);
      ACE_TEST_ASSERT (result == 0);
    }

  for (i = 0; i < table_size; ++i)
    {
      HASH_MAP_BUCKET_ITERATOR iterator (map,
                                         i);

      HASH_MAP_BUCKET_ITERATOR end (map,
                                    i,
                                    1);

      for (;
           iterator != end;
           ++iterator)
        {
          ACE_DEBUG ((LM_DEBUG, "%d ", (*iterator).int_id_));
          ACE_UINT32 key = (*iterator).ext_id_;
          ACE_TEST_ASSERT (((key - i) % table_size) == 0);
        }

      ACE_DEBUG ((LM_DEBUG, "\n"));
    }

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;

  return 0;
}

