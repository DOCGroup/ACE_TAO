/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Shared_Object.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SHARED_OBJECT_H)
#define ACE_SHARED_OBJECT_H

#include "ace/ACE.h"

class ACE_Export ACE_Shared_Object
  // = TITLE
  //     Provide the abstract base class used to access dynamic linking 
  // facilities 
{
public:
  virtual int init (int argc, char *argv[]);
  // Initializes object when dynamic linking occurs.

  virtual int fini (void);
  // Terminates object when dynamic unlinking occurs.

  virtual int info (char **info_string, size_t length = 0) const;
  // Returns information on active object.

  virtual ~ACE_Shared_Object (void);
};

#if defined (__ACE_INLINE__)
#include "ace/Shared_Object.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SHARED_OBJECT_H */
