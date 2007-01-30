// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Foo_C_Custom_Proxy.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_C_CUSTOM_PROXY_H
#define FOO_C_CUSTOM_PROXY_H

#include "CSD_TP_Foo_C_Export.h"
#include "ClientEngine.h"
#include "Foo_C_i.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"


class CSD_TP_Foo_C_Export Foo_C_Custom_Proxy
{
public:

  Foo_C_Custom_Proxy(Foo_C_i* servant,
                     Foo_C_ptr objref,
                       TAO::CSD::TP_Strategy* strategy);
  ~Foo_C_Custom_Proxy();

  bool validate_connection ();

  void op1(void);
  void op2(CORBA::Long x);
  CORBA::Long op3(CORBA::Long x);
  void op4(CORBA::Long x);
  void op5(void);
  void done(void);

  void cust_op1(void);
  void cust_op2(long x);
  long cust_op3(long x);
  void cust_op4(long x);
  void cust_op5(void);

private:
  Foo_C_i*                        servant_;
  Foo_C_var                       objref_;
  TAO::CSD::TP_Strategy_Handle    strategy_;
};

#endif
