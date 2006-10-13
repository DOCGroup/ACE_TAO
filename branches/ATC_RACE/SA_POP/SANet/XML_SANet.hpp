#ifndef XML_SANET_HPP
#define XML_SANET_HPP

// Forward declarations.
//
namespace SANet
{
  namespace XML
  {
    class NodeID;
    class PortID;
    class TaskCost;
    class CondUtil;
    class CondKind;
    class Probability;
    class LinkWeight;
    class MultFactor;
    class TaskNode;
    class CondNode;
    class PrecondLink;
    class EffectLink;
    class Network;
  }
}

#include <memory>
#include <list>
#include "XMLSchema/Types.hpp"

namespace SANet
{
  namespace XML
  {
    class NodeID : public ::XMLSchema::int_
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::int_ Base__;

      public:
      NodeID (::XMLSchema::int_ const& b__);

      NodeID (::XSCRT::XML::Element< wchar_t > const&);
      NodeID (::XSCRT::XML::Attribute< wchar_t > const&);
      NodeID (NodeID const& s);

      NodeID&
      operator= (NodeID const& s);

      private:
      char regulator__;
    };

    class PortID : public ::XMLSchema::string< wchar_t >
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::string< wchar_t > Base__;

      public:
      PortID (::XMLSchema::string< wchar_t > const& b__);

      PortID (::XSCRT::XML::Element< wchar_t > const&);
      PortID (::XSCRT::XML::Attribute< wchar_t > const&);
      PortID (PortID const& s);

      PortID&
      operator= (PortID const& s);

