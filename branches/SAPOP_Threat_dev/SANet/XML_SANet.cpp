#include "XML_SANet.hpp"

namespace SANet
{
  namespace XML
  {
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

  // TaskNode
  // 

  TaskNode::
  TaskNode (::XMLSchema::int_ const& nodeID__,
            ::XMLSchema::string< wchar_t > const& name__,
            ::XMLSchema::double_ const& priorProb__,
            ::XMLSchema::double_ const& attenFactor__,
            ::XMLSchema::double_ const& cost__)
  : 
  ::XSCRT::Type (), 
  nodeID_ (new ::XMLSchema::int_ (nodeID__)),
  name_ (new ::XMLSchema::string< wchar_t > (name__)),
  priorProb_ (new ::XMLSchema::double_ (priorProb__)),
  attenFactor_ (new ::XMLSchema::double_ (attenFactor__)),
  cost_ (new ::XMLSchema::double_ (cost__)),
  regulator__ ()
  {
    nodeID_->container (this);
    name_->container (this);
    priorProb_->container (this);
    attenFactor_->container (this);
    cost_->container (this);
  }

  TaskNode::
  TaskNode (TaskNode const& s)
  :
  ::XSCRT::Type (),
  nodeID_ (new ::XMLSchema::int_ (*s.nodeID_)),
  name_ (new ::XMLSchema::string< wchar_t > (*s.name_)),
  priorProb_ (new ::XMLSchema::double_ (*s.priorProb_)),
  attenFactor_ (new ::XMLSchema::double_ (*s.attenFactor_)),
  cost_ (new ::XMLSchema::double_ (*s.cost_)),
  regulator__ ()
  {
    nodeID_->container (this);
    name_->container (this);
    priorProb_->container (this);
    attenFactor_->container (this);
    cost_->container (this);
  }

  TaskNode& TaskNode::
  operator= (TaskNode const& s)
  {
    nodeID (*s.nodeID_);

    name (*s.name_);

    priorProb (*s.priorProb_);

    attenFactor (*s.attenFactor_);

    cost (*s.cost_);

    return *this;
  }


  // TaskNode
  // 
  ::XMLSchema::int_ const& TaskNode::
  nodeID () const
  {
    return *nodeID_;
  }

  void TaskNode::
  nodeID (::XMLSchema::int_ const& e)
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
  ::XMLSchema::double_ const& TaskNode::
  priorProb () const
  {
    return *priorProb_;
  }

  void TaskNode::
  priorProb (::XMLSchema::double_ const& e)
  {
    *priorProb_ = e;
  }

  // TaskNode
  // 
  ::XMLSchema::double_ const& TaskNode::
  attenFactor () const
  {
    return *attenFactor_;
  }

  void TaskNode::
  attenFactor (::XMLSchema::double_ const& e)
  {
    *attenFactor_ = e;
  }

  // TaskNode
  // 
  ::XMLSchema::double_ const& TaskNode::
  cost () const
  {
    return *cost_;
  }

  void TaskNode::
  cost (::XMLSchema::double_ const& e)
  {
    *cost_ = e;
  }


  // CondNode
  // 

  CondNode::
  CondNode (::XMLSchema::int_ const& nodeID__,
            ::XMLSchema::string< wchar_t > const& name__,
            ::XMLSchema::double_ const& probTrue__,
            ::XMLSchema::double_ const& utility__,
            ::SANet::XML::CondKind const& kind__,
            ::XMLSchema::double_ const& attenFactor__)
  : 
  nodeID_ (new ::XMLSchema::int_ (nodeID__)),
  name_ (new ::XMLSchema::string< wchar_t > (name__)),
  probTrue_ (new ::XMLSchema::double_ (probTrue__)),
  utility_ (new ::XMLSchema::double_ (utility__)),
  kind_ (new ::SANet::XML::CondKind (kind__)),
  attenFactor_ (new ::XMLSchema::double_ (attenFactor__)),
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
  CondNode (CondNode const& s)
  :
  ::XSCRT::Type (),
  nodeID_ (new ::XMLSchema::int_ (*s.nodeID_)),
  name_ (new ::XMLSchema::string< wchar_t > (*s.name_)),
  probTrue_ (new ::XMLSchema::double_ (*s.probTrue_)),
  utility_ (new ::XMLSchema::double_ (*s.utility_)),
  kind_ (new ::SANet::XML::CondKind (*s.kind_)),
  attenFactor_ (new ::XMLSchema::double_ (*s.attenFactor_)),
  regulator__ ()
  {
    nodeID_->container (this);
    name_->container (this);
    probTrue_->container (this);
    utility_->container (this);
    kind_->container (this);
    attenFactor_->container (this);
  }

