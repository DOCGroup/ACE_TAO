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
#include "tao/DynamicAny/DynamicAny.h"

class DynAnyAnalyzer
{
public:
  DynAnyAnalyzer(CORBA::ORB_ptr orb, DynamicAny::DynAnyFactory_ptr dynany_factory, int debug);

  void tab(int t);

  ~DynAnyAnalyzer();

  void resetTab();

  void analyze(DynamicAny::DynAny_ptr da ACE_ENV_ARG_DECL);

private:
  CORBA::ORB_var orb_;
  DynamicAny::DynAnyFactory_var dynany_factory_;
  int level_;
  int debug_;
};


#endif // ANALYZER_H
