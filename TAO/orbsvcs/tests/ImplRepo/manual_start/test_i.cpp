/* -*- C++ -*-  */

#include "test_i.h"
#include "ace/Log_Msg.h"

Test_i::Test_i ()
{
}

Test_i::~Test_i ()
{
}

void
Test_i::contact ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server contacted\n")));
}
