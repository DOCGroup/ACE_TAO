// -*- C++ -*-

/**
 * @file IFR_Service_Loader.h
 *
 * @brief Loads the IFR Service dynamically from a @c svc.conf file or a
 * @c string_to_object call.
 *
 * @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 * @author Priyanka Gontla <pgontla@ece.uci.edu>
 */

#ifndef TAO_IFR_SERVICE_LOADER_H
#define TAO_IFR_SERVICE_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IFRService/IFR_Service_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_IFR_Service_Loader : public TAO_Object_Loader
{
public:
  TAO_IFR_Service_Loader ();

  ~TAO_IFR_Service_Loader () override;

  /// Called by the Service Configurator framework to initialize the
  /// Event Service. Defined in <ace/Service_Config.h>
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Called by the Service Configurator framework to remove the
  /// Event Service. Defined in <ace/Service_Config.h>
  int fini () override;

  /// This function call initializes the IFR Service given a reference to the
  /// ORB and the command line parameters.
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc,
                                   ACE_TCHAR *argv[]) override;

protected:
  /// Instance of the TAO_IFR_Server.
  TAO_IFR_Server ifr_server_;

private:
  TAO_IFR_Service_Loader (TAO_IFR_Service_Loader const &) = delete;
  TAO_IFR_Service_Loader (TAO_IFR_Service_Loader &&) = delete;
  TAO_IFR_Service_Loader &operator= (TAO_IFR_Service_Loader const &) = delete;
  TAO_IFR_Service_Loader &operator= (TAO_IFR_Service_Loader &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_IFRService, TAO_IFR_Service_Loader)

#endif /* TAO_IFR_SERVICE_LOADER_H */
