// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Config_Test.cpp
//
// = DESCRIPTION
//      This is a test that makes sure various classes in
//      <ACE_Configuration> work correctly.
//
// = AUTHOR
//    Michael Searles <msearles@base16.com>,
//    Chris Hafey <chafey@stentor.com>, and
//    Jerry D. Odenwelder Jr. <jerry.o@mindspring.com>
//
// ============================================================================

#include "test_config.h"
#include "Config_Test.h"
#include "ace/Configuration_Import_Export.h"

ACE_RCSID(tests, Config_Test, "$Id$")

static int
test (ACE_Configuration *config)
{
  ACE_TString stvalue;

  ACE_Configuration_Section_Key root =
    config->root_section ();

  // Add a section.
  ACE_Configuration_Section_Key testsection;

  if (config->open_section (root,
                            ACE_TEXT ("test"),
                            1,
                            testsection))
    return -2;

  // Set some values.
  else if (config->set_string_value (testsection,
                                     ACE_TEXT ("stvalue"),
                                     ACE_TEXT ("stvaluetest")))
    return -3;

  else if (config->remove_value (testsection,
                                 ACE_TEXT ("stvalue")))
    return -4;
  // Make sure it's really gone
  else if (0 == config->get_string_value (testsection,
                                          ACE_TEXT ("stvalue"),
                                          stvalue))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test:remove_value didn't remove\n")),
                      -4);

  else if (config->set_string_value (testsection,
                                     ACE_TEXT ("stvalue"),
                                     ACE_TEXT ("stvaluetest")))
    return -3;
  else if (config->set_string_value (testsection,
                                     ACE_TEXT ("stvalue"),
                                     ACE_TEXT ("second stvaluetest")))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("test:set_string_value twice failed\n")),
                      -3);

  else if (config->set_integer_value (testsection,
                                      ACE_TEXT ("intvalue"),
                                      77))
    return -4;
  // Reset to the value we test for below
  else if (config->set_integer_value (testsection,
                                      ACE_TEXT ("intvalue"),
                                      42))
    return -4;

  u_char data[80];

  for (int i = 0; i < 80; i++)
    data[i] = i + 128;

  if (config->set_binary_value (testsection,
                                ACE_TEXT ("binvalue"),
                                data,
                                80))
    return -5;

  // Get the values and compare
  if (config->get_string_value (testsection,
                                ACE_TEXT ("stvalue"),
                                stvalue))
    return -6;
  else if (stvalue != ACE_TEXT ("second stvaluetest"))
    return -7;

  u_int intvalue;

  if (config->get_integer_value (testsection,
                                 ACE_TEXT ("intvalue"),
                                 intvalue))
    return -8;
  else if (intvalue != 42)
    return -9;

  u_char *data_out = 0;
  u_int length = 0;

  if (config->get_binary_value (testsection,
                                ACE_TEXT ("binvalue"),
                                (void*&) data_out,
                                length))
    return -10;

  // compare em
  for (int j = 0; j < 80; j++)
    if (data_out[j] != data[j])
      return -11;

  delete [] data_out;

  // Test iteration.
  ACE_TString name;
  ACE_Configuration::VALUETYPE type;
  u_int index = 0;
  int found[3] = { 0, 0, 0 };   // One for each expected value

  while (!config->enumerate_values (testsection,
                                    index,
                                    name,
                                    type))
    {
      if (name == ACE_TEXT ("stvalue"))
        {
          if (type != ACE_Configuration::STRING)
            return -12;
          if (found[0] != 0)
            return -12;
          found[0] = 1;
        }
      else if (name == ACE_TEXT ("intvalue"))
        {
          if (type != ACE_Configuration::INTEGER)
            return -13;
          if (found[1] != 0)
            return -13;
          found[1] = 1;
        }
      else if (name == ACE_TEXT ("binvalue"))
        {
          if (type != ACE_Configuration::BINARY)
            return -14;
          if (found[2] != 0)
            return -14;
          found[2] = 1;
        }
      index++;
    }

  // Make sure we got three values.
  if (index != 3 || !found[0] || !found[1] || !found[2])
    return -15;

  // Add some subsections.
  ACE_Configuration_Section_Key test2;
  ACE_Configuration_Section_Key test3;
  ACE_Configuration_Section_Key test4;

  if (config->open_section (testsection,
                            ACE_TEXT ("test2"),
                            1,
                            test2))
    return -16;
  else if (config->open_section (testsection,
                                 ACE_TEXT ("test3"),
                                 1,
                                 test3))
    return -17;
  else if (config->open_section (testsection,
                                 ACE_TEXT ("test4"),
                                 1,
                                 test4))
    return -18;

  // Test enumerate sections.
  index = 0;
  found[0] = found[1] = found[2] = 0;
  while (!config->enumerate_sections (testsection,
                                      index,
                                      name))
    {
      if (name == ACE_TEXT ("test2"))
        {
          if (found[0] != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("enumerate_sections, dupl test2\n")),
                              -19);
          found[0] = 1;
        }
      else if (name == ACE_TEXT ("test3"))
        {
          if (found[1] != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("enumerate_sections, dupl test3\n")),
                              -19);
          found[1] = 1;
        }
      else if (name == ACE_TEXT ("test4"))
        {
          if (found[2] != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("enumerate_sections, dupl test4\n")),
                              -19);
          found[2] = 1;
        }
      index++;
    }

  if (index != 3 || !found[0] || !found[1] || !found[2])
    return -19;

  // Remove a subsection
  if (config->remove_section (testsection,
                              ACE_TEXT ("test2"),
                              0))
    return -20;

  // Try to remove it again
  if (!config->remove_section (testsection,
                               ACE_TEXT ("test2"),
                               0))
    return -21;

  // Try to remove the testsection root, it should fail since it still
  // has subkeys
  if (!config->remove_section (root,
                               ACE_TEXT ("test"),
                               0))
    return -22;

  // Test find section
  ACE_Configuration_Section_Key result;

  if (config->open_section (root,
                            ACE_TEXT ("test"),
                            0,
                            result))
    return -23;

  // Now test the recursive remove.
  if (config->remove_section (root,
                              ACE_TEXT ("test"),
                              1))
    return -24;

  // Make sure its not there
  if (!config->open_section (root,
                             ACE_TEXT ("test"),
                             0,
                             testsection))
    return -25;

  return 0;
}

