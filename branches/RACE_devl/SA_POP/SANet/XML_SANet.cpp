#include "XML_SANet.hpp"

namespace SANet
{
  namespace XML
  {
    // NodeID
    // 

    NodeID::
    NodeID (::XMLSchema::int_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    NodeID::
    NodeID (::SANet::XML::NodeID const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::NodeID& NodeID::
    operator= (::SANet::XML::NodeID const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // PortID
    // 

    PortID::
    PortID (::XMLSchema::string< wchar_t > const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    PortID::
    PortID (::SANet::XML::PortID const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::PortID& PortID::
    operator= (::SANet::XML::PortID const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // TaskCost
    // 

    TaskCost::
    TaskCost (::XMLSchema::double_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    TaskCost::
    TaskCost (::SANet::XML::TaskCost const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::TaskCost& TaskCost::
    operator= (::SANet::XML::TaskCost const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // CondUtil
    // 

    CondUtil::
    CondUtil (::XMLSchema::double_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    CondUtil::
    CondUtil (::SANet::XML::CondUtil const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::CondUtil& CondUtil::
    operator= (::SANet::XML::CondUtil const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // CondKind
    // 

    CondKind::Value CondKind::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::SANet::XML::CondKind const& a, ::SANet::XML::CondKind const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::SANet::XML::CondKind const& a, ::SANet::XML::CondKind const& b)
    {
      return a.v_ != b.v_;
    }

    CondKind::
    CondKind (CondKind::Value v)
    : v_ (v)
    {
    }

    // Probability
    // 

    Probability::
    Probability (::XMLSchema::double_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    Probability::
    Probability (::SANet::XML::Probability const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::Probability& Probability::
    operator= (::SANet::XML::Probability const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // LinkWeight
    // 

    LinkWeight::
    LinkWeight (::XMLSchema::double_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    LinkWeight::
    LinkWeight (::SANet::XML::LinkWeight const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::LinkWeight& LinkWeight::
    operator= (::SANet::XML::LinkWeight const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // MultFactor
    // 

    MultFactor::
    MultFactor (::XMLSchema::double_ const& b__)
    : 
    Base__ (b__),
    regulator__ ()
    {
    }

    MultFactor::
    MultFactor (::SANet::XML::MultFactor const& s)
    :
    Base__ (s),
    regulator__ ()
    {
    }

    ::SANet::XML::MultFactor& MultFactor::
    operator= (::SANet::XML::MultFactor const& s)
    {
      static_cast< Base__& > (*this) = static_cast< Base__ const& > (s);

      return *this;
    }



    // TaskNode
    // 

    TaskNode::
    TaskNode (::SANet::XML::NodeID const& nodeID__,
              ::XMLSchema::string< wchar_t > const& name__,
              ::SANet::XML::Probability const& priorProb__,
              ::SANet::XML::MultFactor const& attenFactor__,
              ::SANet::XML::TaskCost const& cost__)
    : 
    ::XSCRT::Type (), 
    nodeID_ (new ::SANet::XML::NodeID (nodeID__)),
    name_ (new ::XMLSchema::string< wchar_t > (name__)),
    priorProb_ (new ::SANet::XML::Probability (priorProb__)),
    attenFactor_ (new ::SANet::XML::MultFactor (attenFactor__)),
    cost_ (new ::SANet::XML::TaskCost (cost__)),
    regulator__ ()
    {
      nodeID_->container (this);
      name_->container (this);
      priorProb_->container (this);
      attenFactor_->container (this);
      cost_->container (this);
    }

    TaskNode::
    TaskNode (::SANet::XML::TaskNode const& s)
    :
    ::XSCRT::Type (),
    nodeID_ (new ::SANet::XML::NodeID (*s.nodeID_)),
    name_ (new ::XMLSchema::string< wchar_t > (*s.name_)),
    priorProb_ (new ::SANet::XML::Probability (*s.priorProb_)),
    attenFactor_ (new ::SANet::XML::MultFactor (*s.attenFactor_)),
    cost_ (new ::SANet::XML::TaskCost (*s.cost_)),
    regulator__ ()
    {
      nodeID_->container (this);
      name_->container (this);
      priorProb_->container (this);
      attenFactor_->container (this);
      cost_->container (this);
    }

    ::SANet::XML::TaskNode& TaskNode::
    operator= (::SANet::XML::TaskNode const& s)
    {
      nodeID (s.nodeID ());

      name (s.name ());

      priorProb (s.priorProb ());

      attenFactor (s.attenFactor ());

      cost (s.cost ());

      return *this;
    }


    // TaskNode
    // 
    ::SANet::XML::NodeID const& TaskNode::
    nodeID () const
    {
      return *nodeID_;
    }

    void TaskNode::
    nodeID (::SANet::XML::NodeID const& e)
    {
      *nodeID_ = e;
    }

    // TaskNode
    // 
    ::XMLSchema::string< wchar_t > const& TaskNode::
    name () const
    {
      return *name_;
    }

    void TaskNode::
    name (::XMLSchema::string< wchar_t > const& e)
    {
      *name_ = e;
    }

    // TaskNode
    // 
    ::SANet::XML::Probability const& TaskNode::
    priorProb () const
    {
      return *priorProb_;
    }

    void TaskNode::
    priorProb (::SANet::XML::Probability const& e)
    {
      *priorProb_ = e;
    }

    // TaskNode
    // 
    ::SANet::XML::MultFactor const& TaskNode::
    attenFactor () const
    {
      return *attenFactor_;
    }

    void TaskNode::
    attenFactor (::SANet::XML::MultFactor const& e)
    {
      *attenFactor_ = e;
    }

    // TaskNode
    // 
    ::SANet::XML::TaskCost const& TaskNode::
    cost () const
    {
      return *cost_;
    }

    void TaskNode::
    cost (::SANet::XML::TaskCost const& e)
    {
      *cost_ = e;
    }


    // CondNode
    // 

    CondNode::
    CondNode (::SANet::XML::NodeID const& nodeID__,
              ::XMLSchema::string< wchar_t > const& name__,
              ::SANet::XML::Probability const& probTrue__,
              ::SANet::XML::CondUtil const& utility__,
              ::SANet::XML::CondKind const& kind__,
              ::SANet::XML::MultFactor const& attenFactor__)
    : 
    ::XSCRT::Type (), 
    nodeID_ (new ::SANet::XML::NodeID (nodeID__)),
    name_ (new ::XMLSchema::string< wchar_t > (name__)),
    probTrue_ (new ::SANet::XML::Probability (probTrue__)),
    utility_ (new ::SANet::XML::CondUtil (utility__)),
    kind_ (new ::SANet::XML::CondKind (kind__)),
    attenFactor_ (new ::SANet::XML::MultFactor (attenFactor__)),
    regulator__ ()
    {
      nodeID_->container (this);
      name_->container (this);
      probTrue_->container (this);
      utility_->container (this);
      kind_->container (this);
      attenFactor_->container (this);
    }

    CondNode::
    CondNode (::SANet::XML::CondNode const& s)
    :
    ::XSCRT::Type (),
    nodeID_ (new ::SANet::XML::NodeID (*s.nodeID_)),
    name_ (new ::XMLSchema::string< wchar_t > (*s.name_)),
    probTrue_ (new ::SANet::XML::Probability (*s.probTrue_)),
    utility_ (new ::SANet::XML::CondUtil (*s.utility_)),
    kind_ (new ::SANet::XML::CondKind (*s.kind_)),
    attenFactor_ (new ::SANet::XML::MultFactor (*s.attenFactor_)),
    regulator__ ()
    {
      nodeID_->container (this);
      name_->container (this);
      probTrue_->container (this);
      utility_->container (this);
      kind_->container (this);
      attenFactor_->container (this);
    }

    ::SANet::XML::CondNode& CondNode::
    operator= (::SANet::XML::CondNode const& s)
    {
      nodeID (s.nodeID ());

      name (s.name ());

      probTrue (s.probTrue ());

      utility (s.utility ());

      kind (s.kind ());

      attenFactor (s.attenFactor ());

      return *this;
    }


    // CondNode
    // 
    ::SANet::XML::NodeID const& CondNode::
    nodeID () const
    {
      return *nodeID_;
    }

    void CondNode::
    nodeID (::SANet::XML::NodeID const& e)
    {
      *nodeID_ = e;
    }

    // CondNode
    // 
    ::XMLSchema::string< wchar_t > const& CondNode::
    name () const
    {
      return *name_;
    }

    void CondNode::
    name (::XMLSchema::string< wchar_t > const& e)
    {
      *name_ = e;
    }

    // CondNode
    // 
    ::SANet::XML::Probability const& CondNode::
    probTrue () const
    {
      return *probTrue_;
    }

    void CondNode::
    probTrue (::SANet::XML::Probability const& e)
    {
      *probTrue_ = e;
    }

    // CondNode
    // 
    ::SANet::XML::CondUtil const& CondNode::
    utility () const
    {
      return *utility_;
    }

    void CondNode::
    utility (::SANet::XML::CondUtil const& e)
    {
      *utility_ = e;
    }

    // CondNode
    // 
    ::SANet::XML::CondKind const& CondNode::
    kind () const
    {
      return *kind_;
    }

    void CondNode::
    kind (::SANet::XML::CondKind const& e)
    {
      *kind_ = e;
    }

    // CondNode
    // 
    ::SANet::XML::MultFactor const& CondNode::
    attenFactor () const
    {
      return *attenFactor_;
    }

    void CondNode::
    attenFactor (::SANet::XML::MultFactor const& e)
    {
      *attenFactor_ = e;
    }


    // PrecondLink
    // 

    PrecondLink::
    PrecondLink (::SANet::XML::NodeID const& condID__,
                 ::SANet::XML::NodeID const& taskID__,
                 ::SANet::XML::PortID const& portID__,
                 ::SANet::XML::Probability const& trueProb__,
                 ::SANet::XML::Probability const& falseProb__)
    : 
    ::XSCRT::Type (), 
    condID_ (new ::SANet::XML::NodeID (condID__)),
    taskID_ (new ::SANet::XML::NodeID (taskID__)),
    portID_ (new ::SANet::XML::PortID (portID__)),
    trueProb_ (new ::SANet::XML::Probability (trueProb__)),
    falseProb_ (new ::SANet::XML::Probability (falseProb__)),
    regulator__ ()
    {
      condID_->container (this);
      taskID_->container (this);
      portID_->container (this);
      trueProb_->container (this);
      falseProb_->container (this);
    }

    PrecondLink::
    PrecondLink (::SANet::XML::PrecondLink const& s)
    :
    ::XSCRT::Type (),
    condID_ (new ::SANet::XML::NodeID (*s.condID_)),
    taskID_ (new ::SANet::XML::NodeID (*s.taskID_)),
    portID_ (new ::SANet::XML::PortID (*s.portID_)),
    trueProb_ (new ::SANet::XML::Probability (*s.trueProb_)),
    falseProb_ (new ::SANet::XML::Probability (*s.falseProb_)),
    regulator__ ()
    {
      condID_->container (this);
      taskID_->container (this);
      portID_->container (this);
      trueProb_->container (this);
      falseProb_->container (this);
    }

    ::SANet::XML::PrecondLink& PrecondLink::
    operator= (::SANet::XML::PrecondLink const& s)
    {
      condID (s.condID ());

      taskID (s.taskID ());

      portID (s.portID ());

      trueProb (s.trueProb ());

      falseProb (s.falseProb ());

      return *this;
    }


    // PrecondLink
    // 
    ::SANet::XML::NodeID const& PrecondLink::
    condID () const
    {
      return *condID_;
    }

    void PrecondLink::
    condID (::SANet::XML::NodeID const& e)
    {
      *condID_ = e;
    }

    // PrecondLink
    // 
    ::SANet::XML::NodeID const& PrecondLink::
    taskID () const
    {
      return *taskID_;
    }

    void PrecondLink::
    taskID (::SANet::XML::NodeID const& e)
    {
      *taskID_ = e;
    }

    // PrecondLink
    // 
    ::SANet::XML::PortID const& PrecondLink::
    portID () const
    {
      return *portID_;
    }

    void PrecondLink::
    portID (::SANet::XML::PortID const& e)
    {
      *portID_ = e;
    }

    // PrecondLink
    // 
    ::SANet::XML::Probability const& PrecondLink::
    trueProb () const
    {
      return *trueProb_;
    }

    void PrecondLink::
    trueProb (::SANet::XML::Probability const& e)
    {
      *trueProb_ = e;
    }

    // PrecondLink
    // 
    ::SANet::XML::Probability const& PrecondLink::
    falseProb () const
    {
      return *falseProb_;
    }

    void PrecondLink::
    falseProb (::SANet::XML::Probability const& e)
    {
      *falseProb_ = e;
    }


    // EffectLink
    // 

    EffectLink::
    EffectLink (::SANet::XML::NodeID const& taskID__,
                ::SANet::XML::NodeID const& condID__,
                ::SANet::XML::PortID const& portID__,
                ::SANet::XML::LinkWeight const& weight__)
    : 
    ::XSCRT::Type (), 
    taskID_ (new ::SANet::XML::NodeID (taskID__)),
    condID_ (new ::SANet::XML::NodeID (condID__)),
    portID_ (new ::SANet::XML::PortID (portID__)),
    weight_ (new ::SANet::XML::LinkWeight (weight__)),
    regulator__ ()
    {
      taskID_->container (this);
      condID_->container (this);
      portID_->container (this);
      weight_->container (this);
    }

    EffectLink::
    EffectLink (::SANet::XML::EffectLink const& s)
    :
    ::XSCRT::Type (),
    taskID_ (new ::SANet::XML::NodeID (*s.taskID_)),
    condID_ (new ::SANet::XML::NodeID (*s.condID_)),
    portID_ (new ::SANet::XML::PortID (*s.portID_)),
    weight_ (new ::SANet::XML::LinkWeight (*s.weight_)),
    regulator__ ()
    {
      taskID_->container (this);
      condID_->container (this);
      portID_->container (this);
      weight_->container (this);
    }

    ::SANet::XML::EffectLink& EffectLink::
    operator= (::SANet::XML::EffectLink const& s)
    {
      taskID (s.taskID ());

      condID (s.condID ());

      portID (s.portID ());

      weight (s.weight ());

      return *this;
    }


    // EffectLink
    // 
    ::SANet::XML::NodeID const& EffectLink::
    taskID () const
    {
      return *taskID_;
    }

    void EffectLink::
    taskID (::SANet::XML::NodeID const& e)
    {
      *taskID_ = e;
    }

    // EffectLink
    // 
    ::SANet::XML::NodeID const& EffectLink::
    condID () const
    {
      return *condID_;
    }

    void EffectLink::
    condID (::SANet::XML::NodeID const& e)
    {
      *condID_ = e;
    }

    // EffectLink
    // 
    ::SANet::XML::PortID const& EffectLink::
    portID () const
    {
      return *portID_;
    }

    void EffectLink::
    portID (::SANet::XML::PortID const& e)
    {
      *portID_ = e;
    }

    // EffectLink
    // 
    ::SANet::XML::LinkWeight const& EffectLink::
    weight () const
    {
      return *weight_;
    }

    void EffectLink::
    weight (::SANet::XML::LinkWeight const& e)
    {
      *weight_ = e;
    }


    // Network
    // 

    Network::
    Network (::SANet::XML::MultFactor const& defaultAttenFactor__,
             ::SANet::XML::TaskCost const& defaultTaskCost__,
             ::SANet::XML::CondUtil const& defaultCondUtil__,
             ::SANet::XML::Probability const& defaultCondProbTrue__,
             ::SANet::XML::LinkWeight const& linkThresh__)
    : 
    ::XSCRT::Type (), 
    defaultAttenFactor_ (new ::SANet::XML::MultFactor (defaultAttenFactor__)),
    defaultTaskCost_ (new ::SANet::XML::TaskCost (defaultTaskCost__)),
    defaultCondUtil_ (new ::SANet::XML::CondUtil (defaultCondUtil__)),
    defaultCondProbTrue_ (new ::SANet::XML::Probability (defaultCondProbTrue__)),
    linkThresh_ (new ::SANet::XML::LinkWeight (linkThresh__)),
    regulator__ ()
    {
      defaultAttenFactor_->container (this);
      defaultTaskCost_->container (this);
      defaultCondUtil_->container (this);
      defaultCondProbTrue_->container (this);
      linkThresh_->container (this);
    }

    Network::
    Network (::SANet::XML::Network const& s)
    :
    ::XSCRT::Type (),
    defaultAttenFactor_ (new ::SANet::XML::MultFactor (*s.defaultAttenFactor_)),
    defaultTaskCost_ (new ::SANet::XML::TaskCost (*s.defaultTaskCost_)),
    defaultCondUtil_ (new ::SANet::XML::CondUtil (*s.defaultCondUtil_)),
    defaultCondProbTrue_ (new ::SANet::XML::Probability (*s.defaultCondProbTrue_)),
    linkThresh_ (new ::SANet::XML::LinkWeight (*s.linkThresh_)),
    regulator__ ()
    {
      defaultAttenFactor_->container (this);
      defaultTaskCost_->container (this);
      defaultCondUtil_->container (this);
      defaultCondProbTrue_->container (this);
      linkThresh_->container (this);
      {
        for (taskNode_const_iterator i (s.taskNode_.begin ());i != s.taskNode_.end ();++i) add_taskNode (*i);
      }

      {
        for (condNode_const_iterator i (s.condNode_.begin ());i != s.condNode_.end ();++i) add_condNode (*i);
      }

      {
        for (precondLink_const_iterator i (s.precondLink_.begin ());i != s.precondLink_.end ();++i) add_precondLink (*i);
      }

      {
        for (effectLink_const_iterator i (s.effectLink_.begin ());i != s.effectLink_.end ();++i) add_effectLink (*i);
      }
    }

    ::SANet::XML::Network& Network::
    operator= (::SANet::XML::Network const& s)
    {
      defaultAttenFactor (s.defaultAttenFactor ());

      defaultTaskCost (s.defaultTaskCost ());

      defaultCondUtil (s.defaultCondUtil ());

      defaultCondProbTrue (s.defaultCondProbTrue ());

      linkThresh (s.linkThresh ());

      taskNode_.clear ();
      {
        for (taskNode_const_iterator i (s.taskNode_.begin ());i != s.taskNode_.end ();++i) add_taskNode (*i);
      }

      condNode_.clear ();
      {
        for (condNode_const_iterator i (s.condNode_.begin ());i != s.condNode_.end ();++i) add_condNode (*i);
      }

      precondLink_.clear ();
      {
        for (precondLink_const_iterator i (s.precondLink_.begin ());i != s.precondLink_.end ();++i) add_precondLink (*i);
      }

      effectLink_.clear ();
      {
        for (effectLink_const_iterator i (s.effectLink_.begin ());i != s.effectLink_.end ();++i) add_effectLink (*i);
      }

      return *this;
    }


    // Network
    // 
    ::SANet::XML::MultFactor const& Network::
    defaultAttenFactor () const
    {
      return *defaultAttenFactor_;
    }

    void Network::
    defaultAttenFactor (::SANet::XML::MultFactor const& e)
    {
      *defaultAttenFactor_ = e;
    }

    // Network
    // 
    ::SANet::XML::TaskCost const& Network::
    defaultTaskCost () const
    {
      return *defaultTaskCost_;
    }

    void Network::
    defaultTaskCost (::SANet::XML::TaskCost const& e)
    {
      *defaultTaskCost_ = e;
    }

    // Network
    // 
    ::SANet::XML::CondUtil const& Network::
    defaultCondUtil () const
    {
      return *defaultCondUtil_;
    }

    void Network::
    defaultCondUtil (::SANet::XML::CondUtil const& e)
    {
      *defaultCondUtil_ = e;
    }

    // Network
    // 
    ::SANet::XML::Probability const& Network::
    defaultCondProbTrue () const
    {
      return *defaultCondProbTrue_;
    }

    void Network::
    defaultCondProbTrue (::SANet::XML::Probability const& e)
    {
      *defaultCondProbTrue_ = e;
    }

    // Network
    // 
    ::SANet::XML::LinkWeight const& Network::
    linkThresh () const
    {
      return *linkThresh_;
    }

    void Network::
    linkThresh (::SANet::XML::LinkWeight const& e)
    {
      *linkThresh_ = e;
    }

    // Network
    // 
    Network::taskNode_iterator Network::
    begin_taskNode ()
    {
      return taskNode_.begin ();
    }

    Network::taskNode_iterator Network::
    end_taskNode ()
    {
      return taskNode_.end ();
    }

    Network::taskNode_const_iterator Network::
    begin_taskNode () const
    {
      return taskNode_.begin ();
    }

    Network::taskNode_const_iterator Network::
    end_taskNode () const
    {
      return taskNode_.end ();
    }

    void Network::
    add_taskNode (::SANet::XML::TaskNode const& e)
    {
      taskNode_.push_back (e);
    }

    size_t Network::
    count_taskNode(void) const
    {
      return taskNode_.size ();
    }

    // Network
    // 
    Network::condNode_iterator Network::
    begin_condNode ()
    {
      return condNode_.begin ();
    }

    Network::condNode_iterator Network::
    end_condNode ()
    {
      return condNode_.end ();
    }

    Network::condNode_const_iterator Network::
    begin_condNode () const
    {
      return condNode_.begin ();
    }

    Network::condNode_const_iterator Network::
    end_condNode () const
    {
      return condNode_.end ();
    }

    void Network::
    add_condNode (::SANet::XML::CondNode const& e)
    {
      condNode_.push_back (e);
    }

    size_t Network::
    count_condNode(void) const
    {
      return condNode_.size ();
    }

    // Network
    // 
    Network::precondLink_iterator Network::
    begin_precondLink ()
    {
      return precondLink_.begin ();
    }

    Network::precondLink_iterator Network::
    end_precondLink ()
    {
      return precondLink_.end ();
    }

    Network::precondLink_const_iterator Network::
    begin_precondLink () const
    {
      return precondLink_.begin ();
    }

    Network::precondLink_const_iterator Network::
    end_precondLink () const
    {
      return precondLink_.end ();
    }

    void Network::
    add_precondLink (::SANet::XML::PrecondLink const& e)
    {
      precondLink_.push_back (e);
    }

    size_t Network::
    count_precondLink(void) const
    {
      return precondLink_.size ();
    }

    // Network
    // 
    Network::effectLink_iterator Network::
    begin_effectLink ()
    {
      return effectLink_.begin ();
    }

    Network::effectLink_iterator Network::
    end_effectLink ()
    {
      return effectLink_.end ();
    }

    Network::effectLink_const_iterator Network::
    begin_effectLink () const
    {
      return effectLink_.begin ();
    }

    Network::effectLink_const_iterator Network::
    end_effectLink () const
    {
      return effectLink_.end ();
    }

    void Network::
    add_effectLink (::SANet::XML::EffectLink const& e)
    {
      effectLink_.push_back (e);
    }

    size_t Network::
    count_effectLink(void) const
    {
      return effectLink_.size ();
    }
  }
}

namespace SANet
{
  namespace XML
  {
    // NodeID
    //

    NodeID::
    NodeID (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    NodeID::
    NodeID (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // PortID
    //

    PortID::
    PortID (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    PortID::
    PortID (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // TaskCost
    //

    TaskCost::
    TaskCost (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    TaskCost::
    TaskCost (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // CondUtil
    //

    CondUtil::
    CondUtil (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    CondUtil::
    CondUtil (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // CondKind
    //

    CondKind::
    CondKind (::XSCRT::XML::Element< wchar_t > const& e)
    : ::XSCRT::Type (e)
    {
      ::std::basic_string< wchar_t > v (e.value ());

      if (v == L"ENVIRON") v_ = ENVIRON_l;
      else if (v == L"SYSTEM") v_ = SYSTEM_l;
      else if (v == L"DATA") v_ = DATA_l;
      else 
      {
      }
    }

    CondKind::
    CondKind (::XSCRT::XML::Attribute< wchar_t > const& a)
    : ::XSCRT::Type (a)
    {
      ::std::basic_string< wchar_t > v (a.value ());

      if (v == L"ENVIRON") v_ = ENVIRON_l;
      else if (v == L"SYSTEM") v_ = SYSTEM_l;
      else if (v == L"DATA") v_ = DATA_l;
      else 
      {
      }
    }

    CondKind const CondKind::ENVIRON (CondKind::ENVIRON_l);
    CondKind const CondKind::SYSTEM (CondKind::SYSTEM_l);
    CondKind const CondKind::DATA (CondKind::DATA_l);

    // Probability
    //

    Probability::
    Probability (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    Probability::
    Probability (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // LinkWeight
    //

    LinkWeight::
    LinkWeight (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    LinkWeight::
    LinkWeight (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // MultFactor
    //

    MultFactor::
    MultFactor (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {
    }

    MultFactor::
    MultFactor (::XSCRT::XML::Attribute< wchar_t > const& a)
    :
    Base__ (a),
    regulator__ ()
    {
    }

    // TaskNode
    //

    TaskNode::
    TaskNode (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"nodeID")
        {
          nodeID_ = ::std::auto_ptr< ::SANet::XML::NodeID > (new ::SANet::XML::NodeID (e));
          nodeID_->container (this);
        }

        else if (n == L"name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
          name_->container (this);
        }

        else if (n == L"priorProb")
        {
          priorProb_ = ::std::auto_ptr< ::SANet::XML::Probability > (new ::SANet::XML::Probability (e));
          priorProb_->container (this);
        }

        else if (n == L"attenFactor")
        {
          attenFactor_ = ::std::auto_ptr< ::SANet::XML::MultFactor > (new ::SANet::XML::MultFactor (e));
          attenFactor_->container (this);
        }

        else if (n == L"cost")
        {
          cost_ = ::std::auto_ptr< ::SANet::XML::TaskCost > (new ::SANet::XML::TaskCost (e));
          cost_->container (this);
        }

        else 
        {
        }
      }
    }

    // CondNode
    //

    CondNode::
    CondNode (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"nodeID")
        {
          nodeID_ = ::std::auto_ptr< ::SANet::XML::NodeID > (new ::SANet::XML::NodeID (e));
          nodeID_->container (this);
        }

        else if (n == L"name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
          name_->container (this);
        }

        else if (n == L"probTrue")
        {
          probTrue_ = ::std::auto_ptr< ::SANet::XML::Probability > (new ::SANet::XML::Probability (e));
          probTrue_->container (this);
        }

        else if (n == L"utility")
        {
          utility_ = ::std::auto_ptr< ::SANet::XML::CondUtil > (new ::SANet::XML::CondUtil (e));
          utility_->container (this);
        }

        else if (n == L"kind")
        {
          kind_ = ::std::auto_ptr< ::SANet::XML::CondKind > (new ::SANet::XML::CondKind (e));
          kind_->container (this);
        }

        else if (n == L"attenFactor")
        {
          attenFactor_ = ::std::auto_ptr< ::SANet::XML::MultFactor > (new ::SANet::XML::MultFactor (e));
          attenFactor_->container (this);
        }

        else 
        {
        }
      }
    }

    // PrecondLink
    //

    PrecondLink::
    PrecondLink (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"condID")
        {
          condID_ = ::std::auto_ptr< ::SANet::XML::NodeID > (new ::SANet::XML::NodeID (e));
          condID_->container (this);
        }

        else if (n == L"taskID")
        {
          taskID_ = ::std::auto_ptr< ::SANet::XML::NodeID > (new ::SANet::XML::NodeID (e));
          taskID_->container (this);
        }

        else if (n == L"portID")
        {
          portID_ = ::std::auto_ptr< ::SANet::XML::PortID > (new ::SANet::XML::PortID (e));
          portID_->container (this);
        }

        else if (n == L"trueProb")
        {
          trueProb_ = ::std::auto_ptr< ::SANet::XML::Probability > (new ::SANet::XML::Probability (e));
          trueProb_->container (this);
        }

        else if (n == L"falseProb")
        {
          falseProb_ = ::std::auto_ptr< ::SANet::XML::Probability > (new ::SANet::XML::Probability (e));
          falseProb_->container (this);
        }

        else 
        {
        }
      }
    }

    // EffectLink
    //

    EffectLink::
    EffectLink (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"taskID")
        {
          taskID_ = ::std::auto_ptr< ::SANet::XML::NodeID > (new ::SANet::XML::NodeID (e));
          taskID_->container (this);
        }

        else if (n == L"condID")
        {
          condID_ = ::std::auto_ptr< ::SANet::XML::NodeID > (new ::SANet::XML::NodeID (e));
          condID_->container (this);
        }

        else if (n == L"portID")
        {
          portID_ = ::std::auto_ptr< ::SANet::XML::PortID > (new ::SANet::XML::PortID (e));
          portID_->container (this);
        }

        else if (n == L"weight")
        {
          weight_ = ::std::auto_ptr< ::SANet::XML::LinkWeight > (new ::SANet::XML::LinkWeight (e));
          weight_->container (this);
        }

        else 
        {
        }
      }
    }

    // Network
    //

    Network::
    Network (::XSCRT::XML::Element< wchar_t > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< wchar_t > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
        ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == L"defaultAttenFactor")
        {
          defaultAttenFactor_ = ::std::auto_ptr< ::SANet::XML::MultFactor > (new ::SANet::XML::MultFactor (e));
          defaultAttenFactor_->container (this);
        }

        else if (n == L"defaultTaskCost")
        {
          defaultTaskCost_ = ::std::auto_ptr< ::SANet::XML::TaskCost > (new ::SANet::XML::TaskCost (e));
          defaultTaskCost_->container (this);
        }

        else if (n == L"defaultCondUtil")
        {
          defaultCondUtil_ = ::std::auto_ptr< ::SANet::XML::CondUtil > (new ::SANet::XML::CondUtil (e));
          defaultCondUtil_->container (this);
        }

        else if (n == L"defaultCondProbTrue")
        {
          defaultCondProbTrue_ = ::std::auto_ptr< ::SANet::XML::Probability > (new ::SANet::XML::Probability (e));
          defaultCondProbTrue_->container (this);
        }

        else if (n == L"linkThresh")
        {
          linkThresh_ = ::std::auto_ptr< ::SANet::XML::LinkWeight > (new ::SANet::XML::LinkWeight (e));
          linkThresh_->container (this);
        }

        else if (n == L"taskNode")
        {
          ::SANet::XML::TaskNode t (e);
          add_taskNode (t);
        }

        else if (n == L"condNode")
        {
          ::SANet::XML::CondNode t (e);
          add_condNode (t);
        }

        else if (n == L"precondLink")
        {
          ::SANet::XML::PrecondLink t (e);
          add_precondLink (t);
        }

        else if (n == L"effectLink")
        {
          ::SANet::XML::EffectLink t (e);
          add_effectLink (t);
        }

        else 
        {
        }
      }
    }
  }
}

namespace SANet
{
  namespace XML
  {
    ::SANet::XML::Network
    network (xercesc::DOMDocument const* d)
    {
      std::cout << "In create netwokr!\n";
      ::XSCRT::XML::Element< wchar_t > e (d->getDocumentElement ());
      std::cout << "Got the element from the network!\n";
      if (e.name () == L"network")
      {
        
        ::SANet::XML::Network r (e);
        std::cout << "Done creating the network\n";
        return r;
      }

      else
      {
        std::wcout << "The network name is not equal to network, but it is " << e.name().c_str() << std::endl;
        throw 1;
      }
    }
  }
}

#include "XMLSchema/TypeInfo.hpp"

namespace SANet
{
  namespace XML
  {
    namespace
    {
      ::XMLSchema::TypeInfoInitializer < wchar_t > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

      struct NodeIDTypeInfoInitializer
      {
        NodeIDTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (NodeID));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::int_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      NodeIDTypeInfoInitializer NodeIDTypeInfoInitializer_;

      struct PortIDTypeInfoInitializer
      {
        PortIDTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (PortID));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::string< wchar_t >));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      PortIDTypeInfoInitializer PortIDTypeInfoInitializer_;

      struct TaskCostTypeInfoInitializer
      {
        TaskCostTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskCost));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::double_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskCostTypeInfoInitializer TaskCostTypeInfoInitializer_;

      struct CondUtilTypeInfoInitializer
      {
        CondUtilTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (CondUtil));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::double_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      CondUtilTypeInfoInitializer CondUtilTypeInfoInitializer_;

      struct CondKindTypeInfoInitializer
      {
        CondKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (CondKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));

          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      CondKindTypeInfoInitializer CondKindTypeInfoInitializer_;

      struct ProbabilityTypeInfoInitializer
      {
        ProbabilityTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Probability));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::double_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ProbabilityTypeInfoInitializer ProbabilityTypeInfoInitializer_;

      struct LinkWeightTypeInfoInitializer
      {
        LinkWeightTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (LinkWeight));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::double_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      LinkWeightTypeInfoInitializer LinkWeightTypeInfoInitializer_;

      struct MultFactorTypeInfoInitializer
      {
        MultFactorTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (MultFactor));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XMLSchema::double_));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      MultFactorTypeInfoInitializer MultFactorTypeInfoInitializer_;

      struct TaskNodeTypeInfoInitializer
      {
        TaskNodeTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TaskNode));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TaskNodeTypeInfoInitializer TaskNodeTypeInfoInitializer_;

      struct CondNodeTypeInfoInitializer
      {
        CondNodeTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (CondNode));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      CondNodeTypeInfoInitializer CondNodeTypeInfoInitializer_;

      struct PrecondLinkTypeInfoInitializer
      {
        PrecondLinkTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (PrecondLink));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      PrecondLinkTypeInfoInitializer PrecondLinkTypeInfoInitializer_;

      struct EffectLinkTypeInfoInitializer
      {
        EffectLinkTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (EffectLink));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      EffectLinkTypeInfoInitializer EffectLinkTypeInfoInitializer_;

      struct NetworkTypeInfoInitializer
      {
        NetworkTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Network));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      NetworkTypeInfoInitializer NetworkTypeInfoInitializer_;
    }
  }
}

namespace SANet
{
  namespace XML
  {
    namespace Traversal
    {
      // NodeID
      //
      //

      void NodeID::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void NodeID::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void NodeID::
      pre (Type&)
      {
      }

      void NodeID::
      pre (Type const&)
      {
      }

      void NodeID::
      post (Type&)
      {
      }

      void NodeID::
      post (Type const&)
      {
      }

      // PortID
      //
      //

      void PortID::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void PortID::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void PortID::
      pre (Type&)
      {
      }

      void PortID::
      pre (Type const&)
      {
      }

      void PortID::
      post (Type&)
      {
      }

      void PortID::
      post (Type const&)
      {
      }

      // TaskCost
      //
      //

      void TaskCost::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void TaskCost::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void TaskCost::
      pre (Type&)
      {
      }

      void TaskCost::
      pre (Type const&)
      {
      }

      void TaskCost::
      post (Type&)
      {
      }

      void TaskCost::
      post (Type const&)
      {
      }

      // CondUtil
      //
      //

      void CondUtil::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void CondUtil::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void CondUtil::
      pre (Type&)
      {
      }

      void CondUtil::
      pre (Type const&)
      {
      }

      void CondUtil::
      post (Type&)
      {
      }

      void CondUtil::
      post (Type const&)
      {
      }

      // Probability
      //
      //

      void Probability::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void Probability::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void Probability::
      pre (Type&)
      {
      }

      void Probability::
      pre (Type const&)
      {
      }

      void Probability::
      post (Type&)
      {
      }

      void Probability::
      post (Type const&)
      {
      }

      // LinkWeight
      //
      //

      void LinkWeight::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void LinkWeight::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void LinkWeight::
      pre (Type&)
      {
      }

      void LinkWeight::
      pre (Type const&)
      {
      }

      void LinkWeight::
      post (Type&)
      {
      }

      void LinkWeight::
      post (Type const&)
      {
      }

      // MultFactor
      //
      //

      void MultFactor::
      traverse (Type& o)
      {
        pre (o);
        post (o);
      }

      void MultFactor::
      traverse (Type const& o)
      {
        pre (o);
        post (o);
      }

      void MultFactor::
      pre (Type&)
      {
      }

      void MultFactor::
      pre (Type const&)
      {
      }

      void MultFactor::
      post (Type&)
      {
      }

      void MultFactor::
      post (Type const&)
      {
      }

      // TaskNode
      //
      //

      void TaskNode::
      traverse (Type& o)
      {
        pre (o);
        nodeID (o);
        name (o);
        priorProb (o);
        attenFactor (o);
        cost (o);
        post (o);
      }

      void TaskNode::
      traverse (Type const& o)
      {
        pre (o);
        nodeID (o);
        name (o);
        priorProb (o);
        attenFactor (o);
        cost (o);
        post (o);
      }

      void TaskNode::
      pre (Type&)
      {
      }

      void TaskNode::
      pre (Type const&)
      {
      }

      void TaskNode::
      nodeID (Type& o)
      {
        dispatch (o.nodeID ());
      }

      void TaskNode::
      nodeID (Type const& o)
      {
        dispatch (o.nodeID ());
      }

      void TaskNode::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void TaskNode::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void TaskNode::
      priorProb (Type& o)
      {
        dispatch (o.priorProb ());
      }

      void TaskNode::
      priorProb (Type const& o)
      {
        dispatch (o.priorProb ());
      }

      void TaskNode::
      attenFactor (Type& o)
      {
        dispatch (o.attenFactor ());
      }

      void TaskNode::
      attenFactor (Type const& o)
      {
        dispatch (o.attenFactor ());
      }

      void TaskNode::
      cost (Type& o)
      {
        dispatch (o.cost ());
      }

      void TaskNode::
      cost (Type const& o)
      {
        dispatch (o.cost ());
      }

      void TaskNode::
      post (Type&)
      {
      }

      void TaskNode::
      post (Type const&)
      {
      }

      // CondNode
      //
      //

      void CondNode::
      traverse (Type& o)
      {
        pre (o);
        nodeID (o);
        name (o);
        probTrue (o);
        utility (o);
        kind (o);
        attenFactor (o);
        post (o);
      }

      void CondNode::
      traverse (Type const& o)
      {
        pre (o);
        nodeID (o);
        name (o);
        probTrue (o);
        utility (o);
        kind (o);
        attenFactor (o);
        post (o);
      }

      void CondNode::
      pre (Type&)
      {
      }

      void CondNode::
      pre (Type const&)
      {
      }

      void CondNode::
      nodeID (Type& o)
      {
        dispatch (o.nodeID ());
      }

      void CondNode::
      nodeID (Type const& o)
      {
        dispatch (o.nodeID ());
      }

      void CondNode::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void CondNode::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void CondNode::
      probTrue (Type& o)
      {
        dispatch (o.probTrue ());
      }

      void CondNode::
      probTrue (Type const& o)
      {
        dispatch (o.probTrue ());
      }

      void CondNode::
      utility (Type& o)
      {
        dispatch (o.utility ());
      }

      void CondNode::
      utility (Type const& o)
      {
        dispatch (o.utility ());
      }

      void CondNode::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void CondNode::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void CondNode::
      attenFactor (Type& o)
      {
        dispatch (o.attenFactor ());
      }

      void CondNode::
      attenFactor (Type const& o)
      {
        dispatch (o.attenFactor ());
      }

      void CondNode::
      post (Type&)
      {
      }

      void CondNode::
      post (Type const&)
      {
      }

      // PrecondLink
      //
      //

      void PrecondLink::
      traverse (Type& o)
      {
        pre (o);
        condID (o);
        taskID (o);
        portID (o);
        trueProb (o);
        falseProb (o);
        post (o);
      }

      void PrecondLink::
      traverse (Type const& o)
      {
        pre (o);
        condID (o);
        taskID (o);
        portID (o);
        trueProb (o);
        falseProb (o);
        post (o);
      }

      void PrecondLink::
      pre (Type&)
      {
      }

      void PrecondLink::
      pre (Type const&)
      {
      }

      void PrecondLink::
      condID (Type& o)
      {
        dispatch (o.condID ());
      }

      void PrecondLink::
      condID (Type const& o)
      {
        dispatch (o.condID ());
      }

      void PrecondLink::
      taskID (Type& o)
      {
        dispatch (o.taskID ());
      }

      void PrecondLink::
      taskID (Type const& o)
      {
        dispatch (o.taskID ());
      }

      void PrecondLink::
      portID (Type& o)
      {
        dispatch (o.portID ());
      }

      void PrecondLink::
      portID (Type const& o)
      {
        dispatch (o.portID ());
      }

      void PrecondLink::
      trueProb (Type& o)
      {
        dispatch (o.trueProb ());
      }

      void PrecondLink::
      trueProb (Type const& o)
      {
        dispatch (o.trueProb ());
      }

      void PrecondLink::
      falseProb (Type& o)
      {
        dispatch (o.falseProb ());
      }

      void PrecondLink::
      falseProb (Type const& o)
      {
        dispatch (o.falseProb ());
      }

      void PrecondLink::
      post (Type&)
      {
      }

      void PrecondLink::
      post (Type const&)
      {
      }

      // EffectLink
      //
      //

      void EffectLink::
      traverse (Type& o)
      {
        pre (o);
        taskID (o);
        condID (o);
        portID (o);
        weight (o);
        post (o);
      }

      void EffectLink::
      traverse (Type const& o)
      {
        pre (o);
        taskID (o);
        condID (o);
        portID (o);
        weight (o);
        post (o);
      }

      void EffectLink::
      pre (Type&)
      {
      }

      void EffectLink::
      pre (Type const&)
      {
      }

      void EffectLink::
      taskID (Type& o)
      {
        dispatch (o.taskID ());
      }

      void EffectLink::
      taskID (Type const& o)
      {
        dispatch (o.taskID ());
      }

      void EffectLink::
      condID (Type& o)
      {
        dispatch (o.condID ());
      }

      void EffectLink::
      condID (Type const& o)
      {
        dispatch (o.condID ());
      }

      void EffectLink::
      portID (Type& o)
      {
        dispatch (o.portID ());
      }

      void EffectLink::
      portID (Type const& o)
      {
        dispatch (o.portID ());
      }

      void EffectLink::
      weight (Type& o)
      {
        dispatch (o.weight ());
      }

      void EffectLink::
      weight (Type const& o)
      {
        dispatch (o.weight ());
      }

      void EffectLink::
      post (Type&)
      {
      }

      void EffectLink::
      post (Type const&)
      {
      }

      // Network
      //
      //

      void Network::
      traverse (Type& o)
      {
        pre (o);
        defaultAttenFactor (o);
        defaultTaskCost (o);
        defaultCondUtil (o);
        defaultCondProbTrue (o);
        linkThresh (o);
        taskNode (o);
        condNode (o);
        precondLink (o);
        effectLink (o);
        post (o);
      }

      void Network::
      traverse (Type const& o)
      {
        pre (o);
        defaultAttenFactor (o);
        defaultTaskCost (o);
        defaultCondUtil (o);
        defaultCondProbTrue (o);
        linkThresh (o);
        taskNode (o);
        condNode (o);
        precondLink (o);
        effectLink (o);
        post (o);
      }

      void Network::
      pre (Type&)
      {
      }

      void Network::
      pre (Type const&)
      {
      }

      void Network::
      defaultAttenFactor (Type& o)
      {
        dispatch (o.defaultAttenFactor ());
      }

      void Network::
      defaultAttenFactor (Type const& o)
      {
        dispatch (o.defaultAttenFactor ());
      }

      void Network::
      defaultTaskCost (Type& o)
      {
        dispatch (o.defaultTaskCost ());
      }

      void Network::
      defaultTaskCost (Type const& o)
      {
        dispatch (o.defaultTaskCost ());
      }

      void Network::
      defaultCondUtil (Type& o)
      {
        dispatch (o.defaultCondUtil ());
      }

      void Network::
      defaultCondUtil (Type const& o)
      {
        dispatch (o.defaultCondUtil ());
      }

      void Network::
      defaultCondProbTrue (Type& o)
      {
        dispatch (o.defaultCondProbTrue ());
      }

      void Network::
      defaultCondProbTrue (Type const& o)
      {
        dispatch (o.defaultCondProbTrue ());
      }

      void Network::
      linkThresh (Type& o)
      {
        dispatch (o.linkThresh ());
      }

      void Network::
      linkThresh (Type const& o)
      {
        dispatch (o.linkThresh ());
      }

      void Network::
      taskNode (Type& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::taskNode_iterator b (o.begin_taskNode()), e (o.end_taskNode());

        if (b != e)
        {
          taskNode_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) taskNode_next (o);
          }

          taskNode_post (o);
        }
      }

      void Network::
      taskNode (Type const& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::taskNode_const_iterator b (o.begin_taskNode()), e (o.end_taskNode());

        if (b != e)
        {
          taskNode_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) taskNode_next (o);
          }

          taskNode_post (o);
        }
      }

      void Network::
      taskNode_pre (Type&)
      {
      }

      void Network::
      taskNode_pre (Type const&)
      {
      }

      void Network::
      taskNode_next (Type&)
      {
      }

      void Network::
      taskNode_next (Type const&)
      {
      }

      void Network::
      taskNode_post (Type&)
      {
      }

      void Network::
      taskNode_post (Type const&)
      {
      }

      void Network::
      condNode (Type& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::condNode_iterator b (o.begin_condNode()), e (o.end_condNode());

        if (b != e)
        {
          condNode_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) condNode_next (o);
          }

          condNode_post (o);
        }
      }

      void Network::
      condNode (Type const& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::condNode_const_iterator b (o.begin_condNode()), e (o.end_condNode());

        if (b != e)
        {
          condNode_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) condNode_next (o);
          }

          condNode_post (o);
        }
      }

      void Network::
      condNode_pre (Type&)
      {
      }

      void Network::
      condNode_pre (Type const&)
      {
      }

      void Network::
      condNode_next (Type&)
      {
      }

      void Network::
      condNode_next (Type const&)
      {
      }

      void Network::
      condNode_post (Type&)
      {
      }

      void Network::
      condNode_post (Type const&)
      {
      }

      void Network::
      precondLink (Type& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::precondLink_iterator b (o.begin_precondLink()), e (o.end_precondLink());

        if (b != e)
        {
          precondLink_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) precondLink_next (o);
          }

          precondLink_post (o);
        }
      }

      void Network::
      precondLink (Type const& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::precondLink_const_iterator b (o.begin_precondLink()), e (o.end_precondLink());

        if (b != e)
        {
          precondLink_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) precondLink_next (o);
          }

          precondLink_post (o);
        }
      }

      void Network::
      precondLink_pre (Type&)
      {
      }

      void Network::
      precondLink_pre (Type const&)
      {
      }

      void Network::
      precondLink_next (Type&)
      {
      }

      void Network::
      precondLink_next (Type const&)
      {
      }

      void Network::
      precondLink_post (Type&)
      {
      }

      void Network::
      precondLink_post (Type const&)
      {
      }

      void Network::
      effectLink (Type& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::effectLink_iterator b (o.begin_effectLink()), e (o.end_effectLink());

        if (b != e)
        {
          effectLink_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) effectLink_next (o);
          }

          effectLink_post (o);
        }
      }

