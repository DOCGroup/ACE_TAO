// -*- C++ -*-
//
// $Id$

/** 
 * @file DomainApplication_Impl.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * DomainApplication implementation.
 */

#ifndef DOMAINAPPLICATIONI_H_
#define DOMAINAPPLICATIONI_H_

#include "ciao/DomainApplicationS.h"
#include "DomainApplication_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class DomainApplication_Export DomainApplication_i
    : public virtual POA_Deployment::DomainApplication
  {
  public:
    // Constructor 
    DomainApplication_i (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         const Deployment::NodeApplications &nas_,
                         const Deployment::Connections &conns);
    
    // Destructor 
    virtual ~DomainApplication_i (void);
    
    virtual
    void finishLaunch (const ::Deployment::Connections & providedReference,
                       ::CORBA::Boolean start)
      throw (::CORBA::SystemException,
             ::Deployment::StartError,
             ::Deployment::InvalidConnection);
    
    virtual
    void start (void)
      throw (::CORBA::SystemException,
             ::Deployment::StartError);
    
  private:
    Deployment::NodeApplications_var nas_;

    Deployment::Connections_var conns_;
    
  };
}

#endif /* DOMAINAPPLICATIONI_H_  */

