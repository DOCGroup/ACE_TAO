// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    ConfigTest.h
//
// = DESCRIPTION
//      This is a test that makes sure the <import_config_as_strings> of
//      <ACE_Configuration_Heap> class works correctly.
//
// = AUTHOR
//    Michael Searles <msearles@base16.com>
//
// ============================================================================

#ifndef __CONFIG_TEST_H
#define __CONFIG_TEST_H

#include "ace/Configuration.h"

const int TEST_MAX_STRING = 256;

class Config_Test
{
  ACE_Configuration_Heap m_ConfigurationHeap;

  int m_nTimeToLive;
  bool m_bDelay;
  char m_pszDestIPAddress[TEST_MAX_STRING];
  int m_nDestPort;
  int m_nReconnectInterval;

  char m_pszLogFilePath[TEST_MAX_STRING];
  char m_pszTransactionFilePath[TEST_MAX_STRING];
  char m_pszHeading[TEST_MAX_STRING];
  int m_nTraceLevel;
  char m_pszJustification[TEST_MAX_STRING];

public:
  Config_Test (void) { }
  ~Config_Test (void) { }
  void read_config (void);

private:
  void get_section_string (ACE_Configuration_Section_Key& SectionKey,
                           const ACE_TCHAR *pszName,
                           char *pszVariable,
                           int nMaxLength);
  void get_section_integer (ACE_Configuration_Section_Key& SectionKey,
                            const ACE_TCHAR *pszName,
                            int *nVariable,
                            int nMinValue,
                            int nMaxValue);
  void get_section_boolean (ACE_Configuration_Section_Key& SectionKey,
                            const ACE_TCHAR *pszName,
                            bool *pVariable);
};

#endif /* __CONFIG_TEST_H */
