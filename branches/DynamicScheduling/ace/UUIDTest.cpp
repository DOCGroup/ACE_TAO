//$Id$
// UUIDTest.cpp
// $Revision$ $Date$

#include <string>
#include "ace/UUID.h"

class Tester
{
public:
  int init( int argc, char* argv[]);
  int test();
};


int Tester::init( int, char*[])
{
  ACE_UUID_GENERATOR::instance ()->init ();
  return 0;
}

int Tester::test()
{
  // Generate UUID
  ACE_UUID* uuid = ACE_UUID_GENERATOR::instance ()->generateUUID ();
  ACE_CString uuid_str (uuid->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
	      "UUID - %s\n",
	      uuid_str.c_str ()));

  // Construct UUID from string
  ACE_UUID new_uuid (uuid_str);
  ACE_DEBUG ((LM_DEBUG,
	      "New UUID - %s\n",
	      new_uuid.to_string ()->c_str ()));
  return 0;
}

int main(int argc, char* argv[])

{
  ACE::init();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), "UUIDTest"));

  Tester tester;

  if (tester.init( argc, argv) == -1) 
    {
	
      ACE_DEBUG((LM_DEBUG,
		 "UUIDTest: Tester::init failed\n"));
      return EXIT_FAILURE;
    }
  int testRetValue = tester.test();

  if (testRetValue == 0) 
    {
      ACE_DEBUG((LM_DEBUG,
		 "UUIDTest succeeded\n"));
    } 
  else 
    {
      ACE_DEBUG((LM_DEBUG,
		 "UUIDTest failed\n"));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n"), "UUIDTest"));
  ACE::fini();
  return testRetValue;
}


