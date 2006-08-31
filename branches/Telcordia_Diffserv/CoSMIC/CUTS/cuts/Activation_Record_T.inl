// $Id$

//
// perform_action_no_logging
//
template <typename ACTION>
CUTS_INLINE
void CUTS_Activation_Record::perform_action_no_logging (
  const ACTION & action)
{
  action.execute ();
}
