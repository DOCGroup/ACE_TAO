// $Id$

#include "test_i.h"

LCD_Display_imp::LCD_Display_imp (CORBA::ORB_ptr orb)

  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
LCD_Display_imp::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "The ORB has been shutdown, Close the windows to exit\n"));
  this->orb_->shutdown (0);
  this->orb_->destroy ();
}

void
LCD_Display_imp::send_val (CORBA::Long val)
{
  ACE_DEBUG ((LM_DEBUG,
              "The value from server is <%d>\n", val));
  emit set_value (val);
}

