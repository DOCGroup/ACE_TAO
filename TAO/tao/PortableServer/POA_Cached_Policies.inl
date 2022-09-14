// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE ::PortableServer::ThreadPolicyValue
    Cached_Policies::thread () const
    {
      return this->thread_;
    }

    ACE_INLINE ::PortableServer::LifespanPolicyValue
    Cached_Policies::lifespan () const
    {
      return this->lifespan_;
    }

    ACE_INLINE ::PortableServer::IdUniquenessPolicyValue
    Cached_Policies::id_uniqueness () const
    {
      return this->id_uniqueness_;
    }

    ACE_INLINE ::PortableServer::IdAssignmentPolicyValue
    Cached_Policies::id_assignment () const
    {
      return this->id_assignment_;
    }

    ACE_INLINE ::PortableServer::ImplicitActivationPolicyValue
    Cached_Policies::implicit_activation () const
    {
      return this->implicit_activation_;
    }

    ACE_INLINE ::PortableServer::ServantRetentionPolicyValue
    Cached_Policies::servant_retention () const
    {
      return this->servant_retention_;
    }

    ACE_INLINE ::PortableServer::RequestProcessingPolicyValue
    Cached_Policies::request_processing () const
    {
      return this->request_processing_;
    }

    ACE_INLINE Cached_Policies::PriorityModel
    Cached_Policies::priority_model () const
    {
      return this->priority_model_;
    }

    ACE_INLINE CORBA::Short
    Cached_Policies::server_priority () const
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
    Cached_Policies::network_priority_model () const
    {
      return this->network_priority_model_;
    }

    ACE_INLINE CORBA::Long
    Cached_Policies::request_diffserv_codepoint () const
    {
      return this->request_diffserv_codepoint_;
    }

    ACE_INLINE CORBA::Long
    Cached_Policies::reply_diffserv_codepoint () const
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
