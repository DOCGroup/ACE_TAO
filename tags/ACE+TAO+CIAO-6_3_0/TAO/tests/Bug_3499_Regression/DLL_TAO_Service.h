// $Id$

#ifndef _ACE_DLL_TAO_SERVICE_H_
#define _ACE_DLL_TAO_SERVICE_H_

#include "DLL_TAO_Service_export.h"
#include "DLL_Service.h"
#include "tao/PortableServer/PortableServer.h"

class ACE_DLL_TAO_Service : public ACE_DLL_Service
{
public:
  ACE_DLL_TAO_Service (void);

  virtual ~ACE_DLL_TAO_Service (void);

  virtual int init (int argc, ACE_TCHAR * argv []);

  virtual int fini (void);

private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;
};

ACE_DLL_SERVICE_DECL (ACE_DLL_TAO_SERVICE_Export, _make_ACE_DLL_TAO_Service);

#endif  // !defined _ACE_DLL_TAO_SERVICE_H_
