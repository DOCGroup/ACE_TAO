// -*- C++ -*-


//=============================================================================
/**
 *  @file    Options.h
 *
 *  This class implements the Options container for the Interface
 *  Repository.
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
  char const *ior_output_file () const;

  /// Is the Interface Repository persistent?
  int persistent () const;

  /// Return the default filename for persistent IFR.
  char const *persistent_file () const;

  /// Are we using a WIN32 registry as the backing store?
  int using_registry () const;

  /// Should the repository have a real mutex or a null mutex?
  int enable_locking () const;

  /// should the service support discovery via multicast?
  int support_multicast_discovery () const;

private:
  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  /// File where the IOR of the server object is stored.
  char *ior_output_file_;

  /// Is the Interface Repository persistent?
  bool persistent_ {};

  /// Default filename for persistent IFR.
  char *persistent_file_;

  /// Are we using a WIN32 registry as the backing store?
  bool using_registry_ {};

  /// Should the repository have a real mutex or a null mutex?
  bool enable_locking_ {};

  /// If not zero multicast is enabled.
  bool support_multicast_ {};
};

// Options Singleton.
using OPTIONS = ACE_Singleton<Options, ACE_Null_Mutex>;

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* OPTIONS_H */

