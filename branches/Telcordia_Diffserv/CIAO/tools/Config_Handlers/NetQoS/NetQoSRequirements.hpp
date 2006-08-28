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
    class Connection;
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


    class NetQoS_Handler_Export Connection : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // connectionName
      // 
      public:
      ::XMLSchema::string< ACE_TCHAR > const& connectionName () const;
      void connectionName (::XMLSchema::string< ACE_TCHAR > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< ACE_TCHAR > > connectionName_;

      // client
      // 
      public:
      ::XMLSchema::string< ACE_TCHAR > const& client () const;
      void client (::XMLSchema::string< ACE_TCHAR > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< ACE_TCHAR > > client_;

      // clientPortName
      // 
      public:
      ::XMLSchema::string< ACE_TCHAR > const& clientPortName () const;
      void clientPortName (::XMLSchema::string< ACE_TCHAR > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< ACE_TCHAR > > clientPortName_;

      // server
      // 
      public:
      ::XMLSchema::string< ACE_TCHAR > const& server () const;
      void server (::XMLSchema::string< ACE_TCHAR > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< ACE_TCHAR > > server_;

      // serverPortName
      // 
      public:
      ::XMLSchema::string< ACE_TCHAR > const& serverPortName () const;
      void serverPortName (::XMLSchema::string< ACE_TCHAR > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< ACE_TCHAR > > serverPortName_;

      public:
      Connection (::XMLSchema::string< ACE_TCHAR > const& connectionName__,
                  ::XMLSchema::string< ACE_TCHAR > const& client__,
                  ::XMLSchema::string< ACE_TCHAR > const& clientPortName__,
                  ::XMLSchema::string< ACE_TCHAR > const& server__,
                  ::XMLSchema::string< ACE_TCHAR > const& serverPortName__);

      Connection (::XSCRT::XML::Element< ACE_TCHAR > const&);
      Connection (Connection const& s);

      Connection&
      operator= (Connection const& s);

      private:
      char regulator__;
    };


    class NetQoS_Handler_Export ConnectionQoS : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // connectionInfo
      // 
      public:
      typedef ::std::list< ::CIAO::Config_Handlers::Connection >::iterator connectionInfo_iterator;
      typedef ::std::list< ::CIAO::Config_Handlers::Connection >::const_iterator connectionInfo_const_iterator;
      connectionInfo_iterator begin_connectionInfo ();
      connectionInfo_iterator end_connectionInfo ();
      connectionInfo_const_iterator begin_connectionInfo () const;
      connectionInfo_const_iterator end_connectionInfo () const;
      void add_connectionInfo (::CIAO::Config_Handlers::Connection const& );
      size_t count_connectionInfo (void) const;

      protected:
      ::std::list< ::CIAO::Config_Handlers::Connection > connectionInfo_;

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

#include "XMLSchema/Traversal.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
      struct NetQoS_Handler_Export NetQoSRequirementsDef : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::NetQoSRequirementsDef >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        connectionQoS (Type&);

        virtual void
        connectionQoS (Type const&);

        virtual void
        connectionQoS_pre (Type&);

        virtual void
        connectionQoS_pre (Type const&);

        virtual void
        connectionQoS_next (Type&);

        virtual void
        connectionQoS_next (Type const&);

        virtual void
        connectionQoS_post (Type&);

        virtual void
        connectionQoS_post (Type const&);

        virtual void
        id (Type&);

        virtual void
        id (Type const&);

        virtual void
        id_none (Type&);

        virtual void
        id_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      typedef
      ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::NetworkQoS >
      NetworkQoS;

      typedef
      ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::AdmissionPriority >
      AdmissionPriority;

      struct NetQoS_Handler_Export Connection : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Connection >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        connectionName (Type&);

        virtual void
        connectionName (Type const&);

        virtual void
        client (Type&);

        virtual void
        client (Type const&);

        virtual void
        clientPortName (Type&);

        virtual void
        clientPortName (Type const&);

        virtual void
        server (Type&);

        virtual void
        server (Type const&);

        virtual void
        serverPortName (Type&);

        virtual void
        serverPortName (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct NetQoS_Handler_Export ConnectionQoS : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ConnectionQoS >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        connectionInfo (Type&);

        virtual void
        connectionInfo (Type const&);

        virtual void
        connectionInfo_pre (Type&);

        virtual void
        connectionInfo_pre (Type const&);

        virtual void
        connectionInfo_next (Type&);

        virtual void
        connectionInfo_next (Type const&);

        virtual void
        connectionInfo_post (Type&);

        virtual void
        connectionInfo_post (Type const&);

        virtual void
        fwdBW (Type&);

        virtual void
        fwdBW (Type const&);

        virtual void
        revBW (Type&);

        virtual void
        revBW (Type const&);

        virtual void
        qos (Type&);

        virtual void
        qos (Type const&);

        virtual void
        admissionPriority (Type&);

        virtual void
        admissionPriority (Type const&);

        virtual void
        admissionPriority_none (Type&);

        virtual void
        admissionPriority_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };
    }
  }
}

#include "XMLSchema/Writer.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      struct NetQoSRequirementsDef : Traversal::NetQoSRequirementsDef, 
      virtual ::XSCRT::Writer< ACE_TCHAR >
      {
        typedef ::CIAO::Config_Handlers::NetQoSRequirementsDef Type;
        NetQoSRequirementsDef (::XSCRT::XML::Element< ACE_TCHAR >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        connectionQoS_pre (Type &o)
        {

          this->connectionQoS_pre (const_cast <Type const &> (o));
        }


        virtual void
        connectionQoS_pre (Type const&);

        virtual void 
        connectionQoS_next (Type &o)
        {

          this->connectionQoS_next (const_cast <Type const &> (o));
        }


        virtual void
        connectionQoS_next (Type const&);

        virtual void 
        connectionQoS_post (Type &o)
        {

          this->connectionQoS_post (const_cast <Type const &> (o));
        }


        virtual void
        connectionQoS_post (Type const&);

        virtual void 
        id (Type &o)
        {

          this->id (const_cast <Type const &> (o));
        }


        virtual void
        id (Type const&);

        protected:
        NetQoSRequirementsDef ();
      };

      struct NetworkQoS : Traversal::NetworkQoS, 
      virtual ::XSCRT::Writer< ACE_TCHAR >
      {
        NetworkQoS (::XSCRT::XML::Element< ACE_TCHAR >&);

        virtual void 
        traverse (Type &o)
        {
          this->traverse (const_cast <Type const &> (o));
        }

        virtual void
        traverse (Type const&);

        protected:
        NetworkQoS ();
      };

      struct AdmissionPriority : Traversal::AdmissionPriority, 
      virtual ::XSCRT::Writer< ACE_TCHAR >
      {
        AdmissionPriority (::XSCRT::XML::Element< ACE_TCHAR >&);

        virtual void 
        traverse (Type &o)
        {
          this->traverse (const_cast <Type const &> (o));
        }

        virtual void
        traverse (Type const&);

        protected:
        AdmissionPriority ();
      };

      struct Connection : Traversal::Connection, 
      virtual ::XSCRT::Writer< ACE_TCHAR >
      {
        typedef ::CIAO::Config_Handlers::Connection Type;
        Connection (::XSCRT::XML::Element< ACE_TCHAR >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        connectionName (Type &o)
        {

          this->connectionName (const_cast <Type const &> (o));
        }


        virtual void
        connectionName (Type const&);

        virtual void 
        client (Type &o)
        {

          this->client (const_cast <Type const &> (o));
        }


        virtual void
        client (Type const&);

        virtual void 
        clientPortName (Type &o)
        {

          this->clientPortName (const_cast <Type const &> (o));
        }


        virtual void
        clientPortName (Type const&);

        virtual void 
        server (Type &o)
        {

          this->server (const_cast <Type const &> (o));
        }


        virtual void
        server (Type const&);

        virtual void 
        serverPortName (Type &o)
        {

          this->serverPortName (const_cast <Type const &> (o));
        }


        virtual void
        serverPortName (Type const&);

        protected:
        Connection ();
      };

      struct ConnectionQoS : Traversal::ConnectionQoS, 
      virtual ::XSCRT::Writer< ACE_TCHAR >
      {
        typedef ::CIAO::Config_Handlers::ConnectionQoS Type;
        ConnectionQoS (::XSCRT::XML::Element< ACE_TCHAR >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        connectionInfo_pre (Type &o)
        {

          this->connectionInfo_pre (const_cast <Type const &> (o));
        }


        virtual void
        connectionInfo_pre (Type const&);

        virtual void 
        connectionInfo_next (Type &o)
        {

          this->connectionInfo_next (const_cast <Type const &> (o));
        }


        virtual void
        connectionInfo_next (Type const&);

        virtual void 
        connectionInfo_post (Type &o)
        {

          this->connectionInfo_post (const_cast <Type const &> (o));
        }


        virtual void
        connectionInfo_post (Type const&);

        virtual void 
        fwdBW (Type &o)
        {

          this->fwdBW (const_cast <Type const &> (o));
        }


        virtual void
        fwdBW (Type const&);

        virtual void 
        revBW (Type &o)
        {

          this->revBW (const_cast <Type const &> (o));
        }


        virtual void
        revBW (Type const&);

        virtual void 
        qos (Type &o)
        {

          this->qos (const_cast <Type const &> (o));
        }


        virtual void
        qos (Type const&);

        virtual void 
        admissionPriority (Type &o)
        {

          this->admissionPriority (const_cast <Type const &> (o));
        }


        virtual void
        admissionPriority (Type const&);

        protected:
        ConnectionQoS ();
      };
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    NetQoS_Handler_Export
    void
    NetQoSRequirements (::CIAO::Config_Handlers::NetQoSRequirementsDef const&, xercesc::DOMDocument*);
  }
}

#endif // NET_QO_SREQUIREMENTS_HPP
