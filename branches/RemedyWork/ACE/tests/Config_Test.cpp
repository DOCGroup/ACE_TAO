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
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"



static int
test (ACE_Configuration *config,
      ACE_Configuration_Section_Key &testsection)
{
  ACE_TString stvalue;

  // Set some values.
  if (config->set_string_value (testsection,
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

  static size_t const data_len = 80;
  u_char data[data_len];

  for (size_t i = 0; i < data_len ; ++i)
    data[i] = static_cast<u_char> (i + 128);

  if (config->set_binary_value (testsection,
                                ACE_TEXT ("binvalue"),
                                data,
                                data_len))
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

  u_char *data_out (0);

  {
    void *data_tmp = 0; // Workaround for GCC strict aliasing warning.
    size_t length = 0;

    if (config->get_binary_value (testsection,
                                  ACE_TEXT ("binvalue"),
                                  data_tmp,
                                  length))
      return -10;

    data_out = reinterpret_cast <u_char *> (data_tmp);
  }

  u_char * the_data = static_cast<u_char *> (data_out);

  // compare em
  for (size_t j = 0; j < data_len; ++j)
    if (the_data[j] != data[j])
      return -11;

  delete [] the_data;

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

  {
    // Add some subsections. This part is a separate scope to be sure
    // the test2, test3, test4 keys are closed before further
    // manipulating/deleting the sections further down in the test.
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
  }

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
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p (%d)\n"),
                       ACE_TEXT ("remove_section test2"),
                       ACE_OS::last_error ()),
                      -20);

  // Try to remove it again
  if (!config->remove_section (testsection,
                               ACE_TEXT ("test2"),
                               0))
    return -21;

  return 0;
}

static int
test (ACE_Configuration *config)
{
  const ACE_Configuration_Section_Key& root =
    config->root_section ();

  {
    // Scope this so the testsection key is closed before trying to
    // remove the "test" section.
    ACE_Configuration_Section_Key testsection;

    if (config->open_section (root,
                              ACE_TEXT ("test"),
                              1,
                              testsection))
      return -2;

    int ret_val = test (config, testsection);
    if (ret_val)
      return ret_val;
  }

  // Try to remove the testsection root, it should fail since it still
  // has subkeys
  if (!config->remove_section (root,
                               ACE_TEXT ("test"),
                               0))
    return -22;

  {
    // Test find section, and be sure the key is closed before testing the
    // remove, below.
    ACE_Configuration_Section_Key result;

    if (config->open_section (root,
                              ACE_TEXT ("test"),
                              0,
                              result))
      return -23;
  }

  // Now test the recursive remove.
  if (config->remove_section (root,
                              ACE_TEXT ("test"),
                              1))
    return -24;

  // Make sure its not there
  ACE_Configuration_Section_Key testsectiongone;
  if (!config->open_section (root,
                             ACE_TEXT ("test"),
                             0,
                             testsectiongone))
    return -25;

  return 0;
}

static int
test_subkey_path (ACE_Configuration* config)
{
  ACE_Configuration_Section_Key root =
    config->root_section ();

  ACE_Configuration_Section_Key testsection;

  if (config->open_section (root,
                            ACE_TEXT ("Software\\ACE\\test"),
                            1,
                            testsection))
    return -26;

  int ret_val = test (config, testsection);
  if (ret_val)
    return ret_val;

  if (config->open_section (root,
                            ACE_TEXT ("Software"),
                            0,
                            testsection))
    return -27;

  if (config->remove_section (testsection,
                              ACE_TEXT ("ACE"),
                              1))
    return -28;

  return 0;
}

static int
run_tests (void)
{
  int status;

  {
    // Test import of a legit INI format from a previously-existing file.
    ACE_Configuration_Heap cf;
    if ((status = cf.open ()) != 0)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_Configuration_Heap::open returned %d\n"),
                  status));
    ACE_Ini_ImpExp import (cf);
    // This one should work...
    ACE_TCHAR import_file_name [MAXPATHLEN];
#if defined (TEST_DIR)
    ACE_OS::strcpy (import_file_name, TEST_DIR);
    ACE_OS::strcat (import_file_name, ACE_DIRECTORY_SEPARATOR_STR);
    ACE_OS::strcat (import_file_name, ACE_TEXT ("Config_Test_Import_1.ini"));
#else
    ACE_OS::strcpy (import_file_name, ACE_TEXT ("Config_Test_Import_1.ini"));
