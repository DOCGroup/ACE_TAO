// $Id$

#include "NameServiceRedirection.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace DAnCE;

NameServiceRedirection::NameServiceRedirection (CosNaming::NamingContext_ptr naming,
                                                CosNaming::NamingContext_ptr domain)
  : naming_ (CosNaming::NamingContext::_duplicate (naming)), 
    domain_ (CosNaming::NamingContext::_duplicate (domain))
{
}

NameServiceRedirection::~NameServiceRedirection()
{
}

void
NameServiceRedirection::start_binding (const ACE_CString& node, const ACE_CString& plan)
{
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::start_binding for node %s plan %s.\n",
                 node.c_str(),
                 plan.c_str()));
  ACE_CString key = node + plan;
  TRecords* records = 0;
  if (0 != this->transactions_.find (key, records))
    {
      records = new TRecords();
      this->transactions_.rebind (key, records);
    }
  else
    {
      records->clear();
    }
}

void
NameServiceRedirection::bind (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, const ACE_CString& port, CORBA::Object_ptr obj)
{
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::bind for node %s plan %s component %s and port %s is started.\n",
                 node.c_str(),
                 plan.c_str(),
                 component.c_str(),
                 port.c_str()));
  SRecord record;
  record.name.length (4);

  ACE_CString kind = "";
  CosNaming::NamingContext_var node_context = this->resolve_context (node, kind, this->naming_.in());
  record.name[0].id = CORBA::string_dup (node.c_str());
  record.name[0].kind = CORBA::string_dup (kind.c_str());

  kind = "DeploymentPlan";
  CosNaming::NamingContext_var plan_context = this->resolve_context (plan, kind, node_context.in());
  record.name[1].id = CORBA::string_dup (plan.c_str());
  record.name[1].kind = CORBA::string_dup (kind.c_str());

  kind = "";
  CosNaming::NamingContext_var component_context = this->resolve_context (component, kind, plan_context.in());
  record.name[2].id = CORBA::string_dup (component.c_str());
  record.name[2].kind = CORBA::string_dup (kind.c_str());

  record.name[3].id = CORBA::string_dup (port.c_str());
  record.name[3].kind = CORBA::string_dup ("Port");

  ACE_CString key = node + plan;
  TRecords* records = 0;
  if (0 != this->transactions_.find (key, records))
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NameServiceRedirection::bind - Cann't find record %C for closing transaction.\n", key.c_str()));
      return;
    }

  record.obj = CORBA::Object::_duplicate (obj);
  records->push_back (record);

  //    try{
  //        component_context->bind(name, obj);
  //    }
  //    catch(CosNaming::NamingContext::AlreadyBound&)
  //    {
  //        component_context->rebind(name, obj);
  //    }
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::bind has finished.\n"));
}

void
NameServiceRedirection::bind (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, CORBA::Object_ptr obj)
{
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::bind for node %s plan %s and component %s  is started.\n",
                 node.c_str(),
                 plan.c_str(),
                 component.c_str()));
  SRecord record;
  record.name.length (3);

  ACE_CString kind = "";
  CosNaming::NamingContext_var node_context = this->resolve_context (node, kind, this->naming_.in());

  record.name[0].id = CORBA::string_dup (node.c_str());
  record.name[0].kind = CORBA::string_dup (kind.c_str());

  kind = "DeploymentPlan";
  CosNaming::NamingContext_var plan_context = this->resolve_context (plan, kind, node_context.in());

  record.name[1].id = CORBA::string_dup (plan.c_str());
  record.name[1].kind = CORBA::string_dup (kind.c_str());

  ACE_CString key = node + plan;
  TRecords* records = 0;
  if (0 != this->transactions_.find (key, records))
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NameServiceRedirection::bind - Cann't find record %s for closing transaction.\n", key.c_str()));
      return;
    }

  record.name[2].id = CORBA::string_dup (component.c_str());
  record.name[2].kind = CORBA::string_dup ("Component");

  record.obj = CORBA::Object::_duplicate (obj);

  records->push_back (record);
  //    try{
  //        plan_context->bind(name, obj);
  //    }
  //    catch(CosNaming::NamingContext::AlreadyBound&)
  //    {
  //        DANCE_DEBUG((LM_DEBUG, "[%M] NameServiceRedirection::bind - Already bound exception was thrown. Rebinding\n"));
  //        plan_context->rebind(name, obj);
  //    }
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::bind has finished.\n"));
}

