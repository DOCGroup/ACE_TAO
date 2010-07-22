// $Id$

/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/


#ifndef NDDS_STANDALONE_TYPE
    #ifdef __cplusplus
        #ifndef ndds_cpp_h
            #include "ndds/ndds_cpp.h"
        #endif
        #ifndef dds_c_log_impl_h              
            #include "dds_c/dds_c_log_impl.h"                                
        #endif        
    #else
        #ifndef ndds_c_h
            #include "ndds/ndds_c.h"
        #endif
    #endif
    
    #ifndef cdr_type_h
        #include "cdr/cdr_type.h"
    #endif    

    #ifndef osapi_heap_h
        #include "osapi/osapi_heap.h" 
    #endif
#else
    #include "ndds_standalone_type.h"
#endif



#include "Base.h"

/* ========================================================================= */
const char *QueryConditionTestTYPENAME = "QueryConditionTest";

DDS_TypeCode* QueryConditionTest_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode QueryConditionTest_g_tc_symbol_string = DDS_INITIALIZE_STRING_TYPECODE(255);

    static DDS_TypeCode_Member QueryConditionTest_g_tc_members[2]=
    {
        {
            (char *)"symbol",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                0/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            DDS_BOOLEAN_TRUE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            0/* Ignored */
        },
        {
            (char *)"iteration",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                0/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            0/* Ignored */
        }
    };

    static DDS_TypeCode QueryConditionTest_g_tc =
    {{
        DDS_TK_STRUCT,/* Kind */
        DDS_BOOLEAN_FALSE, /* Ignored */
        -1,/* Ignored */
        (char *)"QueryConditionTest", /* Name */
        0, /* Ignored */
        0, /* Ignored */
        0, /* Ignored */
        0, /* Ignored */
        2, /* Number of members */
        QueryConditionTest_g_tc_members, /* Members */
        DDS_VM_NONE /* Ignored */
    }}; /* Type code for QueryConditionTest*/

    if (is_initialized) {
        return &QueryConditionTest_g_tc;
    }


    QueryConditionTest_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&QueryConditionTest_g_tc_symbol_string;
    QueryConditionTest_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    is_initialized = RTI_TRUE;

    return &QueryConditionTest_g_tc;
}


RTIBool QueryConditionTest_initialize(
    QueryConditionTest* sample) {
  return ::QueryConditionTest_initialize_ex(sample,RTI_TRUE);
}
        
RTIBool QueryConditionTest_initialize_ex(
    QueryConditionTest* sample,RTIBool allocatePointers)
{

    sample->symbol = DDS_String_alloc((255));
    if (sample->symbol == 0) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_initLong(&sample->iteration)) {
        return RTI_FALSE;
    }                
            

    return RTI_TRUE;
}

void QueryConditionTest_finalize(
    QueryConditionTest* sample)
{
    ::QueryConditionTest_finalize_ex(sample,RTI_TRUE);
}
        
void QueryConditionTest_finalize_ex(
    QueryConditionTest* sample,RTIBool deletePointers)
{        

    DDS_String_free(sample->symbol);                
            
}

RTIBool QueryConditionTest_copy(
    QueryConditionTest* dst,
    const QueryConditionTest* src)
{        

    if (!RTICdrType_copyString(
        dst->symbol, src->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_copyLong(
        &dst->iteration, &src->iteration)) {
        return RTI_FALSE;
    }
            

    return RTI_TRUE;
}


/**
 * <<IMPLEMENTATION>>
 *
 * Defines:  TSeq, T
 *
 * Configure and implement 'QueryConditionTest' sequence class.
 */
#define T QueryConditionTest
#define TSeq QueryConditionTestSeq
#define T_initialize_ex ::QueryConditionTest_initialize_ex
#define T_finalize_ex   ::QueryConditionTest_finalize_ex
#define T_copy       ::QueryConditionTest_copy

#ifndef NDDS_STANDALONE_TYPE
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#ifdef __cplusplus
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#endif
#else
#include "dds_c_sequence_TSeq.gen"
#ifdef __cplusplus
#include "dds_cpp_sequence_TSeq.gen"
#endif
#endif

#undef T_copy
#undef T_finalize_ex
#undef T_initialize_ex
#undef TSeq
#undef T

