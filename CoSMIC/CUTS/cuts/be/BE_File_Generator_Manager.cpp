// $Id$

#include "BE_File_Generator_Manager.h"

#if !defined (__CUTS_INLINE__)
#include "BE_File_Generator_Manager.inl"
#endif

#include "BE_File_Generator.h"
#include "Uml.h"
#include "boost/bind.hpp"
#include <algorithm>
#include <sstream>

//=============================================================================
/**
 * @struct Sort_By_Position
 *
 * @brief Help functor to sort objects by their position. The object
 * sorted by this method are in left-to-right ordering. Their y-value
 * is currently ignored when sorting objects.
 */
//=============================================================================

template <typename T>
struct Sort_By_Position
{
  struct Position
  {
    /// X-value of the position.
    long x;

    /// Y-value of the position
    long y;

    /**
     * Extraction operator. This method takes a string and extracts
     * the coordinates from it. The format of the string should be
     * (x, y). If the source string does not have coordinates stored
     * in this format, it will cause unpredictable behavior.
     *
     * @param[in]     str     Source string with the x-,y-coordinates
     */
    void operator <<= (const std::string & str)
    {
      char tmp;

      std::istringstream istr (str);

      istr >> tmp;
      istr >> this->x;
      istr >> tmp;
      istr >> this->y;
    }
  };

  /**
   * Functor operation. This method will compare the determine
   * which object is the furthest most left object of the two.
   */
  bool operator () (const T & lhs, const T & rhs)
  {
    Position pos_lhs;
    pos_lhs <<= lhs.position ();

    Position pos_rhs;
    pos_rhs <<= rhs.position ();

    return pos_lhs.x < pos_rhs.x;
  }
};

//=============================================================================
//=============================================================================

//
// CUTS_BE_File_Generator_Manager
//
CUTS_BE_File_Generator_Manager::
CUTS_BE_File_Generator_Manager (void)
: ignore_effects_ (false),
  depth_ (0)
{

}

//
// CUTS_BE_File_Generator_Manager
//
CUTS_BE_File_Generator_Manager::
~CUTS_BE_File_Generator_Manager (void)
{
  this->remove_all ();
}

//
// Visit_ComponentImplementationContainer
//
void CUTS_BE_File_Generator_Manager::
Visit_ComponentImplementationContainer (
  const PICML::ComponentImplementationContainer & container)
{
  // Reset all the information in this object.
  this->clear ();

  typedef std::vector <PICML::MonolithicImplementation> Monolithic_Set;
  Monolithic_Set monos = container.MonolithicImplementation_kind_children ();

  // Open all the files for writing if we know we have at least
  // one <MonolithicImplementation> in this <container>.
  if (monos.empty ())
    return;
  
  // Create the pathname for the file.
  std::ostringstream pathname;
  pathname << this->outdir_ << "\\" << container.name ();

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::open,
                              _1,
                              pathname.str ()));

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_preamble,
                              _1,
                              container));

  // Visit all the monolithic implementations.
  std::for_each (monos.begin (),
                 monos.end (),
                 boost::bind (&Monolithic_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // Be sure to close all the open files.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_postamble,
                              _1,
                              container));

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::close,
                              _1));
}

//
// Visit_MonolithicImplementation
//
void CUTS_BE_File_Generator_Manager::
Visit_MonolithicImplementation (
  const PICML::MonolithicImplementation & monolithic)
{
  // Get the <Implements> connection b/t the <MonolithicImplementation>
  // and the <ComponentRef>.
  PICML::Implements implements = monolithic.dstImplements ();

  if (implements == Udm::null)
    return;

  // Get the <Component> that is being implemented.
  PICML::ComponentRef ref = implements.dstImplements_end ();
  PICML::Component component = PICML::Component::Cast (ref.ref ());

  if (component == Udm::null)
    return;

  // Preprocess the <component> and begin writing the monolithic
  // implemenation for the target <component>.
  component.Accept (this->ppd_);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_monolithic_begin,
                              _1,
                              monolithic));

  // We are now visting the component portion of the monolithic
  // implemenation.
  component.Accept (*this);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_monolithic_end,
                              _1));
}

