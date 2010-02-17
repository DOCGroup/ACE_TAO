// $Id$

#include "Domain_Application_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  DomainApplication_Impl::DomainApplication_Impl (
    TNam2Nm & nams,
    const Deployment::Properties & configProperty,
    Deployment::Connections& conn)
      : nams_ (nams)
  {
    DANCE_TRACE("DomainApplication_Impl::DomainApplication_Impl");
    this->startLaunch (configProperty);
    conn = this->connections_;
  }

  DomainApplication_Impl::~DomainApplication_Impl()
  {
    DANCE_TRACE ("DomainApplication_Impl::~DomainApplication_Impl()");

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
    catch (const CORBA::Exception &e)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplication_Impl::~DomainApplication_Impl - ")
                    ACE_TEXT("caught a CORBA exception %C(%C) \"%C\"\n"),
                    e._name(), e._rep_id(), e._info().c_str()));
        throw;
      }
    catch(...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("DomainApplication_Impl::~DomainApplication_Impl - ")
                     ACE_TEXT("caught an unknown exception.\n")));
      }
  }

  void
  DomainApplication_Impl::finishLaunch (const Deployment::Connections & connections,
                                        CORBA::Boolean start)
  {
    DANCE_TRACE ("DomainApplication_Impl::finishLaunch");
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplication_impl::finishLaunch - ")
                  ACE_TEXT("connections length is %d\n"), connections.length()));
    for (TApp2Mgr::iterator iter = this->node_applications_.begin();
         iter != this->node_applications_.end();
         ++iter)
      {
        (*iter).ext_id_->finishLaunch (connections, start);
      }
  }

  void
  DomainApplication_Impl::start ()
  {
    DANCE_TRACE ("DomainApplication_Impl::start");
    for (TApp2Mgr::iterator iter = this->node_applications_.begin();
         iter != this->node_applications_.end();
         ++iter)
      {
        (*iter).ext_id_->start ();
      }
  }

  void
  DomainApplication_Impl::startLaunch (
    const Deployment::Properties & configProperty)
  {
    DANCE_TRACE ("DomainApplication_Impl::startLaunch");

    for (TNam2Nm::iterator iter = this->nams_.begin();
         iter != this->nams_.end();
         ++iter)
      {
        DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                      ACE_TEXT("Invoking startLaunch on an application\n")));
        ::Deployment::Connections_var conn;
        Deployment::Application_ptr na = (*iter).ext_id_->startLaunch (configProperty, conn.out());
        if (CORBA::is_nil (na))
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("DomainApplication_Impl::startLaunch -")
                         ACE_TEXT("Received nil object reference from  startLaunch on node managers.\n")));
            throw Deployment::StartError();
          }
        DANCE_DEBUG (9, (LM_TRACE, DLINFO
                      ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                      ACE_TEXT("Application startLaunch succeeded, received %u connections\n"),
                      conn->length ()));

        this->node_applications_.bind (na, (*iter).ext_id_);

        CORBA::ULong const before = this->connections_.length();
        this->connections_.length (before + conn->length());
        for (CORBA::ULong i = 0; i < conn->length(); ++i)
          {
            this->connections_[before+i] = (*conn) [i];
          }
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                  ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                  ACE_TEXT("Received %u connections from applications\n"),
                  this->connections_.length ()));
  }
} // DAnCE

