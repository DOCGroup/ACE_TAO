// $Id: $
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
    id_ (s.id_.get () ? new ::XMLSchema::ID< wchar_t > (*s.id_) : 0),
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
      else id_ = ::std::auto_ptr< ::XMLSchema::ID< wchar_t > > (0);

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

    ::XMLSchema::ID< wchar_t > const& NetQoSRequirementsDef::
    id () const
    {
      return *id_;
    }

    ::XMLSchema::ID< wchar_t >& NetQoSRequirementsDef::
    id ()
    {
      return *id_;
    }

    void NetQoSRequirementsDef::
    id (::XMLSchema::ID< wchar_t > const& e)
    {
      if (id_.get ())
      {
        *id_ = e;
      }

      else
      {
        id_ = ::std::auto_ptr< ::XMLSchema::ID< wchar_t > > (new ::XMLSchema::ID< wchar_t > (e));
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
    add_connectionName (::XMLSchema::string< wchar_t > const& e)
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
    NetQoSRequirementsDef (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"connectionQoS")
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
        ::XSCRT::XML::Attribute< wchar_t > a (p.next_attribute ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (a.name ()));
        if (n == L"id")
        {
          ::XMLSchema::ID< wchar_t > t (a);
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
    NetworkQoS (::XSCRT::XML::Element< wchar_t > const& e)
    : ::XSCRT::Type (e)
    {
      ::std::basic_string< wchar_t > v (e.value ());

      if (v == L"HIGH_PRIORITY") v_ = HIGH_PRIORITY_l;
      else if (v == L"HIGH_RELIABILITY") v_ = HIGH_RELIABILITY_l;
      else if (v == L"VOICE") v_ = VOICE_l;
      else if (v == L"VIDEO") v_ = VIDEO_l;
      else if (v == L"BEST_EFFORT") v_ = BEST_EFFORT_l;
      else 
      {
      }
    }

    NetworkQoS::
    NetworkQoS (::XSCRT::XML::Attribute< wchar_t > const& a)
    : ::XSCRT::Type (a)
    {
      ::std::basic_string< wchar_t > v (a.value ());

      if (v == L"HIGH_PRIORITY") v_ = HIGH_PRIORITY_l;
      else if (v == L"HIGH_RELIABILITY") v_ = HIGH_RELIABILITY_l;
      else if (v == L"VOICE") v_ = VOICE_l;
      else if (v == L"VIDEO") v_ = VIDEO_l;
      else if (v == L"BEST_EFFORT") v_ = BEST_EFFORT_l;
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
    AdmissionPriority (::XSCRT::XML::Element< wchar_t > const& e)
    : ::XSCRT::Type (e)
    {
      ::std::basic_string< wchar_t > v (e.value ());

      if (v == L"NORMAL") v_ = NORMAL_l;
      else if (v == L"HIGH") v_ = HIGH_l;
      else if (v == L"LOW") v_ = LOW_l;
      else 
      {
      }
    }

    AdmissionPriority::
    AdmissionPriority (::XSCRT::XML::Attribute< wchar_t > const& a)
    : ::XSCRT::Type (a)
    {
      ::std::basic_string< wchar_t > v (a.value ());

      if (v == L"NORMAL") v_ = NORMAL_l;
      else if (v == L"HIGH") v_ = HIGH_l;
      else if (v == L"LOW") v_ = LOW_l;
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
    ConnectionQoS (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"connectionName")
        {
          ::XMLSchema::string< wchar_t > t (e);
          add_connectionName (t);
        }

        else if (n == L"fwdBW")
        {
          fwdBW_ = ::std::auto_ptr< ::XMLSchema::unsignedInt > (new ::XMLSchema::unsignedInt (e));
          fwdBW_->container (this);
        }

        else if (n == L"revBW")
        {
          revBW_ = ::std::auto_ptr< ::XMLSchema::unsignedInt > (new ::XMLSchema::unsignedInt (e));
          revBW_->container (this);
        }

        else if (n == L"qos")
        {
          qos_ = ::std::auto_ptr< ::CIAO::Config_Handlers::NetworkQoS > (new ::CIAO::Config_Handlers::NetworkQoS (e));
          qos_->container (this);
        }

        else if (n == L"admissionPriority")
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
      ::XSCRT::XML::Element< wchar_t > e (d->getDocumentElement ());
      if (e.name () == L"NetQoSRequirements")
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

