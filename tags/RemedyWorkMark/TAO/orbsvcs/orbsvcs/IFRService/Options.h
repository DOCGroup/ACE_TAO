// -*- C++ -*-


//=============================================================================
/**
 *  @file    Options.h
 *
 *  $Id$
 *
 *  This class implements the Options container for the Interface
 *  Repository.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "orbsvcs/IFRService/ifr_service_export.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class Options
 *
 * @brief Maintains the global options.
 *
 * This class is converted into a Singleton by the
 * <ACE_Singleton> template.
 */
class TAO_IFRService_Export Options
{
public:
  /// Default Constructor
  Options ();

  /// dtor
  ~Options();

  /// Parse the command-line arguments and initialize the options.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Return the file where the IOR of the server object is stored.
  const char *ior_output_file (void) const;

  /// Is the Interface Repository persistent?
  int persistent (void) const;

  /// Return the default filename for persistent IFR.
  const char *persistent_file (void) const;

  /// Are we using a WIN32 registry as the backing store?
  int using_registry (void) const;

  /// Should the repository have a real mutex or a null mutex?
  int enable_locking (void) const;

  /// should the service support discovery via multicast?
  int support_multicast_discovery (void) const;

private:
  /// File where the IOR of the server object is stored.
  char *ior_output_file_;

  /// Is the Interface Repository persistent?
  int persistent_;

  /// Default filename for persistent IFR.
  char *persistent_file_;

  /// Are we using a WIN32 registry as the backing store?
  int using_registry_;

  /// Should the repository have a real mutex or a null mutex?
  int enable_locking_;

  /// If not zero multicast is enabled.
  int support_multicast_;
};

// Typedef an Options Singleton.
typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* OPTIONS_H */

