/**
 * @file RTServer_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "RTServer_Setup.h"
#include "RIR_Narrow.h"
#include "tao/RTCORBA/RTCORBA.h"

#include "ace/Log_Msg.h"

#if !defined(__ACE_INLINE__)
#include "RTServer_Setup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, RTServer_Setup, "$Id$")

RTServer_Setup::RTServer_Setup (int use_rt_corba,
                                CORBA::ORB_ptr orb,
                                const RT_Class &rt_class
                                ACE_ENV_ARG_DECL)
  : syncscope_setup_ (orb
                      ACE_ENV_ARG_PARAMETER)
{
  ACE_TRY
    {
      ACE_CHECK;

      if (use_rt_corba)
        {
          this->rtcorba_setup_ =
            auto_ptr<RTCORBA_Setup> (new RTCORBA_Setup (orb,
                                                        rt_class
                                                        ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;

          this->rtpoa_setup_ =
            auto_ptr<RTPOA_Setup> (new RTPOA_Setup (orb,
                                                    *this->rtcorba_setup_
                                                    ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;

          this->priorityband_setup_ =
            auto_ptr<PriorityBand_Setup> (new PriorityBand_Setup (orb,
                                                                  *this->rtcorba_setup_
                                                                  ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;
        }
    } ACE_CATCHANY
      {
        // TODO
      }
  ACE_ENDTRY;
}
