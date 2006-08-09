#include "NetQoS/NetQoS_Handler_Export.h"

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
    class NetQoS_Handler_Export NetQoSRequirementsDef : public ::XSCRT::Type
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
      ::XMLSchema::ID< ACE_TCHAR > const& id () const;
      ::XMLSchema::ID< ACE_TCHAR >& id ();
      void id (::XMLSchema::ID< ACE_TCHAR > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::ID< ACE_TCHAR > > id_;

      public:
      NetQoSRequirementsDef ();

      NetQoSRequirementsDef (::XSCRT::XML::Element< ACE_TCHAR > const&);
      NetQoSRequirementsDef (NetQoSRequirementsDef const& s);

      NetQoSRequirementsDef&
      operator= (NetQoSRequirementsDef const& s);

      private:
      char regulator__;
    };


    class NetQoS_Handler_Export NetworkQoS : public ::XSCRT::Type
    {
      public:
      NetworkQoS (::XSCRT::XML::Element< ACE_TCHAR > const&);
      NetworkQoS (::XSCRT::XML::Attribute< ACE_TCHAR > const&);

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

      friend bool NetQoS_Handler_Export 
      operator== (NetworkQoS const& a, NetworkQoS const& b);

      friend bool NetQoS_Handler_Export 
      operator!= (NetworkQoS const& a, NetworkQoS const& b);

      private:
      NetworkQoS (Value v);

      Value v_;
    };

    bool NetQoS_Handler_Export operator== (NetworkQoS const &a, NetworkQoS const &b);

    bool NetQoS_Handler_Export operator!= (NetworkQoS const &a, NetworkQoS const &b);


    class NetQoS_Handler_Export AdmissionPriority : public ::XSCRT::Type
    {
      public:
      AdmissionPriority (::XSCRT::XML::Element< ACE_TCHAR > const&);
      AdmissionPriority (::XSCRT::XML::Attribute< ACE_TCHAR > const&);

      static AdmissionPriority const NORMAL;
      static AdmissionPriority const HIGH;
      static AdmissionPriority const LOW;

      enum Value
      {
        NORMAL_l,HIGH_l,LOW_l,
      };


      Value
      integral () const;

      friend bool NetQoS_Handler_Export 
      operator== (AdmissionPriority const& a, AdmissionPriority const& b);

      friend bool NetQoS_Handler_Export 
      operator!= (AdmissionPriority const& a, AdmissionPriority const& b);

      private:
      AdmissionPriority (Value v);

      Value v_;
    };

    bool NetQoS_Handler_Export operator== (AdmissionPriority const &a, AdmissionPriority const &b);

    bool NetQoS_Handler_Export operator!= (AdmissionPriority const &a, AdmissionPriority const &b);


    class NetQoS_Handler_Export ConnectionQoS : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // connectionName
      // 
      public:
      typedef ::std::list< ::XMLSchema::string< ACE_TCHAR > >::iterator connectionName_iterator;
      typedef ::std::list< ::XMLSchema::string< ACE_TCHAR > >::const_iterator connectionName_const_iterator;
      connectionName_iterator begin_connectionName ();
      connectionName_iterator end_connectionName ();
      connectionName_const_iterator begin_connectionName () const;
      connectionName_const_iterator end_connectionName () const;
      void add_connectionName (::XMLSchema::string< ACE_TCHAR > const& );
      size_t count_connectionName (void) const;

      protected:
      ::std::list< ::XMLSchema::string< ACE_TCHAR > > connectionName_;

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

      ConnectionQoS (::XSCRT::XML::Element< ACE_TCHAR > const&);
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
    NetQoS_Handler_Export
    ::CIAO::Config_Handlers::NetQoSRequirementsDef
    NetQoSRequirements (xercesc::DOMDocument const*);
  }
}

#endif // NET_QO_SREQUIREMENTS_HPP
