#include "test_i.h"
#include "ace/OS_NS_time.h"

// Implementation skeleton constructor
Test_Dummy_i::Test_Dummy_i ()
{
}

// Implementation skeleton destructor
Test_Dummy_i::~Test_Dummy_i ()
{
}

char * Test_Dummy_i::getMessage ()
{
   // Add your implementation here
   return CORBA::string_dup("Test::Dummy---->Hello World");
}

Test_Time_i::Test_Time_i ()
{
}

// Implementation skeleton destructor
Test_Time_i::~Test_Time_i ()
{
}

::CORBA::Long Test_Time_i::current_time ()
{
   ACE_DEBUG ((LM_DEBUG, "(%P|%t)Test_Time_i::current_time called\n"));
   return CORBA::Long (ACE_OS::time (0));
}

void Test_Time_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "%C\n",
              "Time_i is shutting down"));
}


