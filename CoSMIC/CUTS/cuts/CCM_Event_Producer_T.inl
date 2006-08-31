// $Id$

//
// context
//
template <typename CONTEXT>
CUTS_INLINE
void CUTS_CCM_Event_Producer_T <CONTEXT>::context (CONTEXT * context)
{
  this->context_ = context;
}

//
// activate
//
template <typename CONTEXT>
CUTS_INLINE
void CUTS_CCM_Event_Producer_T <CONTEXT>::activate (long owner)
{
  this->owner_ = owner;
  this->active_ = true;
}

//
// deactivate
//
template <typename CONTEXT>
CUTS_INLINE
void CUTS_CCM_Event_Producer_T <CONTEXT>::deactivate (void)
{
  this->active_ = false;
}
