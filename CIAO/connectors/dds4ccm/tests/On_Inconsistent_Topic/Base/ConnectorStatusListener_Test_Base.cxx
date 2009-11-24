
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from ConnectorStatusListener_Test_Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef ConnectorStatusListener_Test_Base_h
#include "ConnectorStatusListener_Test_Base.h"
#endif



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

    
        
const char *TestTopicTYPENAME = "TestTopic";

    /* ========================================================================= */

DDS_TypeCode* TestTopic_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode TestTopic_g_tc_key_string = DDS_INITIALIZE_STRING_TYPECODE(255);

    static DDS_TypeCode_Member TestTopic_g_tc_members[2]=
    {
        {
            (char *)"key",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_TRUE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"x",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        }
    };

    static DDS_TypeCode TestTopic_g_tc =
    {{
        DDS_TK_STRUCT,/* Kind */
        DDS_BOOLEAN_FALSE, /* Ignored */
        -1,/* Ignored */
        (char *)"TestTopic", /* Name */
        NULL, /* Ignored */
        0, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        2, /* Number of members */
        TestTopic_g_tc_members, /* Members */
        DDS_VM_NONE /* Ignored */
    }}; /* Type code for TestTopic*/

    if (is_initialized) {
        return &TestTopic_g_tc;
    }


    TestTopic_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&TestTopic_g_tc_key_string;
    TestTopic_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    is_initialized = RTI_TRUE;

    return &TestTopic_g_tc;
}



RTIBool TestTopic_initialize(
    TestTopic* sample) {
    return TestTopic_initialize_ex(sample,RTI_TRUE);
}
        
RTIBool TestTopic_initialize_ex(
    TestTopic* sample,RTIBool allocatePointers)
{
     
    
    if (((CORBA::Char *&)sample->key)!=NULL){
        CORBA::string_free((CORBA::Char *&)sample->key);
    }
    
    ((CORBA::Char *&)sample->key)=(CORBA::Char *)CORBA::string_alloc((255));
    
    if (((CORBA::Char *&)sample->key) == NULL) {    
        return RTI_FALSE;
    }
    
    memset((CORBA::Char *&)sample->key,'x',(255));
    ((CORBA::Char *&)sample->key)[(255)] = '\0';
                
    if (!RTICdrType_initLong(&sample->x)) {
        return RTI_FALSE;
    }            
            
    return RTI_TRUE;
}

void TestTopic_finalize(
    TestTopic* sample)
{
    TestTopic_finalize_ex(sample,RTI_TRUE);
}
        
void TestTopic_finalize_ex(
    TestTopic* sample,RTIBool deletePointers)
{

    CORBA::string_free((CORBA::Char *&)sample->key);
    ((CORBA::Char *&)sample->key)=(CORBA::Char *)NULL;
             
}


RTIBool TestTopic_copy(
    TestTopic* dst,
    const TestTopic* src)
{
            
    dst->key = src->key;
                
    if (!RTICdrType_copyLong(
        &dst->x,&src->x)) {
        return RTI_FALSE;
    }                        
            

    return RTI_TRUE;
}


        
/**
 * <<IMPLEMENTATION>>
 *
 * Defines:  TSeq, T
 *
 * Configure and implement 'TestTopic' sequence class.
*/
#define T TestTopic
#define TSeq TestTopicSeq
#define T_initialize_ex TestTopic_initialize_ex
#define T_finalize_ex   TestTopic_finalize_ex
#define T_copy       TestTopic_copy
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#ifdef __cplusplus
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#endif
#undef T_copy
#undef T_finalize_ex
#undef T_initialize_ex
#undef TSeq
#undef T



/* ----------------------------------------------------------------- */

    
        
const char *TestTopic_SeqTYPENAME = "TestTopic_Seq";

    /* ========================================================================= */

