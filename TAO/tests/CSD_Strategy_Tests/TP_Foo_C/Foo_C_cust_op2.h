//=============================================================================
/**
 *  @file    Foo_C_cust_op2.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_C_CUST_OP2_H
#define FOO_C_CUST_OP2_H

#include "CSD_TP_Foo_C_Export.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

class Foo_C_i;

class Foo_C_cust_op2;
typedef TAO_Intrusive_Ref_Count_Handle<Foo_C_cust_op2> Foo_C_cust_op2_Handle;

class CSD_TP_Foo_C_Export Foo_C_cust_op2 : public TAO::CSD::TP_Custom_Request_Operation
{
public:
  Foo_C_cust_op2(Foo_C_i* servant, long arg);
  virtual ~Foo_C_cust_op2();

  void result();

protected:
  virtual void execute_i();
  virtual void cancel_i();

private:
  long arg_;
  bool cancelled_;
  Foo_C_i* servant_;
};

#endif