//
// Visit_Component
//
void CUTS_BE_File_Generator_Manager::
Visit_Component (const PICML::Component & component)
{
  // Begin writing the component.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_component_begin,
                              _1,
                              component));

  // @@ event sinks
  typedef std::vector <PICML::InEventPort> EventSink_Set;
  EventSink_Set sinks = component.InEventPort_kind_children ();

  std::for_each (sinks.begin (),
                 sinks.end (),
                 boost::bind (&EventSink_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // @@ facets
  typedef std::vector <PICML::ProvidedRequestPort> Facet_Set;
  Facet_Set facets = component.ProvidedRequestPort_kind_children ();

  std::for_each (facets.begin (),
                 facets.end (),
                 boost::bind (&Facet_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // @@ periodic actions
  typedef std::vector <PICML::PeriodicAction> Periodic_Set;
  Periodic_Set periodics = component.PeriodicAction_kind_children ();

  std::for_each (periodics.begin (),
                 periodics.end (),
                 boost::bind (&Periodic_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // @@ variables
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_variables_begin,
                              _1));

  typedef std::vector <PICML::Variable> Variable_Set;
  Variable_Set variables = component.Variable_children ();

  std::for_each (variables.begin (),
                 variables.end (),
                 boost::bind (&Variable_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  typedef std::vector <PICML::WorkerType> WorkerType_Set;
  WorkerType_Set worker_types = component.WorkerType_children ();

  std::for_each (worker_types.begin (),
                 worker_types.end (),
                 boost::bind (&WorkerType_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_variables_end,
                              _1));

  // Finish writing the component.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_component_end,
                              _1,
                              component));

  // Locate the factory for the component.
  PICML::ComponentFactory factory = this->get_component_factory (component);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_component_factory_begin,
                              _1,
                              factory,
                              component));
}

//
// Visit_InEventPort
//
void CUTS_BE_File_Generator_Manager::
Visit_InEventPort (const PICML::InEventPort & sink)
{
  typedef
    void (CUTS_BE_File_Generator::*BE_WRITE_METHOD)
    (const PICML::InEventPort &);

  // Signal the start of writing a <InEventPort>.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (static_cast <BE_WRITE_METHOD> (
                              &CUTS_BE_File_Generator::write_method_begin),
                              _1,
                              sink));

  // Visit the input action for this port.
  PICML::Input input = sink.dstInput ();
  if (input != Udm::null)
    input.Accept (*this);

  // Signal the end of writing a <InEventPort>.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_method_end,
                              _1));
}

//
// Visit_PeriodicAction
//
void CUTS_BE_File_Generator_Manager::
Visit_PeriodicAction (const PICML::PeriodicAction & periodic)
{
  typedef
    void (CUTS_BE_File_Generator::*BE_WRITE_METHOD)
    (const PICML::PeriodicAction &);

  // Signal the start of writing a <PeriodicAction>.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (static_cast <BE_WRITE_METHOD> (
                              &CUTS_BE_File_Generator::write_method_begin),
                              _1,
                              periodic));

  PICML::Input input = periodic.srcInput ();
  if (input != Udm::null)
    input.Accept (*this);

  // Signal the end of writing a <PeriodicAction>.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_method_end,
                              _1));
}

//
// Visit_ProvidedRequestPort
//
void CUTS_BE_File_Generator_Manager::
Visit_ProvidedRequestPort (const PICML::ProvidedRequestPort & facet)
{
  typedef
    void (CUTS_BE_File_Generator::*BE_WRITE_METHOD)
    (const PICML::ProvidedRequestPort &);

  // Signal the start of writing a <InEventPort>.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (static_cast <BE_WRITE_METHOD> (
                              &CUTS_BE_File_Generator::write_method_begin),
                              _1,
                              facet));

  // Signal the end of writing a <InEventPort>.
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_method_end,
                              _1));
}

//
// Visit_Input
//
void CUTS_BE_File_Generator_Manager::Visit_Input (
  const PICML::Input & input)
{
  PICML::InputAction action = PICML::InputAction::Cast (input.dstInput_end ());
  action.Accept (*this);
}

