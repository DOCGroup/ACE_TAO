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
                         PortableServer::POA_var poa,
                         PortableServer::ServantBase *servant,
                         const PortableServer::ObjectId &oid
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
    poa->activate_object_with_id(oid,
                                 servant ACE_ENV_ARG_PARAMETER);

    ACE_CHECK;

    CORBA::Object_var object = poa->
      id_to_reference(oid ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    result = T::_narrow(object.in() ACE_ENV_ARG_PARAMETER);
}
#endif