      private:
      char regulator__;
    };

    class TaskCost : public ::XMLSchema::double_
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::double_ Base__;

      public:
      TaskCost (::XMLSchema::double_ const& b__);

      TaskCost (::XSCRT::XML::Element< wchar_t > const&);
      TaskCost (::XSCRT::XML::Attribute< wchar_t > const&);
      TaskCost (TaskCost const& s);

      TaskCost&
      operator= (TaskCost const& s);

      private:
      char regulator__;
    };

    class CondUtil : public ::XMLSchema::double_
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::double_ Base__;

      public:
      CondUtil (::XMLSchema::double_ const& b__);

      CondUtil (::XSCRT::XML::Element< wchar_t > const&);
      CondUtil (::XSCRT::XML::Attribute< wchar_t > const&);
      CondUtil (CondUtil const& s);

      CondUtil&
      operator= (CondUtil const& s);

      private:
      char regulator__;
    };

    class CondKind : public ::XSCRT::Type
    {
      public:
      CondKind (::XSCRT::XML::Element< wchar_t > const&);
      CondKind (::XSCRT::XML::Attribute< wchar_t > const&);

      static CondKind const ENVIRON;
      static CondKind const SYSTEM;
      static CondKind const DATA;

      enum Value
      {
        ENVIRON_l,SYSTEM_l,DATA_l,
      };


      Value
      integral () const;

      friend bool 
      operator== (CondKind const& a, CondKind const& b);

      friend bool 
      operator!= (CondKind const& a, CondKind const& b);

      private:
      CondKind (Value v);

      Value v_;
    };

    bool operator== (CondKind const &a, CondKind const &b);

    bool operator!= (CondKind const &a, CondKind const &b);


    class Probability : public ::XMLSchema::double_
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::double_ Base__;

      public:
      Probability (::XMLSchema::double_ const& b__);

      Probability (::XSCRT::XML::Element< wchar_t > const&);
      Probability (::XSCRT::XML::Attribute< wchar_t > const&);
      Probability (Probability const& s);

      Probability&
      operator= (Probability const& s);

      private:
      char regulator__;
    };

    class LinkWeight : public ::XMLSchema::double_
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::double_ Base__;

      public:
      LinkWeight (::XMLSchema::double_ const& b__);

      LinkWeight (::XSCRT::XML::Element< wchar_t > const&);
      LinkWeight (::XSCRT::XML::Attribute< wchar_t > const&);
      LinkWeight (LinkWeight const& s);

      LinkWeight&
      operator= (LinkWeight const& s);

      private:
      char regulator__;
    };

    class MultFactor : public ::XMLSchema::double_
    {
      //@@ VC6 anathema
      typedef ::XMLSchema::double_ Base__;

      public:
      MultFactor (::XMLSchema::double_ const& b__);

      MultFactor (::XSCRT::XML::Element< wchar_t > const&);
      MultFactor (::XSCRT::XML::Attribute< wchar_t > const&);
      MultFactor (MultFactor const& s);

      MultFactor&
      operator= (MultFactor const& s);

      private:
      char regulator__;
    };

    class TaskNode : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // nodeID
      // 
      public:
      ::SANet::XML::NodeID const& nodeID () const;
      void nodeID (::SANet::XML::NodeID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::NodeID > nodeID_;

      // name
      // 
      public:
      ::XMLSchema::string< wchar_t > const& name () const;
      void name (::XMLSchema::string< wchar_t > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< wchar_t > > name_;

      // priorProb
      // 
      public:
      ::SANet::XML::Probability const& priorProb () const;
      void priorProb (::SANet::XML::Probability const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::Probability > priorProb_;

      // attenFactor
      // 
      public:
      ::SANet::XML::MultFactor const& attenFactor () const;
      void attenFactor (::SANet::XML::MultFactor const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::MultFactor > attenFactor_;

      // cost
      // 
      public:
      ::SANet::XML::TaskCost const& cost () const;
      void cost (::SANet::XML::TaskCost const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::TaskCost > cost_;

      public:
      TaskNode (::SANet::XML::NodeID const& nodeID__,
                ::XMLSchema::string< wchar_t > const& name__,
                ::SANet::XML::Probability const& priorProb__,
                ::SANet::XML::MultFactor const& attenFactor__,
                ::SANet::XML::TaskCost const& cost__);

      TaskNode (::XSCRT::XML::Element< wchar_t > const&);
      TaskNode (TaskNode const& s);

      TaskNode&
      operator= (TaskNode const& s);

      private:
      char regulator__;
    };


    class CondNode : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // nodeID
      // 
      public:
      ::SANet::XML::NodeID const& nodeID () const;
      void nodeID (::SANet::XML::NodeID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::NodeID > nodeID_;

      // name
      // 
      public:
      ::XMLSchema::string< wchar_t > const& name () const;
      void name (::XMLSchema::string< wchar_t > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< wchar_t > > name_;

      // probTrue
      // 
      public:
      ::SANet::XML::Probability const& probTrue () const;
      void probTrue (::SANet::XML::Probability const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::Probability > probTrue_;

      // utility
      // 
      public:
      ::SANet::XML::CondUtil const& utility () const;
      void utility (::SANet::XML::CondUtil const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::CondUtil > utility_;

      // kind
      // 
      public:
      ::SANet::XML::CondKind const& kind () const;
      void kind (::SANet::XML::CondKind const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::CondKind > kind_;

      // attenFactor
      // 
      public:
      ::SANet::XML::MultFactor const& attenFactor () const;
      void attenFactor (::SANet::XML::MultFactor const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::MultFactor > attenFactor_;

      public:
      CondNode (::SANet::XML::NodeID const& nodeID__,
                ::XMLSchema::string< wchar_t > const& name__,
                ::SANet::XML::Probability const& probTrue__,
                ::SANet::XML::CondUtil const& utility__,
                ::SANet::XML::CondKind const& kind__,
                ::SANet::XML::MultFactor const& attenFactor__);

      CondNode (::XSCRT::XML::Element< wchar_t > const&);
      CondNode (CondNode const& s);

      CondNode&
      operator= (CondNode const& s);

      private:
      char regulator__;
    };


    class PrecondLink : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // condID
      // 
      public:
      ::SANet::XML::NodeID const& condID () const;
      void condID (::SANet::XML::NodeID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::NodeID > condID_;

      // taskID
      // 
      public:
      ::SANet::XML::NodeID const& taskID () const;
      void taskID (::SANet::XML::NodeID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::NodeID > taskID_;

      // portID
      // 
      public:
      ::SANet::XML::PortID const& portID () const;
      void portID (::SANet::XML::PortID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::PortID > portID_;

      // trueProb
      // 
      public:
      ::SANet::XML::Probability const& trueProb () const;
      void trueProb (::SANet::XML::Probability const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::Probability > trueProb_;

      // falseProb
      // 
      public:
      ::SANet::XML::Probability const& falseProb () const;
      void falseProb (::SANet::XML::Probability const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::Probability > falseProb_;

      public:
      PrecondLink (::SANet::XML::NodeID const& condID__,
                   ::SANet::XML::NodeID const& taskID__,
                   ::SANet::XML::PortID const& portID__,
                   ::SANet::XML::Probability const& trueProb__,
                   ::SANet::XML::Probability const& falseProb__);

      PrecondLink (::XSCRT::XML::Element< wchar_t > const&);
      PrecondLink (PrecondLink const& s);

      PrecondLink&
      operator= (PrecondLink const& s);

      private:
      char regulator__;
    };


    class EffectLink : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // taskID
      // 
      public:
      ::SANet::XML::NodeID const& taskID () const;
      void taskID (::SANet::XML::NodeID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::NodeID > taskID_;

      // condID
      // 
      public:
      ::SANet::XML::NodeID const& condID () const;
      void condID (::SANet::XML::NodeID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::NodeID > condID_;

      // portID
      // 
      public:
      ::SANet::XML::PortID const& portID () const;
      void portID (::SANet::XML::PortID const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::PortID > portID_;

      // weight
      // 
      public:
      ::SANet::XML::LinkWeight const& weight () const;
      void weight (::SANet::XML::LinkWeight const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::LinkWeight > weight_;

      public:
      EffectLink (::SANet::XML::NodeID const& taskID__,
                  ::SANet::XML::NodeID const& condID__,
                  ::SANet::XML::PortID const& portID__,
                  ::SANet::XML::LinkWeight const& weight__);

      EffectLink (::XSCRT::XML::Element< wchar_t > const&);
      EffectLink (EffectLink const& s);

      EffectLink&
      operator= (EffectLink const& s);

      private:
      char regulator__;
    };


    class Network : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // defaultAttenFactor
      // 
      public:
      ::SANet::XML::MultFactor const& defaultAttenFactor () const;
      void defaultAttenFactor (::SANet::XML::MultFactor const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::MultFactor > defaultAttenFactor_;

      // defaultTaskCost
      // 
      public:
      ::SANet::XML::TaskCost const& defaultTaskCost () const;
      void defaultTaskCost (::SANet::XML::TaskCost const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::TaskCost > defaultTaskCost_;

      // defaultCondUtil
      // 
      public:
      ::SANet::XML::CondUtil const& defaultCondUtil () const;
      void defaultCondUtil (::SANet::XML::CondUtil const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::CondUtil > defaultCondUtil_;

      // defaultCondProbTrue
      // 
      public:
      ::SANet::XML::Probability const& defaultCondProbTrue () const;
      void defaultCondProbTrue (::SANet::XML::Probability const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::Probability > defaultCondProbTrue_;

      // linkThresh
      // 
      public:
      ::SANet::XML::LinkWeight const& linkThresh () const;
      void linkThresh (::SANet::XML::LinkWeight const& );

      protected:
      ::std::auto_ptr< ::SANet::XML::LinkWeight > linkThresh_;

      // taskNode
      // 
      public:
      typedef ::std::list< ::SANet::XML::TaskNode >::iterator taskNode_iterator;
      typedef ::std::list< ::SANet::XML::TaskNode >::const_iterator taskNode_const_iterator;
      taskNode_iterator begin_taskNode ();
      taskNode_iterator end_taskNode ();
      taskNode_const_iterator begin_taskNode () const;
      taskNode_const_iterator end_taskNode () const;
      void add_taskNode (::SANet::XML::TaskNode const& );
      size_t count_taskNode (void) const;

      protected:
      ::std::list< ::SANet::XML::TaskNode > taskNode_;

      // condNode
      // 
      public:
      typedef ::std::list< ::SANet::XML::CondNode >::iterator condNode_iterator;
      typedef ::std::list< ::SANet::XML::CondNode >::const_iterator condNode_const_iterator;
      condNode_iterator begin_condNode ();
      condNode_iterator end_condNode ();
      condNode_const_iterator begin_condNode () const;
      condNode_const_iterator end_condNode () const;
      void add_condNode (::SANet::XML::CondNode const& );
      size_t count_condNode (void) const;

      protected:
      ::std::list< ::SANet::XML::CondNode > condNode_;

      // precondLink
      // 
      public:
      typedef ::std::list< ::SANet::XML::PrecondLink >::iterator precondLink_iterator;
      typedef ::std::list< ::SANet::XML::PrecondLink >::const_iterator precondLink_const_iterator;
      precondLink_iterator begin_precondLink ();
      precondLink_iterator end_precondLink ();
      precondLink_const_iterator begin_precondLink () const;
      precondLink_const_iterator end_precondLink () const;
      void add_precondLink (::SANet::XML::PrecondLink const& );
      size_t count_precondLink (void) const;

      protected:
      ::std::list< ::SANet::XML::PrecondLink > precondLink_;

      // effectLink
      // 
      public:
      typedef ::std::list< ::SANet::XML::EffectLink >::iterator effectLink_iterator;
      typedef ::std::list< ::SANet::XML::EffectLink >::const_iterator effectLink_const_iterator;
      effectLink_iterator begin_effectLink ();
      effectLink_iterator end_effectLink ();
      effectLink_const_iterator begin_effectLink () const;
      effectLink_const_iterator end_effectLink () const;
      void add_effectLink (::SANet::XML::EffectLink const& );
      size_t count_effectLink (void) const;

      protected:
      ::std::list< ::SANet::XML::EffectLink > effectLink_;

      public:
      Network (::SANet::XML::MultFactor const& defaultAttenFactor__,
               ::SANet::XML::TaskCost const& defaultTaskCost__,
               ::SANet::XML::CondUtil const& defaultCondUtil__,
               ::SANet::XML::Probability const& defaultCondProbTrue__,
               ::SANet::XML::LinkWeight const& linkThresh__);

      Network (::XSCRT::XML::Element< wchar_t > const&);
      Network (Network const& s);

      Network&
      operator= (Network const& s);

      private:
      char regulator__;
    };
  }
}

namespace SANet
{
  namespace XML
  {
    ::SANet::XML::Network
    network (xercesc::DOMDocument const*);
  }
}

#include "XMLSchema/Traversal.hpp"

namespace SANet
{
  namespace XML
  {
    namespace Traversal
    {
      struct NodeID : ::XMLSchema::Traversal::Traverser< ::SANet::XML::NodeID >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct PortID : ::XMLSchema::Traversal::Traverser< ::SANet::XML::PortID >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct TaskCost : ::XMLSchema::Traversal::Traverser< ::SANet::XML::TaskCost >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct CondUtil : ::XMLSchema::Traversal::Traverser< ::SANet::XML::CondUtil >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      typedef
      ::XMLSchema::Traversal::Traverser< ::SANet::XML::CondKind >
      CondKind;

      struct Probability : ::XMLSchema::Traversal::Traverser< ::SANet::XML::Probability >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct LinkWeight : ::XMLSchema::Traversal::Traverser< ::SANet::XML::LinkWeight >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct MultFactor : ::XMLSchema::Traversal::Traverser< ::SANet::XML::MultFactor >
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
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct TaskNode : ::XMLSchema::Traversal::Traverser< ::SANet::XML::TaskNode >
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
        nodeID (Type&);

        virtual void
        nodeID (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        priorProb (Type&);

        virtual void
        priorProb (Type const&);

        virtual void
        attenFactor (Type&);

        virtual void
        attenFactor (Type const&);

        virtual void
        cost (Type&);

        virtual void
        cost (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct CondNode : ::XMLSchema::Traversal::Traverser< ::SANet::XML::CondNode >
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
        nodeID (Type&);

        virtual void
        nodeID (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        probTrue (Type&);

        virtual void
        probTrue (Type const&);

        virtual void
        utility (Type&);

        virtual void
        utility (Type const&);

        virtual void
        kind (Type&);

        virtual void
        kind (Type const&);

        virtual void
        attenFactor (Type&);

        virtual void
        attenFactor (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct PrecondLink : ::XMLSchema::Traversal::Traverser< ::SANet::XML::PrecondLink >
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
        condID (Type&);

        virtual void
        condID (Type const&);

        virtual void
        taskID (Type&);

        virtual void
        taskID (Type const&);

        virtual void
        portID (Type&);

        virtual void
        portID (Type const&);

        virtual void
        trueProb (Type&);

        virtual void
        trueProb (Type const&);

        virtual void
        falseProb (Type&);

        virtual void
        falseProb (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct EffectLink : ::XMLSchema::Traversal::Traverser< ::SANet::XML::EffectLink >
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
        taskID (Type&);

        virtual void
        taskID (Type const&);

        virtual void
        condID (Type&);

        virtual void
        condID (Type const&);

        virtual void
        portID (Type&);

        virtual void
        portID (Type const&);

        virtual void
        weight (Type&);

        virtual void
        weight (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Network : ::XMLSchema::Traversal::Traverser< ::SANet::XML::Network >
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
        defaultAttenFactor (Type&);

        virtual void
        defaultAttenFactor (Type const&);

        virtual void
        defaultTaskCost (Type&);

        virtual void
        defaultTaskCost (Type const&);

        virtual void
        defaultCondUtil (Type&);

        virtual void
        defaultCondUtil (Type const&);

        virtual void
        defaultCondProbTrue (Type&);

        virtual void
        defaultCondProbTrue (Type const&);

        virtual void
        linkThresh (Type&);

        virtual void
        linkThresh (Type const&);

        virtual void
        taskNode (Type&);

        virtual void
        taskNode (Type const&);

        virtual void
        taskNode_pre (Type&);

        virtual void
        taskNode_pre (Type const&);

        virtual void
        taskNode_next (Type&);

        virtual void
        taskNode_next (Type const&);

        virtual void
        taskNode_post (Type&);

        virtual void
        taskNode_post (Type const&);

        virtual void
        condNode (Type&);

        virtual void
        condNode (Type const&);

        virtual void
        condNode_pre (Type&);

        virtual void
        condNode_pre (Type const&);

        virtual void
        condNode_next (Type&);

        virtual void
        condNode_next (Type const&);

        virtual void
        condNode_post (Type&);

        virtual void
        condNode_post (Type const&);

        virtual void
        precondLink (Type&);

        virtual void
        precondLink (Type const&);

        virtual void
        precondLink_pre (Type&);

        virtual void
        precondLink_pre (Type const&);

        virtual void
        precondLink_next (Type&);

        virtual void
        precondLink_next (Type const&);

        virtual void
        precondLink_post (Type&);

        virtual void
        precondLink_post (Type const&);

        virtual void
        effectLink (Type&);

        virtual void
        effectLink (Type const&);

        virtual void
        effectLink_pre (Type&);

        virtual void
        effectLink_pre (Type const&);

        virtual void
        effectLink_next (Type&);

        virtual void
        effectLink_next (Type const&);

        virtual void
        effectLink_post (Type&);

        virtual void
        effectLink_post (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };
    }
  }
}

#include "XMLSchema/Writer.hpp"

namespace SANet
{
  namespace XML
  {
    namespace Writer
    {
      struct NodeID : Traversal::NodeID, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::NodeID Type;
        NodeID (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        NodeID ();
      };

      struct PortID : Traversal::PortID, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::PortID Type;
        PortID (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        PortID ();
      };

      struct TaskCost : Traversal::TaskCost, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::TaskCost Type;
        TaskCost (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        TaskCost ();
      };

      struct CondUtil : Traversal::CondUtil, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::CondUtil Type;
        CondUtil (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        CondUtil ();
      };

      struct CondKind : Traversal::CondKind, 
      virtual ::XSCRT::Writer< wchar_t >
      {
        CondKind (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {
          this->traverse (const_cast <Type const &> (o));
        }

        virtual void
        traverse (Type const&);

        protected:
        CondKind ();
      };

      struct Probability : Traversal::Probability, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::Probability Type;
        Probability (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        Probability ();
      };

      struct LinkWeight : Traversal::LinkWeight, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::LinkWeight Type;
        LinkWeight (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        LinkWeight ();
      };

      struct MultFactor : Traversal::MultFactor, 
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >,
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::MultFactor Type;
        MultFactor (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        protected:
        MultFactor ();
      };

      struct TaskNode : Traversal::TaskNode, 
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::TaskNode Type;
        TaskNode (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        nodeID (Type &o)
        {

          this->nodeID (const_cast <Type const &> (o));
        }


        virtual void
        nodeID (Type const&);

        virtual void 
        name (Type &o)
        {

          this->name (const_cast <Type const &> (o));
        }


        virtual void
        name (Type const&);

        virtual void 
        priorProb (Type &o)
        {

          this->priorProb (const_cast <Type const &> (o));
        }


        virtual void
        priorProb (Type const&);

        virtual void 
        attenFactor (Type &o)
        {

          this->attenFactor (const_cast <Type const &> (o));
        }


        virtual void
        attenFactor (Type const&);

        virtual void 
        cost (Type &o)
        {

          this->cost (const_cast <Type const &> (o));
        }


        virtual void
        cost (Type const&);

        protected:
        TaskNode ();
      };

      struct CondNode : Traversal::CondNode, 
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::CondNode Type;
        CondNode (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        nodeID (Type &o)
        {

          this->nodeID (const_cast <Type const &> (o));
        }


        virtual void
        nodeID (Type const&);

        virtual void 
        name (Type &o)
        {

          this->name (const_cast <Type const &> (o));
        }


        virtual void
        name (Type const&);

        virtual void 
        probTrue (Type &o)
        {

          this->probTrue (const_cast <Type const &> (o));
        }


        virtual void
        probTrue (Type const&);

        virtual void 
        utility (Type &o)
        {

          this->utility (const_cast <Type const &> (o));
        }


        virtual void
        utility (Type const&);

        virtual void 
        kind (Type &o)
        {

          this->kind (const_cast <Type const &> (o));
        }


        virtual void
        kind (Type const&);

        virtual void 
        attenFactor (Type &o)
        {

          this->attenFactor (const_cast <Type const &> (o));
        }


        virtual void
        attenFactor (Type const&);

        protected:
        CondNode ();
      };

      struct PrecondLink : Traversal::PrecondLink, 
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::PrecondLink Type;
        PrecondLink (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        condID (Type &o)
        {

          this->condID (const_cast <Type const &> (o));
        }


        virtual void
        condID (Type const&);

        virtual void 
        taskID (Type &o)
        {

          this->taskID (const_cast <Type const &> (o));
        }


        virtual void
        taskID (Type const&);

        virtual void 
        portID (Type &o)
        {

          this->portID (const_cast <Type const &> (o));
        }


        virtual void
        portID (Type const&);

        virtual void 
        trueProb (Type &o)
        {

          this->trueProb (const_cast <Type const &> (o));
        }


        virtual void
        trueProb (Type const&);

        virtual void 
        falseProb (Type &o)
        {

          this->falseProb (const_cast <Type const &> (o));
        }


        virtual void
        falseProb (Type const&);

        protected:
        PrecondLink ();
      };

      struct EffectLink : Traversal::EffectLink, 
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::EffectLink Type;
        EffectLink (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        taskID (Type &o)
        {

          this->taskID (const_cast <Type const &> (o));
        }


        virtual void
        taskID (Type const&);

        virtual void 
        condID (Type &o)
        {

          this->condID (const_cast <Type const &> (o));
        }


        virtual void
        condID (Type const&);

        virtual void 
        portID (Type &o)
        {

          this->portID (const_cast <Type const &> (o));
        }


        virtual void
        portID (Type const&);

        virtual void 
        weight (Type &o)
        {

          this->weight (const_cast <Type const &> (o));
        }


        virtual void
        weight (Type const&);

        protected:
        EffectLink ();
      };

      struct Network : Traversal::Network, 
      virtual ::XSCRT::Writer< wchar_t >
      {
        typedef ::SANet::XML::Network Type;
        Network (::XSCRT::XML::Element< wchar_t >&);

        virtual void 
        traverse (Type &o)
        {

          this->traverse (const_cast <Type const &> (o));
        }


        virtual void
        traverse (Type const&);

        virtual void 
        defaultAttenFactor (Type &o)
        {

          this->defaultAttenFactor (const_cast <Type const &> (o));
        }


        virtual void
        defaultAttenFactor (Type const&);

        virtual void 
        defaultTaskCost (Type &o)
        {

          this->defaultTaskCost (const_cast <Type const &> (o));
        }


        virtual void
        defaultTaskCost (Type const&);

        virtual void 
        defaultCondUtil (Type &o)
        {

          this->defaultCondUtil (const_cast <Type const &> (o));
        }


        virtual void
        defaultCondUtil (Type const&);

        virtual void 
        defaultCondProbTrue (Type &o)
        {

          this->defaultCondProbTrue (const_cast <Type const &> (o));
        }


        virtual void
        defaultCondProbTrue (Type const&);

        virtual void 
        linkThresh (Type &o)
        {

          this->linkThresh (const_cast <Type const &> (o));
        }


        virtual void
        linkThresh (Type const&);

        virtual void 
        taskNode_pre (Type &o)
        {

          this->taskNode_pre (const_cast <Type const &> (o));
        }


        virtual void
        taskNode_pre (Type const&);

        virtual void 
        taskNode_next (Type &o)
        {

          this->taskNode_next (const_cast <Type const &> (o));
        }


        virtual void
        taskNode_next (Type const&);

        virtual void 
        taskNode_post (Type &o)
        {

          this->taskNode_post (const_cast <Type const &> (o));
        }


        virtual void
        taskNode_post (Type const&);

        virtual void 
        condNode_pre (Type &o)
        {

          this->condNode_pre (const_cast <Type const &> (o));
        }


        virtual void
        condNode_pre (Type const&);

        virtual void 
        condNode_next (Type &o)
        {

          this->condNode_next (const_cast <Type const &> (o));
        }


        virtual void
        condNode_next (Type const&);

        virtual void 
        condNode_post (Type &o)
        {

          this->condNode_post (const_cast <Type const &> (o));
        }


        virtual void
        condNode_post (Type const&);

        virtual void 
        precondLink_pre (Type &o)
        {

          this->precondLink_pre (const_cast <Type const &> (o));
        }


        virtual void
        precondLink_pre (Type const&);

        virtual void 
        precondLink_next (Type &o)
        {

          this->precondLink_next (const_cast <Type const &> (o));
        }


        virtual void
        precondLink_next (Type const&);

        virtual void 
        precondLink_post (Type &o)
        {

          this->precondLink_post (const_cast <Type const &> (o));
        }


        virtual void
        precondLink_post (Type const&);

        virtual void 
        effectLink_pre (Type &o)
        {

          this->effectLink_pre (const_cast <Type const &> (o));
        }


        virtual void
        effectLink_pre (Type const&);

        virtual void 
        effectLink_next (Type &o)
        {

          this->effectLink_next (const_cast <Type const &> (o));
        }


        virtual void
        effectLink_next (Type const&);

        virtual void 
        effectLink_post (Type &o)
        {

          this->effectLink_post (const_cast <Type const &> (o));
        }


        virtual void
        effectLink_post (Type const&);

        protected:
        Network ();
      };
    }
  }
}

namespace SANet
{
  namespace XML
  {
    void
    network (::SANet::XML::Network const&, xercesc::DOMDocument*);
  }
}

#endif // XML_SANET_HPP
