// -*- C++ -*-

//=============================================================================
/**
 *  @file   UpdateableHandler.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef UPDATEABLEHANDLER_H
#define UPDATEABLEHANDLER_H
#include "orbsvcs/FTRTS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class AMI_Primary_Replication_Strategy;
class Update_Manager;

class UpdateableHandler : public  POA_FTRT::AMI_UpdateableHandler
{
public:
    UpdateableHandler(AMI_Primary_Replication_Strategy* strategy);
    ~UpdateableHandler();

    FTRT::AMI_UpdateableHandler_ptr activate(
      Update_Manager* mgr, int id,
      PortableServer::ObjectId& oid);
    typedef void (Update_Manager::*Handler)(int);

    void dispatch(Handler handler) ;

    virtual void set_update (
      );
    virtual void set_update_excep (
        ::Messaging::ExceptionHolder * excep_holder
      );

private:
  AMI_Primary_Replication_Strategy* strategy_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
