#ifndef SANET_NETWORK_HPP
#define SANET_NETWORK_HPP

// Fix for Borland compilers, which seem to have a broken
// <string> include.
#ifdef __BORLANDC__
# include <string.h>
#endif

// Forward declarations.
//
namespace SANet
{
  namespace XML
  {
  class CondKind;
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
      ENVIRON_l, SYSTEM_l, DATA_l
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


  class TaskNode : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // nodeID
    // 
    public:
    ::XMLSchema::int_ const& nodeID () const;
    void nodeID (::XMLSchema::int_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::int_ > nodeID_;

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
    ::XMLSchema::double_ const& priorProb () const;
    void priorProb (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > priorProb_;

    // attenFactor
    // 
    public:
    ::XMLSchema::double_ const& attenFactor () const;
    void attenFactor (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > attenFactor_;

    // cost
    // 
    public:
    ::XMLSchema::double_ const& cost () const;
    void cost (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > cost_;

    public:
    TaskNode (::XMLSchema::int_ const& nodeID__,
              ::XMLSchema::string< wchar_t > const& name__,
              ::XMLSchema::double_ const& priorProb__,
              ::XMLSchema::double_ const& attenFactor__,
              ::XMLSchema::double_ const& cost__);

    TaskNode (::XSCRT::XML::Element< wchar_t > const&);
    TaskNode (TaskNode const& s);

    TaskNode&
    operator= (TaskNode const& s);

    private:
    char regulator__;
  };


  class CondNode : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // nodeID
    // 
    public:
    ::XMLSchema::int_ const& nodeID () const;
    void nodeID (::XMLSchema::int_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::int_ > nodeID_;

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
    ::XMLSchema::double_ const& probTrue () const;
    void probTrue (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > probTrue_;

    // utility
    // 
    public:
    ::XMLSchema::double_ const& utility () const;
    void utility (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > utility_;

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
    ::XMLSchema::double_ const& attenFactor () const;
    void attenFactor (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > attenFactor_;

    public:
    CondNode (::XMLSchema::int_ const& nodeID__,
              ::XMLSchema::string< wchar_t > const& name__,
              ::XMLSchema::double_ const& probTrue__,
              ::XMLSchema::double_ const& utility__,
              ::SANet::XML::CondKind const& kind__,
              ::XMLSchema::double_ const& attenFactor__);

    CondNode (::XSCRT::XML::Element< wchar_t > const&);
    CondNode (CondNode const& s);

    CondNode&
    operator= (CondNode const& s);

    private:
    char regulator__;
  };


  class PrecondLink : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // condID
    // 
    public:
    ::XMLSchema::int_ const& condID () const;
    void condID (::XMLSchema::int_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::int_ > condID_;

    // taskID
    // 
    public:
    ::XMLSchema::int_ const& taskID () const;
    void taskID (::XMLSchema::int_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::int_ > taskID_;

    // portID
    // 
    public:
    ::XMLSchema::string< wchar_t > const& portID () const;
    void portID (::XMLSchema::string< wchar_t > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< wchar_t > > portID_;

    // trueProb
    // 
    public:
    ::XMLSchema::double_ const& trueProb () const;
    void trueProb (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > trueProb_;

    // falseProb
    // 
    public:
    ::XMLSchema::double_ const& falseProb () const;
    void falseProb (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > falseProb_;

    public:
    PrecondLink (::XMLSchema::int_ const& condID__,
                 ::XMLSchema::int_ const& taskID__,
                 ::XMLSchema::string< wchar_t > const& portID__,
                 ::XMLSchema::double_ const& trueProb__,
                 ::XMLSchema::double_ const& falseProb__);

    PrecondLink (::XSCRT::XML::Element< wchar_t > const&);
    PrecondLink (PrecondLink const& s);

    PrecondLink&
    operator= (PrecondLink const& s);

    private:
    char regulator__;
  };


  class EffectLink : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // taskID
    // 
    public:
    ::XMLSchema::int_ const& taskID () const;
    void taskID (::XMLSchema::int_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::int_ > taskID_;

    // condID
    // 
    public:
    ::XMLSchema::int_ const& condID () const;
    void condID (::XMLSchema::int_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::int_ > condID_;

    // portID
    // 
    public:
    ::XMLSchema::string< wchar_t > const& portID () const;
    void portID (::XMLSchema::string< wchar_t > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< wchar_t > > portID_;

    // weight
    // 
    public:
    ::XMLSchema::double_ const& weight () const;
    void weight (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > weight_;

    public:
    EffectLink (::XMLSchema::int_ const& taskID__,
                ::XMLSchema::int_ const& condID__,
                ::XMLSchema::string< wchar_t > const& portID__,
                ::XMLSchema::double_ const& weight__);

    EffectLink (::XSCRT::XML::Element< wchar_t > const&);
    EffectLink (EffectLink const& s);

    EffectLink&
    operator= (EffectLink const& s);

    private:
    char regulator__;
  };


  class Network : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // defaultAttenFactor
    // 
    public:
    ::XMLSchema::double_ const& defaultAttenFactor () const;
    void defaultAttenFactor (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > defaultAttenFactor_;

    // defaultTaskCost
    // 
    public:
    ::XMLSchema::double_ const& defaultTaskCost () const;
    void defaultTaskCost (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > defaultTaskCost_;

    // defaultCondUtil
    // 
    public:
    ::XMLSchema::double_ const& defaultCondUtil () const;
    void defaultCondUtil (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > defaultCondUtil_;

    // defaultCondProbTrue
    // 
    public:
    ::XMLSchema::double_ const& defaultCondProbTrue () const;
    void defaultCondProbTrue (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > defaultCondProbTrue_;

    // linkThresh
    // 
    public:
    ::XMLSchema::double_ const& linkThresh () const;
    void linkThresh (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > linkThresh_;

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
    Network (::XMLSchema::double_ const& defaultAttenFactor__,
             ::XMLSchema::double_ const& defaultTaskCost__,
             ::XMLSchema::double_ const& defaultCondUtil__,
             ::XMLSchema::double_ const& defaultCondProbTrue__,
             ::XMLSchema::double_ const& linkThresh__,
             ::std::list< ::SANet::XML::TaskNode > const& taskNode__,
             ::std::list< ::SANet::XML::CondNode > const& condNode__,
             ::std::list< ::SANet::XML::PrecondLink > const& precondLink__,
             ::std::list< ::SANet::XML::EffectLink > const& effectLink__);

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
  namespace reader
  {
    ::SANet::XML::Network
    network (xercesc::DOMDocument const*);
  }
  }
}

#endif // SANET_NETWORK_HPP
