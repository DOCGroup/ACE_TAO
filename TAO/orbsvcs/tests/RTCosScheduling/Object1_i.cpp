/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Object1_i.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#include "Object1_i.h"
#include "ace/ACE.h"
//#include "tao/RTCORBA/RTCORBA.h"
////##include "ace/Task.h"
//
//#if !defined (ACE_LACKS_PRAGMA_ONCE)
//# pragma once
//#endif /* ACE_LACKS_PRAGMA_ONCE */
//
////#include "ace/CORBA_macros.h"
////#include "tao/RTCORBA/Priority_Mapping_Manager.h"
////#include "tao/corba.h"
////#include "tao/ORB_Core.h"
//

Object1_impl::Object1_impl()
  throw (CORBA::SystemException)
{
}


Object1_impl::~Object1_impl()
  throw (CORBA::SystemException)
{
}


void Object1_impl::method1(const char* activity,
    CORBA::Long seconds,
    char* &output)
  throw (testSched::testSchedException)
{
  const int scale_factor = 2000;
  int work;
  const int time_size = 35;
  ACE_TCHAR date_and_time[time_size];
  char buf[128];
  char buf2[128];

  ACE_OS::sprintf(buf,
                  "%s\t%s\tBeginning work on the server\n",
                   ACE::timestamp(date_and_time, time_size),
                   activity);
  ACE_DEBUG((LM_DEBUG,"%s",buf));



  if (strcmp(activity,"Client1") == 0)
    {
       ACE_OS::sleep(5);
    }

  /// Simulate some work
  static CORBA::ULong prime_number = 9619;
  work = scale_factor * seconds;
  for (; work != 0; work--)
    {
      ACE::is_prime (prime_number, 2, prime_number / 2);
    }

  ACE_OS::sprintf(buf2,
                  "%s\t%s\tFinished work on the server\n",
                  ACE::timestamp(date_and_time, time_size),
                  activity);
  ACE_DEBUG((LM_DEBUG,"%s",buf2));
  ACE_OS_String::strcat(buf,buf2);
  output = ACE_OS_String::strdup(buf);

}
