#include "Handler_i.h"
#include "DomainDataManager.h"

Monitor_Handler_I::Monitor_Handler_I (CIAO::DomainDataManager* manager)
  : manager_ (manager)
{
}

void Monitor_Handler_I::get_resource_data (const ::Deployment::Domain
                                           &ami_return_val)
{
  //ACE_DEBUG ((LM_DEBUG, "get resource data returns \n"));

  ::CORBA::StringSeq elements;
  ::Deployment::DomainUpdateKind kind = ::Deployment::UpdateDynamic;

  manager_->update_domain (elements, ami_return_val, kind);
}

void Monitor_Handler_I::get_resource_data_excep (::Messaging::ExceptionHolder
                                                 * /*excep_holder */)
{
  // handle the exception here ....
  ACE_DEBUG ((LM_DEBUG, "get resource exception occured \n"));

  //  ::Deployment::Domain ami_return_val;

  //  ::CORBA::StringSeq elements;
  //  ::Deployment::DomainUpdateKind kind = ::Deployment::Add;

//  manager_->update_domain (elements, ami_return_val, kind);
}

void Monitor_Handler_I::monitor_app_QoS (void)
{
  ACE_DEBUG ((LM_DEBUG, "Monitor app QoS returned\n"));
}

void Monitor_Handler_I::monitor_app_QoS_excep (::Messaging::ExceptionHolder * /*excep_holder*/)
{
  ACE_DEBUG ((LM_DEBUG, "Monitor app QoS exception\n"));
}