  CondNode& CondNode::
  operator= (CondNode const& s)
  {
    nodeID (*s.nodeID_);

    name (*s.name_);

    probTrue (*s.probTrue_);

    utility (*s.utility_);

    kind (*s.kind_);

    attenFactor (*s.attenFactor_);

    return *this;
  }


  // CondNode
  // 
  ::XMLSchema::int_ const& CondNode::
  nodeID () const
  {
    return *nodeID_;
  }

  void CondNode::
  nodeID (::XMLSchema::int_ const& e)
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
  ::XMLSchema::double_ const& CondNode::
  probTrue () const
  {
    return *probTrue_;
  }

  void CondNode::
  probTrue (::XMLSchema::double_ const& e)
  {
    *probTrue_ = e;
  }

  // CondNode
  // 
  ::XMLSchema::double_ const& CondNode::
  utility () const
  {
    return *utility_;
  }

  void CondNode::
  utility (::XMLSchema::double_ const& e)
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
  ::XMLSchema::double_ const& CondNode::
  attenFactor () const
  {
    return *attenFactor_;
  }

  void CondNode::
  attenFactor (::XMLSchema::double_ const& e)
  {
    *attenFactor_ = e;
  }


  // PrecondLink
  // 

  PrecondLink::
  PrecondLink (::XMLSchema::int_ const& condID__,
               ::XMLSchema::int_ const& taskID__,
               ::XMLSchema::string< wchar_t > const& portID__,
               ::XMLSchema::double_ const& trueProb__,
               ::XMLSchema::double_ const& falseProb__)
  : 
  condID_ (new ::XMLSchema::int_ (condID__)),
  taskID_ (new ::XMLSchema::int_ (taskID__)),
  portID_ (new ::XMLSchema::string< wchar_t > (portID__)),
  trueProb_ (new ::XMLSchema::double_ (trueProb__)),
  falseProb_ (new ::XMLSchema::double_ (falseProb__)),
  regulator__ ()
  {
    condID_->container (this);
    taskID_->container (this);
    portID_->container (this);
    trueProb_->container (this);
    falseProb_->container (this);
  }

  PrecondLink::
  PrecondLink (PrecondLink const& s)
  :
  ::XSCRT::Type (),
  condID_ (new ::XMLSchema::int_ (*s.condID_)),
  taskID_ (new ::XMLSchema::int_ (*s.taskID_)),
  portID_ (new ::XMLSchema::string< wchar_t > (*s.portID_)),
  trueProb_ (new ::XMLSchema::double_ (*s.trueProb_)),
  falseProb_ (new ::XMLSchema::double_ (*s.falseProb_)),
  regulator__ ()
  {
    condID_->container (this);
    taskID_->container (this);
    portID_->container (this);
    trueProb_->container (this);
    falseProb_->container (this);
  }

  PrecondLink& PrecondLink::
  operator= (PrecondLink const& s)
  {
    condID (*s.condID_);

    taskID (*s.taskID_);

    portID (*s.portID_);

    trueProb (*s.trueProb_);

    falseProb (*s.falseProb_);

    return *this;
  }


  // PrecondLink
  // 
  ::XMLSchema::int_ const& PrecondLink::
  condID () const
  {
    return *condID_;
  }

