/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    DLL.h
 *
 *  $Id$
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_DLL_H
#define ACE_DLL_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_DLL
 *
 * @brief Provides an abstract interface for handling various DLL
 * operations.
 *
 * This class is an wrapper over the various methods for utilizing
 * a dynamically linked library (DLL), which is called a shared
 * library on some platforms.  Operations <open>, <close>, and
 * <symbol> have been implemented to help opening/closing and
 * extracting symbol information from a DLL, respectively.
 */
class ACE_Export ACE_DLL
{
public:
  // = Initialization and termination methods.

  /// Default constructor.  By default, the <close> operation on the
  /// object will be invoked before it is destroyed.
  ACE_DLL (int close_on_destruction = 1);

  /**
   * This constructor opens and dynamically links <dll_name>.  The
   * default mode is <RTLD_LAZY>, which loads identifier symbols but
   * not the symbols for functions, which are loaded dynamically
   * on-demand.  Other supported modes include: <RTLD_NOW>, which
   * performs all necessary relocations when <dll_name> is first
   * loaded and <RTLD_GLOBAL>, which makes symbols available for
   * relocation processing of any other DLLs.
   */
  ACE_DLL (const ACE_TCHAR *dll_name,
           int open_mode = ACE_DEFAULT_SHLIB_MODE,
           int close_on_destruction = 1);

  /**
   * This method opens and dynamically links <dll_name>.  The default
   * mode is <RTLD_LAZY>, which loads identifier symbols but not the
   * symbols for functions, which are loaded dynamically on-demand.
   * Other supported modes include: <RTLD_NOW>, which performs all
   * necessary relocations when <dll_name> is first loaded and
   * <RTLD_GLOBAL>, which makes symbols available for relocation
   * processing of any other DLLs.  Returns -1 on failure and 0 on
   * success.
   */
  int open (const ACE_TCHAR *dll_name,
            int open_mode = ACE_DEFAULT_SHLIB_MODE,
            int close_on_destruction = 1);

  /// Call to close the DLL object.
  int close (void);

  /**
   * Called when the DLL object is destroyed -- invokes <close> if the
   * <close_on_destruction> flag is set in the constructor or <open>
   * method.
   */
  ~ACE_DLL (void);

  /// If <symbol_name> is in the symbol table of the DLL a pointer to
  /// the <symbol_name> is returned.  Otherwise, returns 0.
  void *symbol (const ACE_TCHAR *symbol_name);

  /// Returns a pointer to a string explaining why <symbol> or <open>
  /// failed.
  ACE_TCHAR *error (void);

  /**
   * Return the handle to the caller.  If <become_owner> is non-0 then
   * caller assumes ownership of the handle and the <ACE_DLL> object
   * won't call <close> when it goes out of scope, even if
   * <close_on_destruction> is set.
   */
  ACE_SHLIB_HANDLE get_handle (int become_owner = 0);

  /// Set the handle for the DLL object. By default, the <close> operation on the
  /// object will be invoked before it is destroyed.
  int set_handle (ACE_SHLIB_HANDLE handle, int close_on_destruction = 1);
private:
  /// This is a handle to the DLL.
  ACE_SHLIB_HANDLE handle_;

  /// This flag keeps track of whether we should close the handle
  /// automatically when the destructor runs.
  int close_on_destruction_;

  // = Disallow copying and assignment since we don't handle these.
  ACE_UNIMPLEMENTED_FUNC (ACE_DLL (const ACE_DLL &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_DLL &))
};

#include "ace/post.h"
#endif /* ACE_DLL_H */
