// $Id$
#include "Foo_C_ClientEngine.h"
#include "AppHelper.h"
#include "ace/Log_Msg.h"


Foo_C_ClientEngine::Foo_C_ClientEngine(Foo_C_ptr obj,
                                       unsigned  client_id)
  : obj_(Foo_C::_duplicate(obj)),
    client_id_ (client_id)
{
}


Foo_C_ClientEngine::~Foo_C_ClientEngine()
{
}


bool
Foo_C_ClientEngine::execute(void)
{
  // Verify the return values and return the results.
  bool check_validity = true;

  // Make sure the connection is established before making
  // remote invocations.
  if (AppHelper::validate_connection (this->obj_.in ()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_ClientEngine::execute  " \
                          "client %d connect failed.\n", this->client_id_));
      return false;
    }

  CORBA::Long i = this->client_id_;

  this->obj_->op1();

  this->obj_->op2(i);

  CORBA::Long value = this->obj_->op3(i);

  if (value != i)
    {
      check_validity = false;
    }

  for (CORBA::Long j = 1; j <= 5; j++)
    {
      this->obj_->op4(495 + (i * 5) + j);
    }

  bool caught_exception = false;

  try
  {
    this->obj_->op5();
  }
  catch (const FooException& )
  {
    // Expected
    caught_exception = true;
  }

  if (! caught_exception)
    {
      check_validity = false;
    }

  this->obj_->done();

  return check_validity;
}


void
Foo_C_ClientEngine::expected_results(Foo_C_Statistics& stats)
{
  // This ClientEngine is used by remote clients.
  stats.expected(1, 1, true);
  stats.expected(2, 1, true);
  stats.expected(3, 1, true);
  stats.expected(4, 5, true);
  stats.expected(5, 1, true);
}
