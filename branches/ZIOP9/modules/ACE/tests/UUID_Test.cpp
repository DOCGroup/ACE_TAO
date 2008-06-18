// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    UUID_Test.cpp
//
// = DESCRIPTION
//    Test the ACE UUID class which generates unique id's
//
// = AUTHOR
//   Andrew T. Finnel <andrew@activesol.net> and
//    Yamuna Krishnmaurthy <yamuna@oomworks.com>
// ============================================================================

#include "test_config.h"
#include "ace/UUID.h"
#include "ace/Auto_Ptr.h"

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
  ACE_Utils::UUID_GENERATOR::instance ()->init ();
  return 0;
}

int
Tester::test (void)
{
  int retval = 0;

  // Generate UUID
  auto_ptr <ACE_Utils::UUID> uuid (ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID ());
  ACE_CString uuid_str (uuid->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
              "Generated UUID\n %s\n",
              uuid_str.c_str ()));

  // Construct UUID from string
  ACE_Utils::UUID new_uuid (uuid_str);
  ACE_DEBUG ((LM_DEBUG,
              "UUID Constructed from above Generated UUID\n %s\n",
              new_uuid.to_string ()->c_str ()));

  // Construct UUID from string by assigning it
  ACE_Utils::UUID new_uuid_assign;
  new_uuid_assign.from_string (new_uuid.to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
              "UUID Constructed from above Generated UUID with assign\n %s\n",
              new_uuid_assign.to_string ()->c_str ()));

  if (new_uuid != new_uuid_assign)
    {
      ACE_ERROR ((LM_ERROR, "Error: UUIDs are not the same\n"));
      retval = -1;
    }

  // Construct UUID using the copy constructor
  ACE_Utils::UUID new_uuid_copy (new_uuid);
  ACE_DEBUG ((LM_DEBUG,
              "UUID Constructed from above Generated UUID with copy\n %s\n",
              new_uuid_copy.to_string ()->c_str ()));

  if (new_uuid != new_uuid_copy)
    {
      ACE_ERROR ((LM_ERROR, "Error: UUIDs are not the same with copy\n"));
      retval = -1;
    }

  // Generate UUID with process and thread ids.
  auto_ptr <ACE_Utils::UUID> uuid_with_tp_id (ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (0x0001,
                                                                                                     0xc0));
  ACE_DEBUG ((LM_DEBUG,
              "UUID with Thread and Process ID\n %s\n",
              uuid_with_tp_id->to_string ()->c_str ()));

  if (new_uuid == *uuid_with_tp_id)
    {
      ACE_ERROR ((LM_ERROR, "Error: UUIDs are the same\n"));
      retval = -1;
    }

  // Construct UUID from string
  ACE_Utils::UUID new_uuid_with_tp_id (uuid_with_tp_id->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
              "UUID with Thread and Process ID reconstructed from above UUID \n %s\n",
              new_uuid_with_tp_id.to_string ()->c_str ()));

  return retval;
}

int run_main(int, ACE_TCHAR* [])
{
  ACE_START_TEST (ACE_TEXT ("UUID_Test"));

  Tester tester;

  if (tester.init () == -1)
    {
      ACE_DEBUG((LM_DEBUG,
                 "UUIDTest: Tester::init failed\n"));
      return -1;
    }

  int result = tester.test();

  if (result == 0)
    ACE_DEBUG((LM_DEBUG,
               "UUID_Test succeeded\n"));
  else
    ACE_ERROR((LM_ERROR,
               "UUID_Test failed\n"));

  ACE_END_TEST;

  return result;
}