  void PrecondLink::
  condID (::XMLSchema::int_ const& e)
  {
    *condID_ = e;
  }

  // PrecondLink
  // 
  ::XMLSchema::int_ const& PrecondLink::
  taskID () const
  {
    return *taskID_;
  }

  void PrecondLink::
  taskID (::XMLSchema::int_ const& e)
  {
    *taskID_ = e;
  }

  // PrecondLink
  // 
  ::XMLSchema::string< wchar_t > const& PrecondLink::
  portID () const
  {
    return *portID_;
  }

  void PrecondLink::
  portID (::XMLSchema::string< wchar_t > const& e)
  {
    *portID_ = e;
  }

  // PrecondLink
  // 
  ::XMLSchema::double_ const& PrecondLink::
  trueProb () const
  {
    return *trueProb_;
  }

  void PrecondLink::
  trueProb (::XMLSchema::double_ const& e)
  {
    *trueProb_ = e;
  }

  // PrecondLink
  // 
  ::XMLSchema::double_ const& PrecondLink::
  falseProb () const
  {
    return *falseProb_;
  }

  void PrecondLink::
  falseProb (::XMLSchema::double_ const& e)
  {
    *falseProb_ = e;
  }


  // EffectLink
  // 

  EffectLink::
  EffectLink (::XMLSchema::int_ const& taskID__,
              ::XMLSchema::int_ const& condID__,
              ::XMLSchema::string< wchar_t > const& portID__,
              ::XMLSchema::double_ const& weight__)
  : 
  taskID_ (new ::XMLSchema::int_ (taskID__)),
  condID_ (new ::XMLSchema::int_ (condID__)),
  portID_ (new ::XMLSchema::string< wchar_t > (portID__)),
  weight_ (new ::XMLSchema::double_ (weight__)),
  regulator__ ()
  {
    taskID_->container (this);
    condID_->container (this);
    portID_->container (this);
    weight_->container (this);
  }

  EffectLink::
  EffectLink (EffectLink const& s)
  :
  ::XSCRT::Type (),
  taskID_ (new ::XMLSchema::int_ (*s.taskID_)),
  condID_ (new ::XMLSchema::int_ (*s.condID_)),
  portID_ (new ::XMLSchema::string< wchar_t > (*s.portID_)),
  weight_ (new ::XMLSchema::double_ (*s.weight_)),
  regulator__ ()
  {
    taskID_->container (this);
    condID_->container (this);
    portID_->container (this);
    weight_->container (this);
  }

  EffectLink& EffectLink::
  operator= (EffectLink const& s)
  {
    taskID (*s.taskID_);

    condID (*s.condID_);

    portID (*s.portID_);

    weight (*s.weight_);

    return *this;
  }


  // EffectLink
  // 
  ::XMLSchema::int_ const& EffectLink::
  taskID () const
  {
    return *taskID_;
  }

  void EffectLink::
  taskID (::XMLSchema::int_ const& e)
  {
    *taskID_ = e;
  }

  // EffectLink
  // 
  ::XMLSchema::int_ const& EffectLink::
  condID () const
  {
    return *condID_;
  }

  void EffectLink::
  condID (::XMLSchema::int_ const& e)
  {
    *condID_ = e;
  }

  // EffectLink
  // 
  ::XMLSchema::string< wchar_t > const& EffectLink::
  portID () const
  {
    return *portID_;
  }

  void EffectLink::
  portID (::XMLSchema::string< wchar_t > const& e)
  {
    *portID_ = e;
  }

  // EffectLink
  // 
  ::XMLSchema::double_ const& EffectLink::
  weight () const
  {
    return *weight_;
  }

  void EffectLink::
  weight (::XMLSchema::double_ const& e)
  {
    *weight_ = e;
  }


  // Network
  // 