static int
run_tests (void)
{
#if defined (ACE_WIN32)
  // test win32 registry implementation.
  HKEY root =
    ACE_Configuration_Win32Registry::resolve_key (HKEY_LOCAL_MACHINE,
                                                  ACE_TEXT ("Software\\ACE\\test"));
  if (!root)
    ACE_ERROR_RETURN((LM_ERROR,
                      "resolve_key is broken\n"),-2);

  // test resolving of forward slashes
  HKEY root_fs =
    ACE_Configuration_Win32Registry::resolve_key (HKEY_LOCAL_MACHINE,
                                                  ACE_TEXT ("Software/ACE/test"), 0);
  if (!root_fs)
    ACE_ERROR_RETURN((LM_ERROR,
                      "resolve_key is broken\n"),-2);

  ACE_Configuration_Win32Registry RegConfig (root);
  {
    int result = test (&RegConfig);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Win32 registry test failed (%d)\n", result),
                        -1);
  }


#endif /* ACE_WIN32 */
  // Test Heap version
  ACE_Configuration_Heap heap_config;

  if (heap_config.open ())
    return 0;
  {
    int result = test (&heap_config);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Heap Configuration test failed (%d)\n",
                         result),
                        -1);
  }

  // Test persistent heap version
  ACE_OS::unlink (ACE_TEXT ("test.reg"));
  ACE_Configuration_Heap pers_config;

  if (pers_config.open (ACE_TEXT ("test.reg")))
    return 0;
  {
    int result = test (&pers_config);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Persistent Heap Configuration test failed (%d)\n",
                         result),
                        -1);
  }

  ACE_DEBUG ((LM_DEBUG, "Test passed\n"));
  return 0;
}

