// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Framework_Component_DLL.h
//
// = DESCRIPTION
//    Define class needed for generating templates. IBM C++ requires this to
//    be in its own file for auto template instantiation.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

#ifndef ACE_TESTS_FRAMEWORK_COMPONENT_DLL_H
#define ACE_TESTS_FRAMEWORK_COMPONENT_DLL_H

// Turn on tracing for this library
#define FRAMEWORK_COMPONENT_DLL_NTRACE 0

#include "Framework_Component_DLL_Export.h"
#include "ace/Log_Msg.h"
#include "ace/Synch_T.h"
#include "ace/Singleton.h"

/// This  we will use to test the ACE_Framework_Repository
class Framework_Component_DLL_Export Simple_Service
{
public:
  Simple_Service (void);

  virtual ~Simple_Service (void);

  virtual const ACE_TCHAR *dll_name (void);

};

// Adapter that contains the required dll_name() method and 
// has the "right" name for our DLL.
template <class TYPE>
class FWCT_DLL_Singleton_Adapter_T : public TYPE
{
public:
  const ACE_TCHAR *dll_name (void) 
  { 
    FRAMEWORK_COMPONENT_DLL_TRACE ("FWCT_DLL_Singleton_Adapter_T::dll_name");
    return ACE_TEXT("Framework_Component_DLL");
  }
};

typedef ACE_DLL_Singleton_T < FWCT_DLL_Singleton_Adapter_T <Simple_Service>, 
                     ACE_SYNCH_MUTEX > SS_SINGLETON;

#endif /* ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H */
