// $Id$

// Regression test Bug 2234
//
// The bug manifests itself in the server.

#include "TestC.h"
#include "ace/OS_NS_string.h"

int
main(
  int    argc,
  char** argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  CORBA::Boolean testFailed= 0;
  ACE_DEBUG( (LM_INFO, "Regression test for Bug 2234 (Interceptor arg list with var len out params)\n"));
  ACE_TRY
  {
    CORBA::ORB_var
       orb= CORBA::ORB_init( argc, argv, 0 ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    CORBA::Object_var
       object= orb->string_to_object( "file://server.ior" ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    Test::Foo_var
       foo= Test::Foo::_narrow( object.in() ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;

    //-----------------------------------------------------------------------
    CORBA::Long
       aL= 1,
       bL,
       cL= 3,
       rL= 0;
    ACE_DEBUG( (LM_INFO, ". Long()            ") );
    rL= foo->TestLong( aL, bL, cL ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is %d, b is %d, c is %d, r is %d:  ", aL, bL, cL, rL) );
    if (static_cast<CORBA::Long>(1) != aL) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if (static_cast<CORBA::Long>(2) != bL) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if (static_cast<CORBA::Long>(4) != cL) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if (static_cast<CORBA::Long>(7) != rL) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    CORBA::String_var
       aS= CORBA::string_dup("1"),
       bS,
       cS= CORBA::string_dup("3"),
       rS;
    ACE_DEBUG( (LM_INFO, ". String()          ") );
    rS= foo->TestString( aS.in(), bS.out(), cS.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is %s, b is %s, c is %s, r is %s:  ", aS.in(), bS.in(), cS.in(), rS.in()) );
    if ((0 != *aS.in()) && (0 != ACE_OS::strcmp(aS.in(), "1"))) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if ((0 != *aS.in()) && (0 != ACE_OS::strcmp(bS.in(), "2"))) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if ((0 != *aS.in()) && (0 != ACE_OS::strcmp(cS.in(), "4"))) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if ((0 != *aS.in()) && (0 != ACE_OS::strcmp(rS.in(), "7"))) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    Test::MyNonVarStruct_var
       aFS= new Test::MyNonVarStruct(),
       bFS= new Test::MyNonVarStruct(),
       cFS= new Test::MyNonVarStruct(),
       rFS;
    aFS->val= 1;
    cFS->val= 3;
    ACE_DEBUG( (LM_INFO, ". MyNonVarStruct()  ") );
    rFS= foo->TestNonVarStruct( aFS.in(), bFS.out(), cFS.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is %d, b is %d, c is %d, r is %d:  ", aFS.in().val, bFS.in().val, cFS.in().val, rFS.in().val) );
    if (static_cast<CORBA::Long>(1) != aFS->val) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if (static_cast<CORBA::Long>(2) != bFS->val) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if (static_cast<CORBA::Long>(4) != cFS->val) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if (static_cast<CORBA::Long>(7) != rFS->val) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    Test::MyVarStruct_var
       aVS= new Test::MyVarStruct(),
       bVS,
       cVS= new Test::MyVarStruct(),
       rVS;
    aVS->val= CORBA::string_dup("1");
    cVS->val= CORBA::string_dup("3");
    ACE_DEBUG( (LM_INFO, ". MyVarStruct()     ") );
    rVS= foo->TestVarStruct( aVS.in(), bVS.out(), cVS.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is %s, b is %s, c is %s, r is %s:  ", aVS->val.in(), bVS->val.in(), cVS->val.in(), rVS->val.in()) );
    if ((0 != *aVS->val) && (0 != ACE_OS::strcmp(aVS->val, "1"))) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if ((0 != *bVS->val) && (0 != ACE_OS::strcmp(bVS->val, "2"))) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if ((0 != *cVS->val) && (0 != ACE_OS::strcmp(cVS->val, "4"))) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if ((0 != *rVS->val) && (0 != ACE_OS::strcmp(rVS->val, "7"))) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    Test::MyNonVarUnion_var
       aFU= new Test::MyNonVarUnion(),
       bFU= new Test::MyNonVarUnion(),
       cFU= new Test::MyNonVarUnion(),
       rFU;
    aFU->valLong( static_cast<CORBA::Long>(1) );
    cFU->valLong( static_cast<CORBA::Long>(3) );
    ACE_DEBUG( (LM_INFO, ". MyNonVarUnion()   ") );
    rFU= foo->TestNonVarUnion( aFU.in(), bFU.out(), cFU.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is ") );
    if (static_cast<CORBA::Short>(1) != aFU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", aFU->valLong()) );
    ACE_DEBUG( (LM_INFO, ", b is ") );
    if (static_cast<CORBA::Short>(1) != bFU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", bFU->valLong()) );
    ACE_DEBUG( (LM_INFO, ", c is ") );
    if (static_cast<CORBA::Short>(1) != cFU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", cFU->valLong()) );
    ACE_DEBUG( (LM_INFO, ", r is ") );
    if (static_cast<CORBA::Short>(1) != rFU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", rFU->valLong()) );
    ACE_DEBUG( (LM_INFO, ":  ") );
    if ((static_cast<CORBA::Short>(1) != aFU->_d()) || (static_cast<CORBA::Long>(1) != aFU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if ((static_cast<CORBA::Short>(1) != bFU->_d()) || (static_cast<CORBA::Long>(2) != bFU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if ((static_cast<CORBA::Short>(1) != cFU->_d()) || (static_cast<CORBA::Long>(4) != cFU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if ((static_cast<CORBA::Short>(1) != rFU->_d()) || (static_cast<CORBA::Long>(7) != rFU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    Test::MyVarUnion_var
       aVU= new Test::MyVarUnion(),
       bVU,
       cVU= new Test::MyVarUnion(),
       rVU;
    aVU->valLong( static_cast<CORBA::Long>(1) );
    cVU->valLong( static_cast<CORBA::Long>(3) );
    ACE_DEBUG( (LM_INFO, ". MyVarUnion()      ") );
    rVU= foo->TestVarUnion( aVU.in(), bVU.out(), cVU.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is ") );
    if (static_cast<CORBA::Short>(1) != aVU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", aVU->valLong()) );
    ACE_DEBUG( (LM_INFO, ", b is ") );
    if (static_cast<CORBA::Short>(1) != bVU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", bVU->valLong()) );
    ACE_DEBUG( (LM_INFO, ", c is ") );
    if (static_cast<CORBA::Short>(1) != cVU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", cVU->valLong()) );
    ACE_DEBUG( (LM_INFO, ", r is ") );
    if (static_cast<CORBA::Short>(1) != rVU->_d())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", rVU->valLong()) );
    ACE_DEBUG( (LM_INFO, ":  ") );
    if ((static_cast<CORBA::Short>(1) != aVU->_d()) || (static_cast<CORBA::Long>(1) != aVU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if ((static_cast<CORBA::Short>(1) != bVU->_d()) || (static_cast<CORBA::Long>(2) != bVU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if ((static_cast<CORBA::Short>(1) != cVU->_d()) || (static_cast<CORBA::Long>(4) != cVU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if ((static_cast<CORBA::Short>(1) != rVU->_d()) || (static_cast<CORBA::Long>(7) != rVU->valLong())) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    Test::MySeqOfLong_var
       aSL= new Test::MySeqOfLong(1),
       bSL,
       cSL= new Test::MySeqOfLong(1),
       rSL;
    aSL->length(1);
    aSL[0]= static_cast<CORBA::Long>(1);
    cSL->length(1);
    cSL[0]= static_cast<CORBA::Long>(3);
    ACE_DEBUG( (LM_INFO, ". MySeqOfLong()     ") );
    rSL= foo->TestSeqOfLong( aSL.in(), bSL.out(), cSL.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_DEBUG( (LM_INFO, "a is ") );
    if (1u != aSL->length())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", aSL[0]) );
    ACE_DEBUG( (LM_INFO, ", b is ") );
    if (1u != bSL->length())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", bSL[0]) );
    ACE_DEBUG( (LM_INFO, ", c is ") );
    if (1u != cSL->length())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", cSL[0]) );
    ACE_DEBUG( (LM_INFO, ", r is ") );
    if (1u != rSL->length())
       ACE_DEBUG( (LM_INFO, "?") );
    else
       ACE_DEBUG( (LM_INFO, "%d", rSL[0]) );
    ACE_DEBUG( (LM_INFO, ":  ") );
    if ((1u != aSL->length()) || (static_cast<CORBA::Long>(1) != aSL[0])) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if ((1u != bSL->length()) || (static_cast<CORBA::Long>(2) != bSL[0])) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if ((1u != cSL->length()) || (static_cast<CORBA::Long>(4) != cSL[0])) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if ((1u != rSL->length()) || (static_cast<CORBA::Long>(7) != rSL[0])) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    CORBA::Any_var
       aA= new CORBA::Any(),
       bA,
       cA= new CORBA::Any(),
       rA;
    aA<<= static_cast<CORBA::Long>(1);
    cA<<= static_cast<CORBA::Long>(3);
    ACE_DEBUG( (LM_INFO, ". Any()             ") );
    rA= foo->TestAny( aA.in(), bA.out(), cA.inout() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    CORBA::Boolean
       aB,
       bB,
       cB,
       rB;
    ACE_DEBUG( (LM_INFO, "a is ") );
    if ((aB= (aA>>= aL)))
       ACE_DEBUG( (LM_INFO, "%d", aL) );
    else
       ACE_DEBUG( (LM_INFO, "?") );
    ACE_DEBUG( (LM_INFO, ", b is ") );
    if ((bB= (bA>>= bL)))
       ACE_DEBUG( (LM_INFO, "%d", bL) );
    else
       ACE_DEBUG( (LM_INFO, "?") );
    ACE_DEBUG( (LM_INFO, ", c is ") );
    if ((cB= (cA>>= cL)))
       ACE_DEBUG( (LM_INFO, "%d", cL) );
    else
       ACE_DEBUG( (LM_INFO, "?") );
    ACE_DEBUG( (LM_INFO, ", r is ") );
    if ((rB= (rA>>= rL)))
       ACE_DEBUG( (LM_INFO, "%d", rL) );
    else
       ACE_DEBUG( (LM_INFO, "?") );
    ACE_DEBUG( (LM_INFO, ":  ") );
    if (!aB || (static_cast<CORBA::Long>(1) != aL)) {
       ACE_DEBUG( (LM_ERROR, "a is wrong\n") ); testFailed= 1;}
    else if (!bB || (static_cast<CORBA::Long>(2) != bL)) {
       ACE_DEBUG( (LM_ERROR, "b is wrong\n") ); testFailed= 1;}
    else if (!cB || (static_cast<CORBA::Long>(4) != cL)) {
       ACE_DEBUG( (LM_ERROR, "c is wrong\n") ); testFailed= 1;}
    else if (!rB || (static_cast<CORBA::Long>(7) != rL)) {
       ACE_DEBUG( (LM_ERROR, "r is wrong\n") ); testFailed= 1;}
    else
       ACE_DEBUG( (LM_INFO, "OK\n") );

    //-----------------------------------------------------------------------
    foo->ShutdownServer( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;

    //-----------------------------------------------------------------------
    orb->shutdown( true ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    orb->destroy( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;
  }
  ACE_CATCH( CORBA::SystemException, ex )
  {
    ACE_PRINT_EXCEPTION( ex, "CORBA::SystemException: " );
    testFailed= 1;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION( ACE_ANY_EXCEPTION, "CORBA::Exception: " );
    testFailed= 1;
  }
  ACE_CATCHALL
  {
    ACE_DEBUG( (LM_ERROR, "Unexpected general exception!\n") );
    testFailed= 1;
  }
  ACE_ENDTRY;

  ACE_DEBUG( (LM_INFO, "Regression test for Bug 2234 ") );
  if (testFailed)
    ACE_DEBUG( (LM_ERROR, "Failed\n") );
  else
    ACE_DEBUG( (LM_INFO, "Passed\n") );

  return testFailed? -1 : 0;
}
