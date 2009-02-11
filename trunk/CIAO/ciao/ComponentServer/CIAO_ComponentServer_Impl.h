// $Id$
/**
 * @file CIAO_ComponentServer_Impl.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_COMPONENTSERVER_IMPL_H_
#define CIAO_COMPONENTSERVER_IMPL_H_

#include "ciao/ComponentServer/CIAO_ComponentServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/String_Base.h"
#include "ace/Unbounded_Set_Ex.h"

namespace CIAO
{
  namespace Deployment
  {
    class  CIAO_ComponentServer_svnt_Export CIAO_ComponentServer_i
      : public virtual POA_CIAO::Deployment::ComponentServer
    {
    public:
      // Constructor
      CIAO_ComponentServer_i (const ACE_CString &uuid, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

      // Destructor
      virtual ~CIAO_ComponentServer_i (void);

      virtual void shutdown (void);

      virtual ::Components::ConfigValues * configuration (void);

      virtual
      ::Components::Deployment::ServerActivator_ptr get_server_activator (void);

      virtual
      ::Components::Deployment::Container_ptr create_container (const ::Components::ConfigValues & config);

      virtual
      void remove_container (::Components::Deployment::Container_ptr cref);

      virtual
      ::Components::Deployment::Containers * get_containers (void);

      virtual
      void remove (void);

      void init (::Components::Deployment::ServerActivator_ptr sa,
                 Components::ConfigValues *cvs);

    private:
      ACE_CString uuid_;
      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      ::Components::Deployment::ServerActivator_var serv_act_;
      Components::ConfigValues_var config_values_;

      template<typename T>
      struct _is_equivalent
      {
        bool operator() (const T &a, const T &b) const
        {
          return a->_is_equivalent (const_cast <T&> (b));
        }
      };

      typedef ACE_Unbounded_Set_Ex <Container_var,
        _is_equivalent<Container_var> > CONTAINERS;

      CONTAINERS containers_;
    };
  }
}

#endif
