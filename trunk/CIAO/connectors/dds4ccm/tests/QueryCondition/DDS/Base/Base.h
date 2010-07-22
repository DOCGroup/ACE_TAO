// $Id$

/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef Base_1279786281770_h
#define Base_1279786281770_h

#ifndef NDDS_STANDALONE_TYPE
    #ifdef __cplusplus
        #ifndef ndds_cpp_h
            #include "ndds/ndds_cpp.h"
        #endif
    #else
        #ifndef ndds_c_h
            #include "ndds/ndds_c.h"
        #endif
    #endif
#else
    #include "ndds_standalone_type.h"
#endif


        
extern const char *QueryConditionTestTYPENAME;
        

typedef struct QueryConditionTest
{
    char*  symbol; /* maximum length = (255) */
    DDS_Long  iteration;

} QueryConditionTest;
    
                            
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* QueryConditionTest_get_typecode(void); /* Type code */
    

DDS_SEQUENCE(QueryConditionTestSeq, QueryConditionTest);
        
NDDSUSERDllExport
RTIBool QueryConditionTest_initialize(
        QueryConditionTest* self);
        
NDDSUSERDllExport
RTIBool QueryConditionTest_initialize_ex(
        QueryConditionTest* self,RTIBool allocatePointers);

NDDSUSERDllExport
void QueryConditionTest_finalize(
        QueryConditionTest* self);
                        
NDDSUSERDllExport
void QueryConditionTest_finalize_ex(
        QueryConditionTest* self,RTIBool deletePointers);
        
NDDSUSERDllExport
RTIBool QueryConditionTest_copy(
        QueryConditionTest* dst,
        const QueryConditionTest* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif



#endif /* Base_1279786281770_h */
