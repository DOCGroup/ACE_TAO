// $Id$

#include "test_i.h"

int Simple_Server_count=0;

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Simple_Server_i::sendCharSeq (const Char_Seq &)
{
  Simple_Server_count++;
  //ACE_DEBUG ((LM_DEBUG, "."));
}

void
Simple_Server_i::sendOctetSeq (const Octet_Seq &)
{
  Simple_Server_count++;
  //ACE_DEBUG ((LM_DEBUG, "."));
}

CORBA::Long
Simple_Server_i::get_number (CORBA::Long)
{
  CORBA::Long tmp = Simple_Server_count;
  Simple_Server_count = 0;
  return tmp;
}

void
Simple_Server_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Simple_Server_i::shutdown\n"));
  try
    {
      this->orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}
