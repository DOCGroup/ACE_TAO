// -*- C++ -*-

//=============================================================================
/**
 *  @file   Replication_Service.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef REPLCATION_SERVICE_H
#define REPLCATION_SERVICE_H

#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Become_Primary_Listener.h"
#include "tao/PortableInterceptorC.h"
#include "ace/Service_Object.h"
#include "orbsvcs/FtRtEvent/EventChannel/ftrtec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace FTRTEC
{
  class TAO_FTRTEC_Export Replication_Service
    : public TAO_FTEC_Become_Primary_Listener
    , public ACE_Service_Object
  {
  public:
    static Replication_Service* instance();

    Replication_Service();
    ~Replication_Service();

    virtual int init (int argc,
      ACE_TCHAR* []);

    virtual void become_primary();

    /**
     * Used for checking if the incoming replication message is out of sequence.
     */
    void check_validity(void);

    typedef void (FtRtecEventChannelAdmin::EventChannelFacade::*RollbackOperation)
      (const FtRtecEventChannelAdmin::ObjectId&);

   /**
    * Replicate a request.
    *
    * @param state The request to be replicated.
    * @param rollback The rollback operation when the replication failed.
    */
    void replicate_request(const FtRtecEventChannelAdmin::Operation& update,
      RollbackOperation rollback);

   /**
    * Inform the backup replicas that a new replica has joined the object
    * group.
    */
    void add_member(const FTRT::ManagerInfo & info,
                    CORBA::ULong object_group_ref_version);

    int  acquire_read (void);
    int  acquire_write (void);
    int  release (void);

    int threads() const;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_FTRTEC, Replication_Service)
ACE_STATIC_SVC_REQUIRE(Replication_Service)
ACE_FACTORY_DECLARE (TAO_FTRTEC, Replication_Service)

#endif //REPLCATION_SERVICE_H
