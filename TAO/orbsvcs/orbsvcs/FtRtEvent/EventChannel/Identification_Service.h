// -*- C++ -*-

//=============================================================================
/**
 *  @file   Identification_Service.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef IDENTIFICATION_SERVICE_H
#define IDENTIFICATION_SERVICE_H

#include "ace/Service_Object.h"
#include "PortableServer/PortableServerC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/FTRTC.h"
#include "ftrtec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace FTRTEC
{
  class Identification_Service : public ACE_Service_Object
  {
  public:
    Identification_Service();
    ~Identification_Service();

    /**
    * This function returns 0 when init is not called yet.
    */
    static Identification_Service* instance();

    int init(int argc, ACE_TCHAR* argv[]);

    const PortableServer::ObjectId& object_id() const;
    const CosNaming::Name& name() const;
  private:
    PortableServer::ObjectId object_id_;
    CosNaming::Name name_;
  };


  ACE_STATIC_SVC_DECLARE_EXPORT (TAO_FTRTEC, Identification_Service)
  ACE_STATIC_SVC_REQUIRE(Identification_Service);
  ACE_FACTORY_DECLARE (TAO_FTRTEC, Identification_Service)
}

#endif
