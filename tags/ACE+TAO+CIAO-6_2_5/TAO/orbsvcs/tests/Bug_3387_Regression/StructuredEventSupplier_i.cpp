// $Id$
#include "StructuredEventSupplier_i.h"
#include "tao/PortableServer/PS_CurrentC.h"

StructuredEventSupplier_i::StructuredEventSupplier_i(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

void
StructuredEventSupplier_i::disconnect_structured_push_supplier ()
{

   CORBA::Object_var obj = orb_->resolve_initial_references ("POACurrent");
   PortableServer::Current_var current =
                          PortableServer::Current::_narrow (obj.in());
   PortableServer::POA_var poa = current->get_POA ();
   PortableServer::ObjectId_var objectId = current->get_object_id ();
   poa->deactivate_object (objectId.in());

}

void
StructuredEventSupplier_i::subscription_change (
                    const CosNotification::EventTypeSeq &,
                    const CosNotification::EventTypeSeq &
      )
{
}

