// $Id$

// ===========================================================================================
// FILENAME
//   IFR_Service_Loader.h
//
// DESCRIPTION
//   This class loads the IFR Service dynamically
//   either from svc.conf file or <string_to_object> call.
//
// AUTHORS
//   Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
//   Priyanka Gontla <pgontla@ece.uci.edu>
//
// ==========================================================================================

#ifndef TAO_IFR_SERVICE_LOADER_H
#define TAO_IFR_SERVICE_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IFR_Service_Utils.h"

class TAO_IFRService_Export TAO_IFR_Service_Loader : public TAO_Object_Loader
{
public:

  // Constructor
  TAO_IFR_Service_Loader (void);

  // Destructor
  ~TAO_IFR_Service_Loader (void);

  // Called by the Service Configurator framework to initialize the
  // Event Service. Defined in <ace/Service_Config.h>
  virtual int init (int argc, ACE_TCHAR *argv[]);

  // Called by the Service Configurator framework to remove the
  // Event Service. Defined in <ace/Service_Config.h>
  virtual int fini (void);

  // This function call initializes the IFR Service given a reference to the
  // ORB and the command line parameters.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]
                                           ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException));


 protected:
  TAO_IFR_Server ifr_server_;
  // Instance of the TAO_IFR_Server

private:

ACE_UNIMPLEMENTED_FUNC (TAO_IFR_Service_Loader (const TAO_IFR_Service_Loader &))
ACE_UNIMPLEMENTED_FUNC (TAO_IFR_Service_Loader &operator = (const TAO_IFR_Service_Loader &))
};

ACE_FACTORY_DECLARE (TAO_IFRService, TAO_IFR_Service_Loader)

#endif /* TAO_IFR_SERVICE_LOADER_H */
