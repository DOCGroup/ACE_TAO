// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CDR
//
// = FILENAME
//    basic_types.cpp
//
// = DESCRIPTION
//   Test the basic funcionality of a CDR stream by encoding some
//   basic types and then decoding them again.
//
// = AUTHORS
//    Dave Meyer (dmeyer@std.saic.com)
//    Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "tao/CDR.h"

ACE_Message_Block * m1;
ACE_Message_Block * m2;
ACE_Message_Block * m3;

void dump(char * msg)
{
  cout << msg << "\n"
       << "m1: " << m1->data_block()->reference_count() << "\n"
       << "m2: " << m2->data_block()->reference_count() << "\n"
       << "m3: " << m3->data_block()->reference_count() << endl;
}

int main(int, char*[])
{
  // setup mb's
  char buf[1024];
  m1=new ACE_Message_Block(1024);
  memset(buf,'1',512);
  m1->copy(buf,512);
  m2=new ACE_Message_Block(1024);
  memset(buf,'2',512);
  m2->copy(buf,512);
  m3=new ACE_Message_Block(1024);
  memset(buf,'3',512);
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
    cout << "total cdr length is " << cdr.total_length() << endl;
  }
  dump("expect 2,2,2"); // that's what I expected, anyway

  ACE_Message_Block::release (m);
  dump("expect 1,1,1"); // that's what I expected, anyway
  ACE_Message_Block::release (m1);

  return 0;
}
