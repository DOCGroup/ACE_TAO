
//=============================================================================
/**
 *  @file    Config_Test.h
 *
 *  $Id$
 *
 *    This is a test that makes sure the <import_config_as_strings> of
 *    <ACE_Configuration_Heap> class works correctly.
 *
 *
 *  @author Michael Searles <msearles@base16.com> and Jerry D. Odenwelder Jr. <jerry.o@mindspring.com>
 */
//=============================================================================


#ifndef __CONFIG_TEST_H
#define __CONFIG_TEST_H

#include "ace/Configuration.h"

const int TEST_MAX_STRING = 256;

class Config_Test
{
public:
  Config_Test (void): equality_tested_ (0) { }
  ~Config_Test (void) { }

  // Used to test the equality and inequality operations.
  int testEquality ();

  // Used to test INI Import Export class
  int testIniFormat ();

  // Used to test Regiastry Import Export class
  //  1. Creates an ACE_Configuration_Heap object
  //  2. Calls buildConfigObject to populate
  //  3. Exports
  //  5. Creates a new ACE_Configuration_Heap object
  //  6. Imports
  //  7. Compares to origional.
  //  8. Clean-up
  int testRegFormat ();


private:
  // change a value for value change test
  int change_one (ACE_Configuration &l, u_int a = 101);

  void get_section_string (ACE_Configuration&             config,
                           ACE_Configuration_Section_Key& SectionKey,
                           const ACE_TCHAR*               pszName,
                           ACE_TCHAR*                     pszVariable,
                           int                            nMaxLength);

  void get_section_integer (ACE_Configuration&             config,
                            ACE_Configuration_Section_Key& SectionKey,
                            const ACE_TCHAR*               pszName,
                            int*                           nVariable,
                            int                            nMinValue,
                            int                            nMaxValue);

  void get_section_boolean (ACE_Configuration&             config,
                            ACE_Configuration_Section_Key& SectionKey,
                            const ACE_TCHAR*               pszName,
                            int*                           pVariable);

private:
  int equality_tested_; // make sure the equality operator works before proceeding.
};

#endif /* __CONFIG_TEST_H */
