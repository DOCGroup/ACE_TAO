// $Id$

#include "CCM_Component_Registry.h"
#include "ace/Guard_T.h"

#include  <typeinfo>

namespace CUTS
{
  //===========================================================================
  //
  // CCM_Component_Registry_Node
  //
  //===========================================================================

  //
  // CCM_Component_Registry_Node
  //
  CCM_Component_Registry_Node::CCM_Component_Registry_Node (void)
    : benchmark_agent_ (::CUTS::Benchmark_Agent::_nil ())
  {

  }

  //
  // CCM_Component_Registry_Node
  //
  CCM_Component_Registry_Node::CCM_Component_Registry_Node (
    ::CUTS::Benchmark_Agent_ptr agent)
    : benchmark_agent_ (::CUTS::Benchmark_Agent::_duplicate (agent))
  {

  }

  //
  // ~CCM_Component_Registry_Node
  //
  CCM_Component_Registry_Node::~CCM_Component_Registry_Node (void)
  {
    if (!::CORBA::is_nil (this->benchmark_agent_))
    {
      try
      {
        ::CORBA::release (this->benchmark_agent_);
      }
      catch (...)
      {

      }

      this->benchmark_agent_ = ::CUTS::Benchmark_Agent::_nil ();
    }
  }

  //
  // benchmark_agent
  //
  ::CUTS::Benchmark_Agent_ptr
    CCM_Component_Registry_Node::benchmark_agent (void)
  {
    return this->benchmark_agent_;
  }

  //
  // benchmark_agent
  //
  void CCM_Component_Registry_Node::benchmark_agent (
    ::CUTS::Benchmark_Agent_ptr agent)
  {
    if (!::CORBA::is_nil (this->benchmark_agent_))
    {
      try
      {
        ::CORBA::release (this->benchmark_agent_);
      }
      catch (...)
      {

      }
    }

    this->benchmark_agent_ = agent;
  }

  //
  // reset
  //
  void CCM_Component_Registry_Node::reset (void)
  {
    if (!::CORBA::is_nil (this->benchmark_agent_))
    {
      try
      {
        ::CORBA::release (this->benchmark_agent_);
      }
      catch (...)
      {

      }
    }

    this->benchmark_agent_ = ::CUTS::Benchmark_Agent::_nil ();
  }

  //===========================================================================
  //
  // CCM_Component_Registry
  //
  //===========================================================================

  //
  // CCM_Component_Registry
  //
  CCM_Component_Registry::CCM_Component_Registry (void)
  {

  }

  //
  // ~CCM_Component_Registry
  //
  CCM_Component_Registry::~CCM_Component_Registry (void)
  {

  }

  //
  // get_registration
  //
  long CCM_Component_Registry::get_registration (
    const char * uuid, bool auto_register)
  {
    // Use the base class to get the <registration> for the
    // <uuid> without registering it since we are using a
    // different <node> type.
    long regid = CUTS_Component_Registry::get_registration (uuid, false);

    if (regid == 0 && auto_register)
    {
      ACE_DEBUG ((
        LM_DEBUG,
        "CCM component registration not found; registering component\n"));

      regid = CCM_Component_Registry::register_component (uuid,
                ::CUTS::Benchmark_Agent::_nil ());
    }

    if (regid == 0)
    {
      regid =
        this->register_component (uuid, ::CUTS::Benchmark_Agent::_nil ());
    }

    return regid;
  }

  //
  // register_component
  //
  long CCM_Component_Registry::register_component (
    const char * uuid,
    ::CUTS::Benchmark_Agent_ptr agent)
  {
    // Determine if the component has already been registered.
    CCM_Component_Registry_Node * node = get_node (uuid);

    if (node == 0)
    {
      // Create a new <CCM_Component_Registry_Node>.
      node = new CCM_Component_Registry_Node (agent);
      ACE_Auto_Ptr <CCM_Component_Registry_Node> auto_clean (node);

      // Insert the node into the <registry_>.
      std::pair <
        CUTS_Component_Registry_Map::iterator, bool> result =
        this->registry_.insert (
        CUTS_Component_Registry_Map::value_type (
        uuid, node));

      if (result.second)
      {
        // Since we were able to insert the <node> into the
        // <registry_> then we can unmanage the resource.
        // Otherwise, we let the <auto_clean> continue to
        // manage the resource, which will handle the reference
        // counting for the <agent> as well.
        auto_clean.release ();
      }
    }

    return reinterpret_cast <long> (node);
  }

  //
  // unregister_component
  //
  bool CCM_Component_Registry::unregister_component (
    const char * uuid,
    ::CUTS::Benchmark_Agent_ptr agent)
  {
    CCM_Component_Registry_Node * node = get_node (uuid);

    // Verify we have a <node> and the <node> contains
    // the same <agent>.
    if (node == 0 ||
        !agent->_is_equivalent (node->benchmark_agent ()))
    {
      return false;
    }

    // Just reset the <node> but do not remove it. We need to
    // maintain persistance as long as the registry is active.
    node->reset ();
    return true;
  }

  //
  // get_node
  //
  CCM_Component_Registry_Node *
  CCM_Component_Registry::get_node (const char * uuid)
  {
    CCM_Component_Registry_Node * node = 0;

    ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex,
                           guard,
                           this->lock_,
                           node);

    // Locate the <uuid> in the <registry_> and save the
    // <node> if we can find the <uuid>.
    CUTS_Component_Registry_Map::iterator iter = this->registry_.find (uuid);

    if (iter != this->registry_.end ())
    {
      try
      {
        node = dynamic_cast <CCM_Component_Registry_Node *> (iter->second);
      }
      catch (std::bad_cast &)
      {

      }
    }

    return node;
  }

  //
  // get_benchmark_agents
  //
  size_t CCM_Component_Registry::get_benchmark_agents (
    Benchmark_Agent_Handler * handler, size_t & count)
  {
    ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex,
         guard,
         this->lock_,
         0);

    // Store the size of the <registry_>.
    size_t agents = count = this->registry_.size ();

    CUTS_Component_Registry_Map ::iterator iter;

    for (iter = this->registry_.begin ();
         iter != this->registry_.end ();
         iter ++)
    {
      try
      {
        // Cast the pointer to a <CCM_Component_Registry_Node>.
        CCM_Component_Registry_Node * node =
          dynamic_cast <CCM_Component_Registry_Node *> (iter->second);

        // Pass the benchmark agent to the handler.
        handler->handle_agent (iter->first.c_str (),
                               node->benchmark_agent ());
      }
      catch (std::bad_cast &)
      {
        // Since we did not handle this agent we can decrement the
        // counter.
        count --;
      }
    }

    return agents;
  }

  //
  // reset_registration
  //
  void CCM_Component_Registry::reset_registration (const char * uuid)
  {
    // Get the node with the registration. If the node is valid then
    // reset it. This will set the agent value to NIL.
    CCM_Component_Registry_Node * node = get_node (uuid);

    if (node)
    {
      node->reset ();
    }
  }
}