      void Network::
      effectLink (Type const& o)
      {
        // VC6 anathema strikes again
        //
        Network::Type::effectLink_const_iterator b (o.begin_effectLink()), e (o.end_effectLink());

        if (b != e)
        {
          effectLink_pre (o);
          for (; b != e;)
          {
            dispatch (*b);
            if (++b != e) effectLink_next (o);
          }

          effectLink_post (o);
        }
      }

      void Network::
      effectLink_pre (Type&)
      {
      }

      void Network::
      effectLink_pre (Type const&)
      {
      }

      void Network::
      effectLink_next (Type&)
      {
      }

      void Network::
      effectLink_next (Type const&)
      {
      }

      void Network::
      effectLink_post (Type&)
      {
      }

      void Network::
      effectLink_post (Type const&)
      {
      }

      void Network::
      post (Type&)
      {
      }

      void Network::
      post (Type const&)
      {
      }
    }
  }
}

namespace SANet
{
  namespace XML
  {
    namespace Writer
    {
      // NodeID
      //
      //

      NodeID::
      NodeID (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      NodeID::
      NodeID ()
      {
      }

      void NodeID::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::int_, wchar_t >::traverse (o);
        Traversal::NodeID::traverse (o);
      }

      // PortID
      //
      //

      PortID::
      PortID (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      PortID::
      PortID ()
      {
      }

      void PortID::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >::traverse (o);
        Traversal::PortID::traverse (o);
      }

