/* -*- C++ -*- */
//=============================================================================
/**
 *  @file  Repository_Configuration.h
 *
 *  $Id$
 *
 *  @brief  Class to maintain the non-Win32 Registry Server
 *  Configuration. Also is a wrapper around ACE_Configuration_Heap
 *  class.
 *  
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef REPOSITORY_CONFIGURATION_H
#define REPOSITORY_CONFIGURATION_H

#include "ace/Configuration.h"

/**
 * @class Repository_Configuration
 *
 * @brief Maintains the server configurations.
 *
 * Class to help maintain the xml and non-win32 registry configuration.
 */
class Repository_Configuration
{
 public:
  
  /// Default Constructor
  Repository_Configuration (const char *repository_mode);
  
  /// Destructor
  ~Repository_Configuration ();

  const ACE_Configuration_Section_Key& root_section ();

  /// 
  int open ();
  
  /// opens a configuration based on a file name
  int open (const char* file_name);
    
  int open_section (const ACE_Configuration_Section_Key& base,
                    const char* sub_section,
                    int create, ACE_Configuration_Section_Key& result);
  
  int remove_section (const ACE_Configuration_Section_Key& key,
                      const char* sub_section,
                      int recursive);
  
  int enumerate_sections (const ACE_Configuration_Section_Key& key,
                          int Index,
                          ACE_CString& name);
  
  int set_string_value (const ACE_Configuration_Section_Key& key,
                        const char* name,
                        const ACE_CString& value);
  
  int set_integer_value (const ACE_Configuration_Section_Key& key,
                         const char* name,
                         u_int value);
  
  int get_string_value (const ACE_Configuration_Section_Key& key,
                        const char* name,
                        ACE_CString& value);
  
  int get_integer_value (const ACE_Configuration_Section_Key& key,
                         const char* name,
                         u_int& value);
  
 private:
  
  ACE_Configuration *config_;

};

#endif /* REPOSITORY_CONFIGURATION_H */