DDS_TypeCode* TestTopic_Seq_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode TestTopic_Seq_g_tc_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE(100,NULL);

    static DDS_TypeCode TestTopic_Seq_g_tc =
    {{
        DDS_TK_ALIAS, /* Kind*/
        DDS_BOOLEAN_FALSE, /* Is a pointer? */
        -1, /* Ignored */
        (char *)"TestTopic_Seq", /* Name */
        NULL, /* Content type code is assigned later */
        0, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        DDS_VM_NONE /* Ignored */
    }}; /* Type code for TestTopic_Seq */

    if (is_initialized) {
        return &TestTopic_Seq_g_tc;
    }

    TestTopic_Seq_g_tc_sequence._data._typeCode = (RTICdrTypeCode *)TestTopic_get_typecode();
    TestTopic_Seq_g_tc._data._typeCode = (RTICdrTypeCode *)&TestTopic_Seq_g_tc_sequence; /* Content type code */

    is_initialized = RTI_TRUE;

    return &TestTopic_Seq_g_tc;
}



RTIBool TestTopic_Seq_initialize(
    TestTopic_Seq* sample) {
    return TestTopic_Seq_initialize_ex(sample,RTI_TRUE);
}
        
RTIBool TestTopic_Seq_initialize_ex(
    TestTopic_Seq* sample,RTIBool allocatePointers)
{

    void *buffer = NULL;
    buffer = NULL;

    (*sample).length(0); 
            
    return RTI_TRUE;
}

void TestTopic_Seq_finalize(
    TestTopic_Seq* sample)
{
    TestTopic_Seq_finalize_ex(sample,RTI_TRUE);
}
        
void TestTopic_Seq_finalize_ex(
    TestTopic_Seq* sample,RTIBool deletePointers)
{

    void *buffer = NULL;
    buffer = NULL;

             
}


RTIBool TestTopic_Seq_copy(
    TestTopic_Seq* dst,
    const TestTopic_Seq* src)
{
    
    (*dst) = (*src);
            

    return RTI_TRUE;
}



/* ----------------------------------------------------------------- */

    
        
const char *TestSecondTopicTYPENAME = "TestSecondTopic";

    /* ========================================================================= */

DDS_TypeCode* TestSecondTopic_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode TestSecondTopic_g_tc_key_string = DDS_INITIALIZE_STRING_TYPECODE(255);

    static DDS_TypeCode_Member TestSecondTopic_g_tc_members[3]=
    {
        {
            (char *)"key",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_TRUE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"x",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"y",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        }
    };

    static DDS_TypeCode TestSecondTopic_g_tc =
    {{
        DDS_TK_STRUCT,/* Kind */
        DDS_BOOLEAN_FALSE, /* Ignored */
        -1,/* Ignored */
        (char *)"TestSecondTopic", /* Name */
        NULL, /* Ignored */
        0, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        3, /* Number of members */
        TestSecondTopic_g_tc_members, /* Members */
        DDS_VM_NONE /* Ignored */
    }}; /* Type code for TestSecondTopic*/

    if (is_initialized) {
        return &TestSecondTopic_g_tc;
    }


    TestSecondTopic_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&TestSecondTopic_g_tc_key_string;
    TestSecondTopic_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;
    TestSecondTopic_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    is_initialized = RTI_TRUE;

    return &TestSecondTopic_g_tc;
}



RTIBool TestSecondTopic_initialize(
    TestSecondTopic* sample) {
    return TestSecondTopic_initialize_ex(sample,RTI_TRUE);
}
        
RTIBool TestSecondTopic_initialize_ex(
    TestSecondTopic* sample,RTIBool allocatePointers)
{
     
    
    if (((CORBA::Char *&)sample->key)!=NULL){
        CORBA::string_free((CORBA::Char *&)sample->key);
    }
    
    ((CORBA::Char *&)sample->key)=(CORBA::Char *)CORBA::string_alloc((255));
    
    if (((CORBA::Char *&)sample->key) == NULL) {    
        return RTI_FALSE;
    }
    
    memset((CORBA::Char *&)sample->key,'x',(255));
    ((CORBA::Char *&)sample->key)[(255)] = '\0';
                
    if (!RTICdrType_initLong(&sample->x)) {
        return RTI_FALSE;
    }            
                
    if (!RTICdrType_initLong(&sample->y)) {
        return RTI_FALSE;
    }            
            
    return RTI_TRUE;
}