//
// Visit_InputAction
//
void CUTS_BE_File_Generator_Manager::Visit_InputAction (
  const PICML::InputAction & action)
{
  // Insert the action on the call stack.
  this->call_stack_.push (action);

  // Visit all the properties for this input action.
  typedef std::vector <PICML::Property> Property_Set;
  Property_Set props = action.Property_children ();

  std::for_each (props.begin (),
                 props.end (),
                 boost::bind (&Property_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // Visit the effect.
  PICML::Effect effect = action.dstInputEffect ();

  if (effect != Udm::null)
    effect.Accept (*this);
}

//
// Visit_Effect
//
void CUTS_BE_File_Generator_Manager::Visit_Effect (
  const PICML::Effect & effect)
{
  // Write the postcondition for this <effect>.
  std::string postcondition = effect.Postcondition ();

  if (!postcondition.empty ())
  {
    std::for_each (this->generators_.begin (),
                   this->generators_.end (),
                   boost::bind (&CUTS_BE_File_Generator::write_postcondition,
                                _1,
                                postcondition));
  }

  // Visit the next state in the chain.
  PICML::State state = effect.dstInputEffect_end ();
  state.Accept (*this);
}

//
// Visit_State
//
void CUTS_BE_File_Generator_Manager::Visit_State (
  const PICML::State & state)
{
  if (!this->ignore_effects_)
  {
    // Determine if this state is a finishing state for conditional
    // flows. If the state has more than one source, then we can
    // assume that we are ending one or more conditional flows.
    typedef std::set <PICML::Effect> Effect_Set;
    Effect_Set effects = state.srcInputEffect ();

    if (effects.size () > 1)
    {
      if (this->holding_state_.empty ())
        this->holding_state_.push (state);
      else if (this->holding_state_.top () != state)
        this->holding_state_.push (state);

      return;
    }
  }
  else
    this->ignore_effects_ = false;

  // Check for a finishing transition from this state.
  PICML::Finish finish_conn = state.dstFinish ();

  if (finish_conn != Udm::null)
  {
    // Get the name of the input port.
    PICML::InputAction finish =
      PICML::InputAction::Cast (finish_conn.dstFinish_end ());

    if (finish == this->call_stack_.top ())
    {
      // Remove the action from the top of the call stack.
      this->call_stack_.pop ();
      return;
    }
  }

  // Get all the transitions from this state. If there is more than
  // one transition, then we need to prepare generating flow control
  // in execution path.
  typedef std::set <PICML::Transition> Transition_Set;
  Transition_Set transitions = state.dstInternalPrecondition ();

  size_t transition_count = transitions.size ();

  if (transition_count > 1)
    ++ this->depth_;

  std::for_each (transitions.begin (),
                 transitions.end (),
                 boost::bind (&Transition_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // Now that we have visited all the transitions from the
  // state, we can jump to the state were all the branching
  // transitions from this state merge.
  if (transition_count > 1)
  {
    if (this->depth_ -- == this->holding_state_.size ())
    {
      // Get the topmost holding state and jump to it. We need to make
      // sure we tell the holding state to ignore any effect connections
      // since it will cause it to return.
      PICML::State jmp_state = this->holding_state_.top ();

      this->holding_state_.pop ();
      this->ignore_effects_ = true;

      jmp_state.Accept (*this);
    }
  }
}

//
// Visit_Transition
//
void CUTS_BE_File_Generator_Manager::
Visit_Transition (const PICML::Transition & transition)
{
  std::string precondition = transition.Precondition ();

  if (!precondition.empty ())
  {
    std::for_each (this->generators_.begin (),
                   this->generators_.end (),
                   boost::bind (&CUTS_BE_File_Generator::write_precondition,
                                _1,
                                precondition));
  }

  // Get the action connected to the end of the transaction.
  PICML::ActionBase action =
    PICML::ActionBase::Cast (transition.dstInternalPrecondition_end ());
  std::string _typename = action.type ().name ();

  // We only continue at this point if the action is an instance
  // of a predefined action, or it has more than 1 repetition
  // specified.
  if (action.Repetitions () > 0)
  {
    if (_typename == (std::string)PICML::Action::meta.name ())
    {
      PICML::Action::Cast (action).Accept (*this);
    }
    else if (_typename == (std::string) PICML::CompositeAction::meta.name ())
    {
      PICML::CompositeAction::Cast (action).Accept (*this);
    }
    else if (_typename == (std::string) PICML::OutputAction::meta.name ())
    {
      PICML::OutputAction::Cast (action).Accept (*this);
    }
  }

  // Continue down the chain.
  PICML::Effect effect = action.dstInputEffect ();
  effect.Accept (*this);
}

//
// Visit_Property
//
void CUTS_BE_File_Generator_Manager::
Visit_Property (const PICML::Property & property)
{
  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_action_property,
                              _1,
                              property));
}

//
// Visit_Action
//
void CUTS_BE_File_Generator_Manager::
Visit_Action (const PICML::Action & action)
{
  // Get action type and it's worker parent. This will help all
  // the generators since they will not have to do this themselves.
  PICML::Action action_type = action;

  while (PICML::Action (action_type.Archetype ()) != Udm::null)
    action_type = action_type.Archetype ();

  PICML::Worker worker = action_type.Worker_parent ();

  // Let's tell the generator to begin generating an action.
  typedef
    void (CUTS_BE_File_Generator::*BE_METHOD)
    (const PICML::Worker &, const PICML::Action &);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (static_cast <BE_METHOD> (
                              &CUTS_BE_File_Generator::write_action_begin),
                              _1,
                              worker,
                              action));

  // Generate the parameters for the action.
  typedef std::set <PICML::Property,
                    Sort_By_Position <PICML::Property> > Property_Set;

  Property_Set properties =
    action.Property_kind_children_sorted (Sort_By_Position <PICML::Property> ());

  std::for_each (properties.begin (),
                 properties.end (),
                 boost::bind (&Property_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // Let's tell the generator to end generating an action.
  typedef
    void (CUTS_BE_File_Generator::*BE_METHOD)
    (const PICML::Worker &, const PICML::Action &);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_action_end,
                              _1));
}

//
// Visit_OutputAction
//
void CUTS_BE_File_Generator_Manager::
Visit_OutputAction (const PICML::OutputAction & action)
{
  typedef
    void (CUTS_BE_File_Generator::*BE_METHOD)
    (const PICML::OutputAction &);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (
                    static_cast <BE_METHOD> (
                    &CUTS_BE_File_Generator::write_action_begin),
                    _1,
                    action));


  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (&CUTS_BE_File_Generator::write_action_end,
                              _1));
}

