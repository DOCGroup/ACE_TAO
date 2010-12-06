/**
 * @file RTClient_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "RTClient_Setup.h"
#include "RIR_Narrow.h"
#include "tao/RTCORBA/RTCORBA.h"

#include "ace/Log_Msg.h"

#if !defined(__ACE_INLINE__)
#include "RTClient_Setup.inl"
#endif /* __ACE_INLINE__ */

RTClient_Setup::RTClient_Setup (int use_rt_corba,
                                CORBA::ORB_ptr orb,
                                const RT_Class &rt_class,
                                int nthreads)
  : use_rt_corba_ (use_rt_corba)
  , syncscope_setup_ (orb)
{

  if (use_rt_corba)
    {
      ACE_auto_ptr_reset (this->rtcorba_setup_,
                          new RTCORBA_Setup (orb,
                                             rt_class,
                                             nthreads));

#if 0
      ACE_auto_ptr_reset (this->priorityband_setup_,
                          new PriorityBand_Setup (orb,
                                                  *this->rtcorba_setup_));

#endif /* 0 */
    }
}
