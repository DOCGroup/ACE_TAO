/* $Id$ */
#include "EC_Event_Limit.h"
#include "ace/CORBA_macros.h"
#include "tao/ORB_Core.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
//#include "ec_dsui_config.h"
//#include "ec_dsui_families.h"
#endif //ACE_HAS_DSUI

#ifdef ACE_HAS_DSUI
EC_Event_Limit::EC_Event_Limit (TAO_ORB_Core* orb_ptr, ds_control* ds_ptr)
  : m_orb_ptr (orb_ptr),
    m_ds_ptr (ds_ptr)
{}
#else
EC_Event_Limit::EC_Event_Limit (TAO_ORB_Core* orb_ptr)
  : m_orb_ptr (orb_ptr)
{}
#endif //ACE_HAS_DSUI

int EC_Event_Limit::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG((LM_DEBUG, "*************** STOP! *************\n"));
#ifdef ACE_HAS_DSUI
      //DSUI_EVENT_LOG (TIMEOUT_FAM, TIMEOUT_EVENT, 0, 0, NULL);
      delete m_ds_ptr;
      m_ds_ptr = 0;
#endif //ACE_HAS_DSUI
      /*
        ACE_Reactor* rx_ptr = m_orb_ptr->reactor();
        rx_ptr->cancel_timer(this);
        m_orb_ptr->shutdown (false
                          ACE_ENV_ARG_PARAMETER);
      */
      exit(0);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
 return 0;
}
