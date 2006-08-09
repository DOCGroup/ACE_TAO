#include "NetQoSRequirements.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // NetQoSRequirementsDef
    // 

    NetQoSRequirementsDef::
    NetQoSRequirementsDef ()
    : 
    ::XSCRT::Type (), 
    regulator__ ()
    {
    }

    NetQoSRequirementsDef::
    NetQoSRequirementsDef (::CIAO::Config_Handlers::NetQoSRequirementsDef const& s)
    :
    ::XSCRT::Type (),
    id_ (s.id_.get () ? new ::XMLSchema::ID< ACE_TCHAR > (*s.id_) : 0),
    regulator__ ()
    {
      {
        for (connectionQoS_const_iterator i (s.connectionQoS_.begin ());i != s.connectionQoS_.end ();++i) add_connectionQoS (*i);
      }

      if (id_.get ()) id_->container (this);
    }

    ::CIAO::Config_Handlers::NetQoSRequirementsDef& NetQoSRequirementsDef::
    operator= (::CIAO::Config_Handlers::NetQoSRequirementsDef const& s)
    {
      connectionQoS_.clear ();
      {
        for (connectionQoS_const_iterator i (s.connectionQoS_.begin ());i != s.connectionQoS_.end ();++i) add_connectionQoS (*i);
      }

      if (s.id_.get ()) id (*(s.id_));
      else id_ = ::std::auto_ptr< ::XMLSchema::ID< ACE_TCHAR > > (0);

      return *this;
    }


    // NetQoSRequirementsDef
    // 
    NetQoSRequirementsDef::connectionQoS_iterator NetQoSRequirementsDef::
    begin_connectionQoS ()
    {
      return connectionQoS_.begin ();
    }

    NetQoSRequirementsDef::connectionQoS_iterator NetQoSRequirementsDef::
    end_connectionQoS ()
    {
      return connectionQoS_.end ();
    }

    NetQoSRequirementsDef::connectionQoS_const_iterator NetQoSRequirementsDef::
    begin_connectionQoS () const
    {
      return connectionQoS_.begin ();
    }

    NetQoSRequirementsDef::connectionQoS_const_iterator NetQoSRequirementsDef::
    end_connectionQoS () const
    {
      return connectionQoS_.end ();
    }

    void NetQoSRequirementsDef::
    add_connectionQoS (::CIAO::Config_Handlers::ConnectionQoS const& e)
    {
      connectionQoS_.push_back (e);
    }

    size_t NetQoSRequirementsDef::
    count_connectionQoS(void) const
    {
      return connectionQoS_.size ();
    }

    // NetQoSRequirementsDef
    // 
    bool NetQoSRequirementsDef::
    id_p () const
    {
      return id_.get () != 0;
    }

    ::XMLSchema::ID< ACE_TCHAR > const& NetQoSRequirementsDef::
    id () const
    {
      return *id_;
    }

    ::XMLSchema::ID< ACE_TCHAR >& NetQoSRequirementsDef::
    id ()
    {
      return *id_;
    }

    void NetQoSRequirementsDef::
    id (::XMLSchema::ID< ACE_TCHAR > const& e)
    {
      if (id_.get ())
      {
        *id_ = e;
      }

      else
      {
        id_ = ::std::auto_ptr< ::XMLSchema::ID< ACE_TCHAR > > (new ::XMLSchema::ID< ACE_TCHAR > (e));
        id_->container (this);
      }
    }


    // NetworkQoS
    // 

    NetworkQoS::Value NetworkQoS::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::CIAO::Config_Handlers::NetworkQoS const& a, ::CIAO::Config_Handlers::NetworkQoS const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::CIAO::Config_Handlers::NetworkQoS const& a, ::CIAO::Config_Handlers::NetworkQoS const& b)
    {
      return a.v_ != b.v_;
    }

    NetworkQoS::
    NetworkQoS (NetworkQoS::Value v)
    : v_ (v)
    {
    }

    // AdmissionPriority
    // 

    AdmissionPriority::Value AdmissionPriority::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::CIAO::Config_Handlers::AdmissionPriority const& a, ::CIAO::Config_Handlers::AdmissionPriority const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::CIAO::Config_Handlers::AdmissionPriority const& a, ::CIAO::Config_Handlers::AdmissionPriority const& b)
    {
      return a.v_ != b.v_;
    }

    AdmissionPriority::
    AdmissionPriority (AdmissionPriority::Value v)
    : v_ (v)
    {
    }

    // ConnectionQoS
    // 

    ConnectionQoS::
    ConnectionQoS (::XMLSchema::unsignedInt const& fwdBW__,
                   ::XMLSchema::unsignedInt const& revBW__,
                   ::CIAO::Config_Handlers::NetworkQoS const& qos__)
    : 
    ::XSCRT::Type (), 
    fwdBW_ (new ::XMLSchema::unsignedInt (fwdBW__)),
    revBW_ (new ::XMLSchema::unsignedInt (revBW__)),
    qos_ (new ::CIAO::Config_Handlers::NetworkQoS (qos__)),
    regulator__ ()
    {
      fwdBW_->container (this);
      revBW_->container (this);
      qos_->container (this);
    }

    ConnectionQoS::
    ConnectionQoS (::CIAO::Config_Handlers::ConnectionQoS const& s)
    :
    ::XSCRT::Type (),
    fwdBW_ (new ::XMLSchema::unsignedInt (*s.fwdBW_)),
    revBW_ (new ::XMLSchema::unsignedInt (*s.revBW_)),
    qos_ (new ::CIAO::Config_Handlers::NetworkQoS (*s.qos_)),
    admissionPriority_ (s.admissionPriority_.get () ? new ::CIAO::Config_Handlers::AdmissionPriority (*s.admissionPriority_) : 0),
    regulator__ ()
    {
      {
        for (connectionName_const_iterator i (s.connectionName_.begin ());i != s.connectionName_.end ();++i) add_connectionName (*i);
      }

      fwdBW_->container (this);
      revBW_->container (this);
      qos_->container (this);
      if (admissionPriority_.get ()) admissionPriority_->container (this);
    }

    ::CIAO::Config_Handlers::ConnectionQoS& ConnectionQoS::
    operator= (::CIAO::Config_Handlers::ConnectionQoS const& s)
    {
      connectionName_.clear ();
      {
        for (connectionName_const_iterator i (s.connectionName_.begin ());i != s.connectionName_.end ();++i) add_connectionName (*i);
      }

      fwdBW (s.fwdBW ());

      revBW (s.revBW ());

      qos (s.qos ());

      if (s.admissionPriority_.get ()) admissionPriority (*(s.admissionPriority_));
      else admissionPriority_ = ::std::auto_ptr< ::CIAO::Config_Handlers::AdmissionPriority > (0);

      return *this;
    }


    // ConnectionQoS
    // 
    ConnectionQoS::connectionName_iterator ConnectionQoS::
    begin_connectionName ()
    {
      return connectionName_.begin ();
    }

    ConnectionQoS::connectionName_iterator ConnectionQoS::
    end_connectionName ()
    {
      return connectionName_.end ();
    }

    ConnectionQoS::connectionName_const_iterator ConnectionQoS::
    begin_connectionName () const
    {
      return connectionName_.begin ();
    }

    ConnectionQoS::connectionName_const_iterator ConnectionQoS::
    end_connectionName () const
    {
      return connectionName_.end ();
    }

    void ConnectionQoS::
    add_connectionName (::XMLSchema::string< ACE_TCHAR > const& e)
    {
      connectionName_.push_back (e);
    }

    size_t ConnectionQoS::
    count_connectionName(void) const
    {
      return connectionName_.size ();
    }

    // ConnectionQoS
    // 
    ::XMLSchema::unsignedInt const& ConnectionQoS::
    fwdBW () const
    {
      return *fwdBW_;
    }

    void ConnectionQoS::
    fwdBW (::XMLSchema::unsignedInt const& e)
    {
      *fwdBW_ = e;
    }

    // ConnectionQoS
    // 
    ::XMLSchema::unsignedInt const& ConnectionQoS::
    revBW () const
    {
      return *revBW_;
    }

    void ConnectionQoS::
    revBW (::XMLSchema::unsignedInt const& e)
    {
      *revBW_ = e;
    }

    // ConnectionQoS
    // 
    ::CIAO::Config_Handlers::NetworkQoS const& ConnectionQoS::
    qos () const
    {
      return *qos_;
    }

    void ConnectionQoS::
    qos (::CIAO::Config_Handlers::NetworkQoS const& e)
    {
      *qos_ = e;
    }

    // ConnectionQoS
    // 
    bool ConnectionQoS::
    admissionPriority_p () const
    {
      return admissionPriority_.get () != 0;
    }

    ::CIAO::Config_Handlers::AdmissionPriority const& ConnectionQoS::
    admissionPriority () const
    {
      return *admissionPriority_;
    }

    void ConnectionQoS::
    admissionPriority (::CIAO::Config_Handlers::AdmissionPriority const& e)
    {
      if (admissionPriority_.get ())
      {
        *admissionPriority_ = e;
      }

      else
      {
        admissionPriority_ = ::std::auto_ptr< ::CIAO::Config_Handlers::AdmissionPriority > (new ::CIAO::Config_Handlers::AdmissionPriority (e));
        admissionPriority_->container (this);
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    // NetQoSRequirementsDef
    //

    NetQoSRequirementsDef::
    NetQoSRequirementsDef (::XSCRT::XML::Element< ACE_TCHAR > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< ACE_TCHAR > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< ACE_TCHAR > e (p.next_element ());
        ::std::basic_string< ACE_TCHAR > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "connectionQoS")
        {
          ::CIAO::Config_Handlers::ConnectionQoS t (e);
          add_connectionQoS (t);
        }

        else 
        {
        }
      }

      while (p.more_attributes ())
      {
        ::XSCRT::XML::Attribute< ACE_TCHAR > a (p.next_attribute ());
        ::std::basic_string< ACE_TCHAR > n (::XSCRT::XML::uq_name (a.name ()));
        if (n == "id")
        {
          ::XMLSchema::ID< ACE_TCHAR > t (a);
          id (t);
        }

        else 
        {
        }
      }
    }

    // NetworkQoS
    //

    NetworkQoS::
    NetworkQoS (::XSCRT::XML::Element< ACE_TCHAR > const& e)
    : ::XSCRT::Type (e)
    {
      ::std::basic_string< ACE_TCHAR > v (e.value ());

      if (v == "HIGH_PRIORITY") v_ = HIGH_PRIORITY_l;
      else if (v == "HIGH_RELIABILITY") v_ = HIGH_RELIABILITY_l;
      else if (v == "VOICE") v_ = VOICE_l;
      else if (v == "VIDEO") v_ = VIDEO_l;
      else if (v == "BEST_EFFORT") v_ = BEST_EFFORT_l;
      else 
      {
      }
    }

    NetworkQoS::
    NetworkQoS (::XSCRT::XML::Attribute< ACE_TCHAR > const& a)
    : ::XSCRT::Type (a)
    {
      ::std::basic_string< ACE_TCHAR > v (a.value ());

      if (v == "HIGH_PRIORITY") v_ = HIGH_PRIORITY_l;
      else if (v == "HIGH_RELIABILITY") v_ = HIGH_RELIABILITY_l;
      else if (v == "VOICE") v_ = VOICE_l;
      else if (v == "VIDEO") v_ = VIDEO_l;
      else if (v == "BEST_EFFORT") v_ = BEST_EFFORT_l;
      else 
      {
      }
    }

    NetworkQoS const NetworkQoS::HIGH_PRIORITY (NetworkQoS::HIGH_PRIORITY_l);
    NetworkQoS const NetworkQoS::HIGH_RELIABILITY (NetworkQoS::HIGH_RELIABILITY_l);
    NetworkQoS const NetworkQoS::VOICE (NetworkQoS::VOICE_l);
    NetworkQoS const NetworkQoS::VIDEO (NetworkQoS::VIDEO_l);
    NetworkQoS const NetworkQoS::BEST_EFFORT (NetworkQoS::BEST_EFFORT_l);

    // AdmissionPriority
    //

    AdmissionPriority::
    AdmissionPriority (::XSCRT::XML::Element< ACE_TCHAR > const& e)
    : ::XSCRT::Type (e)
    {
      ::std::basic_string< ACE_TCHAR > v (e.value ());

      if (v == "NORMAL") v_ = NORMAL_l;
      else if (v == "HIGH") v_ = HIGH_l;
      else if (v == "LOW") v_ = LOW_l;
      else 
      {
      }
    }

    AdmissionPriority::
    AdmissionPriority (::XSCRT::XML::Attribute< ACE_TCHAR > const& a)
    : ::XSCRT::Type (a)
    {
      ::std::basic_string< ACE_TCHAR > v (a.value ());

      if (v == "NORMAL") v_ = NORMAL_l;
      else if (v == "HIGH") v_ = HIGH_l;
      else if (v == "LOW") v_ = LOW_l;
      else 
      {
      }
    }

    AdmissionPriority const AdmissionPriority::NORMAL (AdmissionPriority::NORMAL_l);
    AdmissionPriority const AdmissionPriority::HIGH (AdmissionPriority::HIGH_l);
    AdmissionPriority const AdmissionPriority::LOW (AdmissionPriority::LOW_l);

    // ConnectionQoS
    //

    ConnectionQoS::
    ConnectionQoS (::XSCRT::XML::Element< ACE_TCHAR > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< ACE_TCHAR > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< ACE_TCHAR > e (p.next_element ());
        ::std::basic_string< ACE_TCHAR > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "connectionName")
        {
          ::XMLSchema::string< ACE_TCHAR > t (e);
          add_connectionName (t);
        }

        else if (n == "fwdBW")
        {
          fwdBW_ = ::std::auto_ptr< ::XMLSchema::unsignedInt > (new ::XMLSchema::unsignedInt (e));
          fwdBW_->container (this);
        }

        else if (n == "revBW")
        {
          revBW_ = ::std::auto_ptr< ::XMLSchema::unsignedInt > (new ::XMLSchema::unsignedInt (e));
          revBW_->container (this);
        }

        else if (n == "qos")
        {
          qos_ = ::std::auto_ptr< ::CIAO::Config_Handlers::NetworkQoS > (new ::CIAO::Config_Handlers::NetworkQoS (e));
          qos_->container (this);
        }

        else if (n == "admissionPriority")
        {
          ::CIAO::Config_Handlers::AdmissionPriority t (e);
          admissionPriority (t);
        }

        else 
        {
        }
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    ::CIAO::Config_Handlers::NetQoSRequirementsDef
    NetQoSRequirements (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< ACE_TCHAR > e (d->getDocumentElement ());
      if (e.name () == "NetQoSRequirements")
      {
        ::CIAO::Config_Handlers::NetQoSRequirementsDef r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }
  }
}

