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
  //   a dynamically linked library (DLL). The operations like open,
  //   close, symbol have been implemented to help opening and
  //   extracting symbol information from a DLL.
public:
  // = Initialization and termination methods.

  ACE_DLL (int close_on_destruction = 0);
  // Default constructor. Also, by default the close operation
  // on the object will be done before it is destroyed.

  ACE_DLL (ACE_DL_TYPE dll_name = 0,
           int open_mode = ACE_DEFAULT_SHLIB_MODE,
           int close_on_destruction = 0);
  // This is the library open operation that uses ACE_SHLIB_HANDLE
  // object internally to open the library.  The default mode is
  // RTLD_LAZY which means that the identifier symbols are loaded but
  // not the symbols for the functions.  They are loaded dynamically
  // on need.  The other modes include: RTLD_NOW All necessary
  // relocations are performed when the object is first loaded.
  // RTLD_GLOBAL The object symbols are made available for the
  // relocation processing of any other object.

  int open (ACE_DL_TYPE dll_name, 
            int open_mode = ACE_DEFAULT_SHLIB_MODE,
            int close_on_destruction = 0);
  // This is the library open operation that uses ACE_SHLIB_HANDLE
  // object internally to open the library.  The default mode is
  // RTLD_LAZY which means that the identifier symbols are loaded but
  // not the symbols for the functions.  They are loaded dynamically
  // on need.  The other modes include: RTLD_NOW All necessary
  // relocations are performed when the object is first loaded.
  // RTLD_GLOBAL The object symbols are made available for the
  // relocation processing of any other object.
  
  ~ACE_DLL (void);
  // Called when object is destroyed.

  int close (void);
  // The library is closed.

  void *symbol (ACE_DL_TYPE sym_name);
  // The symbol reference is returned corresponding to the symbol
  // name.

  char *error (void);
  // The error on any of the dl-procedures.

  ACE_SHLIB_HANDLE get_handle (int become_owner = 0);
  // Return the handle to the user either forever or for temporary
  // use.

private:
  ACE_SHLIB_HANDLE handle_;
  // This is the reference to the library.
  
  int close_on_destruction_;
  // This flag keeps track of whether we should close the handle
  // automatically when the destructor is run.
};

#endif /* ACE_DLL_H */
