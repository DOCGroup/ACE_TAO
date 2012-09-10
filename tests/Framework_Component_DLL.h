
//=============================================================================
/**
 *  @file    Framework_Component_DLL.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Don Hinton <dhinton@ieee.org>
 */
//=============================================================================


#ifndef ACE_TESTS_FRAMEWORK_COMPONENT_DLL_H
#define ACE_TESTS_FRAMEWORK_COMPONENT_DLL_H

#include "Framework_Component_DLL_Export.h"
#include "ace/Log_Msg.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"

/// This  we will use to test the ACE_Framework_Repository
class Simple_Service
{
public:
  Simple_Service (void);

  virtual ~Simple_Service (void);

  //virtual const ACE_TCHAR *dll_name (void);

  virtual const ACE_TCHAR *name (void);

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
