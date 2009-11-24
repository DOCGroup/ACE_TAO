
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from ConnectorStatusListener_Test_Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef ConnectorStatusListener_Test_Base_h
#define ConnectorStatusListener_Test_Base_h

#include "ConnectorStatusListener_Test_BaseC.h"


#ifdef __cplusplus
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#else
#ifndef ndds_c_h
#include "ndds/ndds_c.h"
#endif
#endif


        
extern const char *TestTopicTYPENAME;



#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols. */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* TestTopic_get_typecode(); /* Type code */
    
DDS_SEQUENCE(TestTopicSeq, TestTopic);


NDDSUSERDllExport 
RTIBool TestTopic_initialize(
        TestTopic* self);

NDDSUSERDllExport 
RTIBool TestTopic_initialize_ex(
        TestTopic* self,RTIBool allocatePointers);

NDDSUSERDllExport 
void TestTopic_finalize(
        TestTopic* self);

NDDSUSERDllExport 
void TestTopic_finalize_ex(
        TestTopic* self,RTIBool deletePointers);

NDDSUSERDllExport 
RTIBool TestTopic_copy(
        TestTopic* dst,
        const TestTopic* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif


        
extern const char *TestTopic_SeqTYPENAME;



#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols. */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* TestTopic_Seq_get_typecode(); /* Type code */
    

NDDSUSERDllExport 
RTIBool TestTopic_Seq_initialize(
        TestTopic_Seq* self);

NDDSUSERDllExport 
RTIBool TestTopic_Seq_initialize_ex(
        TestTopic_Seq* self,RTIBool allocatePointers);

NDDSUSERDllExport 
void TestTopic_Seq_finalize(
        TestTopic_Seq* self);

NDDSUSERDllExport 
void TestTopic_Seq_finalize_ex(
        TestTopic_Seq* self,RTIBool deletePointers);

NDDSUSERDllExport 
RTIBool TestTopic_Seq_copy(
        TestTopic_Seq* dst,
        const TestTopic_Seq* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif


        
extern const char *TestSecondTopicTYPENAME;



#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols. */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* TestSecondTopic_get_typecode(); /* Type code */
    
DDS_SEQUENCE(TestSecondTopicSeq, TestSecondTopic);


NDDSUSERDllExport 
RTIBool TestSecondTopic_initialize(
        TestSecondTopic* self);

NDDSUSERDllExport 
RTIBool TestSecondTopic_initialize_ex(
        TestSecondTopic* self,RTIBool allocatePointers);

NDDSUSERDllExport 
void TestSecondTopic_finalize(
        TestSecondTopic* self);

NDDSUSERDllExport 
void TestSecondTopic_finalize_ex(
        TestSecondTopic* self,RTIBool deletePointers);

NDDSUSERDllExport 
RTIBool TestSecondTopic_copy(
        TestSecondTopic* dst,
        const TestSecondTopic* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif


        
extern const char *TestSecondTopic_SeqTYPENAME;



#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols. */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* TestSecondTopic_Seq_get_typecode(); /* Type code */
    

NDDSUSERDllExport 
RTIBool TestSecondTopic_Seq_initialize(
        TestSecondTopic_Seq* self);

NDDSUSERDllExport 
RTIBool TestSecondTopic_Seq_initialize_ex(
        TestSecondTopic_Seq* self,RTIBool allocatePointers);

NDDSUSERDllExport 
void TestSecondTopic_Seq_finalize(
        TestSecondTopic_Seq* self);

NDDSUSERDllExport 
void TestSecondTopic_Seq_finalize_ex(
        TestSecondTopic_Seq* self,RTIBool deletePointers);

NDDSUSERDllExport 
RTIBool TestSecondTopic_Seq_copy(
        TestSecondTopic_Seq* dst,
        const TestSecondTopic_Seq* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif



#endif /* ConnectorStatusListener_Test_Base_h */
