//$Id$
// UUIDTest.cpp
// $Revision$ $Date$

#include "test_config.h"
#include "ace/UUID.h"

class Tester
{
public:
  int init (void);
  int test (void);
};


int
Tester::init (void)
{
  ///Initialise the UUID Generator
  ACE_UUID_GENERATOR::instance ()->init ();
  return 0;
}

int
Tester::test (void)
{
  // Generate UUID
  ACE_UUID* uuid = ACE_UUID_GENERATOR::instance ()->generateUUID ();
  ACE_CString uuid_str (uuid->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
	      "Generated UUID\n %s\n",
	      uuid_str.c_str ()));
  delete uuid;

  // Construct UUID from string
  ACE_UUID new_uuid (uuid_str);
  ACE_DEBUG ((LM_DEBUG,
	      "UUID Constructed from above Generated UUID\n %s\n",
	      new_uuid.to_string ()->c_str ()));

  // Generate UUID with process and thread ids.
  ACE_UUID* uuid_with_tp_id = ACE_UUID_GENERATOR::instance ()->generateUUID (0x0001, 0xc0);
  ACE_DEBUG ((LM_DEBUG,
	      "UUID with Thread and Process ID\n %s\n",
	      uuid_with_tp_id->to_string ()->c_str ()));

  // Construct UUID from string
  ACE_UUID new_uuid_with_tp_id (uuid_with_tp_id->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
	      "UUID with Thread and Process ID reconstructed from above UUID \n %s\n",
	      new_uuid_with_tp_id.to_string ()->c_str ()));
  delete uuid_with_tp_id;

  return 0;
}

int run_main(int, ACE_TCHAR* [])
{
  ACE_START_TEST (ACE_TEXT ("UUIDTest"));

  Tester tester;

  if (tester.init () == -1)
    {

      ACE_DEBUG((LM_DEBUG,
		 "UUIDTest: Tester::init failed\n"));
      return -1;
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

  ACE_END_TEST;

  return testRetValue;
}

