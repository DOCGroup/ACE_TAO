/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    DLL.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_DLL_H
#define ACE_DLL_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_DLL
{
  // = TITLE
  //    Provides an abstract interface for handling various DLL
  //    operations.
  //
  // = DESCRIPTION
  //   This class is an wrapper over the various methods for utilizing
  //   a dynamically linked library (DLL), which is called a shared
  //   library on some platforms.  Operations <open>, <close>, and
  //   <symbol> have been implemented to help opening/closing and
  //   extracting symbol information from a DLL, respectively.
public:
  // = Initialization and termination methods.

  ACE_DLL (int close_on_destruction = 1);
  // Default constructor.  By default, the <close> operation on the
  // object will be invoked before it is destroyed.

  ACE_DLL (ACE_DL_TYPE dll_name,
           int open_mode = ACE_DEFAULT_SHLIB_MODE,
           int close_on_destruction = 1);
  // This constructor opens and dynamically links <dll_name>.  The
  // default mode is <RTLD_LAZY>, which loads identifier symbols but
  // not the symbols for functions, which are loaded dynamically
  // on-demand.  Other supported modes include: <RTLD_NOW>, which
  // performs all necessary relocations when <dll_name> is first
  // loaded and <RTLD_GLOBAL>, which makes symbols available for
  // relocation processing of any other DLLs.

  int open (ACE_DL_TYPE dll_name, 
            int open_mode = ACE_DEFAULT_SHLIB_MODE,
            int close_on_destruction = 1);
  // This method opens and dynamically links <dll_name>.  The default
  // mode is <RTLD_LAZY>, which loads identifier symbols but not the
  // symbols for functions, which are loaded dynamically on-demand.
  // Other supported modes include: <RTLD_NOW>, which performs all
  // necessary relocations when <dll_name> is first loaded and
  // <RTLD_GLOBAL>, which makes symbols available for relocation
  // processing of any other DLLs.  Returns -1 on failure and 0 on
  // success.
  
  int close (void);
  // Call to close the DLL object.

  ~ACE_DLL (void);
  // Called when the DLL object is destroyed -- invokes <close> if the
  // <close_on_destruction> flag is set in the constructor or <open>
  // method.

  void *symbol (ACE_DL_TYPE symbol_name);
  // If <symbol_name> is in the symbol table of the DLL a pointer to
  // the <symbol_name> is returned.  Otherwise, returns 0.

  char *error (void);
  // Returns a pointer to a string explaining why <symbol> or <open>
  // failed.

  ACE_SHLIB_HANDLE get_handle (int become_owner = 0);
  // Return the handle to the caller.  If <become_owner> is non-0 then
  // caller assumes ownership of the handle and the <ACE_DLL> object
  // won't call <close> when it goes out of scope, even if
  // <close_on_destruction> is set.

private:
  ACE_SHLIB_HANDLE handle_;
  // This is a handle to the DLL.
  
  int close_on_destruction_;
  // This flag keeps track of whether we should close the handle
  // automatically when the destructor runs.

  // = Disallow copying and assignment since we don't handle these.
  ACE_UNIMPLEMENTED_FUNC (ACE_DLL (const ACE_DLL &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_DLL &))
};

#endif /* ACE_DLL_H */
