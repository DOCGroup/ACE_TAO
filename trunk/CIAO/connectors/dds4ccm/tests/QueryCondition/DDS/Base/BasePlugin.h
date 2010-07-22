
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef BasePlugin_1279786281770_h
#define BasePlugin_1279786281770_h

#include "Base.h"




struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif


#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif


/* The type used to store keys for instances of type struct
 * QueryConditionTest.
 *
 * By default, this type is struct QueryConditionTest
 * itself. However, if for some reason this choice is not practical for your
 * system (e.g. if sizeof(struct QueryConditionTest)
 * is very large), you may redefine this typedef in terms of another type of
 * your choosing. HOWEVER, if you define the KeyHolder type to be something
 * other than struct QueryConditionTest, the
 * following restriction applies: the key of struct
 * QueryConditionTest must consist of a
 * single field of your redefined KeyHolder type and that field must be the
 * first field in struct QueryConditionTest.
*/
typedef  struct QueryConditionTest QueryConditionTestKeyHolder;


#define QueryConditionTestPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
#define QueryConditionTestPlugin_return_sample PRESTypePluginDefaultEndpointData_returnSample 
#define QueryConditionTestPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define QueryConditionTestPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

#define QueryConditionTestPlugin_get_key PRESTypePluginDefaultEndpointData_getKey 
#define QueryConditionTestPlugin_return_key PRESTypePluginDefaultEndpointData_returnKey
 

#define QueryConditionTestPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define QueryConditionTestPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
    Support functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern QueryConditionTest*
QueryConditionTestPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern QueryConditionTest*
QueryConditionTestPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPluginSupport_copy_data(
    QueryConditionTest *out,
    const QueryConditionTest *in);

NDDSUSERDllExport extern void 
QueryConditionTestPluginSupport_destroy_data_ex(
    QueryConditionTest *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
QueryConditionTestPluginSupport_destroy_data(
    QueryConditionTest *sample);

NDDSUSERDllExport extern void 
QueryConditionTestPluginSupport_print_data(
    const QueryConditionTest *sample,
    const char *desc,
    unsigned int indent);


NDDSUSERDllExport extern QueryConditionTest*
QueryConditionTestPluginSupport_create_key_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern QueryConditionTest*
QueryConditionTestPluginSupport_create_key(void);

NDDSUSERDllExport extern void 
QueryConditionTestPluginSupport_destroy_key_ex(
    QueryConditionTestKeyHolder *key,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
QueryConditionTestPluginSupport_destroy_key(
    QueryConditionTestKeyHolder *key);
 

/* ----------------------------------------------------------------------------
    Callback functions:
 * ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
QueryConditionTestPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
QueryConditionTestPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);
    
NDDSUSERDllExport extern PRESTypePluginEndpointData 
QueryConditionTestPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
QueryConditionTestPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *out,
    const QueryConditionTest *in);

/* --------------------------------------------------------------------------------------
    (De)Serialize functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const QueryConditionTest *sample,
    struct RTICdrStream *stream, 
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);



NDDSUSERDllExport extern RTIBool
QueryConditionTestPlugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    RTIBool skip_encapsulation,  
    RTIBool skip_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern unsigned int 
QueryConditionTestPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int size);

NDDSUSERDllExport extern unsigned int 
QueryConditionTestPlugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int size);

NDDSUSERDllExport extern unsigned int
QueryConditionTestPlugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const QueryConditionTest * sample);


/* --------------------------------------------------------------------------------------
    Key Management functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginKeyKind 
QueryConditionTestPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
QueryConditionTestPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const QueryConditionTest *sample,
    struct RTICdrStream *stream,
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest * sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);


NDDSUSERDllExport extern RTIBool
QueryConditionTestPlugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_instance_to_key(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTestKeyHolder *key, 
    const QueryConditionTest *instance);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_key_to_instance(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *instance, 
    const QueryConditionTestKeyHolder *key);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_instance_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    DDS_KeyHash_t *keyhash,
    const QueryConditionTest *instance);

NDDSUSERDllExport extern RTIBool 
QueryConditionTestPlugin_serialized_sample_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    DDS_KeyHash_t *keyhash,
    RTIBool deserialize_encapsulation,
    void *endpoint_plugin_qos); 
     
/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
QueryConditionTestPlugin_new(void);

NDDSUSERDllExport extern void
QueryConditionTestPlugin_delete(struct PRESTypePlugin *);
 

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

        

#endif /* BasePlugin_1279786281770_h */
