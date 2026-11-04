
//=============================================================================
/**
 *  @file   octet_sequence.cpp
 *
 * Verifies that reference counting is performed correctly for octet
 * sequence marshaling.
 *
 *  @author Dave Meyer (dmeyer@std.saic.com)Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#include "ace/Log_Msg.h"

#include "tao/CDR.h"

#include <cstring>
#include <memory>
struct Message_Block_Release
{
  void operator() (ACE_Message_Block *message_block) const
  {
    ACE_Message_Block::release (message_block);
  }
};

using Message_Block_Ptr = std::unique_ptr<ACE_Message_Block, Message_Block_Release>;

void
dump (char const *const msg,
      ACE_Message_Block const *const m1,
      ACE_Message_Block const *const m2,
      ACE_Message_Block const *const m3)
{
  ACE_DEBUG ((LM_INFO,
              "%C\n"
              "m1: %d\n"
              "m2: %d\n"
              "m3: %d\n",
              msg,
              m1->data_block()->reference_count(),
              m2->data_block()->reference_count(),
              m3->data_block()->reference_count()));
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // setup mb's
  char buf[1024];
  Message_Block_Ptr const m1 (new ACE_Message_Block (1024));
  Message_Block_Ptr m2 (new ACE_Message_Block (1024));
  Message_Block_Ptr m3 (new ACE_Message_Block (1024));
  // Non-owning aliases remain valid while m1 owns the chained message blocks.
  ACE_Message_Block *const m2_alias = m2.get ();
  ACE_Message_Block *const m3_alias = m3.get ();
  std::memset (buf, '1', 512);
  m1->copy (buf, 512);
  m1->cont (m2.release ());
  std::memset (buf, '2', 512);
  m2_alias->copy (buf, 512);
  m2_alias->cont (m3.release ());
  std::memset (buf, '3', 512);
  m3_alias->copy (buf, 512);

  dump ("expect 1,1,1", m1.get (), m2_alias, m3_alias);

  {
    Message_Block_Ptr const m (ACE_Message_Block::duplicate (m1.get ()));
    dump ("expect 2,2,2", m1.get (), m2_alias, m3_alias);

    {
      TAO_OutputCDR cdr;
      cdr.write_octet_array_mb (m1.get ());
      dump ("expect 3,3,3", m1.get (), m2_alias, m3_alias);
      ACE_DEBUG ((LM_INFO, "total cdr length is %u\n",
                  static_cast<unsigned int> (cdr.total_length ())));
    }
    dump ("expect 2,2,2", m1.get (), m2_alias, m3_alias);
  }
  dump ("expect 1,1,1", m1.get (), m2_alias, m3_alias);

  return 0;
}