#endif

    status = import.import_config (import_file_name);
    if (status != 0) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p: %s\n"),
                  ACE_TEXT ("Config_Test_Import_1.ini failed"),
                  import_file_name));
    }
    else {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s imported\n"), import_file_name));

      // Imported clean; verify content.  See ini file for expected content.
      // Verify the expected sections are there, but no others. Verify the
      // expected keys are there, but no others.
      int section1_seen = 0, section2_seen = 0;
      int somekey_seen = 0, someotherkey_seen = 0;
      int index;
      ACE_TString sect_name;
      const ACE_Configuration_Section_Key &root = cf.root_section ();
      for (index = 0;
           (status = cf.enumerate_sections (root, index, sect_name)) == 0;
           ++index) {
        if (index > 1)   // There are only two sections.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Enumerated %d sections; expected 2\n"),
                      index + 1));
        else {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Enumerated to section %s\n"),
                      sect_name.c_str ()));
          if (sect_name == ACE_TEXT ("SectionOne")) {
            if (section1_seen)
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Saw %s multiple times!\n"),
                          sect_name.c_str ()));
            section1_seen = 1;
            // Check for values in this section.
            ACE_Configuration_Section_Key sect1;
            if (cf.open_section (root, sect_name.c_str (), 0, sect1) != 0)
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Failed to open section: %s\n"),
                          sect_name.c_str ()));
            else {
              int val_index = 0, val_status;
              ACE_TString val_name, value;
              ACE_Configuration::VALUETYPE val_type;
              while ((val_status =
                      cf.enumerate_values
                      (sect1, val_index, val_name, val_type)) == 0) {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Enumerated %s, type %d\n"),
                            val_name.c_str (),
                            val_type));
                if (val_type != ACE_Configuration::STRING)
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Expected %s to be STRING, but %d\n"),
                              val_name.c_str (),
                              val_type));
                if (val_name == ACE_TEXT ("SomeKey")) {
                  if (somekey_seen)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("Saw %s more than once\n"),
                                val_name.c_str ()));
                  somekey_seen = 1;
                }
                else
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected key %s\n"),
                              val_name.c_str ()));
                if ((val_status = cf.get_string_value
                                     (sect1, val_name.c_str (), value)) != 0)
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Can't get value of %s\n"),
                              val_name.c_str ()));
                else {
                  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s value: %s\n"),
                              val_name.c_str (), value.c_str ()));
                  if (value != ACE_TEXT ("SomeValue")) {
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("SomeKey: %s; expected SomeValue\n")));
                  }
                }
                ++val_index;
              }
              if (val_status == 1) {
                if (val_index != 1)     // Should have only seen 1
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Expected 1 value; saw %d\n"),
                              index));
              }
              else
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("Error enumerating %s; status %d\n"),
                            sect_name.c_str (), val_status));
            }
          }
          else if (sect_name == ACE_TEXT ("SectionTwo")) {
            if (section2_seen)
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Saw %s multiple times!\n"),
                          sect_name.c_str ()));
            section2_seen = 1;
            // Check for values in this section.
            ACE_Configuration_Section_Key sect2;
            if (cf.open_section (root, sect_name.c_str (), 0, sect2) != 0)
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Failed to open section: %s\n"),
                          sect_name.c_str ()));
            else {
              int val_index = 0, val_status;
              ACE_TString val_name, value;
              ACE_Configuration::VALUETYPE val_type;
              while ((val_status = cf.enumerate_values
                        (sect2, val_index, val_name, val_type)) == 0) {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Enumerated %s, type %d\n"),
                            val_name.c_str (),
                            val_type));
                if (val_type != ACE_Configuration::STRING)
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Expected %s to be STRING, but %d\n"),
                              val_name.c_str (), val_type));
                if (val_name == ACE_TEXT ("SomeOtherKey")) {
                  if (someotherkey_seen)
                    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Saw %s more than once\n"),
                                val_name.c_str ()));
                  someotherkey_seen = 1;
                }
                else
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected key %s\n"),
                              val_name.c_str ()));
                if ((val_status = cf.get_string_value
                                     (sect2, val_name.c_str (), value)) != 0)
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Can't get value of %s\n"),
                              val_name.c_str ()));
                else {
                  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s value: %s\n"),
                              val_name.c_str (), value.c_str ()));
                  if (value != ACE_TEXT ("SomeOtherValue")) {
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("SomeOtherKey: %s; expected SomeOtherValue\n")));
                  }
                }
                ++val_index;
              }
              if (val_status == 1) {
                if (val_index != 1)     // Should have only seen 1
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Expected 1 value; saw %d\n"),
                              index));
              }
              else
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("Error enumerating %s; status %d\n"),
                            sect_name.c_str (), val_status));
            }
          }
          else {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Saw unexpected section: %s\n"),
                        sect_name.c_str ()));
          }
        }
      }
      if (status == 1) {    // Ran out of sections
        if (index != 2)
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Saw %d sections; expected 2\n"),
                      index));
      }
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Error enumerating sections; status %d\n"),
                    status));
    }
  }

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
  {
    ACE_Configuration_Win32Registry RegConfig (HKEY_LOCAL_MACHINE);
    int result = test_subkey_path (&RegConfig);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Win32Registry test HKEY_LOCAL_MACHINE")
                         ACE_TEXT (" failed (%d)\n"), result),
                        -1);
  }
  // test win32 registry implementation.
  HKEY root =
    ACE_Configuration_Win32Registry::resolve_key (HKEY_LOCAL_MACHINE,
                                                  ACE_TEXT ("Software\\ACE\\test"));
  if (!root)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("resolve_key is broken\n")), -2);

  // test resolving of forward slashes
  HKEY root_fs =
    ACE_Configuration_Win32Registry::resolve_key (HKEY_LOCAL_MACHINE,
                                                  ACE_TEXT ("Software/ACE/test"), 0);
  if (!root_fs)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("resolve_key resolving slashes is broken\n")),
                      -2);

  ACE_Configuration_Win32Registry RegConfig (root);
  {
    int const result = test (&RegConfig);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Win32 registry test root failed (%d)\n"),
                         result),
                        -1);
  }

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_REGISTRY */

  // Test Heap version
  ACE_Configuration_Heap heap_config;

  if (heap_config.open () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open %p\n"),
                       ACE_TEXT ("local-heap config")),
                      -1);
  if (heap_config.open () == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Re-open heap allowed; bugzilla 3724\n")),
                        -1);
    }
  else if (errno != EBUSY)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Re-open heap expected EBUSY (%d), ")
                         ACE_TEXT ("got %d: bugzilla 3724\n"),
                         EBUSY, ACE_ERRNO_GET),
                        -1);
    }
  {
    int result = test_subkey_path (&heap_config);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Heap Config subkey test failed (%d)\n"),
                         result),
                        -1);
  }

  {
    int result = test (&heap_config);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Heap Configuration test failed (%d)\n"),
                         result),
                        -1);
  }

