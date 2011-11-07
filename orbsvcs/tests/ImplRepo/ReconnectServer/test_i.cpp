// $Id$
#include "test_i.h"
#include "ace/OS_NS_time.h"


// Implementation skeleton constructor
Test_Dummy_i::Test_Dummy_i (void)
{
}

// Implementation skeleton destructor
Test_Dummy_i::~Test_Dummy_i (void)
{
}

char * Test_Dummy_i::getMessage (
    void
  )
{
   // Add your implementation here
   return CORBA::string_dup("Test::Dummy---->Hello World");
}

Test_Time_i::Test_Time_i (void)
{
}

// Implementation skeleton destructor
Test_Time_i::~Test_Time_i (void)
{
}

::CORBA::Long Test_Time_i::current_time (
    void
  )
{
   ACE_DEBUG ((LM_DEBUG, "(%P|%t)Test_Time_i::current_time called\n"));
   return CORBA::Long (ACE_OS::time (0));
}

void Test_Time_i::shutdown (
    void
  )
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_i is shutting down"));

}


