
//=============================================================================
/**
 *  @file    Message_Block_Large_Copy_Test.cpp
 *
 *  $Id$
 *
 *  This test program tests large Message Block duplication and cloning.
 *
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Message_Block.h"
#include "ace/OS_NS_string.h"

size_t
run_duplicate_test (const size_t msg_block_count,
                    const char * block,
                    const size_t msg_block_size)
{
    size_t rc = 0;

    ACE_Message_Block* mb_top = new ACE_Message_Block ();
    ACE_Message_Block* mb = mb_top;

    for (size_t j = 0; j != msg_block_count; ++j)
    {
      ACE_Message_Block* next = new ACE_Message_Block (block, msg_block_size);
      next->wr_ptr (msg_block_size);
      mb->cont (next);
      mb = next;
    }

    ACE_Message_Block* mb_test = mb_top->duplicate ();
    if (mb_test != 0)
    {
      rc = mb_test->total_size();
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) %u top reference_count ()\n"),
                  mb_top->reference_count ()));
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) duplicated: %@ %d %d\n"),
                  mb_test,
                  mb_test->total_size(),
                  mb_test->total_length()));
      mb_test-> release();
    }

    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) %u top reference_count ()\n"),
                mb_top->reference_count ()));

    mb_top-> release ();
    return rc;
}

size_t
run_clone_test (const size_t msg_block_count,
                const char * block,
                const size_t msg_block_size)
{
    size_t rc = 0;

    ACE_Message_Block* mb_top = new ACE_Message_Block ();
    ACE_Message_Block* mb = mb_top;

    for (size_t j = 0; j != msg_block_count; ++j)
    {
      ACE_Message_Block* next = new ACE_Message_Block (block, msg_block_size);
      next->wr_ptr (msg_block_size);
      mb->cont (next);
      mb = next;
    }

    ACE_Message_Block* mb_test = mb_top->clone ();
    if (mb_test != 0)
    {
      rc = mb_test->total_size();
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) %u top reference_count ()\n"),
                  mb_top->reference_count ()));
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) cloned: %@ %d %d\n"),
                  mb_test,
                  mb_test->total_size(),
                  mb_test->total_length()));
      mb_test-> release();
    }

    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) %u top reference_count ()\n"),
                mb_top->reference_count ()));

    mb_top-> release();
    return rc;
}

int
run_main (int , ACE_TCHAR *[])
{

  int rc = 0;

  ACE_START_TEST (ACE_TEXT ("Message_Block_Large_Copy_Test"));
  {

    // Message_Block size() and Length() of 24,000,000
    const size_t MSG_BLOCK_COUNT = 8000;
    const size_t MSG_BLOCK_SIZE  = 3000;
    const size_t MSG_BLOCK_TOTAL = MSG_BLOCK_COUNT * MSG_BLOCK_SIZE;

    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) %u blocks %u bytes each, total %u\n"),
                MSG_BLOCK_COUNT,
                MSG_BLOCK_SIZE,
                MSG_BLOCK_TOTAL));

    char block[MSG_BLOCK_SIZE];
    ACE_OS::memset (block, 'A', MSG_BLOCK_SIZE);

    size_t duplicate_total = run_duplicate_test (
      MSG_BLOCK_COUNT,
      block,
      MSG_BLOCK_SIZE);
    if (duplicate_total != MSG_BLOCK_TOTAL )
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) duplicate(): returned total of %u\n"),
        duplicate_total));
      rc = 1;
    }

    size_t clone_total = run_clone_test (
      MSG_BLOCK_COUNT,
      block,
      MSG_BLOCK_SIZE);
    if (clone_total != MSG_BLOCK_TOTAL )
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) clone(): returned total of %u \n"),
        clone_total));
      rc = 1;
    }

  }
  ACE_END_TEST;
  return rc;
}
