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
                                const RT_Class &rt_class,
                                int nthreads
                                ACE_ENV_ARG_DECL)
  : RTClient_Setup (use_rt_corba,
                    orb,
                    rt_class,
                    nthreads
                    ACE_ENV_ARG_DECL)
{
  ACE_CHECK;

  if (use_rt_corba)
    {
      ACE_AUTO_PTR_RESET (this->rtpoa_setup_,
                          new RTPOA_Setup (orb,
                                           *this->rtcorba_setup ()
                                           ACE_ENV_ARG_PARAMETER),
                          RTPOA_Setup
                         );
      ACE_CHECK;

      this->poa_ =
        this->rtpoa_setup_->poa ();
    }
  else
    {
      this->poa_ =
        RIR_Narrow<RTPortableServer::POA>::resolve (orb,
                                                    "RootPOA"
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}