static int
build_config_object (ACE_Configuration& cfg)
{
  ACE_Configuration_Section_Key root = cfg.root_section ();
  ACE_Configuration_Section_Key NetworkSection;
  ACE_Configuration_Section_Key LoggerSection;
  ACE_Configuration_Section_Key BinarySection;

  if (cfg.open_section (root,
                      ACE_TEXT ("network"),
                      1,
                      NetworkSection))
    return -1;

  if (cfg.set_integer_value (NetworkSection,
                           ACE_TEXT ("TimeToLive"),
                           100))
    return -2;
  else if (cfg.set_string_value (NetworkSection,
                                 ACE_TEXT ("Delay"),
                                 ACE_TString (ACE_TEXT ("FALSE"))))
    return -3;
  else if (cfg.set_string_value (NetworkSection,
                                 ACE_TEXT ("DestIPAddress"),
                                 ACE_TString (ACE_TEXT ("localhost"))))
    return -4;
  else if (cfg.set_integer_value (NetworkSection,
                                  ACE_TEXT ("DestPort"),
                                  12670))
    return -5;
  else if (cfg.set_integer_value (NetworkSection,
                                  ACE_TEXT ("ReconnectInterval"),
                                  3))
    return -6;

  if (cfg.open_section (root,
                      ACE_TEXT ("logger"),
                      1,
                      LoggerSection))
    return -7;


  if (cfg.set_string_value (LoggerSection,
                            ACE_TEXT ("Heading"),
                            ACE_TString (ACE_TEXT ("ACE - Adaptive Communication Environment"))))
    return -8;
  else if (cfg.set_integer_value (LoggerSection,
                                  ACE_TEXT ("SeekIndex"),
                                  14))
    return -9;
  else if (cfg.set_integer_value (LoggerSection,
                                  ACE_TEXT ("TraceLevel"),
                                  6))
    return -10;
  else if (cfg.set_string_value (LoggerSection,
                                 ACE_TEXT ("Justification"),
                                 ACE_TString (ACE_TEXT ("left_justified"))))
    return -11;
  else if (cfg.set_string_value (LoggerSection,
                                 ACE_TEXT ("LogFilePath"),
                                 ACE_TString (ACE_TEXT ("log/"))))
    return -12;
  else if (cfg.set_string_value (LoggerSection,
                                 ACE_TEXT ("TransactionFilePath"),
                                 ACE_TString (ACE_TEXT ("data/"))))
    return -13;

  if (cfg.open_section (root,
                        ACE_TEXT ("binary"),
                        1,
                        BinarySection))
    return -14;

  u_char data[80];

  for (int i = 0; i < 80; i++)
    data[i] = i + 128;

  if (cfg.set_binary_value (BinarySection,
                            ACE_TEXT ("data"),
                            data,
                            80))
    return -15;

  return 0;
}

/*
 * Test ACE_Configuration::operator==
 */