void
NameServiceRedirection::finish_binding (const ACE_CString& node, const ACE_CString& plan)
{
  ACE_CString key = node + plan;
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::finish_binding started for %s.\n", key.c_str()));
  TRecords* records = 0;
  if (0 != this->transactions_.find (key, records))
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NameServiceRedirection::finish_binding - Cann't find record %s for closing transaction.\n", key.c_str()));
      return;
    }
  for (unsigned int i = 0; i < records->size(); i++)
    {
      //this->naming_->bind((*records)[i].name, (*records)[i].obj.in());
      try
        {
          this->naming_->bind ( (*records) [i].name, (*records) [i].obj.in());
        }
      catch (CosNaming::NamingContext::AlreadyBound&)
        {
          DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::finish_bind - Already bound exception was thrown. Rebinding\n"));
          this->naming_->rebind ( (*records) [i].name, (*records) [i].obj.in());
        }

    }
  if (!CORBA::is_nil (this->domain_.in()))
    {
      ACE_CString kind = "";
      CosNaming::NamingContext_var context = this->resolve_context (node, kind, this->naming_.in());
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup (node.c_str());
      name[0].kind = CORBA::string_dup (kind.c_str());
      try
        {
          this->domain_->bind_context (name, context.in());
        }
      catch (CosNaming::NamingContext::AlreadyBound&)
        {
          this->domain_->rebind_context (name, context.in());
        }
    }
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::finish_bind has finished.\n"));
}

void
NameServiceRedirection::unbind_context (const ACE_CString& node, const ACE_CString& plan)
{
  DANCE_DEBUG ( (LM_TRACE, "[%M] NameServiceRedirection::unbind_context started...\n"));
  ACE_CString kind = "";
  CosNaming::NamingContext_var node_context = this->resolve_context (node, kind, this->naming_.in());
  kind = "DeploymentPlan";
  CosNaming::NamingContext_var plan_context = this->resolve_context (plan, kind, node_context.in());
  DANCE_DEBUG ( (LM_TRACE, "[%M] NameServiceRedirection::unbind_context before clear_context for %s.%s\n", plan.c_str(), kind.c_str()));
  this->clear_context (plan_context.inout());
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (plan.c_str());
  name[0].kind = CORBA::string_dup (kind.c_str());
  DANCE_DEBUG ( (LM_TRACE, "[%M] NameServiceRedirection::unbind_context before unbinding %s.%s\n"
                 , name[0].id.in(), name[0].kind.in()));
  node_context->unbind (name);
  DANCE_DEBUG ( (LM_TRACE, "[%M] NameServiceRedirection::unbind_context before destroying plan context.\n"));
  plan_context->destroy();
  DANCE_DEBUG ( (LM_TRACE, "[%M] NameServiceRedirection::unbind_context fininshed.\n"));
}

CosNaming::NamingContext_ptr
NameServiceRedirection::resolve_context (const ACE_CString& context_name, const ACE_CString& context_kind, CosNaming::NamingContext_ptr naming)
{
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::resolve_context is started for context %s.%s\n", context_name.c_str(), context_kind.c_str()));
  if (CORBA::is_nil (naming))
    {
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::resolve_context source context is nil!.\n"));
      return CosNaming::NamingContext::_nil ();
    }
  CORBA::Object_var obj;
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (context_name.c_str());
  name[0].kind = CORBA::string_dup (context_kind.c_str());
  try
    {
      obj = naming->resolve (name);
    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      obj = naming->bind_new_context (name);
    }
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NameServiceRedirection::resolve_context has finished.\n"));
  return CosNaming::NamingContext::_narrow (obj._retn());
}

void
NameServiceRedirection::clear_context (CosNaming::NamingContext_ptr& naming)
{
  CosNaming::BindingList_var bl;
  CosNaming::BindingIterator_var bi;
  naming->list (0, bl.out(), bi.out());

  if (CORBA::is_nil (bi.in()))
    {
      return;
    }
  while (bi->next_n (100, bl.out()))
    {
      for (unsigned int i = 0; i < bl->length(); i++)
        {
          if (bl[i].binding_type == CosNaming::ncontext)
            {
              CORBA::Object_var obj = naming->resolve ( (*bl) [i].binding_name);
              CosNaming::NamingContext_var sub_context = CosNaming::NamingContext::_narrow (obj);
              this->clear_context (sub_context.inout());
              naming->unbind ( (*bl) [i].binding_name);
              sub_context->destroy();
            }
          else
            {
              naming->unbind ( (*bl) [i].binding_name);
            }
        }
    }
}

void
NameServiceRedirection::add_node (const ACE_CString& node)
{
  CosNaming::NamingContext_var new_nc = this->resolve_context (node, "", this->naming_);
  if (!CORBA::is_nil (this->domain_.in()))
    {
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup (node.c_str());
      name[0].kind = CORBA::string_dup ("");
      this->domain_->bind_context (name, new_nc.in());
    }
}
