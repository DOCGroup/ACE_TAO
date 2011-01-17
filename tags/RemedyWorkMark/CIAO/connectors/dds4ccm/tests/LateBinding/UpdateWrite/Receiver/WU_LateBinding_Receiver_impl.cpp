// $Id$
#include "WU_LateBinding_Receiver_impl.h"

WU_LateBinding_Receiver_impl::WU_LateBinding_Receiver_impl ()
{
}

WU_LateBinding_Receiver_impl::~WU_LateBinding_Receiver_impl ()
{
}

void
WU_LateBinding_Receiver_impl::start (
  ::WU_LateBinding::CCM_Receiver_Context_ptr ctx)
{
  ::CCM_DDS::DataListenerControl_var dlc =
    ctx->get_connection_info_out_event_data_control ();
  dlc->mode (::CCM_DDS::ONE_BY_ONE);

  dlc = ctx->get_connection_info_out_state_data_control ();
  dlc->mode (::CCM_DDS::ONE_BY_ONE);
}

void
WU_LateBinding_Receiver_impl::on_one_data (const ::WU_LateBindingTest & datum)
{
  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("WU_LateBinding_Receiver_impl::on_one_data - ")
          ACE_TEXT ("received data for <%C> at %u\n"),
          datum.key.in (),
          datum.iteration));
}

void
WU_LateBinding_Receiver_impl::on_many_data (void)
{
    ACE_ERROR ((LM_ERROR, "WU_LateBinding_Receiver_impl::on_many_data : "
                          "Error: No callback to this method expected.\n"));
}

void
WU_LateBinding_Receiver_impl::on_creation (const ::WU_LateBindingTest & datum)
{
  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("WU_LateBinding_Receiver_impl::on_creation - ")
          ACE_TEXT ("Data created for <%C> at %u\n"),
          datum.key.in (),
          datum.iteration));
}

void
WU_LateBinding_Receiver_impl::on_one_update (const ::WU_LateBindingTest & datum)
{
  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("WU_LateBinding_Receiver_impl::on_one_update - ")
          ACE_TEXT ("received updated data for <%C> at %u\n"),
          datum.key.in (),
          datum.iteration));
}

