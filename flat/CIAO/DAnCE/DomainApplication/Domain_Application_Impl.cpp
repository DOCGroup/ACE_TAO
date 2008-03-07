// $Id$

#include "Domain_Application_Impl.h"
#include "ace/Log_Msg.h"
#include "ace/streams.h"

namespace DAnCE
{

  DomainApplication_Impl::DomainApplication_Impl (TNam2Nm & nams,
                                                  const Deployment::Properties & configProperty,
                                                  Deployment::Connections& conn)
    : nams_ (nams)
  {
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_Impl constructor\n"));
    this->startLaunch (configProperty);
    conn = this->connections_;
  }

  DomainApplication_Impl::~DomainApplication_Impl()
  {
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_Impl destructor started.\n"));
    try
      {
	for (TApp2Mgr::iterator iter = this->node_applications_.begin();
	     iter != this->node_applications_.end();
	     ++iter)
	  {
	    (*iter).int_id_->destroyApplication ( (*iter).ext_id_.in());
	  }
	this->node_applications_.unbind_all();
      }
    catch (CORBA::Exception &e)
      {
	ACE_ERROR((LM_ERROR, "[%M] DomainApplication_Impl::~DomainApplication_Imp failed with a CORBA exception %s(%s) \"%s\"\n"
		   , e._name(), e._rep_id(), e._info().c_str()));
      }
    catch(...)
      {
	ACE_ERROR((LM_ERROR, "[%M] DomainApplication_Impl::~DomainApplication_Imp failed with an exception.\n"));
      }
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_Impl destructor finished.\n"));
  }

  void
  DomainApplication_Impl::finishLaunch (const Deployment::Connections & connections,
                                        CORBA::Boolean start)
  {
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_impl::finishLaunch - started, connections length is %d\n", connections.length()));
    for (TApp2Mgr::iterator iter = this->node_applications_.begin();
         iter != this->node_applications_.end();
         ++iter)
      {
        (*iter).ext_id_->finishLaunch (connections, start);
      }
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_impl::finishLaunch - finished\n"));
  }

  void
  DomainApplication_Impl::start ()
  {
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_impl::start - started\n"));
    for (TApp2Mgr::iterator iter = this->node_applications_.begin();
         iter != this->node_applications_.end();
         ++iter)
      {
        (*iter).ext_id_->start ();
      }
    ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_impl::start - finished\n"));
  }

  void
  DomainApplication_Impl::startLaunch (const Deployment::Properties & configProperty)
  {
    for (TNam2Nm::iterator iter = this->nams_.begin();
         iter != this->nams_.end();
         ++iter)
      {
        ::Deployment::Connections_var conn;
        Deployment::Application_ptr na = (*iter).ext_id_->startLaunch (configProperty, conn.out());
        //Deployment::NodeApplication_ptr na = Deployment::NodeApplication::_narrow (a);
        if (CORBA::is_nil (na))
          {
            ACE_ERROR ( (LM_ERROR, "[%M] DAnCE (%P|%t) DomainApplication_Impl.cpp -"
                         "DomainApplication_Impl::startLaunch -"
                         "ERROR while calling startLaunch on node managers.\n"));
            throw Deployment::StartError();
          }
        ACE_DEBUG ( (LM_DEBUG, "[%M] DomainApplication_Impl::startLaunch - NodeApplication_ptr received\n"));
        this->node_applications_.bind (na, (*iter).ext_id_);

        size_t before = this->connections_.length();
        this->connections_.length (before + conn->length());
        for (size_t i = 0; i < conn->length(); ++i)
          {
            this->connections_[before+i] = (*conn) [i];
          }
      }
  }

} // DAnCE

