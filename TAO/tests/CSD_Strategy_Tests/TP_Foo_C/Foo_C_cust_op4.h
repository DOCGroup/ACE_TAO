//=============================================================================
/**
 *  @file    Foo_C_cust_op4.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_C_CUST_OP4_H
#define FOO_C_CUST_OP4_H

#include "CSD_TP_Foo_C_Export.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

class Foo_C_i;

class Foo_C_cust_op4;
using Foo_C_cust_op4_Handle = TAO_Intrusive_Ref_Count_Handle<Foo_C_cust_op4>;

class CSD_TP_Foo_C_Export Foo_C_cust_op4 : public TAO::CSD::TP_Custom_Request_Operation
{
public:
  Foo_C_cust_op4(Foo_C_i* servant, long arg);
  ~Foo_C_cust_op4() override;

protected:
  void execute_i() override;
  void cancel_i() override;

  Foo_C_cust_op4 (Foo_C_cust_op4 const &) = delete;
  Foo_C_cust_op4 (Foo_C_cust_op4 &&) = delete;
  Foo_C_cust_op4 & operator= (Foo_C_cust_op4 const &) = delete;
  Foo_C_cust_op4 & operator= (Foo_C_cust_op4 &&) = delete;

private:
  long arg_;
  Foo_C_i* servant_;
};

#endif