  Network::
  Network (::XMLSchema::double_ const& defaultAttenFactor__,
           ::XMLSchema::double_ const& defaultTaskCost__,
           ::XMLSchema::double_ const& defaultCondUtil__,
           ::XMLSchema::double_ const& defaultCondProbTrue__,
           ::XMLSchema::double_ const& linkThresh__,
           ::std::list< ::SANet::XML::TaskNode > const& taskNode__,
           ::std::list< ::SANet::XML::CondNode > const& condNode__,
           ::std::list< ::SANet::XML::PrecondLink > const& precondLink__,
           ::std::list< ::SANet::XML::EffectLink > const& effectLink__)
  : 
  defaultAttenFactor_ (new ::XMLSchema::double_ (defaultAttenFactor__)),
  defaultTaskCost_ (new ::XMLSchema::double_ (defaultTaskCost__)),
  defaultCondUtil_ (new ::XMLSchema::double_ (defaultCondUtil__)),
  defaultCondProbTrue_ (new ::XMLSchema::double_ (defaultCondProbTrue__)),
  linkThresh_ (new ::XMLSchema::double_ (linkThresh__)),
  taskNode_ (taskNode__),
  condNode_ (condNode__),
  precondLink_ (precondLink__),
  effectLink_ (effectLink__),
  regulator__ ()
  {
    defaultAttenFactor_->container (this);
    defaultTaskCost_->container (this);
    defaultCondUtil_->container (this);
    defaultCondProbTrue_->container (this);
    linkThresh_->container (this);
  }

  Network::
  Network (Network const& s)
  :
  ::XSCRT::Type (),
  defaultAttenFactor_ (new ::XMLSchema::double_ (*s.defaultAttenFactor_)),
  defaultTaskCost_ (new ::XMLSchema::double_ (*s.defaultTaskCost_)),
  defaultCondUtil_ (new ::XMLSchema::double_ (*s.defaultCondUtil_)),
  defaultCondProbTrue_ (new ::XMLSchema::double_ (*s.defaultCondProbTrue_)),
  linkThresh_ (new ::XMLSchema::double_ (*s.linkThresh_)),
  taskNode_ (s.taskNode_),
  condNode_ (s.condNode_),
  precondLink_ (s.precondLink_),
  effectLink_ (s.effectLink_),
  regulator__ ()
  {
    defaultAttenFactor_->container (this);
    defaultTaskCost_->container (this);
    defaultCondUtil_->container (this);
    defaultCondProbTrue_->container (this);
    linkThresh_->container (this);
  }

  Network& Network::
  operator= (Network const& s)
  {
    defaultAttenFactor (*s.defaultAttenFactor_);

    defaultTaskCost (*s.defaultTaskCost_);

    defaultCondUtil (*s.defaultCondUtil_);

    defaultCondProbTrue (*s.defaultCondProbTrue_);

    linkThresh (*s.linkThresh_);

    taskNode_ = s.taskNode_;

    condNode_ = s.condNode_;

    precondLink_ = s.precondLink_;

    effectLink_ = s.effectLink_;

    return *this;
  }


  // Network
  // 
  ::XMLSchema::double_ const& Network::
  defaultAttenFactor () const
  {
    return *defaultAttenFactor_;
  }

  void Network::
  defaultAttenFactor (::XMLSchema::double_ const& e)
  {
    *defaultAttenFactor_ = e;
  }

  // Network
  // 
  ::XMLSchema::double_ const& Network::
  defaultTaskCost () const
  {
    return *defaultTaskCost_;
  }

  void Network::
  defaultTaskCost (::XMLSchema::double_ const& e)
  {
    *defaultTaskCost_ = e;
  }

  // Network
  // 
  ::XMLSchema::double_ const& Network::
  defaultCondUtil () const
  {
    return *defaultCondUtil_;
  }

  void Network::
  defaultCondUtil (::XMLSchema::double_ const& e)
  {
    *defaultCondUtil_ = e;
  }

  // Network
  // 
  ::XMLSchema::double_ const& Network::
  defaultCondProbTrue () const
  {
    return *defaultCondProbTrue_;
  }

