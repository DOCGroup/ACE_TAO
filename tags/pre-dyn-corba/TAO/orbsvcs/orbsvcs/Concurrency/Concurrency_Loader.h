// $Id$

// ===========================================================================================
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
// ==========================================================================================

#ifndef TAO_CONCURRENCY_LOADER_H
#define TAO_CONCURRENCY_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Concurrency/Concurrency_Utils.h"

class TAO_Concurrency_Export TAO_Concurrency_Loader : public TAO_Object_Loader
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
                                   int argc, char *argv[]
                                   ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException));


 protected:
  /// Instance of the TAO_Concurrency_Server
  TAO_Concurrency_Server concurrency_server_;

private:

ACE_UNIMPLEMENTED_FUNC (TAO_Concurrency_Loader (const TAO_Concurrency_Loader &))
ACE_UNIMPLEMENTED_FUNC (TAO_Concurrency_Loader &operator = (const TAO_Concurrency_Loader &))
};

ACE_FACTORY_DECLARE (TAO_Concurrency, TAO_Concurrency_Loader)

#endif /* TAO_CONCURRENCY_LOADER_H */