      // TaskCost
      //
      //

      TaskCost::
      TaskCost (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskCost::
      TaskCost ()
      {
      }

      void TaskCost::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >::traverse (o);
        Traversal::TaskCost::traverse (o);
      }

      // CondUtil
      //
      //

      CondUtil::
      CondUtil (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      CondUtil::
      CondUtil ()
      {
      }

      void CondUtil::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >::traverse (o);
        Traversal::CondUtil::traverse (o);
      }

      // CondKind
      //
      //

      CondKind::
      CondKind (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      CondKind::
      CondKind ()
      {
      }

      void CondKind::
      traverse (Type const& o)
      {
        ::std::basic_string< wchar_t > s;

        if (o == ::SANet::XML::CondKind::ENVIRON) s = L"ENVIRON";
        else if (o == ::SANet::XML::CondKind::SYSTEM) s = L"SYSTEM";
        else if (o == ::SANet::XML::CondKind::DATA) s = L"DATA";
        else 
        {
        }

        if (::XSCRT::XML::Attribute< wchar_t >* a = attr_ ())
        {
          a->value (s);
        }

        else
        {
          top_().value (s);
        }
      }

      // Probability
      //
      //

      Probability::
      Probability (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      Probability::
      Probability ()
      {
      }

      void Probability::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >::traverse (o);
        Traversal::Probability::traverse (o);
      }

      // LinkWeight
      //
      //

      LinkWeight::
      LinkWeight (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      LinkWeight::
      LinkWeight ()
      {
      }

      void LinkWeight::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >::traverse (o);
        Traversal::LinkWeight::traverse (o);
      }

      // MultFactor
      //
      //

      MultFactor::
      MultFactor (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      MultFactor::
      MultFactor ()
      {
      }

      void MultFactor::
      traverse (Type const& o)
      {
        ::XMLSchema::Writer::FundamentalType< ::XMLSchema::double_, wchar_t >::traverse (o);
        Traversal::MultFactor::traverse (o);
      }

      // TaskNode
      //
      //

      TaskNode::
      TaskNode (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      TaskNode::
      TaskNode ()
      {
      }

      void TaskNode::
      traverse (Type const& o)
      {
        Traversal::TaskNode::traverse (o);
      }

      void TaskNode::
      nodeID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"nodeID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::TaskNode::nodeID (o);
        pop_ ();
      }

      void TaskNode::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"name", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::TaskNode::name (o);
        pop_ ();
      }

      void TaskNode::
      priorProb (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"priorProb", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::TaskNode::priorProb (o);
        pop_ ();
      }

      void TaskNode::
      attenFactor (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"attenFactor", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::TaskNode::attenFactor (o);
        pop_ ();
      }

      void TaskNode::
      cost (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"cost", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::TaskNode::cost (o);
        pop_ ();
      }

      // CondNode
      //
      //

      CondNode::
      CondNode (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      CondNode::
      CondNode ()
      {
      }

      void CondNode::
      traverse (Type const& o)
      {
        Traversal::CondNode::traverse (o);
      }

      void CondNode::
      nodeID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"nodeID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::CondNode::nodeID (o);
        pop_ ();
      }

      void CondNode::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"name", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::CondNode::name (o);
        pop_ ();
      }

