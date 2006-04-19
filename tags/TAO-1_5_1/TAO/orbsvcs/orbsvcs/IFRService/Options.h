// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    Options.h
//
// = DESCRIPTION
//    This class implements the Options container for the Interface
//    Repository.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "orbsvcs/IFRService/ifr_service_export.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export Options
{
  // = TITLE
  //   Maintains the global options.
  //
  // = DESCRIPTION
  //   This class is converted into a Singleton by the
  //   <ACE_Singleton> template.
public:
  Options ();
  // Default Constructor

  ~Options();
  // dtor

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);
  // Parse the command-line arguments and initialize the options.

  const char *ior_output_file (void) const;
  // Return the file where the IOR of the server object is stored.

  int persistent (void) const;
  // Is the Interface Repository persistent?

  const char *persistent_file (void) const;
  // Return the default filename for persistent IFR.

  int using_registry (void) const;
  // Are we using a WIN32 registry as the backing store?

  int enable_locking (void) const;
  // Should the repository have a real mutex or a null mutex?

  int support_multicast_discovery (void) const;
  // should the service support discovery via multicast?

private:
  char *ior_output_file_;
  // File where the IOR of the server object is stored.

  int persistent_;
  // Is the Interface Repository persistent?

  char *persistent_file_;
  // Default filename for persistent IFR.

  int using_registry_;
  // Are we using a WIN32 registry as the backing store?

  int enable_locking_;
  // Should the repository have a real mutex or a null mutex?

  int support_multicast_;
  // If not zero multicast is enabled.
};

// Typedef an Options Singleton.
typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* OPTIONS_H */

