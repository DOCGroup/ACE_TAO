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

#include "ace/Auto_Ptr.h"
#include "ace/Hash_Map_Manager_T.h"

#include "ciao/DomainApplicationS.h"
#include "DomainApplication_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  // Forward declarations
  class Connection_Manager;

  class DomainApplication_Export DomainApplication_Impl
    : public virtual POA_Deployment::DomainApplication
  {
  public:
    struct Node_Info
    {
      ::Deployment::NodeApplication_var na_;
      ::Deployment::NodeApplicationManager_var nam_;
    };

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
				    Node_Info,
				    ACE_Hash<ACE_CString>,
				    ACE_Equal_To<ACE_CString>,
				    ACE_Null_Mutex> Node_Infos;

    // Constructor 
    DomainApplication_Impl (CORBA::ORB_ptr orb,
			    PortableServer::POA_ptr poa,
			    Node_Infos *nas,
			    Connection_Manager *connections);
    
    // Destructor 
    virtual ~DomainApplication_Impl (void);
    
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
    CORBA::ORB_var orb_;
    
    PortableServer::POA_var poa_;

    auto_ptr<Node_Infos>  nas_;

    auto_ptr<Connection_Manager> conns_;
    
  };
}

#endif /* DOMAINAPPLICATIONI_H_  */