      void CondNode::
      probTrue (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"probTrue", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::CondNode::probTrue (o);
        pop_ ();
      }

      void CondNode::
      utility (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"utility", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::CondNode::utility (o);
        pop_ ();
      }

      void CondNode::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"kind", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::CondNode::kind (o);
        pop_ ();
      }

      void CondNode::
      attenFactor (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"attenFactor", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::CondNode::attenFactor (o);
        pop_ ();
      }

      // PrecondLink
      //
      //

      PrecondLink::
      PrecondLink (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      PrecondLink::
      PrecondLink ()
      {
      }

      void PrecondLink::
      traverse (Type const& o)
      {
        Traversal::PrecondLink::traverse (o);
      }

      void PrecondLink::
      condID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"condID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::PrecondLink::condID (o);
        pop_ ();
      }

      void PrecondLink::
      taskID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"taskID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::PrecondLink::taskID (o);
        pop_ ();
      }

      void PrecondLink::
      portID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"portID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::PrecondLink::portID (o);
        pop_ ();
      }

      void PrecondLink::
      trueProb (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"trueProb", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::PrecondLink::trueProb (o);
        pop_ ();
      }

      void PrecondLink::
      falseProb (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"falseProb", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::PrecondLink::falseProb (o);
        pop_ ();
      }

      // EffectLink
      //
      //

      EffectLink::
      EffectLink (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      EffectLink::
      EffectLink ()
      {
      }

      void EffectLink::
      traverse (Type const& o)
      {
        Traversal::EffectLink::traverse (o);
      }

      void EffectLink::
      taskID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"taskID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::EffectLink::taskID (o);
        pop_ ();
      }

      void EffectLink::
      condID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"condID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::EffectLink::condID (o);
        pop_ ();
      }

      void EffectLink::
      portID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"portID", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::EffectLink::portID (o);
        pop_ ();
      }

      void EffectLink::
      weight (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"weight", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::EffectLink::weight (o);
        pop_ ();
      }

      // Network
      //
      //

      Network::
      Network (::XSCRT::XML::Element< wchar_t >& e)
      : ::XSCRT::Writer< wchar_t > (e)
      {
      }

      Network::
      Network ()
      {
      }

      void Network::
      traverse (Type const& o)
      {
        Traversal::Network::traverse (o);
      }

      void Network::
      defaultAttenFactor (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"defaultAttenFactor", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::Network::defaultAttenFactor (o);
        pop_ ();
      }

      void Network::
      defaultTaskCost (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"defaultTaskCost", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::Network::defaultTaskCost (o);
        pop_ ();
      }

      void Network::
      defaultCondUtil (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"defaultCondUtil", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::Network::defaultCondUtil (o);
        pop_ ();
      }

      void Network::
      defaultCondProbTrue (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"defaultCondProbTrue", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::Network::defaultCondProbTrue (o);
        pop_ ();
      }

      void Network::
      linkThresh (Type const& o)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"linkThresh", L"http://www.vanderbilt.edu/SANet", top_ ()));
        Traversal::Network::linkThresh (o);
        pop_ ();
      }

      void Network::
      taskNode_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"taskNode", L"http://www.vanderbilt.edu/SANet", top_ ()));
      }

      void Network::
      taskNode_next (Type const& o)
      {
        taskNode_post (o);
        taskNode_pre (o);
      }

      void Network::
      taskNode_post (Type const&)
      {
        pop_ ();
      }

      void Network::
      condNode_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"condNode", L"http://www.vanderbilt.edu/SANet", top_ ()));
      }

      void Network::
      condNode_next (Type const& o)
      {
        condNode_post (o);
        condNode_pre (o);
      }

      void Network::
      condNode_post (Type const&)
      {
        pop_ ();
      }

      void Network::
      precondLink_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"precondLink", L"http://www.vanderbilt.edu/SANet", top_ ()));
      }

      void Network::
      precondLink_next (Type const& o)
      {
        precondLink_post (o);
        precondLink_pre (o);
      }

      void Network::
      precondLink_post (Type const&)
      {
        pop_ ();
      }

      void Network::
      effectLink_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< wchar_t > (L"effectLink", L"http://www.vanderbilt.edu/SANet", top_ ()));
      }

      void Network::
      effectLink_next (Type const& o)
      {
        effectLink_post (o);
        effectLink_pre (o);
      }

      void Network::
      effectLink_post (Type const&)
      {
        pop_ ();
      }
    }
  }
}

