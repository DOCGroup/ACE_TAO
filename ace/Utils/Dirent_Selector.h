/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Dirent_Selector.h
 *
 *  $Id$
 *
 *  Define a portable C++ interface to the <ACE_OS_Dirent::scandir> method.
 *
 *  @author Rich Newman <RNewman@directv.com>
 */
//=============================================================================

#ifndef ACE_DIRENT_SELECTOR_H
#define ACE_DIRENT_SELECTOR_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Dirent
 *
 * @brief Define a portable C++ directory-entry iterator based on the
 * POSIX @param scandir API.
 */
class ACE_Export ACE_Dirent_Selector
{
public:
  /// Constructor
  ACE_Dirent_Selector (void);

  /// Destructor.
  virtual ~ACE_Dirent_Selector (void);

  /// Return the length of the list of matching directory entries.
  int length (void) const;

  /// Return the entry at <index>.
  dirent *operator[] (const int index) const;

  /// Free up resources.
  int close (void);

  /// Open the director <dir> and populate the <namelist_> array with
  /// directory entries that match the <selector> and <comparator>.
  int open (const ACE_TCHAR *dir,
            int (*selector)(const dirent *d) = 0,
            int (*comparator)(const dirent **d1, const dirent **d2) = 0);

protected:
  /// Ptr to the namelist array.
  dirent **namelist_;

  /// # of entries in the array.
  int n_;
};

#if defined (__ACE_INLINE__)
#include "ace/Utils/Dirent_Selector.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_DIRENT_SELECTOR_H */
