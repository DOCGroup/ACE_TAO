//
// $Id$
//
#include "Trigger_i.h"
#include "testC.h"

ACE_RCSID(Trigger, Trigger, "$Id$")

Trigger::Trigger (CORBA::ORB_ptr orb,
		  const std::string & server_ior)
: orb_ (CORBA::ORB::_duplicate (orb)),
  server_ior_ (server_ior)
{
}

void
Trigger::start (void)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "Trigger::start called (ior = %s).\n", 
		  server_ior_.c_str ()));

      CORBA::Object_var tmp = 
	this->orb_->string_to_object (server_ior_.c_str ());

      test_var test = test::_narrow (tmp.in ());
      test->method (0, 0, 0, 0, 1);
    }
  catch (CORBA::Exception & ex)
    {
      ACE_DEBUG ((LM_ERROR, "Trigger::start caught %s\n", ex._info ().c_str ()));
    }
}

void
Trigger::shutdown (void)
{
  this->orb_->shutdown (0);
}
