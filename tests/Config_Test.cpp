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
//    Michael Searles <msearles@base16.com> and Chris Hafey <chafey@stentor.com>
//
// ============================================================================

#include "tests/test_config.h"
#include "Config_Test.h"

ACE_RCSID(tests, Config_Test, "$Id$")

static int
test (ACE_Configuration *config)
{
  ACE_Configuration_Section_Key root =
    config->root_section ();

  // Add a section.
  ACE_Configuration_Section_Key testsection;

  if (config->open_section (root,
                            "test",
                            1,
                            testsection))
    return -2;

  // Set some values.
  else if (config->set_string_value (testsection,
                                "stvalue",
                                "stvaluetest"))
    return -3;

  else if (config->remove_value (testsection,
                            "stvalue"))
    return -4;

  else if (config->set_string_value (testsection,
                                     "stvalue",
                                     "stvaluetest"))
    return -3;

  else if (config->set_integer_value (testsection,
                                      "intvalue",
                                      42))
    return -4;

  u_char data[80];

  for(int i = 0; i < 80; i++)
    data[i] = i + 128;

  if (config->set_binary_value (testsection,
                                "binvalue",
                                data,
                                80))
    return -5;

  // Get the values and compare
  ACE_TString stvalue;

  if (config->get_string_value (testsection,
                                "stvalue",
                                stvalue))
    return -6;
  else if (stvalue != "stvaluetest")
    return -7;

  u_int intvalue;

  if (config->get_integer_value (testsection,
                                 "intvalue",
                                 intvalue))
    return -8;
  else if (intvalue != 42)
    return -9;

  u_char *data_out = 0;
  u_int length = 0;

  if (config->get_binary_value (testsection,
                                "binvalue",
                                (void*&) data_out,
                                length))
    return -10;

  // compare em
  for(int j = 0; j < 80; j++)
    if (data_out[j] != data[j])
      return -11;

  delete data_out;

  // Test iteration.
  ACE_TString name;
  ACE_Configuration::VALUETYPE type;
  u_int index = 0;
  int count = 0;

  while (!config->enumerate_values (testsection,
                                    index,
                                    name,
                                    type))
    {
      if (name == "stvalue")
        {
          if (type != ACE_Configuration::STRING)
            return -12;
          count++;
        }
      else if (name == "intvalue")
        {
          if (type != ACE_Configuration::INTEGER)
            return -13;
          count++;
        }
      else if (name == "binvalue")
        {
          if (type != ACE_Configuration::BINARY)
            return -14;
          count++;
        }
      index++;
    }

  // Make sure we got three values.
  if (index != 3 || count !=3)
    return -15;

  // Add some subsections.
  ACE_Configuration_Section_Key test2;
  ACE_Configuration_Section_Key test3;
  ACE_Configuration_Section_Key test4;

  if (config->open_section (testsection,
                            "test2",
                            1,
                            test2))
    return -16;
  else if (config->open_section (testsection,
                                 "test3",
                                 1,
                                 test3))
    return -17;
  else if (config->open_section (testsection,
                                 "test4",
                                 1,
                                 test4))
    return -18;

  // Test enumerate sections.
  index = 0;
  count = 0;
  while (!config->enumerate_sections (testsection,
                                      index,
                                      name))
    {
      if (name == "test2")
        count++;
      else if (name == "test3")
        count++;
      else if (name == "test4")
        count++;
      index++;
    }

  if (index != 3 || count != 3)
    return -19;

  // Remove a subsection
  if (config->remove_section (testsection,
                              "test2",
                              0))
    return -20;

  // Try to remove it again
  if (!config->remove_section (testsection,
                               "test2",
                               0))
    return -21;

  // Try to remove the testsection root, it should fail since it still
  // has subkeys
  if (!config->remove_section (root,
                               "test",
                               0))
    return -22;

  // Test find section
  ACE_Configuration_Section_Key result;

  if (config->open_section (root,
                            "test",
                            0,
                            result))
    return -23;

  // Now test the recursive remove.
  if (config->remove_section (root,
                              "test",
                              1))
    return -24;

  // Make sure its not there
  if (!config->open_section (root,
                             "test",
                             0,
                             testsection))
    return -25;

  return 0;
}

static int
test_io (ACE_Configuration *config)
{
  // Populate with some data
  ACE_Configuration_Section_Key root =
    config->root_section ();
  ACE_Configuration_Section_Key test;

  if (config->open_section(root,
                           "test",
                           1,
                           test))
    return -1;

  ACE_TString value ("string value");

  if (config->set_string_value (test,
                                "stvalue",
                                value))
    return -2;
  else if (config->set_string_value (test,
                                     "stvalue1",
                                     value))
    return -3;
  else if (config->set_integer_value (test,
                                      "intvalue",
                                      42))
    return -4;

  ACE_Configuration_Section_Key test2;

  if (config->open_section (test,
                            "test2",
                            1,
                            test2))
    return -5;
  else if (config->set_string_value (test2,
                                     "2stvalue",
                                     value))
    return -6;
  else if (config->set_string_value (test2,
                                     "2stvalue1",
                                     value))
    return -7;
  else if (config->set_integer_value (test2,
                                      "2intvalue",
                                      42))
    return -8;

  // Export it to a file
  if(config->export_config ("config.ini"))
    return -9;
  // reimport

  if (config->import_config ("config.ini"))
    return -10;

  return 0;
}