int
Config_Test::testEquality ()
{
  // create and open 2 ACE_Configuration objects.
  ACE_Configuration_Heap  heap1;
  ACE_Configuration_Heap  heap2;
  if ((heap1.open ()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open heap1\n"),
                        -1);

    }
  else if ((heap2.open ()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open heap2\n"),
                        -1);
    }

  // populate them equally
  build_config_object (heap1);
  build_config_object (heap2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, "The objects should equal...\n"));
  if (heap1 == heap2)
    {
      ACE_DEBUG ((LM_DEBUG, "And they do ;-)\n"));
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "And they do not :- (\nThe Equality Operator Failed when objects equal\n"),
                        -1);
    }

  // add a section and value to heap1
  ACE_Configuration_Section_Key root1 = heap1.root_section ();
  ACE_Configuration_Section_Key NewSection;
  if (heap1.open_section (root1,
                          ACE_TEXT ("NewSection"),
                          1,
                          NewSection))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding section to heap1\n"),
                      -1);
  else if (heap1.set_integer_value (NewSection,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding value to heap1\n"),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, "The objects should NOT equal...\n"));
  if (heap1 == heap2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "They Do :- (\nThe Equality Operator Failed when lhs contains data not in rhs\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, "And they do not ;-)\n"));

  //
  // add same section to heap2
  //
  ACE_Configuration_Section_Key root2 = heap2.root_section ();
  ACE_Configuration_Section_Key NewSection2;
  if (heap2.open_section (root2,
                          ACE_TEXT ("NewSection"),
                          1,
                          NewSection2))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding section to heap2\n"),
                      -1);
  else if (heap2.set_integer_value (NewSection2,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding value to heap2\n"),
                      -2);
  else if (heap2.set_integer_value (NewSection2,
                                    ACE_TEXT ("TestIntValue2"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding second value to heap2\n"),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, "The objects should NOT equal...\n"));
  if (heap1 == heap2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "And They Do :- (\nThe Equality Operator Failed when rhs contains value not in lhs\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, "And they do not ;-)\n"));

  // add new value in heap 1
  if (heap1.set_integer_value (NewSection,
                               ACE_TEXT ("TestIntValue2"),
                               100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding second value to heap1\n"),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, "The objects should be equal...\n"));
  if (heap1 == heap2)
    ACE_DEBUG ((LM_DEBUG, "And they are ;-)\n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "And they are not :- (\nThe Equality Operator Failed\n"),
                      -1);

  // Add a new section to heap2
  ACE_Configuration_Section_Key AnotherNewSection2;
  if (heap2.open_section (root2,
                          ACE_TEXT ("AnotherNewSection"),
                          1,
                          AnotherNewSection2))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding second section to heap2\n"),
                      -1);
  else if (heap2.set_integer_value (AnotherNewSection2,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding value in second section to heap2\n"),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, "The objects should NOT equal...\n"));
  if (heap1 == heap2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "And they do :- (\nThe Equality Operator Failed when rhs contains data not in lhs\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, "And they do not :-)\n"));

  // add section back to heap1
  ACE_Configuration_Section_Key AnotherNewSection1;
  if (heap1.open_section (root1,
                          ACE_TEXT ("AnotherNewSection"),
                          1,
                          AnotherNewSection1))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding second section to heap1\n"),
                      -1);
  else if (heap1.set_integer_value (AnotherNewSection1,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error adding second value to second section in heap1\n"),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, "The objects should be equal...\n"));
  if (heap1 == heap2)
    ACE_DEBUG ((LM_DEBUG, "And they are ;-)\n"));
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "And they are not :-)\nThe Equality Operator Failed\n"),
                      -1);

  this->equality_tested_ = 1;
  return 0;
}

/*
 * Compare INI import data in fromFile to origional data exported (in origional)
 *
 * This compare is destructive to the origional object.
 * I realize that normally you would not do such an obscene thing but
 * this funciton has a special purpose and I know my origional is not needed
 * after calling this routine.
 * This is done because configuration objects that are imported using the INI
 * import store all data as strings. My origional has type information and I need to
 * know if the import worked.
*/
static int
iniCompare (ACE_Configuration_Heap& fromFile, ACE_Configuration_Heap& original)
{
  int rc = 1; // start by guessing they are equal

  int         sectionIndex = 0;
  ACE_TString sectionName;

  const ACE_Configuration_Section_Key& fromFileRoot  = fromFile.root_section ();
  const ACE_Configuration_Section_Key& originalRoot = original.root_section ();
  ACE_Configuration_Section_Key originalSection;
  ACE_Configuration_Section_Key fromFileSection;

  // loop through each section in the fromFile object
  while ((rc) &&
         (!fromFile.enumerate_sections (fromFileRoot,
                                        sectionIndex,
                                        sectionName)) )
    {
      // find that section in the original object
      if (original.open_section (originalRoot,
                                  sectionName.c_str (),
                                  0,
                                  originalSection) != 0)
        // If the original object does not contain the section then we are not equal.
        rc = 0;
      else if (fromFile.open_section (fromFileRoot,
                                      sectionName.c_str (),
                                      0,
                                      fromFileSection) != 0)
        // if there is some error opening the section in the fromFile
        rc = 0;
      else
        {
          // Well the sections match
          int         valueIndex = 0;
          ACE_TString valueName;
          ACE_Configuration::VALUETYPE   valueType;
          ACE_Configuration::VALUETYPE   originalType;

          // Enumerate each value in the fromFile section
          while ((rc) &&
                 (!fromFile.enumerate_values (fromFileSection,
                                              valueIndex,
                                              valueName,
                                              valueType)))
            {
              // look for the same value in the original section
              if (original.find_value (originalSection,
                                        valueName.c_str (),
                                        originalType) != 0)
                // We're not equal if the same value cannot be found
                // in the original object.
                rc = 0;
              else
                {
                  ACE_TString fromFileString, originalString;


                  if (fromFile.get_string_value (fromFileSection,
                                                 valueName.c_str (),
                                                 fromFileString) != 0)
                    // we're not equal if we cannot get this string
                    rc = 0;
                  else if (originalType != ACE_Configuration::STRING)  // If the original type is not a string
                    {
                      // convert original data to a string.

                      if (originalType == ACE_Configuration::INTEGER)
                        {
                          u_int intValue;
                          ACE_TCHAR int_value[32];

                          if (original.get_integer_value (originalSection,
                                                           valueName.c_str (),
                                                           intValue) != 0)
                            // we're not equal if we cannot get rhs int
                            rc = 0;

                          ACE_OS::sprintf (int_value, ACE_LIB_TEXT ("%08x"), intValue);
                          originalString = int_value;
                        }
                      else if (originalType == ACE_Configuration::BINARY)
                        {
                          void* binary_data;
                          u_int binary_length;

                          if (original.get_binary_value (originalSection,
                                                          valueName.c_str (),
                                                          binary_data,
                                                          binary_length))
                            // we're not equal if we cannot get this string
                            rc = 0;
                          else
                            {
                              ACE_TCHAR bin_value[3];

                              unsigned char* ptr = (unsigned char*)binary_data;
                              while (binary_length)
                                {
                                  if (ptr != binary_data)
                                    originalString += ACE_LIB_TEXT (",");

                                  ACE_OS::sprintf (bin_value, ACE_LIB_TEXT ("%02x"), *ptr);
                                  originalString += bin_value;
                                  --binary_length;
                                  ++ptr;
                                }
                              delete (char *)binary_data;
                            }// end successful binary read
                        }// end if originalType was binary
                      else
                        // if the type is invalid, then go ahead and fail it.
                        rc = 0;

                    }// end if the original type was not a string.
                  else
                    {
                      if (original.get_string_value (originalSection,
                                                      valueName.c_str (),
                                                      originalString) != 0)
                        {
                          // we're not equal if we cannot get rhs string
                          rc = 0;
                        }

                    }

                  rc &= fromFileString == originalString;

                  if (rc)
                    // before we move on remove this value from the original.
                    original.remove_value (originalSection,
                                           valueName.c_str ());

                }// end else if values match.

              valueIndex++;

            }// end value while loop

          // at this point the original should have no values.  look
          // for values in the original section
          valueIndex = 0;
          while ((rc) &&
                 (!original.enumerate_values (originalSection,
                                               valueIndex,
                                               valueName,
                                               originalType)))
            valueIndex++;

          // having a value indicates a mismatch
          rc = valueIndex == 0;

        }// end else if sections match.

      if (rc)
        // before we move on remove the section from the original.
        original.remove_section (originalRoot,
                                 sectionName.c_str (),
                                 0); // do not remove subsections.

      sectionIndex++;

    }// end section while loop

  // Finally, if the original has any sections, then we're not equal
  sectionIndex = 0;
  while ((rc) &&
         (!original.enumerate_sections (originalRoot,
                                         sectionIndex,
                                         sectionName)))
    sectionIndex++;

  rc = sectionIndex == 0;

  return rc;
}

// Used to test INI Import Export class

int
Config_Test::testIniFormat ()
{
  int rc = 0;
  if (!this->equality_tested_)
    {
      rc = this->testEquality ();
      if (rc != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Equality Test Failed\n"));
          return rc;
        }
    }
  ACE_DEBUG ((LM_DEBUG, "Testing INI Format Import/Export\n"));
  ACE_Configuration_Heap  fromFile;

  //  1. Creates an ACE_Configuration_Heap object
  ACE_Configuration_Heap original;

  rc = original.open ();
  if (rc == 0)
    {
      rc = build_config_object (original);
      //  2. Calls build_config_object to populate
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error populating original config object (%d)\n",
                           rc),
                          -1);

      //  3. Export
      ACE_Ini_ImpExp importExport (original);

      rc = importExport.export_config (ACE_TEXT ("testConfig.ini"));
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error Exporting (%d)\n",
                           rc),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not open original object (%d)\n",
                       rc),
                      -1);

  // At this point we've successfully created, populated and written
  // the configuration object
  //  5. Creates a new ACE_Configuration_Heap object
  rc = fromFile.open ();
  if (rc == 0)
    {
      //  6. Imports
      ACE_Ini_ImpExp importExport (fromFile);

      rc = importExport.import_config (ACE_TEXT ("testConfig.ini"));
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error Exporting (%d)\n",
                           rc),
                          -1);

      //  7. Compares to original.
      // This is a special compare since files imported using the
      // INI file import do not contain type information
      //
      // NOTE: After this call the original object will be invalid!!!
      //
      if (!iniCompare (fromFile, original))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object read from file does not equal original (%d)\n",
                           rc),
                          -1);
    }// end if heap could not be opened.
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not open fromFile object (%d)\n",
                       rc),
                      -1);

  //  8. Calls old "read_config" methods on the new object

  int       nTimeToLive;
  int       bDelay;
  int       nDestPort;
  int       nReconnectInterval;
  int       nTraceLevel;

  ACE_TCHAR pszDestIPAddress[TEST_MAX_STRING];
  ACE_TCHAR pszLogFilePath[TEST_MAX_STRING];
  ACE_TCHAR pszTransactionFilePath[TEST_MAX_STRING];
  ACE_TCHAR pszHeading[TEST_MAX_STRING];
  ACE_TCHAR pszJustification[TEST_MAX_STRING];

  ACE_Configuration_Section_Key root = fromFile.root_section ();

  // Process [network] section
  ACE_Configuration_Section_Key NetworkSection;
  if (fromFile.open_section (root,
                             ACE_TEXT ("network"),
                             1,
                             NetworkSection) == 0)
    {
      this->get_section_integer (fromFile,
                                 NetworkSection,
                                 ACE_TEXT ("TimeToLive"),
                                 &nTimeToLive,
                                 1,
                                 20);

      this->get_section_boolean (fromFile,
                                 NetworkSection,
                                 ACE_TEXT ("Delay"),
                                 &bDelay);

      this->get_section_string (fromFile,
                                NetworkSection,
                                ACE_TEXT ("DestIPAddress"),
                                pszDestIPAddress,
                                TEST_MAX_STRING);

      this->get_section_integer (fromFile,
                                 NetworkSection,
                                 ACE_TEXT ("DestPort"),
                                 &nDestPort,
                                 0,
                                 65535);

      this->get_section_integer (fromFile,
                                 NetworkSection,
                                 ACE_TEXT ("ReconnectInterval"),
                                 &nReconnectInterval,
                                 0,
                                 65535);
    }// end of "network" section

  // Process [logger] section
  ACE_Configuration_Section_Key LoggerSection;
  if (fromFile.open_section (root,
                             ACE_TEXT ("logger"),
                             1,
                             LoggerSection) == 0)
    {
      this->get_section_string (fromFile,
                                LoggerSection,
                                ACE_TEXT ("Heading"),
                                pszHeading,
                                TEST_MAX_STRING);
      this->get_section_integer (fromFile,
                                 LoggerSection,
                                 ACE_TEXT ("TraceLevel"),
                                 &nTraceLevel,
                                 1,
                                 20);
      this->get_section_string (fromFile,
                                LoggerSection,
                                ACE_TEXT ("Justification"),
                                pszJustification,
                                TEST_MAX_STRING);
      this->get_section_string (fromFile,
                                LoggerSection,
                                ACE_TEXT ("LogFilePath"),
                                pszLogFilePath,
                                TEST_MAX_STRING);
      this->get_section_string (fromFile,
                                LoggerSection,
                                ACE_TEXT ("TransactionFilePath"),
                                pszTransactionFilePath,
                                TEST_MAX_STRING);
    }// end of "logger" section

  if (!rc)
    ACE_DEBUG ((LM_DEBUG, "INI Format Import/Export Works ;-)\n"));
  return rc;
}

