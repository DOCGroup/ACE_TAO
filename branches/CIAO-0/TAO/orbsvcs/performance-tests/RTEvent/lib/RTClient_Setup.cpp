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

ACE_RCSID(TAO_PERF_RTEC, RTClient_Setup, "$Id$")

RTClient_Setup::RTClient_Setup (int use_rt_corba,
                                CORBA::ORB_ptr orb,
                                const RT_Class &rt_class,
                                int nthreads
                                ACE_ENV_ARG_DECL)
  : use_rt_corba_ (use_rt_corba)
  , syncscope_setup_ (orb ACE_ENV_ARG_PARAMETER)
{
  ACE_CHECK;

  if (use_rt_corba)
    {
      this->rtcorba_setup_ =
        auto_ptr<RTCORBA_Setup> (new RTCORBA_Setup (orb,
                                                    rt_class,
                                                    nthreads
                                                    ACE_ENV_ARG_PARAMETER));
      ACE_CHECK;

#if 0
      this->priorityband_setup_ =
        auto_ptr<PriorityBand_Setup> (new PriorityBand_Setup (orb,
                                                              *this->rtcorba_setup_
                                                              ACE_ENV_ARG_PARAMETER));
      ACE_CHECK;
#endif /* 0 */
    }
}
