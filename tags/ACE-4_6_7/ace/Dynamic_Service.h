/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Dynamic_Service.h
//
// = AUTHOR
//    Prashant Jain, Doug Schmidt
//
// ============================================================================

#ifndef ACE_DYNAMIC_SERVICE_H
#define ACE_DYNAMIC_SERVICE_H

template <class SERVICE>
class ACE_Dynamic_Service
{
  // = TITLE
  //     Provides a general interface to retrieve arbitrary objects
  //     from the ACE service repository.
  //
  // = DESCRIPTION
  //     Uses "name" for lookup in the ACE service repository. Obtains
  //     the object and returns it as the appropriate type.
public:
  static SERVICE *instance (const char *name);
  // Return instance using <name> to search the Service_Repository.

  void dump (void) const;
  // Dump the current state of the object.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# include "ace/Dynamic_Service.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
# pragma implementation ("Dynamic_Service.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_DYNAMIC_SERVICE_H */
