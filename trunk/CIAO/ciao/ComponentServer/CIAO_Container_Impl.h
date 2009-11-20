// $Id$
/**
 * @file CIAO_Container_Impl.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_CONTAINER_IMPL_H_
#define CIAO_CONTAINER_IMPL_H_

#include "CIAO_ComponentServerS.h"
#include "CIAO_CS_ClientC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Containers/Session/Session_Container.h"

#include "CIAO_ComponentServer_svnt_export.h"

namespace CIAO
{
  namespace Deployment
  {
    /**
     * @class CIAO_Container
     * @brief Implements external interface for CIAO container.
     */
    class CIAO_COMPONENTSERVER_SVNT_Export CIAO_Container_i
      : public virtual POA_CIAO::Deployment::Container
    {
    public:
      /// Constructor
      CIAO_Container_i (const Components::ConfigValues &config,
                        const Static_Config_EntryPoints_Maps *,
                        const char *name,
                        const CORBA::PolicyList *,
                        Components::Deployment::ComponentInstallation_ptr,
                        CORBA::ORB_ptr,
                        PortableServer::POA_ptr);

      /// Destructor
      virtual ~CIAO_Container_i (void);

      virtual ::Components::CCMObject_ptr install_component (
        const char * id,
        const char * entrypt,
        const ::Components::ConfigValues & config);

      virtual void remove_component (::Components::CCMObject_ptr cref);

      virtual ::Components::CCMObjectSeq * get_components (void);

      virtual ::Components::ConfigValues * configuration (void);

      virtual
        ::Components::Deployment::ComponentServer_ptr get_component_server (void);

      virtual ::Components::CCMHome_ptr install_home (const char * id,
                                                const char * entrypt,
                                                const ::Components::ConfigValues & config);

      virtual void remove_home (::Components::CCMHome_ptr href);

      virtual ::Components::CCMHomes * get_homes (void);

      virtual void remove (void);

      virtual void activate_component (::Components::CCMObject_ptr comp);

      virtual void passivate_component (::Components::CCMObject_ptr comp);

      virtual PortableServer::POA_ptr _default_POA (void);

    private:
      /// Keep a pointer to the managing ORB serving this servant.
      CORBA::ORB_var orb_;

      PortableServer::POA_var poa_;

      CIAO::Container_var container_;

      Components::ConfigValues config_;

      const Static_Config_EntryPoints_Maps* static_entrypts_maps_;

      /// To store all created CCMHome object
      typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                      Components::CCMHome_var,
                                      ACE_Hash<ACE_CString>,
                                      ACE_Equal_To<ACE_CString>,
                                      ACE_Null_Mutex> CCMHome_Map;

      typedef CCMHome_Map::iterator Home_Iterator;
      CCMHome_Map home_map_;

      /// To store all created Component object.
      // @@Gan, see how this caching is duplicated..
      typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                      Components::CCMObject_var,
                                      ACE_Hash<ACE_CString>,
                                      ACE_Equal_To<ACE_CString>,
                                      ACE_Null_Mutex> CCMComponent_Map;

      typedef CCMComponent_Map::iterator Component_Iterator;
      CCMComponent_Map component_map_;

      typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                      ACE_CString,
                                      ACE_Hash<ACE_CString>,
                                      ACE_Equal_To<ACE_CString>,
                                      ACE_Null_Mutex> CCMNaming_Map;
      CCMNaming_Map naming_map_;

      Components::Deployment::ComponentInstallation_var ci_;
    };
  }
}
#endif /* CIAO_CONTAINER_IMPL_H_ */
