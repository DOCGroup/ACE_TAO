// -*- C++ -*-
//=============================================================================
/**
 *  @file   activate_with_id.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef ACTIVATE_WITH_ID_H
#define ACTIVATE_WITH_ID_H

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T>
void
activate_object_with_id (T * &result,
                         PortableServer::POA_ptr poa,
                         PortableServer::ServantBase *servant,
                         const FtRtecEventComm::ObjectId &oid)
{
  const PortableServer::ObjectId& id =
    reinterpret_cast<const PortableServer::ObjectId&> (oid);
  poa->activate_object_with_id(id,
                               servant);

  CORBA::Object_var object =
    poa->id_to_reference(id);

  result = T::_narrow(object.in());
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
