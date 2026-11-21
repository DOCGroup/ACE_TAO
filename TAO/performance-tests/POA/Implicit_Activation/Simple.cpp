#include "Simple.h"

void
Simple::destroy ()
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);

  poa->deactivate_object (oid.in ());
}
