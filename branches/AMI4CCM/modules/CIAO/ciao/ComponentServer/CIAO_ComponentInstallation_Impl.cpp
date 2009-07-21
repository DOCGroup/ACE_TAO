// $Id$

#include "CIAO_ComponentInstallation_Impl.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace Deployment
  {

    ComponentInstallation_Impl::ComponentInstallation_Impl()
    {
      CIAO_TRACE ("ComponentInstallation_Impl::ComponentInstallation_Impl");
    }

    ComponentInstallation_Impl::~ComponentInstallation_Impl()
    {
      CIAO_TRACE ("ComponentInstallation_Impl::~ComponentInstallation_Impl");
    }

    void
    ComponentInstallation_Impl::install (const char * implUUID,    const char * component_loc)
    {
      CIAO_TRACE ("ComponentInstallation_Impl::install");

      CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentInstallation_Impl::install - "
                   "implUUID %s, component_loc %s\n", implUUID, component_loc));
  
      ACE_CString location = component_loc;

      if (0 == this->locations_.find (implUUID))
        {
          this->locations_.bind (implUUID, location);
        }
      else
        {
          CIAO_ERROR ((LM_WARNING, CLINFO "ComponentInstallation_Impl::install - Location for artifact %s "
                       "already present, replacing.\n",
                       implUUID));
          this->locations_.rebind (implUUID, location);
        }
    }


    void
    ComponentInstallation_Impl::replace (const char *uuid , const char *loc)
    {
      CIAO_TRACE ("ComponentInstallation_Impl::replace");
  
      this->locations_.rebind (uuid, loc);
    }

    void
    ComponentInstallation_Impl::remove (const char *uuid)
    {
      CIAO_TRACE ("ComponentInstallation_Impl::remove");
      this->locations_.unbind (uuid);
    }

    char *
    ComponentInstallation_Impl::get_implementation (const char * implUUID)
    {
      CIAO_TRACE ("ComponentInstallation_Impl::get_implementation");

      ACE_CString s;
      if (0 == this->locations_.find (implUUID, s))
        {
          CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentInstallation_Impl::get_implementation - "
                       "ComponentInstallation_Impl::get_implementation for UUID %s, location %s\n", 
                       implUUID, s.c_str()));

          return CORBA::string_dup (s.c_str());
        }
      else
        {
          CIAO_ERROR ((LM_ERROR, CLINFO "ComponentInstallation_Impl::get_implementation - "
                       "cannot find location for specified implementation UUID\n"));
          throw ::Components::Deployment::UnknownImplId();
          return 0;
        }
    }


    char *
    ComponentInstallation_Impl::get_valuetypefactory_location (const char * implUUID,
                                                               const char * repid)
    {
      CIAO_TRACE ("ComponentInstallation_Impl::get_valuetypefactory_location");
  
      ACE_CString key = ComponentInstallation_Impl::valuefactory_key (implUUID, repid);
      ACE_CString s;
      if (0 == this->locations_.find (implUUID, s))
        {
          CIAO_DEBUG ((LM_DEBUG, CLINFO "ComponentInstallation_Impl::get_valuetypefactory_location - "
                       "UUID:\"%s\" repid:\"%s\" -> location:\"%s\"\n", implUUID, repid, s.c_str()));
          return CORBA::string_dup (s.c_str());
        }
      else
        {
          CIAO_ERROR ((LM_ERROR, CLINFO "ComponentInstallation_Impl::get_valuetypefactory_location - "
                       "cannot find location for specified implementation UUID and repid.\n"));
          throw ::Components::Deployment::InstallationFailure();
          return 0;
        }
      return 0;
    }


    void
    ComponentInstallation_Impl::install_valuetypefactory_location (const char * implUUID,
                                                                   const char * repid,
                                                                   const char * loc)
    {
      CIAO_TRACE ("ComponentInstallation_Impl::install_valuetypefactory_location");
      ACE_CString key = ComponentInstallation_Impl::valuefactory_key (implUUID, repid);
      if (0 != this->locations_.find (key))
        {
          ACE_CString location = loc;
          this->locations_.bind (implUUID, location);
        }
      else
        {
          // I don't certaint that we should throw exception here
          //throw ::Components::Deployment::InstallationFailure();
        }
    }
  }
}

