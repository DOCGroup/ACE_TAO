/* $Id$ */
#include "EC_Event_Limit.h"
#include "ace/CORBA_macros.h"
#include "tao/ORB_Core.h"

#include <dsui.h>
//#include "ec_dsui_config.h"
//#include "ec_dsui_families.h"

EC_Event_Limit::EC_Event_Limit (TAO_ORB_Core* orb_ptr, ds_control* ds_ptr)
  : m_orb_ptr (orb_ptr),
    m_ds_ptr (ds_ptr)
{}

int EC_Event_Limit::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_TRY_NEW_ENV
    {
      //DSUI_EVENT_LOG (TIMEOUT_FAM, TIMEOUT_EVENT, 0, 0, NULL);
      ACE_DEBUG((LM_DEBUG, "*************** STOP! *************\n"));
      /*
	ACE_Reactor* rx_ptr = m_orb_ptr->reactor();
	rx_ptr->cancel_timer(this);
	m_orb_ptr->shutdown (false
                          ACE_ENV_ARG_PARAMETER);
      */
      delete m_ds_ptr;
      exit(0);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
 return 0;
}
