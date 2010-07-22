// $Id$

/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef BaseSupport_1279786281770_h
#define BaseSupport_1279786281770_h

/* Uses */
#include "Base.h"



#ifdef __cplusplus
#ifndef ndds_cpp_h
  #include "ndds/ndds_cpp.h"
#endif
#else
#ifndef ndds_c_h
  #include "ndds/ndds_c.h"
#endif
#endif

        

/* ========================================================================= */
/**
   Uses:     T

   Defines:  TTypeSupport, TDataWriter, TDataReader

   Organized using the well-documented "Generics Pattern" for
   implementing generics in C and C++.
*/

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)

#ifdef __cplusplus
  /* If we're building on Windows, explicitly import the superclasses of
   * the types declared below.
   */        
  class __declspec(dllimport) ::DDSTypeSupport;
  class __declspec(dllimport) ::DDSDataWriter;
  class __declspec(dllimport) ::DDSDataReader;
#endif

#endif

#ifdef __cplusplus

DDS_TYPESUPPORT_CPP(QueryConditionTestTypeSupport, QueryConditionTest);

DDS_DATAWRITER_CPP(QueryConditionTestDataWriter, QueryConditionTest);
DDS_DATAREADER_CPP(QueryConditionTestDataReader, QueryConditionTestSeq, QueryConditionTest);


#else

DDS_TYPESUPPORT_C(QueryConditionTestTypeSupport, QueryConditionTest);
DDS_DATAWRITER_C(QueryConditionTestDataWriter, QueryConditionTest);
DDS_DATAREADER_C(QueryConditionTestDataReader, QueryConditionTestSeq, QueryConditionTest);

#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif



#endif  /* BaseSupport_1279786281770_h */
