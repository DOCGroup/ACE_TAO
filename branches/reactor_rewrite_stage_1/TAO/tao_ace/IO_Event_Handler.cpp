#include "IO_Event_Handler.h"


ACE_RCSID(tao_ace, IO_Event_Handler, "$Id$")


int
TAO_ACE_IO_Event_Handler::handle_event (TAO_ACE_IO_Event_Masks::Event_Masks t)
{
  ACE_TRACE ("TAO_ACE_IO_Event_Handler::handle_event");

  switch (t)
    {
    case TAO_ACE_IO_Event_Masks::CONNECT_MASK:
    case TAO_ACE_IO_Event_Masks::WRITE_MASK:
      return this->handle_output (this->get_handle ());

    case TAO_ACE_IO_Event_Masks::ACCEPT_MASK:
    case TAO_ACE_IO_Event_Masks::READ_MASK:
      return this->handle_input (this->get_handle ());

    case TAO_ACE_IO_Event_Masks::EXCEPT_MASK:
      return this->handle_exception (this->get_handle ());
    default:
      return 0;
    }

  // default, if it is a NULL mask or anything else
  return 0;
}
