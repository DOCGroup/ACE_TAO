// $Id$

#include "ace/Configuration.h"

int
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


int
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

int
main (int, char *[])
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
