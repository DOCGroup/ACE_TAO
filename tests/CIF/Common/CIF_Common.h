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

  int init (int argc,
            ACE_TCHAR *argv[],
            const char * artifact_name);

  void shutdown (::CIAO::Deployment::ComponentServer_ptr server,
                 ::CIAO::Deployment::Container_ptr cont,
                 ::Components::CCMObject_ptr comp,
                 bool orb_shutdown=true);

  ::CIAO::Deployment::ComponentServer_ptr
  create_componentserver ();

  ::CIAO::Deployment::Container_ptr
  create_container (::CIAO::Deployment::ComponentServer_ptr server);

  ::Components::CCMObject_ptr
  install_component (::CIAO::Deployment::Container_ptr cont,
                     const char * entrypoint_name);

private:
  ::CORBA::ORB_var orb_;
  ::PortableServer::POA_var root_poa_;
  ::CIAO::Deployment::ServerActivator_var sa_;
  const char * artifact_name_;
  const char *cs_path_;
  CORBA::ULong spawn_delay_;

  int parse_args (int argc, ACE_TCHAR *argv[]);

};

#endif
