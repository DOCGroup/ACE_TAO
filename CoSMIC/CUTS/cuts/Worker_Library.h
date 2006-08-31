// -*- C++ -*-

//=============================================================================
/**
 * @file      Worker_Library.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_WORKER_LIBRARY_H_
#define _CUTS_WORKER_LIBRARY_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "ace/SStringfwd.h"
#include "ace/String_Base.h"

// Forward decl.
class CUTS_Worker;

//=============================================================================
/**
 * @class CUTS_Worker_Library
 *
 * @brief Base class for all worker libraries in WML/CUTS.
 */
//=============================================================================

class CUTS_Export CUTS_Worker_Library
{
public:
  /// Default destructor.
  virtual ~CUTS_Worker_Library (void) = 0;

  /**
   * Get the name of the library.
   *
   * @return      NULL-terminated string.
   */
  virtual const char * name (void) const;

  /**
   * Create a new worker with the specified type.
   *
   * @param[in]     type        Type of worker to create; NULL-terminated.
   */
  virtual CUTS_Worker * create_worker (const char * type);

  /// Release all the resource of this object and destroy it. This
  /// method should be used instead of ~CUTS_Worker_Library ().
  virtual void release (void);

protected:
  /// Type definition for the worker creation function.
  typedef CUTS_Worker * (*WORKER_CREATE_FUNC)(void);

  /**
   * Initializing constructor.
   *
   * @param[in]     name      Name of the library; NULL-terminated.
   */
  CUTS_Worker_Library (const char * name);

private:
  /// Type definition for the worker creation table.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                WORKER_CREATE_FUNC,
                                ACE_Null_Mutex> CUTS_Worker_Map;

  /// Name of the library.
  ACE_CString name_;

  /// Mapping of tyoes to worker creation functions.
  CUTS_Worker_Map worker_map_;

  // prevent the following operations
  CUTS_Worker_Library (const CUTS_Worker_Library &);
  const CUTS_Worker_Library & operator = (const CUTS_Worker_Library &);
};

#endif  // !defined _CUTS_WORKER_LIBRARY_H_