static int
run_tests (void)
{
#if defined (ACE_WIN32)
  // test win32 registry implementation.
  HKEY root =
    ACE_Configuration_Win32Registry::resolve_key (HKEY_LOCAL_MACHINE,
                                                  "Software\\ACE\\test");
  if (!root)
    return -1;

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
  ACE_OS::unlink ("test.reg");
  ACE_Configuration_Heap pers_config;

  if (pers_config.open ("test.reg"))
    return 0;
  {
    int result = test (&pers_config);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Persistent Heap Configuration test failed (%d)\n",
                         result),
                        -1);
  }

  // Test file i/o using a transient heap
  ACE_Configuration_Heap io_config;
  if (io_config.open ())
    return -2;

  int result = test_io (&io_config);
  if (result)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "IO Test Failed (%d)\n",
                       result),
                      -3);

  ACE_DEBUG ((LM_DEBUG,
              "Test passed\n"));

  return 0;
}

void
Config_Test::read_config (void)
{
  if (m_ConfigurationHeap.open () == 0)
    {
      if (m_ConfigurationHeap.import_config_as_strings ("Config_Test.ini") == 0)
        {
          ACE_Configuration_Section_Key root = m_ConfigurationHeap.root_section ();
          // Process [network] section
          ACE_Configuration_Section_Key NetworkSection;
          if (m_ConfigurationHeap.open_section (root, "network", 1,
                                                NetworkSection) == 0)
            {
              this->get_section_integer (NetworkSection,
                                         "TimeToLive",
                                         &m_nTimeToLive,
                                         1,
                                         20);
              this->get_section_boolean (NetworkSection,
                                         "Delay",
                                         &m_bDelay);
              this->get_section_string (NetworkSection,
                                        "DestIPAddress",
                                        m_pszDestIPAddress,
                                        TEST_MAX_STRING);
              this->get_section_integer (NetworkSection,
                                         "DestPort",
                                         &m_nDestPort,
                                         0,
                                         65535);
              this->get_section_integer (NetworkSection,
                                         "ReconnectInterval",
                                         &m_nReconnectInterval,
                                         0,
                                         65535);
            }

          // Process [logger] section
          ACE_Configuration_Section_Key LoggerSection;

          if (m_ConfigurationHeap.open_section (root,
                                                "logger", 1,
                                                LoggerSection) == 0)
            {
              this->get_section_string (LoggerSection, 
                                        "Heading",
                                        m_pszHeading,
                                        TEST_MAX_STRING);
              this->get_section_integer (LoggerSection,
                                         "TraceLevel",
                                         &m_nTraceLevel,
                                         1,
                                         20);
              this->get_section_string (LoggerSection,
                                        "Justification",
                                        m_pszJustification,
                                        TEST_MAX_STRING);
              this->get_section_string (LoggerSection,
                                        "LogFilePath",
                                        m_pszLogFilePath,
                                        TEST_MAX_STRING);
              this->get_section_string (LoggerSection,
                                        "TransactionFilePath",
                                        m_pszTransactionFilePath,
                                        TEST_MAX_STRING);
            }
        }
    }
}

void
Config_Test::get_section_string (ACE_Configuration_Section_Key& SectionKey,
                                 const ACE_TCHAR *pszName,
                                 ACE_TCHAR *pszVariable,
                                 int nMaxLength)
{
  ACE_TString StringValue;

  if (m_ConfigurationHeap.get_string_value (SectionKey,
                                            pszName,
                                            StringValue) == 0)
    {
      ACE_OS::strncpy (pszVariable,
                       StringValue.c_str (),
                       nMaxLength);
      ACE_OS::printf ("%s = %s\n",
                      pszName,
                      pszVariable);
    }
}

void
Config_Test::get_section_integer (ACE_Configuration_Section_Key& SectionKey,
                                  const ACE_TCHAR *pszName,
                                  int *nVariable,
                                  int nMinValue,
                                  int nMaxValue)
{
  ACE_TString StringValue;
  ACE_TCHAR pszString[30];
  ACE_OS::strcpy(pszString, "0");
  int IntegerValue = 0;

  if (m_ConfigurationHeap.get_string_value (SectionKey,
                                            pszName,
                                            StringValue) == 0)
    {
      ACE_OS::strncpy (pszString,
                       StringValue.c_str (),
                       30);
      ACE_OS::printf ("%s = %s\n",
                      pszName,
                      pszString);
    }

  // convert to integer
  IntegerValue = ACE_OS::atoi (pszString);

  IntegerValue = (IntegerValue < nMinValue) ? nMinValue : IntegerValue;
  IntegerValue = (IntegerValue > nMaxValue) ? nMaxValue : IntegerValue;

  *nVariable = IntegerValue;
}

void
Config_Test::get_section_boolean (ACE_Configuration_Section_Key& SectionKey,
                                  const ACE_TCHAR* pszName,
                                  bool* pVariable)
{
  ACE_TString StringValue;
  char pszString[10];
  ACE_OS::strcpy (pszString, "0");

  if (m_ConfigurationHeap.get_string_value (SectionKey,
                                            pszName,
                                            StringValue) == 0)
    {
      ACE_OS::strncpy (pszString,
                       StringValue.c_str (),
                       10);
      for (ACE_TCHAR* pSrc = pszString;
           *pSrc != '\0';
           pSrc++)
        // Convert to uppercase
        if (islower (*pSrc))
          *pSrc = tolower (*pSrc);

      ACE_OS::printf ("%s = %s\n",
                      pszName,
                      pszString);

      if (ACE_OS::strcmp (pszString,
                          "TRUE") == 0)
        *pVariable = true;
      else if (ACE_OS::strcmp (pszString,
                               "FALSE") == 0)
        *pVariable = false;
    }
}

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Config_Test"));

  Config_Test manager;
  manager.read_config ();

  run_tests ();

  ACE_END_TEST;
  return 0;
}

