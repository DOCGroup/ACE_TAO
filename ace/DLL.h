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
  //= TITLE
  //    Provides an abstract interface for handling various DLL operations.
  //= DESCRIPTION
  //   This class is an wrapper over the various methods for utilizing
  //   an dynamically linked library. The operations like open, close,
  //   symbol have been implemented which will help in opening anmd
  //   extracting symbol information from the library.
 
public:
  ACE_DLL (void);
  // Initialisation.
 
  ~ACE_DLL (void);
  // Called when object is destroyed.

  int open (ACE_DL_TYPE dll_name, 
            int mode = ACE_DEFAULT_SHLIB_MODE);
  // This is the library open operation which uses ACE_SHLIB_HANDLE
  // object internally to open the library.
  // The default mode is RTLD_LAZY which means that the identifier
  // symbols are loaded but not the symbols for the functions.
  // They are loaded dynamically on need.
  // The other modes include:
  //  RTLD_NOW     All necessary relocations are performed when the
  //               object is first loaded.
  //  RTLD_GLOBAL  The object symbols are made available for the
  //               relocation processing of any other object. 

  void *symbol (ACE_DL_TYPE sym_name);
  // The symbol reference is returned corresponding to the symbol
  // name.

  int close (void);
  // The library is closed.

  char *error ();
  // The error on any of the dl-procedures.

private:
  ACE_SHLIB_HANDLE handle_;
  // This is the reference to the library.
};

#endif /* ACE_DLL_H */