namespace SANet
{
  namespace XML
  {
    void
    network (::SANet::XML::Network const& s, xercesc::DOMDocument* d)
    {
      ::XSCRT::XML::Element< wchar_t > e (d->getDocumentElement ());
      if (e.name () != L"network")
      {
        throw 1;
      }

      struct W : virtual ::SANet::XML::Writer::Network,
      virtual ::SANet::XML::Writer::MultFactor,
      virtual ::SANet::XML::Writer::TaskCost,
      virtual ::SANet::XML::Writer::CondUtil,
      virtual ::SANet::XML::Writer::Probability,
      virtual ::SANet::XML::Writer::LinkWeight,
      virtual ::SANet::XML::Writer::TaskNode,
      virtual ::SANet::XML::Writer::NodeID,
      virtual ::XMLSchema::Writer::FundamentalType< ::XMLSchema::string< wchar_t >, wchar_t >,
      virtual ::SANet::XML::Writer::CondNode,
      virtual ::SANet::XML::Writer::CondKind,
      virtual ::SANet::XML::Writer::PrecondLink,
      virtual ::SANet::XML::Writer::PortID,
      virtual ::SANet::XML::Writer::EffectLink,
      virtual ::XSCRT::Writer< wchar_t >
      {
        W (::XSCRT::XML::Element< wchar_t >& e)
        : ::XSCRT::Writer< wchar_t > (e)
        {
        }
      };

      W w (e);
      w.dispatch (s);
    }
  }
}

