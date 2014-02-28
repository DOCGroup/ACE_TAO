// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE ::PortableServer::ThreadPolicyValue
    Cached_Policies::thread (void) const
    {
      return this->thread_;
    }

    ACE_INLINE ::PortableServer::LifespanPolicyValue
    Cached_Policies::lifespan (void) const
    {
      return this->lifespan_;
    }

    ACE_INLINE ::PortableServer::IdUniquenessPolicyValue
    Cached_Policies::id_uniqueness (void) const
    {
      return this->id_uniqueness_;
    }

    ACE_INLINE ::PortableServer::IdAssignmentPolicyValue
    Cached_Policies::id_assignment (void) const
    {
      return this->id_assignment_;
    }

    ACE_INLINE ::PortableServer::ImplicitActivationPolicyValue
    Cached_Policies::implicit_activation (void) const
    {
      return this->implicit_activation_;
    }

    ACE_INLINE ::PortableServer::ServantRetentionPolicyValue
    Cached_Policies::servant_retention (void) const
    {
      return this->servant_retention_;
    }

    ACE_INLINE ::PortableServer::RequestProcessingPolicyValue
    Cached_Policies::request_processing (void) const
    {
      return this->request_processing_;
    }

    ACE_INLINE Cached_Policies::PriorityModel
    Cached_Policies::priority_model (void) const
    {
      return this->priority_model_;
    }

    ACE_INLINE CORBA::Short
    Cached_Policies::server_priority (void) const
    {
      return this->server_priority_;
    }

    ACE_INLINE void
    Cached_Policies::priority_model (PriorityModel priority_model)
    {
      this->priority_model_ = priority_model;
    }

    ACE_INLINE void
    Cached_Policies::server_priority (CORBA::Short priority)
    {
      this->server_priority_ = priority;
    }

     ACE_INLINE Cached_Policies::NetworkPriorityModel
    Cached_Policies::network_priority_model (void) const
    {
      return this->network_priority_model_;
    }

    ACE_INLINE CORBA::Long
    Cached_Policies::request_diffserv_codepoint (void) const
    {
      return this->request_diffserv_codepoint_;
    }

    ACE_INLINE CORBA::Long
    Cached_Policies::reply_diffserv_codepoint (void) const
    {
      return this->reply_diffserv_codepoint_;
    }

    ACE_INLINE void
    Cached_Policies::network_priority_model (
       NetworkPriorityModel network_priority_model)
    {
      this->network_priority_model_ = network_priority_model;
    }

    ACE_INLINE void
    Cached_Policies::request_diffserv_codepoint (
      CORBA::Long diffserv_codepoint)
    {
      this->request_diffserv_codepoint_ = diffserv_codepoint;
    }

    ACE_INLINE void
    Cached_Policies::reply_diffserv_codepoint (CORBA::Long diffserv_codepoint)
    {
      this->reply_diffserv_codepoint_ = diffserv_codepoint;
    }

    ACE_INLINE void
    Cached_Policies::implicit_activation (PortableServer::ImplicitActivationPolicyValue value)
    {
      this->implicit_activation_ = value;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
