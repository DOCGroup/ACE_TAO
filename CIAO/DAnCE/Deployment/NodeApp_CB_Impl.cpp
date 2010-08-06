// $Id$

#include "NodeApp_CB_Impl.h"

CIAO::NodeApplication_Callback_Impl::
NodeApplication_Callback_Impl  (CORBA::ORB_ptr o,
                                PortableServer::POA_ptr p,
                                Deployment::NodeApplicationManager_ptr s,
                                const Deployment::Properties &properties,
                                ACE_Condition<TAO_SYNCH_MUTEX> &wait,
                                TAO_SYNCH_MUTEX& mutex)
  : is_callback_completed_ (false),
    orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    nam_ (Deployment::NodeApplicationManager::_duplicate (s)),
    waitCond_ (wait),
    mutex_ (mutex)
{
  try
  {
    //@@ Note: this properties is useless unless
    //   we have some specific properties for the callback obj.
    Deployment::Properties * tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                    Deployment::Properties (properties),
                    CORBA::NO_MEMORY ());
    this->properties_ = tmp;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("NodeApp_CB_Impl::Constructor\t\n");
    throw;
  }
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
    Deployment::Properties_out properties)
{
  this->is_callback_completed_ = true;
  properties = this->properties_._retn ();

  this->nodeapp_ = Deployment::NodeApplication::_duplicate (na);

  this->waitCond_.signal ();
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

bool
CIAO::NodeApplication_Callback_Impl::is_callback_completed ()
{
  return this->is_callback_completed_;
}
