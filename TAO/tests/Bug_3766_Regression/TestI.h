// -*- C++ -*-
/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v1.7.4
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 *       http://doc.ece.uci.edu/
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.dre.vanderbilt.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// .\be\be_codegen.cpp:1364

#ifndef TESTI_H_
#define TESTI_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Test_i
  : public virtual POA_Test
{
public:
  // Constructor
  Test_i (CORBA::ORB_ptr o);

  // Destructor
  virtual ~Test_i ();

  virtual
  void do_something_FixedLength (
    ::FixedLengthInfo_out my_info);

  virtual
  void do_something_VariableLength (
    ::VariableLengthInfo_out my_info);

  virtual void shutdown ();

private:
  CORBA::ORB_var orb_;
};


#endif /* TESTI_H_  */

