/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Dynamic_Service.h
 *
 *  $Id$
 *
 *  @author Prashant Jain
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_DYNAMIC_SERVICE_H
#define ACE_DYNAMIC_SERVICE_H
#include "ace/pre.h"

#include "ace/OS.h"

/**
 * @class ACE_Dynamic_Service
 *
 * @brief Provides a general interface to retrieve arbitrary objects
 * from the ACE service repository.
 *
 * Uses "name" for lookup in the ACE service repository. Obtains
 * the object and returns it as the appropriate type.
 */
template <class SERVICE>
class ACE_Dynamic_Service
{
public:
  /// Return instance using <name> to search the Service_Repository.
  static SERVICE *instance (const ACE_TCHAR *name);

  /// Dump the current state of the object.
  void dump (void) const;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# include "ace/Dynamic_Service.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
# pragma implementation ("Dynamic_Service.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_DYNAMIC_SERVICE_H */
