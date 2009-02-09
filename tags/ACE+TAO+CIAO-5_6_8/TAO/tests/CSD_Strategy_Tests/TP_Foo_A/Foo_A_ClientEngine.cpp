// $Id$
#include "Foo_A_ClientEngine.h"
#include "Foo_A_Statistics.h"
#include "TestAppExceptionC.h"
#include "AppHelper.h"
#include "ace/Log_Msg.h"

Foo_A_ClientEngine::Foo_A_ClientEngine(Foo_A_ptr obj,
                                       unsigned  client_id,
                                       bool collocated)
  : obj_(Foo_A::_duplicate(obj)),
    client_id_ (client_id),
    collocated_ (collocated)
{
}


Foo_A_ClientEngine::~Foo_A_ClientEngine()
{
}


bool
Foo_A_ClientEngine::execute(void)
{
  // Make sure the connection is established before making
  // remote invocations.
  if (AppHelper::validate_connection (this->obj_.in ()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_ClientEngine::execute  " \
                          "client %d connect failed.\n", this->client_id_));
      return false;
    }

  // Verify the return values and return the results.
  bool check_validity = true;

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
Foo_A_ClientEngine::expected_results(Foo_A_Statistics& stats)
{
  stats.expected(1, 1);
  stats.expected(2, 1);
  stats.expected(3, 1);
  stats.expected(4, 5);
  stats.expected(5, 1);
}
