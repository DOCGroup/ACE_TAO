// $Id$

#include "test_i.h"

#if defined(ACE_HAS_QT)

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(FL_Cube, test_i, "$Id$")

LCD_Display_imp::LCD_Display_imp (CORBA::ORB_ptr orb)

  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
LCD_Display_imp::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "The ORB has been shutdown, Close the windows to exit \n"));
  this->orb_->shutdown (0);
  this->orb_->destroy ();
}

void
LCD_Display_imp::send_val (CORBA::Long val
                           TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "The value from server is <%d> \n", val));
  emit set_value (val);
}
#endif /* ACE_HAS_QT */
