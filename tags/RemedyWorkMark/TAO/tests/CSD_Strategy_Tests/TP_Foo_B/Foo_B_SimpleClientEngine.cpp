// $Id$
#include "Foo_B_SimpleClientEngine.h"
#include "Callback_i.h"
#include "Foo_B_Statistics.h"
#include "TestAppExceptionC.h"
#include "AppHelper.h"
#include "ace/Log_Msg.h"

extern const char* ONEWAY_ARG_TEST_STR;

Foo_B_SimpleClientEngine::Foo_B_SimpleClientEngine(Foo_B_ptr obj,
                                                   Callback_ptr callback,
                                                   unsigned client_id,
                                                   bool collocated)
  : obj_(Foo_B::_duplicate(obj)),
    callback_(Callback::_duplicate(callback)),
    client_id_(client_id),
    collocated_(collocated)
{
}


Foo_B_SimpleClientEngine::~Foo_B_SimpleClientEngine()
{
}


bool
Foo_B_SimpleClientEngine::execute(void)
{
  // Make sure the connection is established before making
  // remote invocations.
  if (AppHelper::validate_connection (this->obj_.in ()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_SimpleClientEngine::execute  " \
                          "client %d connect failed.\n", this->client_id_));
      return false;
    }

  // Verify the return values and return the results.
  bool check_validity = true;

  // Callback test.
  try
  {
    if (CORBA::is_nil(this->callback_.in()))
      {
        ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_SimpleClientEngine::execute  " \
                   "The callback_ objref is nil before op7() call!\n"));
      }

    this->obj_->op7 (this->callback_.in ());
  }
  catch (const FooException& )
  {
    ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_SimpleClientEngine::execute  " \
               "FooException raised by op7() call.\n"));
    check_validity = false;
  }

  // Tell the servant that this client is done.
  this->obj_->done();

  return check_validity;
}


void
Foo_B_SimpleClientEngine::expected_results(Foo_B_Statistics& stats)
{
  stats.expected(7, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
}


unsigned
Foo_B_SimpleClientEngine::expected_callbacks ()
{
  return 1;
}
