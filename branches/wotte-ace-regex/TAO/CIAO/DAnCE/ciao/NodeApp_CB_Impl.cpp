// $Id$
#include "NodeApp_CB_Impl.h"

CIAO::NodeApplication_Callback_Impl::
NodeApplication_Callback_Impl  (CORBA::ORB_ptr o,
                                PortableServer::POA_ptr p,
                                Deployment::NodeApplicationManager_ptr s,
                                const Deployment::Properties &properties)
    ACE_THROW_SPEC ((CORBA::SystemException))
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    nam_ (Deployment::NodeApplicationManager::_duplicate (s))
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    //@@ Note: this properties is useless unless
    //   we have some specific properties for the callback obj.
    Deployment::Properties * tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                    Deployment::Properties (properties),
                    CORBA::NO_MEMORY ());
    ACE_TRY_CHECK;
    this->properties_ = tmp;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "NodeApp_CB_Impl::Constructor\t\n");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK;
}

CIAO::NodeApplication_Callback_Impl::~NodeApplication_Callback_Impl ()
{
}

PortableServer::POA_ptr
CIAO::NodeApplication_Callback_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeApplication_Callback_Impl::register_node_application (
    Deployment::NodeApplication_ptr na,
    Deployment::Properties_out properties
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  properties = this->properties_._retn ();

  this->nodeapp_ = Deployment::NodeApplication::_duplicate (na);
  return Deployment::NodeApplicationManager::_duplicate (this->nam_.in ());
}

Deployment::NodeApplication_ptr
CIAO::NodeApplication_Callback_Impl::get_nodeapp_ref (void)
{
  // @@ (OO) How are you relinquishing ownership here?  Since you're
  //         duplicating the reference you actually maintain
  //         ownership.  Is the below comment wrong, or is the code
  //         wrong?

  // Relinquish the ownership of the nodeapplication reference.
  //This method should only be called from the NodeApplicationManager.
  return Deployment::NodeApplication::_duplicate (this->nodeapp_.in ());
}
