// -*- C++ -*-
//=============================================================================
/**
 *  @file   FtRtEvent_Test.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FTRTEVENT_TEST_H
#define FTRTEVENT_TEST_H

#include "orbsvcs/RtecEventChannelAdminC.h"

namespace TAO_FTRTEC {
  class FTEC_Gateway;
}

struct Options
{
  ACE_Time_Value timer_interval;
  int num_iterations;
  ACE_CString proxy_consumer_file;
};

class FtRtEvent_Test_Base
{
public:
  FtRtEvent_Test_Base();
  virtual ~FtRtEvent_Test_Base();

  int run(int argc, ACE_TCHAR** argv);
protected:
  CORBA::ORB_var orb_;
  RtecEventChannelAdmin::EventChannel_var channel_;
  TAO_FTRTEC::FTEC_Gateway* gateway_;
  int use_gateway_;
  Options options_;
private:
  int parse_args(int argc, ACE_TCHAR** argv 
                 ACE_ENV_ARG_DECL);
  RtecEventChannelAdmin::EventChannel_ptr
    get_event_channel(ACE_ENV_SINGLE_ARG_DECL);

  virtual int init(ACE_ENV_SINGLE_ARG_DECL)=0;
};

template <class T>
class FtRtEvent_Test : public FtRtEvent_Test_Base
{
  virtual int init();
  T client_;
};

template <class T>
int FtRtEvent_Test<T>::init(ACE_ENV_SINGLE_ARG_DECL)
{
  return client_.init(orb_.in(), channel_.in(), options_ 
                      ACE_ENV_ARG_PARAMETER);
}

#endif