// Used to test registry Import Export class

int
Config_Test::testRegFormat ()
{
  int rc = 0;
  if (!this->equality_tested_)
    {
      rc = this->testEquality ();
      if (rc != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Equality Test Failed\n"));
          return rc;
        }

    }
  ACE_DEBUG ((LM_DEBUG, "Testing Registry Format Import/Export\n"));
  ACE_Configuration_Heap  fromFile;

  //  1. Creates an ACE_Configuration_Heap object
  ACE_Configuration_Heap original;

  rc = original.open ();
  if (rc == 0)
    {
      //  2. Calls build_config_object to populate
      rc = build_config_object (original);
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error populating original config object (%d)\n",
                           rc),
                          -1);
      //  3. Export
      ACE_Registry_ImpExp importExport (original);

      rc = importExport.export_config (ACE_TEXT ("testConfig.ini"));
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error Exporting (%d)\n",
                           rc),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not open original object (%d)\n",
                       rc),
                      -1);

  // At this point we've successfully created, populated and written
  // the configuration object
  //  5. Creates a new ACE_Configuration_Heap object
  rc = fromFile.open ();
  if (rc == 0)
    {
      //  6. Imports
      ACE_Registry_ImpExp importExport (fromFile);

      rc = importExport.import_config (ACE_TEXT ("testConfig.ini"));
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error Exporting (%d)\n",
                           rc),
                          -1);

      //  7. Compares to original.
      if (fromFile != original)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object read from file does not equal original (%d)\n",
                           rc),
                          -1);
    }// end if heap could not be opened.
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not open fromFile object (%d)\n",
                       rc),
                      -1);

  if (!rc)
    ACE_DEBUG ((LM_DEBUG, "Registry Format Import/Export Works ;-)\n"));
  return rc;
}