void TestSecondTopic_finalize(
    TestSecondTopic* sample)
{
    TestSecondTopic_finalize_ex(sample,RTI_TRUE);
}
        
void TestSecondTopic_finalize_ex(
    TestSecondTopic* sample,RTIBool deletePointers)
{

    CORBA::string_free((CORBA::Char *&)sample->key);
    ((CORBA::Char *&)sample->key)=(CORBA::Char *)NULL;
             
}


RTIBool TestSecondTopic_copy(
    TestSecondTopic* dst,
    const TestSecondTopic* src)
{
            
    dst->key = src->key;
                
    if (!RTICdrType_copyLong(
        &dst->x,&src->x)) {
        return RTI_FALSE;
    }                        
                
    if (!RTICdrType_copyLong(
        &dst->y,&src->y)) {
        return RTI_FALSE;
    }                        
            

    return RTI_TRUE;
}


        
/**
 * <<IMPLEMENTATION>>
 *
 * Defines:  TSeq, T
 *
 * Configure and implement 'TestSecondTopic' sequence class.
*/
#define T TestSecondTopic
#define TSeq TestSecondTopicSeq
#define T_initialize_ex TestSecondTopic_initialize_ex
#define T_finalize_ex   TestSecondTopic_finalize_ex
#define T_copy       TestSecondTopic_copy
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#ifdef __cplusplus
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#endif
#undef T_copy
#undef T_finalize_ex
#undef T_initialize_ex
#undef TSeq
#undef T



/* ----------------------------------------------------------------- */

    
        
const char *TestSecondTopic_SeqTYPENAME = "TestSecondTopic_Seq";

    /* ========================================================================= */

DDS_TypeCode* TestSecondTopic_Seq_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode TestSecondTopic_Seq_g_tc_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE(100,NULL);

    static DDS_TypeCode TestSecondTopic_Seq_g_tc =
    {{
        DDS_TK_ALIAS, /* Kind*/
        DDS_BOOLEAN_FALSE, /* Is a pointer? */
        -1, /* Ignored */
        (char *)"TestSecondTopic_Seq", /* Name */
        NULL, /* Content type code is assigned later */
        0, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        DDS_VM_NONE /* Ignored */
    }}; /* Type code for TestSecondTopic_Seq */

    if (is_initialized) {
        return &TestSecondTopic_Seq_g_tc;
    }

    TestSecondTopic_Seq_g_tc_sequence._data._typeCode = (RTICdrTypeCode *)TestSecondTopic_get_typecode();
    TestSecondTopic_Seq_g_tc._data._typeCode = (RTICdrTypeCode *)&TestSecondTopic_Seq_g_tc_sequence; /* Content type code */

    is_initialized = RTI_TRUE;

    return &TestSecondTopic_Seq_g_tc;
}



RTIBool TestSecondTopic_Seq_initialize(
    TestSecondTopic_Seq* sample) {
    return TestSecondTopic_Seq_initialize_ex(sample,RTI_TRUE);
}
        
RTIBool TestSecondTopic_Seq_initialize_ex(
    TestSecondTopic_Seq* sample,RTIBool allocatePointers)
{

    void *buffer = NULL;
    buffer = NULL;

    (*sample).length(0); 
            
    return RTI_TRUE;
}

void TestSecondTopic_Seq_finalize(
    TestSecondTopic_Seq* sample)
{
    TestSecondTopic_Seq_finalize_ex(sample,RTI_TRUE);
}
        
void TestSecondTopic_Seq_finalize_ex(
    TestSecondTopic_Seq* sample,RTIBool deletePointers)
{

    void *buffer = NULL;
    buffer = NULL;

             
}


RTIBool TestSecondTopic_Seq_copy(
    TestSecondTopic_Seq* dst,
    const TestSecondTopic_Seq* src)
{
    
    (*dst) = (*src);
            

    return RTI_TRUE;
}



/* ----------------------------------------------------------------- */

