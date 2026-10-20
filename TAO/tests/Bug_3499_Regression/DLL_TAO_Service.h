#ifndef _ACE_DLL_TAO_SERVICE_H_
#define _ACE_DLL_TAO_SERVICE_H_

#include "DLL_TAO_Service_export.h"
#include "DLL_Service.h"
#include "tao/PortableServer/PortableServer.h"

class ACE_DLL_TAO_Service : public ACE_DLL_Service
{
public:
  ACE_DLL_TAO_Service ();

  ~ACE_DLL_TAO_Service () override;

  ACE_DLL_TAO_Service (ACE_DLL_TAO_Service const &) = delete;
  ACE_DLL_TAO_Service (ACE_DLL_TAO_Service &&) = delete;
  ACE_DLL_TAO_Service &operator= (ACE_DLL_TAO_Service const &) = delete;
  ACE_DLL_TAO_Service &operator= (ACE_DLL_TAO_Service &&) = delete;

  int init (int argc, ACE_TCHAR * argv []) override;

  int fini () override;

private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;
};

ACE_DLL_SERVICE_DECL (ACE_DLL_TAO_SERVICE_Export, _make_ACE_DLL_TAO_Service);

#endif  // !defined _ACE_DLL_TAO_SERVICE_H_