#if !defined (ACE_LACKS_MMAP)
  // Test persistent heap version
  ACE_OS::unlink (ACE_TEXT ("test.reg"));
  ACE_Configuration_Heap pers_config;

  if (pers_config.open (ACE_TEXT ("test.reg")))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open %p\n"),
                       ACE_TEXT ("test.reg")),
                      -1);
  if (pers_config.open (ACE_TEXT ("test.reg")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Re-open(mmap) allowed; bugzilla 3724\n")),
                        -1);
    }
  else if (errno != EBUSY)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Re-open(mmap) expected EBUSY (%d), ")
                         ACE_TEXT ("got %d: bugzilla 3724\n"),
                         EBUSY, ACE_ERRNO_GET),
                        -1);
    }
  if (pers_config.open () == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Re-open(new) allowed; bugzilla 3724\n")),
                        -1);
    }
  else if (errno != EBUSY)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Re-open(new) expected EBUSY (%d), ")
                         ACE_TEXT ("got %d: bugzilla 3724\n"),
                         EBUSY, ACE_ERRNO_GET),
                        -1);
    }

  {
    int result = test (&pers_config);
    if (result)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Persistent Heap Config test failed (%d)\n"),
                         result),
                        -1);
  }
#endif /* !ACE_LACKS_MMAP */

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test passed\n")));
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

  ACE_TString string((ACE_TCHAR*) 0);// = '0';
  // Try to set the unnamed, default value.
  if (cfg.set_string_value (LoggerSection,
                            0,//string.c_str (),//0, //ACE_TEXT ("x"),
                            ACE_TString (ACE_TEXT ("some string"))))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("could not set value with null name\n")),
                      -16);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("here\n")));
  //return 0;
  //ACE_TString string;
  ACE_TString name ((ACE_TCHAR*)0);
  if (cfg.get_string_value (LoggerSection,
                            name.c_str (), //0, //ACE_TEXT ("x"),
                            string))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("error using ACE_TString::c_str() == %d, len == %d\n"),
                       name.c_str(), name.length ()),
                      -17);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("the value for the unnamed var=(%s)\n"),
              string.c_str ()));

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
                         ACE_TEXT ("Cannot open heap1\n")),
                        -1);

    }
  else if ((heap2.open ()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Cannot open heap2\n")),
                        -1);
    }

  // populate them equally
  if (build_config_object (heap1) != 0 ||
      build_config_object (heap2) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("could not build config object\n")),
                      -1);

  // test equality
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The objects should equal...\n")));
  if (heap1 == heap2)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("And they do ;-)\n")));
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("And they do not :-(\n")
                         ACE_TEXT ("operator== Failed when objects equal\n")),
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
                       ACE_TEXT ("Error adding section to heap1\n")),
                      -1);
  else if (heap1.set_integer_value (NewSection,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding value to heap1\n")),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The objects should NOT equal...\n")));
  if (heap1 == heap2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("They Do :-(\noperator== Failed ")
                       ACE_TEXT ("when lhs contains data not in rhs\n")),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("And they do not ;-)\n")));

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
                       ACE_TEXT ("Error adding section to heap2\n")),
                      -1);
  else if (heap2.set_integer_value (NewSection2,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding value to heap2\n")),
                      -2);
  else if (heap2.set_integer_value (NewSection2,
                                    ACE_TEXT ("TestIntValue2"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding second value to heap2\n")),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The objects should NOT equal...\n")));
  if (heap1 == heap2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("And They Do :-(\noperator== Failed ")
                       ACE_TEXT ("when rhs contains value not in lhs\n")),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("And they do not ;-)\n")));

  // add new value in heap 1
  if (heap1.set_integer_value (NewSection,
                               ACE_TEXT ("TestIntValue2"),
                               100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding second value to heap1\n")),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The objects should be equal...\n")));
  if (heap1 == heap2)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("And they are ;-)\n")));
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("And they are not :-(\n")
                       ACE_TEXT ("operator== Failed\n")),
                      -1);

  // Add a new section to heap2
  ACE_Configuration_Section_Key AnotherNewSection2;
  if (heap2.open_section (root2,
                          ACE_TEXT ("AnotherNewSection"),
                          1,
                          AnotherNewSection2))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding second section to heap2\n")),
                      -1);
  else if (heap2.set_integer_value (AnotherNewSection2,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding value in second section")
                       ACE_TEXT (" to heap2\n")),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The objects should NOT equal...\n")));
  if (heap1 == heap2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("And they do :-(\noperator== Failed ")
                       ACE_TEXT ("when rhs contains data not in lhs\n")),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("And they do not :-)\n")));

  // add section back to heap1
  ACE_Configuration_Section_Key AnotherNewSection1;
  if (heap1.open_section (root1,
                          ACE_TEXT ("AnotherNewSection"),
                          1,
                          AnotherNewSection1))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding second section to heap1\n")),
                      -1);
  else if (heap1.set_integer_value (AnotherNewSection1,
                                    ACE_TEXT ("TestIntValue"),
                                    100))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error adding second value to second ")
                       ACE_TEXT ("section in heap1\n")),
                      -2);

  // test equality
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The objects should be equal...\n")));
  if (heap1 == heap2)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("And they are ;-)\n")));
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("And they are not :-(\n")
                       ACE_TEXT ("operator== Failed\n")),
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
  bool rc = true; // start by guessing they are equal

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
        // If the original object does not contain the section then we
        // are not equal.
        rc = false;
      else if (fromFile.open_section (fromFileRoot,
                                      sectionName.c_str (),
                                      0,
                                      fromFileSection) != 0)
        // if there is some error opening the section in the fromFile
        rc = false;
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
                rc = false;
              else
                {
                  ACE_TString fromFileString, originalString;


                  if (fromFile.get_string_value (fromFileSection,
                                                 valueName.c_str (),
                                                 fromFileString) != 0)
                    // we're not equal if we cannot get this string
                    rc = false;
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
                            rc = false;

                          ACE_OS::sprintf (int_value, ACE_TEXT ("%08x"), intValue);
                          originalString = int_value;
                        }
                      else if (originalType == ACE_Configuration::BINARY)
                        {
                          void* binary_data;
                          size_t binary_length;

                          if (original.get_binary_value (originalSection,
                                                         valueName.c_str (),
                                                         binary_data,
                                                         binary_length))
                            // we're not equal if we cannot get this string
                            rc = false;
                          else
                            {
                              ACE_TCHAR bin_value[3];

                              unsigned char* ptr = (unsigned char*)binary_data;
                              while (binary_length)
                                {
                                  if (ptr != binary_data)
                                    originalString += ACE_TEXT (",");

                                  ACE_OS::sprintf (bin_value,
                                                   ACE_TEXT ("%02x"),
                                                   *ptr);
                                  originalString += bin_value;
                                  --binary_length;
                                  ++ptr;
                                }
                              delete [] (char *)binary_data;
                            }// end successful binary read
                        }// end if originalType was binary
                      else
                        // if the type is invalid, then go ahead and fail it.
                        rc = false;

                    }// end if the original type was not a string.
                  else
                    {
                      if (original.get_string_value (originalSection,
                                                     valueName.c_str (),
                                                     originalString) != 0)
                        {
                          // we're not equal if we cannot get rhs string
                          rc = false;
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

      ++sectionIndex;

    }// end section while loop

  // Finally, if the original has any sections, then we're not equal
  sectionIndex = 0;
  while ((rc) &&
         (!original.enumerate_sections (originalRoot,
                                        sectionIndex,
                                        sectionName)))
    ++sectionIndex;

  rc = sectionIndex == 0;

  return rc;
}

