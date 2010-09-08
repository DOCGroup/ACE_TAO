// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Pluggable
//
// = FILENAME
//    PP_Test_i.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "tao/Timeprobe.h"
#include "tao/ORB_Constants.h"
#include "PP_Test_i.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *PP_Test_i_Timeprobe_Description[] =
{
  "PP_Test_i::send_oneway - start",
  "PP_Test_i::send_oneway - end",

  "PP_Test_i::send_void - start",
  "PP_Test_i::send_void - end",

  "PP_Test_i::make_pluggable - start",
  "PP_Test_i::make_pluggable - end",

  "PP_Test_i::server_shutdown - start",
  "PP_Test_i::server_shutdown - end"
};

enum
{
  // Timeprobe description table start key
  PP_TEST_I_SEND_ONEWAY_START = 10100,
  PP_TEST_I_SEND_ONEWAY_END,

  PP_TEST_I_SEND_VOID_START,
  PP_TEST_I_SEND_VOID_END,

  PP_TEST_I_MAKE_PLUGGABLE_START,
  PP_TEST_I_MAKE_PLUGGABLE_END,

  PP_TEST_I_SERVER_SHUTDOWN_START,
  PP_TEST_I_SERVER_SHUTDOWN_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (PP_Test_i_Timeprobe_Description,
                                  PP_TEST_I_SEND_ONEWAY_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// Factory Constructor

Pluggable_Test_Factory_i::Pluggable_Test_Factory_i (CORBA::ORB_ptr orb)
  : my_pluggable_test_ (orb)
{
}

// Factory Destructor

Pluggable_Test_Factory_i::~Pluggable_Test_Factory_i (void)
{
}

Pluggable_Test_ptr
Pluggable_Test_Factory_i::make_pluggable_test (void)
{
  ACE_FUNCTION_TIMEPROBE (PP_TEST_I_MAKE_PLUGGABLE_START);
  return my_pluggable_test_._this ();
}

// Constructor

PP_Test_i::PP_Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Destructor

PP_Test_i::~PP_Test_i (void)
{
}

// Oneway send

void
PP_Test_i::send_oneway (void)
{
  ACE_FUNCTION_TIMEPROBE (PP_TEST_I_SEND_ONEWAY_START);
}

// Twoway send

void
PP_Test_i::send_void (void)
{
  ACE_FUNCTION_TIMEPROBE (PP_TEST_I_SEND_VOID_START);
}

// Shutdown.

void PP_Test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "PP_Test_i is shutting down"));

  ACE_FUNCTION_TIMEPROBE (PP_TEST_I_SERVER_SHUTDOWN_START);

  this->orb_->shutdown ();
}
