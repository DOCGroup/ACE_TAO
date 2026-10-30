//=============================================================================
/**
 *  @file    Foo_C_cust_op1.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_C_CUST_OP1_H
#define FOO_C_CUST_OP1_H

#include "CSD_TP_Foo_C_Export.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

class Foo_C_i;

class Foo_C_cust_op1;
using Foo_C_cust_op1_Handle = TAO_Intrusive_Ref_Count_Handle<Foo_C_cust_op1>;

class CSD_TP_Foo_C_Export Foo_C_cust_op1 : public TAO::CSD::TP_Custom_Request_Operation
{
public:
  explicit Foo_C_cust_op1(Foo_C_i* servant);
  ~Foo_C_cust_op1() override;

  void result();

protected:
  void execute_i() override;
  void cancel_i() override;

  Foo_C_cust_op1 (Foo_C_cust_op1 const &) = delete;
  Foo_C_cust_op1 (Foo_C_cust_op1 &&) = delete;
  Foo_C_cust_op1 & operator= (Foo_C_cust_op1 const &) = delete;
  Foo_C_cust_op1 & operator= (Foo_C_cust_op1 &&) = delete;

private:
  bool cancelled_;
  Foo_C_i* servant_;
};

#endif
