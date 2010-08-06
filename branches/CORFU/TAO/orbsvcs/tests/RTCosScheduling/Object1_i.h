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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Object1_i
 *
 * @brief This class is used to test the RTCORBA 1.0 Scheduling Service
 *
 */
class Object1_impl : public POA_testSched::Object1
{
  public:
    Object1_impl();

    virtual ~Object1_impl();

    virtual void method1(const char *activity,
                         CORBA::Long seconds,
                         char *&output
                         );
};

#endif /* Object1_I_H  */