//
// Visit_CompositeAction
//
void CUTS_BE_File_Generator_Manager::
Visit_CompositeAction (const PICML::CompositeAction & ca)
{
  PICML::InputAction ia = ca.InputAction_child ();

  if (ia != Udm::null)
    ia.Accept (*this);
}

//
// Visit_Variable
//
void CUTS_BE_File_Generator_Manager::
Visit_Variable (const PICML::Variable & variable)
{
  typedef
    void (CUTS_BE_File_Generator::
    * BE_METHOD) (const PICML::Variable &);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (static_cast <BE_METHOD> (
                              &CUTS_BE_File_Generator::write_variable),
                              _1,
                              variable));
}

//
// Visit_WorkerType
//
void CUTS_BE_File_Generator_Manager::
Visit_WorkerType (const PICML::WorkerType & worker_type)
{
  typedef
    void (CUTS_BE_File_Generator::* BE_METHOD)
    (const PICML::WorkerType &);

  std::for_each (this->generators_.begin (),
                 this->generators_.end (),
                 boost::bind (static_cast <BE_METHOD> (
                              &CUTS_BE_File_Generator::write_variable),
                              _1,
                              worker_type));

}

//
// remove_all
//
void CUTS_BE_File_Generator_Manager::remove_all (void)
{
  for (File_Generator_Set::iterator iter = this->generators_.begin ();
       iter != this->generators_.end ();
       iter ++)
  {
    delete (*iter);
  }

  this->generators_.clear ();
}

//
// clear
//
void CUTS_BE_File_Generator_Manager::clear (void)
{
  this->ignore_effects_ = false;
  this->depth_ = 0;

  this->ppd_.clear ();

  while (!this->holding_state_.empty ())
    this->holding_state_.pop ();

  while (!this->call_stack_.empty ())
    this->call_stack_.pop ();
}

//
// insert
//
void CUTS_BE_File_Generator_Manager::
insert (CUTS_BE_File_Generator * generator)
{
  if (generator == 0)
    return;

  generator->init (&this->ppd_);
  this->generators_.insert (generator);
}

//
// get_component_factory
//
PICML::ComponentFactory CUTS_BE_File_Generator_Manager::
get_component_factory (const PICML::Component & component)
{
  // Try and locate the real factory for the component.
  typedef std::set <PICML::ManagesComponent> Manages_Set;
  Manages_Set manages = component.srcManagesComponent ();

  if (manages.size () > 0)
  {
    PICML::ManagesComponent manage = (*manages.begin ());
    return manage.srcManagesComponent_end ();
  }

  typedef std::set <PICML::ComponentRef> ComponentRef_Set;
  ComponentRef_Set refs = component.referedbyComponentRef ();

  PICML::MgaObject parent;
  std::string parent_type;

  for (ComponentRef_Set::iterator iter = refs.begin ();
       iter != refs.end ();
       iter ++)
  {
    parent = iter->parent ();
    parent_type = parent.type ().name ();

    if (parent_type == (std::string) PICML::File::meta.name () ||
        parent_type == (std::string) PICML::Package::meta.name ())
    {
      PICML::ComponentFactory factory =
        this->get_component_factory (iter->ref ());

      if (factory != Udm::null)
        return factory;
    }
  }

  return PICML::ComponentFactory ();
}
