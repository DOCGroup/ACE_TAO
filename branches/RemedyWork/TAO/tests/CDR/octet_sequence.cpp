
//=============================================================================
/**
 *  @file   octet_sequence.cpp
 *
 *  $Id$
 *
 * Verifies that reference counting is performed correctly for octet
 * sequence marshaling.
 *
 *
 *  @author Dave Meyer (dmeyer@std.saic.com)Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#include "ace/Log_Msg.h"

#include "tao/CDR.h"
#include "ace/OS_NS_string.h"

ACE_Message_Block * m1;
ACE_Message_Block * m2;
ACE_Message_Block * m3;

void dump (const char * msg)
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
  m1=new ACE_Message_Block(1024);
  ACE_OS::memset(buf,'1',512);
  m1->copy(buf,512);
  m2=new ACE_Message_Block(1024);
  ACE_OS::memset(buf,'2',512);
  m2->copy(buf,512);
  m3=new ACE_Message_Block(1024);
  ACE_OS::memset(buf,'3',512);
  m3->copy(buf,512);

  m1->cont(m2);
  m2->cont(m3);
  dump("expect 1,1,1");

  ACE_Message_Block* m = ACE_Message_Block::duplicate (m1);
  dump("expect 2,2,2");

  {
    TAO_OutputCDR cdr;
    cdr.write_octet_array_mb(m1);
    dump("expect 3,3,3"); // that's what I expected, anyway
    ACE_DEBUG ((LM_INFO, "total cdr length is %u\n",
                static_cast<u_int> (cdr.total_length())));
  }
  dump("expect 2,2,2"); // that's what I expected, anyway

  ACE_Message_Block::release (m);
  dump("expect 1,1,1"); // that's what I expected, anyway
  ACE_Message_Block::release (m1);

  return 0;
}
