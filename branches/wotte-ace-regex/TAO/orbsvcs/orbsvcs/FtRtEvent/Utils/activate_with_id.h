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

template<class T>
void
activate_object_with_id (T * &result,
                         PortableServer::POA_ptr poa,
                         PortableServer::ServantBase *servant,
                         const FtRtecEventComm::ObjectId &oid
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  const PortableServer::ObjectId& id = ACE_reinterpret_cast(const PortableServer::ObjectId& ,oid);
  poa->activate_object_with_id(id,
    servant ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;

  CORBA::Object_var object = poa->
    id_to_reference(id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  result = T::_narrow(object.in() ACE_ENV_ARG_PARAMETER);
}
#endif