  void Network::
  defaultCondProbTrue (::XMLSchema::double_ const& e)
  {
    *defaultCondProbTrue_ = e;
  }

  // Network
  // 
  ::XMLSchema::double_ const& Network::
  linkThresh () const
  {
    return *linkThresh_;
  }

  void Network::
  linkThresh (::XMLSchema::double_ const& e)
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
  // CondKind
  //

  CondKind::CondKind (::XSCRT::XML::Element< wchar_t > const& e)
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

  // TaskNode
  //

  TaskNode::
  TaskNode (::XSCRT::XML::Element< wchar_t > const& e)
  :Base (e), regulator__ ()
  {

    ::XSCRT::Parser< wchar_t > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
      ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == L"nodeID")
      {
        nodeID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        nodeID_->container (this);
      }

      else if (n == L"name")
      {
        name_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
        name_->container (this);
      }

      else if (n == L"priorProb")
      {
        priorProb_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        priorProb_->container (this);
      }

      else if (n == L"attenFactor")
      {
        attenFactor_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        attenFactor_->container (this);
      }

      else if (n == L"cost")
      {
        cost_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
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
  :Base (e), regulator__ ()
  {

    ::XSCRT::Parser< wchar_t > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
      ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == L"nodeID")
      {
        nodeID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        nodeID_->container (this);
      }

      else if (n == L"name")
      {
        name_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
        name_->container (this);
      }

      else if (n == L"probTrue")
      {
        probTrue_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        probTrue_->container (this);
      }

      else if (n == L"utility")
      {
        utility_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        utility_->container (this);
      }

      else if (n == L"kind")
      {
        kind_ = ::std::auto_ptr< ::SANet::XML::CondKind > (new ::SANet::XML::CondKind (e));
        kind_->container (this);
      }

      else if (n == L"attenFactor")
      {
        attenFactor_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
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
  :Base (e), regulator__ ()
  {

    ::XSCRT::Parser< wchar_t > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
      ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == L"condID")
      {
        condID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        condID_->container (this);
      }

      else if (n == L"taskID")
      {
        taskID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        taskID_->container (this);
      }

      else if (n == L"portID")
      {
        portID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
        portID_->container (this);
      }

      else if (n == L"trueProb")
      {
        trueProb_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        trueProb_->container (this);
      }

      else if (n == L"falseProb")
      {
        falseProb_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
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
  :Base (e), regulator__ ()
  {

    ::XSCRT::Parser< wchar_t > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
      ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == L"taskID")
      {
        taskID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        taskID_->container (this);
      }

      else if (n == L"condID")
      {
        condID_ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        condID_->container (this);
      }

      else if (n == L"portID")
      {
        portID_ = ::std::auto_ptr< ::XMLSchema::string< wchar_t > > (new ::XMLSchema::string< wchar_t > (e));
        portID_->container (this);
      }

      else if (n == L"weight")
      {
        weight_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
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
  :Base (e), regulator__ ()
  {

    ::XSCRT::Parser< wchar_t > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< wchar_t > e (p.next_element ());
      ::std::basic_string< wchar_t > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == L"defaultAttenFactor")
      {
        defaultAttenFactor_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        defaultAttenFactor_->container (this);
      }

      else if (n == L"defaultTaskCost")
      {
        defaultTaskCost_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        defaultTaskCost_->container (this);
      }

      else if (n == L"defaultCondUtil")
      {
        defaultCondUtil_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        defaultCondUtil_->container (this);
      }

      else if (n == L"defaultCondProbTrue")
      {
        defaultCondProbTrue_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        defaultCondProbTrue_->container (this);
      }

      else if (n == L"linkThresh")
      {
        linkThresh_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
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
  namespace reader
  {
    ::SANet::XML::Network
    network (xercesc::DOMDocument const* d)
    {
      ::XSCRT::XML::Element< wchar_t > e (d->getDocumentElement ());
      if (e.name () == L"network")
      {
        ::SANet::XML::Network r (e);
        return r;
      }

      else
      {
        throw 1;
      }
    }
  }
  }
}