// Reads a string value from a configuration object.

void
Config_Test::get_section_string (ACE_Configuration&             config,
                                 ACE_Configuration_Section_Key& SectionKey,
                                 const ACE_TCHAR*               pszName,
                                 ACE_TCHAR*                     pszVariable,
                                 int                            nMaxLength)
{
  ACE_TString StringValue;

  if (config.get_string_value (SectionKey,
                               pszName,
                               StringValue) == 0)
    {
      ACE_OS::strncpy (pszVariable,
                       StringValue.c_str (),
                       nMaxLength);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %s\n"),
                  pszName,
                  pszVariable));
    }
}

// Reads an integer value from a congifuration object (when it's stored as a string)
void
Config_Test::get_section_integer (ACE_Configuration&             config,
                                  ACE_Configuration_Section_Key& SectionKey,
                                  const ACE_TCHAR*               pszName,
                                  int*                           nVariable,
                                  int                            nMinValue,
                                  int                            nMaxValue)
{
  ACE_TString StringValue;
  ACE_TCHAR pszString[30];
  ACE_OS::strcpy (pszString, ACE_TEXT ("0"));
  int IntegerValue = 0;

  if (config.get_string_value (SectionKey,
                               pszName,
                               StringValue) == 0)
    {
      ACE_OS::strncpy (pszString,
                       StringValue.c_str (),
                       30);
      ACE_DEBUG ((LM_DEBUG,
                  "%s = %s\n",
                  pszName,
                  pszString));
    }

  // convert to integer
  IntegerValue = ACE_OS::atoi (pszString);
  IntegerValue = (IntegerValue < nMinValue) ? nMinValue : IntegerValue;
  IntegerValue = (IntegerValue > nMaxValue) ? nMaxValue : IntegerValue;
  *nVariable = IntegerValue;
}

