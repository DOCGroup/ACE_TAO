// $Id$
#ifndef CIF_COMMON_H_
#define CIF_COMMON_H_

#include "ciao/ComponentServer/CIAO_CS_ClientS.h"
#include "ciao/ComponentServer/CIAO_ComponentServerC.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"
#include "ciao/ComponentServer/CIAO_ComponentInstallation_Impl.h"
#include "ciao/ComponentServer/CIAO_PropertiesC.h"
#include "ciao/Valuetype_Factories/ConfigValue.h"
#include "CIF_Common_export.h"

class CIF_COMMON_Export CIF_Common
{
public:
  CIF_Common (void);
  ~CIF_Common (void);

  void init (int argc, ACE_TCHAR *argv[]);

  void shutdown (::CIAO::Deployment::ComponentServer_ptr server,
                 ::CIAO::Deployment::Container_ptr cont,
                 ::Components::CCMObject_ptr comp);

  ::CIAO::Deployment::ComponentServer_ptr
  create_componentserver (const int& spawn_delay,
                          const char * cs_path,
                          const char * artifact_name);

  ::CIAO::Deployment::Container_ptr
  create_container (::CIAO::Deployment::ComponentServer_ptr server);

  ::Components::CCMObject_ptr
  install_component (::CIAO::Deployment::Container_ptr cont,
                     const char * artifact_name);

private:
  ::CORBA::ORB_var orb_;
  ::PortableServer::POA_var root_poa_;
  ::CIAO::Deployment::ServerActivator_var sa_;
};

#endif