// -*- C++ -*-
//
// $Id$
//

#include "test_i.h"
#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"

Forward_Test_i::Forward_Test_i (CORBA::ORB_ptr orb)
	: orb_ (CORBA::ORB::_duplicate (orb))
{
}

char * Forward_Test_i::do_forward (const char *text)
{
	ACE_DEBUG ((LM_DEBUG,"Server echoing test string\n"));
	return CORBA::string_dup (text);
}

void
Forward_Test_i::shutdown ()
{
	ACE_OS::sleep( 2 );
	this->orb_->shutdown (0);
}
