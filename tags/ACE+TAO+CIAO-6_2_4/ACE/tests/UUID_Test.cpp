
//=============================================================================
/**
 *  @file    UUID_Test.cpp
 *
 *  $Id$
 *
 *  Test the ACE UUID class which generates unique id's
 *
 *
 *  @author Andrew T. Finnel <andrew@activesol.net> and Yamuna Krishnmaurthy <yamuna@oomworks.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/UUID.h"
#include "ace/Auto_Ptr.h"

class Tester
{
public:
  int test (void);
};

int
Tester::test (void)
{
  int retval = 0;

  // Generate UUID
  auto_ptr <ACE_Utils::UUID> uuid (ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID ());
  ACE_CString uuid_str (uuid->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Generated UUID\n %C\n"),
              uuid_str.c_str ()));

  // Construct UUID from string
  ACE_Utils::UUID new_uuid (uuid_str);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID Constructed from above Generated UUID\n %C\n"),
              new_uuid.to_string ()->c_str ()));

  // Construct UUID from string by assigning it
  ACE_Utils::UUID new_uuid_assign;
  new_uuid_assign.from_string (new_uuid.to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID Constructed from above Generated UUID ")
              ACE_TEXT ("with assign\n %C\n"),
              new_uuid_assign.to_string ()->c_str ()));

  if (new_uuid != new_uuid_assign)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: UUIDs are not the same\n")),
                       -1);

  // Check the hash value of the 2 UUIDs

  if (new_uuid.hash () != new_uuid_assign.hash ())
    ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("Error: hash value of UUIDs are ")
                        ACE_TEXT ("not the same")),
                        -1);

  // Construct UUID using the copy constructor
  ACE_Utils::UUID new_uuid_copy (new_uuid);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID constructed from above Generated UUID")
              ACE_TEXT (" with copy\n %C\n"),
              new_uuid_copy.to_string ()->c_str ()));

  if (new_uuid != new_uuid_copy)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: UUIDs are not the same ")
                       ACE_TEXT ("with copy\n")),
                       -1);

  ACE_Utils::UUID nil_uuid (*ACE_Utils::UUID::NIL_UUID.to_string ());
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID Constructed from NIL_UUID with ")
              ACE_TEXT ("string copy\n %C\n"),
              nil_uuid.to_string ()->c_str ()));

  if (nil_uuid != ACE_Utils::UUID::NIL_UUID)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: UUIDs are not the same with ")
                       ACE_TEXT ("NIL_UUID string copy\n")),
                       -1);

  // Construct UUID using the assignment constructor
  ACE_Utils::UUID new_uuid_assigment;
  new_uuid_assigment = new_uuid;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID Constructed from above Generated UUID ")
              ACE_TEXT ("with assignment\n %C\n"),
              new_uuid_assigment.to_string ()->c_str ()));

  if (new_uuid != new_uuid_assigment)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: UUIDs are not the same "
                       ACE_TEXT ("with assignment\n"))),
                       -1);

  // Generate UUID with process and thread ids.
  auto_ptr <ACE_Utils::UUID>
    uuid_with_tp_id (ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (0x0001, 0xc0));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID with Thread and Process ID\n %C\n"),
              uuid_with_tp_id->to_string ()->c_str ()));

  if (new_uuid == *uuid_with_tp_id)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: UUIDs are the same\n")),
                       -1);

  // Construct UUID from string
  ACE_Utils::UUID new_uuid_with_tp_id (uuid_with_tp_id->to_string ()->c_str ());
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("UUID with Thread and Process ID reconstructed ")
              ACE_TEXT ("from above UUID \n %C\n"),
              new_uuid_with_tp_id.to_string ()->c_str ()));

  return retval;
}

int run_main(int, ACE_TCHAR* [])
{
  ACE_START_TEST (ACE_TEXT ("UUID_Test"));

  Tester tester;

  int const result = tester.test();

  if (result == 0)
    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT ("UUID_Test succeeded\n")));
  else
    ACE_ERROR((LM_ERROR,
               ACE_TEXT ("UUID_Test failed\n")));

  ACE_END_TEST;

  return result;
}
