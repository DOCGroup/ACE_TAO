/* $Id$ */
#include "EC_Event_Limit.h"
#include "ace/CORBA_macros.h"
#include "tao/ORB.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "ec_dsui_config.h"
#include "ec_dsui_families.h"
#endif //ACE_HAS_DSUI

#ifdef ACE_HAS_DSUI
EC_Event_Limit::EC_Event_Limit (CORBA::ORB_var orb, ds_control* ds_ptr)
  : m_orb_ (orb),
    m_ds_ptr (ds_ptr)
{}
#else
EC_Event_Limit::EC_Event_Limit (CORBA::ORB_var orb)

  : m_orb_ (orb)
{}
#endif //ACE_HAS_DSUI

int EC_Event_Limit::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG((LM_DEBUG, "*************** STOP! *************\n"));

#ifdef ACE_HAS_DSUI
      DSTRM_EVENT (TIMEOUT_FAM, PROGRAM_STOP, 0, 0, NULL);
      delete m_ds_ptr;
      m_ds_ptr = 0;
#endif //ACE_HAS_DSUI
      /*
      //@BT
      //DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
      ACE_Time_Value now(ACE_OS::gettimeofday());
      ACE_OS::printf("EC_Event_Limit (%P|%t) STOP at %isec %iusec\n",now.sec(),now.usec());
      DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
      */

      exit(0);
      ACE_TRY_CHECK;
      /*
      //Attempt to exit cleanly
      m_orb_->shutdown(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      */
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
 return 0;
}
