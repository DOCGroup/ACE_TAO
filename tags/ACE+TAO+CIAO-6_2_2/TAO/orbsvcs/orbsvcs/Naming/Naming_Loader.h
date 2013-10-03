// -*- C++ -*-

// $Id$

// ============================================================================
// FILENAME
//   Naming_Loader.h
//
// DESCRIPTION
//   This class loads the Naming Service dynamically
//   either from svc.conf file or <string_to_object> call.
//
// AUTHORS
//   Priyanka Gontla <pgontla@ece.uci.edu>
//
// ============================================================================

#ifndef TAO_NAMING_LOADER_H
#define TAO_NAMING_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Naming/Naming_Server.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Naming_Serv_Export TAO_Naming_Loader : public TAO_Object_Loader
{
public:

  /// Constructor
  /// By default will use the standard Naming_Server.  If a server is provided
  /// it will use that one instead. This object takes ownership of the provided
  /// Naming Server.
  TAO_Naming_Loader (TAO_Naming_Server *server = 0);

  /// Destructor
  ~TAO_Naming_Loader (void);

  /// Called by the Service Configurator framework to initialize the
  /// Event Service. Defined in <ace/Service_Config.h>
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Called by the Service Configurator framework to remove the
  /// Event Service. Defined in <ace/Service_Config.h>
  virtual int fini (void);

  /// This function call initializes the Naming Service given a reference to the
  /// ORB and the command line parameters.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]);

 protected:
  /// TODO: Need to set up service configurator to initialize the naming server
  /// Instance of the TAO_Naming_Server
  TAO_Naming_Server* naming_server_;

private:
  TAO_Naming_Loader (const TAO_Naming_Loader &);
  TAO_Naming_Loader &operator = (const TAO_Naming_Loader &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Naming_Serv, TAO_Naming_Loader)

#endif /* TAO_NAMING_LOADER_H */
