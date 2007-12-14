// -*- C++ -*-

//=============================================================================
/**
 *  @file    analyzer.h
 *
 *  $Id$
 *
 *  This file contains a DynAny analyzer that dumps the contents of a DynAny
 *  to the ace logging using ACE_DEBUG.
 *
 *  @author  Johnny Willemsen  (jwillemsen@remedy.nl)
 */
//=============================================================================

#ifndef ANALYZER_H
#define ANALYZER_H

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "tao/ORB.h"

class DynAnyAnalyzer
{
public:
  DynAnyAnalyzer (CORBA::ORB_ptr orb,
                  DynamicAny::DynAnyFactory_ptr dynany_factory,
                  int debug);

  void tab (int t);

  ~DynAnyAnalyzer (void);

  void resetTab (void);

  void analyze (DynamicAny::DynAny_ptr da);

  void analyze_basic_seq (CORBA::TypeCode_ptr tc,
                          DynamicAny::DynAny_ptr da);

private:
  CORBA::ORB_var orb_;
  DynamicAny::DynAnyFactory_var dynany_factory_;
  int level_;
  int debug_;
};


#endif // ANALYZER_H
