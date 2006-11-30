// -*- C++ -*-
//
// $Id$

#include "ciao/CIAO_common.h"
#include "ciao/Connection_Manager.h"

#include "DomainApplication_Impl.h"

namespace CIAO
{
  // Implementation skeleton constructor
  DomainApplication_Impl::DomainApplication_Impl (CORBA::ORB_ptr orb,
					    PortableServer::POA_ptr poa,
					    Node_Infos *nas,
					    Connection_Manager *conns)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      nas_ (nas),
      conns_ (conns)
  {
  }

  // Implementation skeleton destructor
  DomainApplication_Impl::~DomainApplication_Impl (void)
  {
  }

  void DomainApplication_Impl::finishLaunch (const ::Deployment::Connections & providedReference,
					  ::CORBA::Boolean start)
    throw (::CORBA::SystemException,
	   ::Deployment::StartError,
	   ::Deployment::InvalidConnection)

  {
    CIAO_TRACE("CIAO::DomainApplicationManager_Impl::finishLaunch");
    try
      {
	for (Node_Infos::ITERATOR i = this->nas_->begin ();
	     i != this->nas_->end ();
	     ++i)
	  {
	    // Get the Connections variable, if ReDaC is true, then we get
	    // those new connections only. NOTE: get_outgoing_connections
	    // by default will get *all* connections.
	    Deployment::Connections my_connections =
	      this->conns_->fetch_connections ();
	  
	    /* I am not sure this is necessary, given the new connection
	       method
	       if (my_connections == 0)
	       {
	       ACE_ERROR ((LM_ERROR,
	       "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
	       "CIAO::DomainApplicationManager_Impl::finishLaunch -"
	       "ERROR while getting the outgoing connections "
	       "for the node [%s] \n",
	       this->node_manager_names_[i].c_str ()));
              
	       throw Deployment::StartError
	       ("DomainApplicationManager_Impl::finish_launch",
	       "There was some error establishing connections."));
	       }
	    */
	  
	    /* @@ TODO:  Need to narrow down the list of connections
	       to only those necessary for the node in question.
	       // Dump the connections for debug purpose.
	       if (CIAO::debug_level () > 1)
	       {
	       ACE_DEBUG ((LM_DEBUG,
	       "==============================================\n"));
	       ACE_DEBUG ((LM_DEBUG,
	       "dump outgoing connections for child plan:%s\n",
	       (entry->int_id_).child_plan_->UUID.in ()));
	       dump_connections (*my_connections);
	       ACE_DEBUG ((LM_DEBUG,
	       "==============================================\n"));
	       }
	    */
	    // Invoke finishLaunch() operation on NodeApplication.
	    i->item ().na_->finishLaunch (my_connections,
				      start);
	  }
      }
    catch (Deployment::StartError &ex)
      {
	ACE_ERROR ((LM_ERROR, "DAM_Impl::finishLaunch - StartError: %s, %s\n",
		    ex.name.in (),
		    ex.reason.in ()));
	throw;
      }
    catch (CORBA::SystemException &ex)
      {
	CIAO_PRINT_EXCEPTION (1, ex, "DomainApplication_Impl::FinishLaunch\n");
      
	// Invoke destroyManager() operation on each cached
	// NodeManager object.
	for (Node_Infos::ITERATOR i = this->nas_->begin ();
	     i != this->nas_->end ();
	     ++i)
	  {
	    // Invoke destoryApplication() operation on the NodeApplicationManger.
	    i->item ().nam_->destroyApplication (i->item ().na_.in ());
	  }
	throw;
      }
  }

  void DomainApplication_Impl::start (void)
    throw (::CORBA::SystemException,
	   ::Deployment::StartError)
  {
    // Add your implementation here
  }

}