// Reads a boolean value from a configuration object (when it's stored as a string).

void
Config_Test::get_section_boolean (ACE_Configuration&             config,
                                  ACE_Configuration_Section_Key& SectionKey,
                                  const ACE_TCHAR*               pszName,
                                  int*                           pVariable)
{
  ACE_TString StringValue;
  ACE_TCHAR pszString[10];
  ACE_OS::strcpy (pszString, ACE_TEXT ("0"));

  if (config.get_string_value (SectionKey,
                               pszName,
                               StringValue) == 0)
    {
      ACE_OS::strncpy (pszString,
                       StringValue.c_str (),
                       10);
      for (ACE_TCHAR* pSrc = pszString;
           *pSrc != ACE_TEXT ('\0');
           pSrc++)
        // Convert to uppercase
        if (islower (*pSrc))
          *pSrc = tolower (*pSrc);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s = %s\n"),
                  pszName,
                  pszString));

      if (ACE_OS::strcmp (pszString,
                          ACE_TEXT ("TRUE")) == 0)
        *pVariable = 1;
      else if (ACE_OS::strcmp (pszString,
                               ACE_TEXT ("FALSE")) == 0)
        *pVariable = 0;
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Config_Test"));

  Config_Test manager;

  if (manager.testEquality () != 0)
    ACE_DEBUG ((LM_DEBUG, "Failed the equality Test\n"));

  if (manager.testRegFormat () != 0)
    ACE_DEBUG ((LM_DEBUG, "Failed the REG Format Test\n"));

  if (manager.testIniFormat () != 0)
    ACE_DEBUG ((LM_DEBUG, "Failed the INI Format Test\n"));

  if (run_tests () != 0)
    ACE_DEBUG ((LM_DEBUG, "Failed in run_tests\n"));

  ACE_END_TEST;
  return 0;
}