// change a network section value
int Config_Test::change_one (ACE_Configuration &cfg, u_int a)
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
                             a))
    return -2;

  return 0;
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
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Equality Test Failed\n")));
          return rc;
        }
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing INI Format Import/Export\n")));
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
                           ACE_TEXT ("Error populating original config ")
                           ACE_TEXT ("object (%d)\n"),
                           rc),
                          -1);
      //  3. Export
      ACE_Ini_ImpExp importExport (original);

      rc = importExport.export_config (ACE_TEXT ("testConfig.ini"));
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Error Exporting (%d)\n"),
                           rc),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Could not open original object (%d)\n"),
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
                           ACE_TEXT ("Error Exporting (%d)\n"),
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
                           ACE_TEXT ("Object read from file does not ")
                           ACE_TEXT ("equal original (%d)\n"),
                           rc),
                          -1);
    }// end if heap could not be opened.
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Could not open fromFile object (%d)\n"),
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
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("INI Format Import/Export Works ;-)\n")));
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
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Equality Test Failed\n")));
          return rc;
        }

    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing Registry Format Import/Export\n")));
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
                           ACE_TEXT ("Error populating original config ")
                           ACE_TEXT ("object (%d)\n"),
                           rc),
                          -1);
      //  3. Export
      ACE_Registry_ImpExp importExport (original);

      rc = importExport.export_config (ACE_TEXT ("testConfig.ini"));
      if (rc != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Error Exporting (%d)\n"),
                           rc),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Could not open original object (%d)\n"),
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
                           ACE_TEXT("Error Exporting (%d)\n"),
                           rc),
                          -1);

      //  7. Compares to original.
      if (fromFile != original)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Object read from file does not ")
                           ACE_TEXT ("equal original (%d)\n"),
                           rc),
                          -1);

      // 7.1 Change a value and test NOT equal case
      change_one (original, 101);
      if (fromFile == original)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("not pass value change test (%d)\n"),
                             rc),
                            -1);
        }

      // 7.2 change value back, they should be equal now
      change_one (original, 100);
        if (fromFile != original)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("not pass value change test (%d)\n"),
                             rc),
                            -1);
        }

    }// end if heap could not be opened.
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Could not open fromFile object (%d)\n"),
                       rc),
                      -1);

  if (!rc)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Registry Format Import/Export Works ;-)\n")));
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

// Reads an integer value from a configuration object (when it's
// stored as a string)
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
                  ACE_TEXT ("%s = %s\n"),
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
        if (ACE_OS::ace_islower (*pSrc))
          *pSrc = ACE_OS::ace_tolower (*pSrc);

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
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Config_Test"));
  int errors = 0;
  Config_Test manager;

  if ((errors += manager.testEquality ()) != 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Failed the equality Test\n")));

  if ((errors += manager.testRegFormat ()) != 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Failed the REG Format Test\n")));

  if ((errors += manager.testIniFormat ()) != 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Failed the INI Format Test\n")));

  if ((errors += run_tests ()) != 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Failed in run_tests\n")));

  ACE_END_TEST;
  return errors == 0 ? 0 : 1;
}
