// $Id$

#include "RedirectionService.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace DAnCE;

RedirectionService::RedirectionService (CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa,
                                        CosNaming::NamingContext_ptr hosting_naming,
                                        CosNaming::NamingContext_ptr domain_naming,
                                        bool ns,
                                        bool sl)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , poa_ (PortableServer::POA::_duplicate (poa))
    , naming_ (hosting_naming, domain_naming)
    , ns_ (ns)
    , sl_ (sl)
{
  DANCE_TRACE ("RedirectionService::RedirectionService");
  if (this->ns_)
    {
      if (CORBA::is_nil (hosting_naming))
        {
          DANCE_DEBUG ((LM_DEBUG, DLINFO "RedirectionService::RedirectionService - "
                        "Name Service redirection is enabled but name context is nil.\n"));
        }
    }
}

RedirectionService::~RedirectionService()
{
  DANCE_TRACE ("RedirectionService::~RedirectionService");
  for (TLocators::iterator it = this->locators_.begin ();
       it != this->locators_.end ();
       ++it)
    {
      delete (*it).int_id_;
    }
}

void
RedirectionService::add_node (const ACE_CString& node)
{
  DANCE_TRACE ("RedirectionService::add_node");
  if (this->sl_)
    {
      CCMObjectLocator* locator = 0;
      if (0 != this->locators_.find (node, locator))
        {
          locator = new CCMObjectLocator (this->orb_.in(), this->poa_.in(), node.c_str());
          this->locators_.rebind (node, locator);
        }
    }

  if (this->ns_)
    {
      this->naming_.add_node (node);
    }
}

void
RedirectionService::registration_start (const ACE_CString& node, const ACE_CString& plan)
{
  DANCE_TRACE ("RedirectionService::registration_start");
  if (this->sl_)
    {
      CCMObjectLocator* locator = 0;
      if (0 != this->locators_.find (node, locator))
        {
          locator = new CCMObjectLocator (this->orb_.in(), this->poa_.in(), node.c_str());
          this->locators_.rebind (node, locator);
        }
      locator->start_register (plan);
    }
  if (this->ns_)
    {
      this->naming_.start_binding (node, plan);
    }
}

void
RedirectionService::registration (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, const ACE_CString& port, CORBA::Object_ptr obj)
{
  DANCE_TRACE ("RedirectionService::registration");
  DANCE_DEBUG ((LM_DEBUG, DLINFO "RedirectionService::registration - "
                "for node %s plan %s component %s and port %s is started.\n",
                node.c_str(),
                plan.c_str(),
                component.c_str(),
                port.c_str()));
  CORBA::Object_var ns_obj = CORBA::Object::_duplicate (obj);
  if (this->sl_)
    {
      CCMObjectLocator* locator = 0;
      if (0 != this->locators_.find (node, locator))
        {
          locator = new CCMObjectLocator (this->orb_.in(), this->poa_.in(), node.c_str());
          this->locators_.rebind (node, locator);
        }
      ns_obj = locator->register_object (plan, component, port, obj);
    }
  if (this->ns_)
    {
      if (0 == port.length())
        {
          this->naming_.bind (node, plan, component, ns_obj.in());
        }
      else
        {
          this->naming_.bind (node, plan, component, port, ns_obj.in());
        }
    }
  DANCE_DEBUG ((LM_DEBUG, DLINFO "RedirectionService::registration - "
                "Registration has been finished.\n"));
}

void
RedirectionService::registration (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, CORBA::Object_ptr obj)
{
  DANCE_TRACE ("RedirectionService::registration");
  this->registration (node, plan, component, "", obj);
}

void
RedirectionService::registration_finish (const ACE_CString& node, const ACE_CString& plan)
{
  DANCE_TRACE ("RedirectionService::registration_finish");
  if (this->sl_)
    {
      CCMObjectLocator* locator;
      if (0 != this->locators_.find (node, locator))
        {
          locator = new CCMObjectLocator (this->orb_.in(), this->poa_.in(), node.c_str());
          this->locators_.rebind (node, locator);
        }
      locator->finish_register (plan);
    }
  if (this->ns_)
    {
      this->naming_.finish_binding (node, plan);
    }
}

void
RedirectionService::unregister (const ACE_CString& node, const ACE_CString& plan)
{
  DANCE_TRACE ("RedirectionService::unregister");
  DANCE_DEBUG((LM_TRACE, DLINFO "RedirectionService::unregister - "
               "unregistering %s/%s...\n", node.c_str(), plan.c_str()));
  if (this->ns_)
    {
      this->naming_.unbind_context (node, plan);
      DANCE_DEBUG((LM_TRACE, DLINFO "RedirectionService::unregister - "
                   "finished.\n"));
    }
  else
    {
      DANCE_DEBUG((LM_TRACE, DLINFO "RedirectionService::unregister - "
                   "nothing to do.\n"));
    }
}
