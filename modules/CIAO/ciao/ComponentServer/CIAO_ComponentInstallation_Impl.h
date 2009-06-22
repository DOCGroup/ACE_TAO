// -*- C++ -*-

//=============================================================================
/**
 *  @file    CIAO_ComponentInstallation_Impl.h
 *
 *  $Id$
 *
 * @Brief  Return location of component artifact
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef COMPONENTINSTALLATION_IMPL_H_
#define COMPONENTINSTALLATION_IMPL_H_

#include "ace/Map_Manager.h"
#include "ciao/ComponentServer/CIAO_CS_ClientS.h"

#include "tao/ORB.h"
#include "ciao/ComponentServer/CIAO_CS_Client_svnt_export.h"

namespace CIAO
{
  namespace Deployment
  {

    class CIAO_CS_Client_svnt_Export ComponentInstallation_Impl
      : public virtual POA_CIAO::Deployment::ComponentInstallation
    {
    public:
      ComponentInstallation_Impl();

      virtual ~ComponentInstallation_Impl();

      virtual void install (const char * implUUID,
                            const char * component_loc);

      virtual void replace (const char * implUUID,
                            const char * component_loc);

      virtual void remove (const char * implUUID);

      virtual char * get_implementation (const char * implUUID);

      virtual char * get_valuetypefactory_location (const char * implUUID,
                                                    const char * repid);

      void install_valuetypefactory_location (const char * implUUID,
                                              const char * repid,
                                              const char * loc);

    private:
      typedef ACE_Map_Manager< ACE_CString, ACE_CString, ACE_Null_Mutex > TLocations;
      TLocations locations_;

      static ACE_CString valuefactory_key (const char* uuid, const char* repid)
      {
        return ACE_CString (repid) + "@" + uuid;
      };
    };
  }
}
#endif /*COMPONENTINSTALLATION_IMPL_H_*/
