/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Object1_i.h
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#ifndef Object1_I_H
#define Object1_I_H

#include "testSchedS.h"
//#include "tao/RTCORBA/RTCORBA.h"
//##include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#include "ace/CORBA_macros.h"
//#include "tao/RTCORBA/Priority_Mapping_Manager.h"
//#include "tao/corba.h"
//#include "tao/ORB_Core.h"



/**
 * @class Object1_i
 *
 * @brief This class is used to test the RTCORBA 1.0 Scheduling Service
 *
 */


class Object1_impl : public POA_testSched::Object1,
                            PortableServer::RefCountServantBase
{
  public:
    Object1_impl()
      throw (CORBA::SystemException);

    virtual ~Object1_impl()
      throw (CORBA::SystemException);

    virtual void method1(const char *activity,
                         CORBA::Long seconds,
                         char *&output)
      throw (testSched::testSchedException);
};

#endif /* Object1_I_H  */
