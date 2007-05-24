// -*- C++ -*-

// $Id$

// ===========================================================================
// FILENAME
//   Concurrency_Loader.h
//
// DESCRIPTION
//   This class loads the Concurrency Service dynamically
//   either from svc.conf file or <string_to_object> call.
//
// AUTHORS
//   Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
//   Priyanka Gontla <pgontla<ece.uci.edu>
//
// ===========================================================================

#ifndef TAO_CONCURRENCY_LOADER_H
#define TAO_CONCURRENCY_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Concurrency/Concurrency_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Concurrency_Serv_Export TAO_Concurrency_Loader : public TAO_Object_Loader
{
public:

  /// Constructor
  TAO_Concurrency_Loader (void);

  /// Destructor
  ~TAO_Concurrency_Loader (void);

  /// Called by the Service Configurator framework to initialize the
  /// Event Service. Defined in <ace/Service_Config.h>
  virtual int init (int argc, char *argv[]);

  /// Called by the Service Configurator framework to remove the
  /// Event Service. Defined in <ace/Service_Config.h>
  virtual int fini (void);

  /// This function call initializes the Concurrency Service given a
  /// reference to the ORB and the command line parameters.
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc, char *argv[]);


 protected:
  /// Instance of the TAO_Concurrency_Server
  TAO_Concurrency_Server concurrency_server_;

private:

  // Disallow copying and assignment.
  TAO_Concurrency_Loader (const TAO_Concurrency_Loader &);
  TAO_Concurrency_Loader &operator = (const TAO_Concurrency_Loader &);

};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Concurrency_Serv, TAO_Concurrency_Loader)

#endif /* TAO_CONCURRENCY_LOADER_H */
