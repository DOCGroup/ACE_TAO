// $Id$

//
// CUTS_Event_Guard_T
//
template <typename COMPONENT>
CUTS_Event_Guard_T <COMPONENT>::CUTS_Event_Guard_T (Component_Type * parent,
                                                    Method_Pointer method)
: parent_ (parent),
  method_ (method)
{

}

//
// CUTS_Event_Guard_T
//
template <typename COMPONENT>
CUTS_Event_Guard_T <COMPONENT>::~CUTS_Event_Guard_T (void)
{

}

//
// try_execute_and_reset
//
template <typename COMPONENT>
void CUTS_Event_Guard_T <COMPONENT>::try_execute_and_reset (void)
{
  // Verify the guard is enabled before continuing.
  if (CUTS_Event_Guard::enabled ())
  {
    // Reset the guard so we can continue collecting events.
    CUTS_Event_Guard::reset ();

    // Process the workload guarded by these events.
    (this->parent_->*this->method_) ();
  }
}

//
// try_execute_and_reset
//
template <typename COMPONENT>
void CUTS_Event_Guard_T <COMPONENT>::try_execute (void)
{
  // Verify the guard is enabled before continuing.
  if (CUTS_Event_Guard::enabled ())
  {
    // Process the workload guarded by these events.
    (this->parent_->*this->method_) ();
  }
}
