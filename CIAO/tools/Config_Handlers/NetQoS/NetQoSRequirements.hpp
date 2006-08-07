// $Id: $
#ifndef NET_QO_SREQUIREMENTS_HPP
#define NET_QO_SREQUIREMENTS_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class NetQoSRequirementsDef;
    class NetworkQoS;
    class AdmissionPriority;
    class ConnectionQoS;
  }
}

#include <memory>
#include <list>
#include "XMLSchema/Types.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class NetQoSRequirementsDef : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // connectionQoS
      // 
      public:
      typedef ::std::list< ::CIAO::Config_Handlers::ConnectionQoS >::iterator connectionQoS_iterator;
      typedef ::std::list< ::CIAO::Config_Handlers::ConnectionQoS >::const_iterator connectionQoS_const_iterator;
      connectionQoS_iterator begin_connectionQoS ();
      connectionQoS_iterator end_connectionQoS ();
      connectionQoS_const_iterator begin_connectionQoS () const;
      connectionQoS_const_iterator end_connectionQoS () const;
      void add_connectionQoS (::CIAO::Config_Handlers::ConnectionQoS const& );
      size_t count_connectionQoS (void) const;

      protected:
      ::std::list< ::CIAO::Config_Handlers::ConnectionQoS > connectionQoS_;

      // id
      // 
      public:
      bool id_p () const;
      ::XMLSchema::ID< wchar_t > const& id () const;
      ::XMLSchema::ID< wchar_t >& id ();
      void id (::XMLSchema::ID< wchar_t > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::ID< wchar_t > > id_;

      public:
      NetQoSRequirementsDef ();

      NetQoSRequirementsDef (::XSCRT::XML::Element< wchar_t > const&);
      NetQoSRequirementsDef (NetQoSRequirementsDef const& s);

      NetQoSRequirementsDef&
      operator= (NetQoSRequirementsDef const& s);

      private:
      char regulator__;
    };


    class NetworkQoS : public ::XSCRT::Type
    {
      public:
      NetworkQoS (::XSCRT::XML::Element< wchar_t > const&);
      NetworkQoS (::XSCRT::XML::Attribute< wchar_t > const&);

      static NetworkQoS const HIGH_PRIORITY;
      static NetworkQoS const HIGH_RELIABILITY;
      static NetworkQoS const VOICE;
      static NetworkQoS const VIDEO;
      static NetworkQoS const BEST_EFFORT;

      enum Value
      {
        HIGH_PRIORITY_l,HIGH_RELIABILITY_l,VOICE_l,VIDEO_l,BEST_EFFORT_l,
      };


      Value
      integral () const;

      friend bool 
      operator== (NetworkQoS const& a, NetworkQoS const& b);

      friend bool 
      operator!= (NetworkQoS const& a, NetworkQoS const& b);

      private:
      NetworkQoS (Value v);

      Value v_;
    };

    bool operator== (NetworkQoS const &a, NetworkQoS const &b);

    bool operator!= (NetworkQoS const &a, NetworkQoS const &b);


    class AdmissionPriority : public ::XSCRT::Type
    {
      public:
      AdmissionPriority (::XSCRT::XML::Element< wchar_t > const&);
      AdmissionPriority (::XSCRT::XML::Attribute< wchar_t > const&);

      static AdmissionPriority const NORMAL;
      static AdmissionPriority const HIGH;
      static AdmissionPriority const LOW;

      enum Value
      {
        NORMAL_l,HIGH_l,LOW_l,
      };


      Value
      integral () const;

      friend bool 
      operator== (AdmissionPriority const& a, AdmissionPriority const& b);

      friend bool 
      operator!= (AdmissionPriority const& a, AdmissionPriority const& b);

      private:
      AdmissionPriority (Value v);

      Value v_;
    };

    bool operator== (AdmissionPriority const &a, AdmissionPriority const &b);

    bool operator!= (AdmissionPriority const &a, AdmissionPriority const &b);


    class ConnectionQoS : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // connectionName
      // 
      public:
      typedef ::std::list< ::XMLSchema::string< wchar_t > >::iterator connectionName_iterator;
      typedef ::std::list< ::XMLSchema::string< wchar_t > >::const_iterator connectionName_const_iterator;
      connectionName_iterator begin_connectionName ();
      connectionName_iterator end_connectionName ();
      connectionName_const_iterator begin_connectionName () const;
      connectionName_const_iterator end_connectionName () const;
      void add_connectionName (::XMLSchema::string< wchar_t > const& );
      size_t count_connectionName (void) const;

      protected:
      ::std::list< ::XMLSchema::string< wchar_t > > connectionName_;

      // fwdBW
      // 
      public:
      ::XMLSchema::unsignedInt const& fwdBW () const;
      void fwdBW (::XMLSchema::unsignedInt const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::unsignedInt > fwdBW_;

      // revBW
      // 
      public:
      ::XMLSchema::unsignedInt const& revBW () const;
      void revBW (::XMLSchema::unsignedInt const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::unsignedInt > revBW_;

      // qos
      // 
      public:
      ::CIAO::Config_Handlers::NetworkQoS const& qos () const;
      void qos (::CIAO::Config_Handlers::NetworkQoS const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::NetworkQoS > qos_;

      // admissionPriority
      // 
      public:
      bool admissionPriority_p () const;
      ::CIAO::Config_Handlers::AdmissionPriority const& admissionPriority () const;
      void admissionPriority (::CIAO::Config_Handlers::AdmissionPriority const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::AdmissionPriority > admissionPriority_;

      public:
      ConnectionQoS (::XMLSchema::unsignedInt const& fwdBW__,
                     ::XMLSchema::unsignedInt const& revBW__,
                     ::CIAO::Config_Handlers::NetworkQoS const& qos__);

      ConnectionQoS (::XSCRT::XML::Element< wchar_t > const&);
      ConnectionQoS (ConnectionQoS const& s);

      ConnectionQoS&
      operator= (ConnectionQoS const& s);

      private:
      char regulator__;
    };
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    ::CIAO::Config_Handlers::NetQoSRequirementsDef
    NetQoSRequirements (xercesc::DOMDocument const*);
  }
}

#endif // NET_QO_SREQUIREMENTS_HPP
