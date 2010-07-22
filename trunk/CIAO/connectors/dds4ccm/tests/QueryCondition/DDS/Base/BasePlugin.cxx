// $Id$

/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Base.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/


#include <string.h>

#ifdef __cplusplus
#ifndef ndds_cpp_h
  #include "ndds/ndds_cpp.h"
#endif
#else
#ifndef ndds_c_h
  #include "ndds/ndds_c.h"
#endif
#endif

#ifndef osapi_type_h
  #include "osapi/osapi_type.h"
#endif
#ifndef osapi_heap_h
  #include "osapi/osapi_heap.h"
#endif

#ifndef osapi_utility_h
  #include "osapi/osapi_utility.h"
#endif

#ifndef cdr_type_h
  #include "cdr/cdr_type.h"
#endif

#ifndef cdr_encapsulation_h
  #include "cdr/cdr_encapsulation.h"
#endif

#ifndef cdr_stream_h
  #include "cdr/cdr_stream.h"
#endif

#ifndef pres_typePlugin_h
  #include "pres/pres_typePlugin.h"
#endif



#include "BasePlugin.h"


/* --------------------------------------------------------------------------------------
 *  Type QueryConditionTest
 * -------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------
    Support functions:
 * -------------------------------------------------------------------------------------- */


QueryConditionTest *
QueryConditionTestPluginSupport_create_data_ex(RTIBool allocate_pointers){
    QueryConditionTest *sample = 0;

    RTIOsapiHeap_allocateStructure(
        &sample, QueryConditionTest);

    if(sample != 0) {
        if (!::QueryConditionTest_initialize_ex(sample,allocate_pointers)) {
            RTIOsapiHeap_freeStructure(&sample);
            return 0;
        }
    }
    return sample; 
}


QueryConditionTest *
QueryConditionTestPluginSupport_create_data(void)
{
    return ::QueryConditionTestPluginSupport_create_data_ex(RTI_TRUE);
}


void 
QueryConditionTestPluginSupport_destroy_data_ex(
    QueryConditionTest *sample,RTIBool deallocate_pointers) {

    ::QueryConditionTest_finalize_ex(sample,deallocate_pointers);

    RTIOsapiHeap_freeStructure(sample);
}


void 
QueryConditionTestPluginSupport_destroy_data(
    QueryConditionTest *sample) {

    ::QueryConditionTestPluginSupport_destroy_data_ex(sample,RTI_TRUE);

}


RTIBool 
QueryConditionTestPluginSupport_copy_data(
    QueryConditionTest *dst,
    const QueryConditionTest *src)
{
    return ::QueryConditionTest_copy(dst,src);
}


void 
QueryConditionTestPluginSupport_print_data(
    const QueryConditionTest *sample,
    const char *desc,
    unsigned int indent_level)
{


    RTICdrType_printIndent(indent_level);

    if (desc != 0) {
      RTILog_debug("%s:\n", desc);
    } else {
      RTILog_debug("\n");
    }

    if (sample == 0) {
      RTILog_debug("NULL\n");
      return;
    }


    if (&sample->symbol==0) {
        RTICdrType_printString(
            0, "symbol", indent_level + 1);
    } else {
        RTICdrType_printString(
            sample->symbol, "symbol", indent_level + 1);                
    }
            
    RTICdrType_printLong(
        &sample->iteration, "iteration", indent_level + 1);
            

}

QueryConditionTest *
QueryConditionTestPluginSupport_create_key_ex(RTIBool allocate_pointers){
    QueryConditionTest *key = 0;

    RTIOsapiHeap_allocateStructure(
        &key, QueryConditionTestKeyHolder);

    ::QueryConditionTest_initialize_ex(key,allocate_pointers);
    return key;
}


QueryConditionTest *
QueryConditionTestPluginSupport_create_key(void)
{
    return  ::QueryConditionTestPluginSupport_create_key_ex(RTI_TRUE);
}


void 
QueryConditionTestPluginSupport_destroy_key_ex(
    QueryConditionTestKeyHolder *key,RTIBool deallocate_pointers)
{
    ::QueryConditionTest_finalize_ex(key,deallocate_pointers);

    RTIOsapiHeap_freeStructure(key);
}


void 
QueryConditionTestPluginSupport_destroy_key(
    QueryConditionTestKeyHolder *key) {

  ::QueryConditionTestPluginSupport_destroy_key_ex(key,RTI_TRUE);

}



/* ----------------------------------------------------------------------------
    Callback functions:
 * ---------------------------------------------------------------------------- */



PRESTypePluginParticipantData 
QueryConditionTestPlugin_on_participant_attached(
    void *registration_data,
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration,
    void *container_plugin_context,
    RTICdrTypeCode *type_code)
{

    return PRESTypePluginDefaultParticipantData_new(participant_info);

}


void 
QueryConditionTestPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data)
{

  PRESTypePluginDefaultParticipantData_delete(participant_data);
}


PRESTypePluginEndpointData
QueryConditionTestPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *containerPluginContext)
{
    PRESTypePluginEndpointData epd = 0;

    unsigned int serializedKeyMaxSize;


    epd = PRESTypePluginDefaultEndpointData_new(
            participant_data,
            endpoint_info,
            (PRESTypePluginDefaultEndpointDataCreateSampleFunction)
            ::QueryConditionTestPluginSupport_create_data,
            (PRESTypePluginDefaultEndpointDataDestroySampleFunction)
            ::QueryConditionTestPluginSupport_destroy_data,
            (PRESTypePluginDefaultEndpointDataCreateKeyFunction)
            ::QueryConditionTestPluginSupport_create_key,
            (PRESTypePluginDefaultEndpointDataDestroyKeyFunction)
            ::QueryConditionTestPluginSupport_destroy_key);

    if (epd == 0) {
        return 0;
    }
   
    serializedKeyMaxSize = ::QueryConditionTestPlugin_get_serialized_key_max_size(
        epd,RTI_FALSE,RTI_CDR_ENCAPSULATION_ID_CDR_BE,0);
    
    if (!PRESTypePluginDefaultEndpointData_createMD5Stream(
            epd,serializedKeyMaxSize)) 
    {
        PRESTypePluginDefaultEndpointData_delete(epd);
        return 0;
    }

    
    if (endpoint_info->endpointKind == PRES_TYPEPLUGIN_ENDPOINT_WRITER) {
        if (PRESTypePluginDefaultEndpointData_createWriterPool(
                epd,
                endpoint_info,
            (PRESTypePluginGetSerializedSampleMaxSizeFunction)
                ::QueryConditionTestPlugin_get_serialized_sample_max_size, epd,
            (PRESTypePluginGetSerializedSampleSizeFunction)
            ::QueryConditionTestPlugin_get_serialized_sample_size,
            epd) == RTI_FALSE) {
            PRESTypePluginDefaultEndpointData_delete(epd);
            return 0;
        }
    }
    


    return epd;    
}


void 
QueryConditionTestPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data)
{  

    PRESTypePluginDefaultEndpointData_delete(endpoint_data);
}


RTIBool 
QueryConditionTestPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *dst,
    const QueryConditionTest *src)
{
    return ::QueryConditionTestPluginSupport_copy_data(dst,src);
}

/* --------------------------------------------------------------------------------------
    (De)Serialize functions:
 * -------------------------------------------------------------------------------------- */


RTIBool 
QueryConditionTestPlugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const QueryConditionTest *sample, 
    struct RTICdrStream *stream,    
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos)
{
    char * position = 0;


  if(serialize_encapsulation) {

    if (!RTICdrStream_serializeAndSetCdrEncapsulation(stream, encapsulation_id)) {
        return RTI_FALSE;
    }

    position = RTICdrStream_resetAlignment(stream);

  }


  if(serialize_sample) {

    if (sample->symbol == 0) {
        return RTI_FALSE;
    }
    if (!RTICdrStream_serializeString(
        stream, sample->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrStream_serializeLong(
        stream, &sample->iteration)) {
        return RTI_FALSE;
    }
            
  }


  if(serialize_encapsulation) {
    RTICdrStream_restoreAlignment(stream,position);
  }


  return RTI_TRUE;
}


RTIBool 
QueryConditionTestPlugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *sample,
    struct RTICdrStream *stream,   
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos)
{
    char * position = 0;


    if(deserialize_encapsulation) {
        /* Deserialize encapsulation */

        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);

    }
    

    if(deserialize_sample) {

    if (!RTICdrStream_deserializeString(
        stream, sample->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrStream_deserializeLong(
        stream, &sample->iteration)) {
        return RTI_FALSE;
    }
            
    }


    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }


    return RTI_TRUE;
}

 
 
RTIBool 
QueryConditionTestPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest **sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,   
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos)
{

    return ::QueryConditionTestPlugin_deserialize_sample( 
        endpoint_data, (sample != 0)?*sample:0,
        stream, deserialize_encapsulation, deserialize_sample, 
        endpoint_plugin_qos);
 
}



RTIBool QueryConditionTestPlugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream,   
    RTIBool skip_encapsulation,
    RTIBool skip_sample, 
    void *endpoint_plugin_qos)
{
    char * position = 0;


    if(skip_encapsulation) {
        if (!RTICdrStream_skipEncapsulation(stream)) {
            return RTI_FALSE;
        }


        position = RTICdrStream_resetAlignment(stream);

    }

    if (skip_sample) {

    if (!RTICdrStream_skipString(stream, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrStream_skipLong(stream)) {
        return RTI_FALSE;
    }
            
    }


    if(skip_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }


    return RTI_TRUE;
}


unsigned int 
QueryConditionTestPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;


    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }

        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;

    }


    current_alignment +=  RTICdrType_getStringMaxSizeSerialized(
        current_alignment, (255) + 1);
            
    current_alignment +=  RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
            
    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }

    return current_alignment - initial_alignment;
}


unsigned int 
QueryConditionTestPlugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;


    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }

        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;

    }


    current_alignment +=  RTICdrType_getStringMaxSizeSerialized(
        current_alignment, 1);
            
    current_alignment +=  RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
            
    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }

    return current_alignment - initial_alignment;
}


/* Returns the size of the sample in its serialized form (in bytes).
 * It can also be an estimation in excess of the real buffer needed 
 * during a call to the serialize() function.
 * The value reported does not have to include the space for the
 * encapsulation flags.
 */
unsigned int
QueryConditionTestPlugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const QueryConditionTest * sample) 
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;


    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }

        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;

    }


    current_alignment += RTICdrType_getStringSerializedSize(
        current_alignment, sample->symbol);
            
    current_alignment += RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
            
    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }

    return current_alignment - initial_alignment;
}

/* --------------------------------------------------------------------------------------
    Key Management functions:
 * -------------------------------------------------------------------------------------- */


PRESTypePluginKeyKind 
QueryConditionTestPlugin_get_key_kind(void)
{

    return PRES_TYPEPLUGIN_USER_KEY;
     
}


RTIBool 
QueryConditionTestPlugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const QueryConditionTest *sample, 
    struct RTICdrStream *stream,    
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos)
{
    char * position = 0;


    if(serialize_encapsulation) {
        if (!RTICdrStream_serializeAndSetCdrEncapsulation(stream, encapsulation_id)) {
            return RTI_FALSE;
        }


        position = RTICdrStream_resetAlignment(stream);

    }

    if(serialize_key) {

    if (sample->symbol == 0) {
        return RTI_FALSE;
    }
    if (!RTICdrStream_serializeString(
        stream, sample->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    }


    if(serialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }


    return RTI_TRUE;
}


RTIBool QueryConditionTestPlugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos)
{
    char * position = 0;


    if(deserialize_encapsulation) {
        /* Deserialize encapsulation */
        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;  
        }


        position = RTICdrStream_resetAlignment(stream);

    }

    if (deserialize_key) {

    if (!RTICdrStream_deserializeString(
        stream, sample->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    }


    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }


    return RTI_TRUE;
}


 
RTIBool QueryConditionTestPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos)
{
    return ::QueryConditionTestPlugin_deserialize_key_sample(
        endpoint_data, (sample != 0)?*sample:0, stream,
        deserialize_encapsulation, deserialize_key, endpoint_plugin_qos);
}



unsigned int
QueryConditionTestPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int encapsulation_size = current_alignment;


    unsigned int initial_alignment = current_alignment;

    if (include_encapsulation) {
        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }


        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;

    }
        

    current_alignment +=  RTICdrType_getStringMaxSizeSerialized(
        current_alignment, (255) + 1);
            
    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }

    return current_alignment - initial_alignment;
}


RTIBool 
QueryConditionTestPlugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos)
{
    char * position = 0;


    if(deserialize_encapsulation) {
        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);

    }

    if (deserialize_key) {

    if (!RTICdrStream_deserializeString(
        stream, sample->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrStream_skipLong(stream)) {
        return RTI_FALSE;
    }
            
    }


    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }


    return RTI_TRUE;
}





RTIBool 
QueryConditionTestPlugin_instance_to_key(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTestKeyHolder *dst, 
    const QueryConditionTest *src)
{

    if (!RTICdrType_copyString(
        dst->symbol, src->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    return RTI_TRUE;
}


RTIBool 
QueryConditionTestPlugin_key_to_instance(
    PRESTypePluginEndpointData endpoint_data,
    QueryConditionTest *dst, const
    QueryConditionTestKeyHolder *src)
{

    if (!RTICdrType_copyString(
        dst->symbol, src->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    return RTI_TRUE;
}


RTIBool 
QueryConditionTestPlugin_instance_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    DDS_KeyHash_t *keyhash,
    const QueryConditionTest *instance)
{
    struct RTICdrStream * md5Stream = 0;

    md5Stream = PRESTypePluginDefaultEndpointData_getMD5Stream(endpoint_data);

    if (md5Stream == 0) {
        return RTI_FALSE;
    }

    RTIOsapiMemory_zero(
        RTICdrStream_getBuffer(md5Stream),
        RTICdrStream_getBufferLength(md5Stream));
    RTICdrStream_resetPosition(md5Stream);
    RTICdrStream_setDirtyBit(md5Stream, RTI_TRUE);

    if (!::QueryConditionTestPlugin_serialize_key(
            endpoint_data,instance,md5Stream, RTI_FALSE, RTI_CDR_ENCAPSULATION_ID_CDR_BE, RTI_TRUE,0)) {
        return RTI_FALSE;
    }
    
    if (PRESTypePluginDefaultEndpointData_getMaxSizeSerializedKey(endpoint_data) > (unsigned int)(MIG_RTPS_KEY_HASH_MAX_LENGTH)) {
        RTICdrStream_computeMD5(md5Stream, keyhash->value);
    } else {
        RTIOsapiMemory_zero(keyhash->value,MIG_RTPS_KEY_HASH_MAX_LENGTH);
        RTIOsapiMemory_copy(
            keyhash->value, 
            RTICdrStream_getBuffer(md5Stream), 
            RTICdrStream_getCurrentPositionOffset(md5Stream));
    }

    keyhash->length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
    return RTI_TRUE;
}


RTIBool 
QueryConditionTestPlugin_serialized_sample_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    DDS_KeyHash_t *keyhash,
    RTIBool deserialize_encapsulation,
    void *endpoint_plugin_qos) 
{   
    char * position = 0;
    QueryConditionTest * sample;



    if(deserialize_encapsulation) {
        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }


    sample = (QueryConditionTest *)
                PRESTypePluginDefaultEndpointData_getTempSample(endpoint_data);

    if (sample == 0) {
        return RTI_FALSE;
    }


    if (!RTICdrStream_deserializeString(
        stream, sample->symbol, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }


    if (!::QueryConditionTestPlugin_instance_to_keyhash(
            endpoint_data, keyhash, sample)) {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}
 

/* ------------------------------------------------------------------------
 * Plug-in Installation Methods
 * ------------------------------------------------------------------------ */
 
struct PRESTypePlugin *QueryConditionTestPlugin_new(void) 
{ 
    struct PRESTypePlugin *plugin = 0;
    const struct PRESTypePluginVersion PLUGIN_VERSION = 
        PRES_TYPE_PLUGIN_VERSION_2_0;

    RTIOsapiHeap_allocateStructure(
        &plugin, struct PRESTypePlugin);
    if (plugin == 0) {
       return 0;
    }

    plugin->version = PLUGIN_VERSION;

    /* set up parent's function pointers */
    plugin->onParticipantAttached =
        (PRESTypePluginOnParticipantAttachedCallback)
        ::QueryConditionTestPlugin_on_participant_attached;
    plugin->onParticipantDetached =
        (PRESTypePluginOnParticipantDetachedCallback)
        ::QueryConditionTestPlugin_on_participant_detached;
    plugin->onEndpointAttached =
        (PRESTypePluginOnEndpointAttachedCallback)
        ::QueryConditionTestPlugin_on_endpoint_attached;
    plugin->onEndpointDetached =
        (PRESTypePluginOnEndpointDetachedCallback)
        ::QueryConditionTestPlugin_on_endpoint_detached;

    plugin->copySampleFnc =
        (PRESTypePluginCopySampleFunction)
        ::QueryConditionTestPlugin_copy_sample;
    plugin->createSampleFnc =
        (PRESTypePluginCreateSampleFunction)
        QueryConditionTestPlugin_create_sample;
    plugin->destroySampleFnc =
        (PRESTypePluginDestroySampleFunction)
        QueryConditionTestPlugin_destroy_sample;

    plugin->serializeFnc =
        (PRESTypePluginSerializeFunction)
        ::QueryConditionTestPlugin_serialize;
    plugin->deserializeFnc =
        (PRESTypePluginDeserializeFunction)
        ::QueryConditionTestPlugin_deserialize;
    plugin->getSerializedSampleMaxSizeFnc =
        (PRESTypePluginGetSerializedSampleMaxSizeFunction)
        ::QueryConditionTestPlugin_get_serialized_sample_max_size;
    plugin->getSerializedSampleMinSizeFnc =
        (PRESTypePluginGetSerializedSampleMinSizeFunction)
        ::QueryConditionTestPlugin_get_serialized_sample_min_size;

    plugin->getSampleFnc =
        (PRESTypePluginGetSampleFunction)
        QueryConditionTestPlugin_get_sample;
    plugin->returnSampleFnc =
        (PRESTypePluginReturnSampleFunction)
        QueryConditionTestPlugin_return_sample;

    plugin->getKeyKindFnc =
        (PRESTypePluginGetKeyKindFunction)
        ::QueryConditionTestPlugin_get_key_kind;


    plugin->getSerializedKeyMaxSizeFnc =   
        (PRESTypePluginGetSerializedKeyMaxSizeFunction)
        ::QueryConditionTestPlugin_get_serialized_key_max_size;
    plugin->serializeKeyFnc =
        (PRESTypePluginSerializeKeyFunction)
        ::QueryConditionTestPlugin_serialize_key;
    plugin->deserializeKeyFnc =
        (PRESTypePluginDeserializeKeyFunction)
        ::QueryConditionTestPlugin_deserialize_key;
    plugin->deserializeKeySampleFnc =
        (PRESTypePluginDeserializeKeySampleFunction)
        ::QueryConditionTestPlugin_deserialize_key_sample;

    plugin->instanceToKeyHashFnc = 
        (PRESTypePluginInstanceToKeyHashFunction)
        ::QueryConditionTestPlugin_instance_to_keyhash;
    plugin->serializedSampleToKeyHashFnc = 
        (PRESTypePluginSerializedSampleToKeyHashFunction)
        ::QueryConditionTestPlugin_serialized_sample_to_keyhash;

    plugin->getKeyFnc =
        (PRESTypePluginGetKeyFunction)
        QueryConditionTestPlugin_get_key;
    plugin->returnKeyFnc =
        (PRESTypePluginReturnKeyFunction)
        QueryConditionTestPlugin_return_key;

    plugin->instanceToKeyFnc =
        (PRESTypePluginInstanceToKeyFunction)
        ::QueryConditionTestPlugin_instance_to_key;
    plugin->keyToInstanceFnc =
        (PRESTypePluginKeyToInstanceFunction)
        ::QueryConditionTestPlugin_key_to_instance;
    plugin->serializedKeyToKeyHashFnc = 0; /* Not supported yet */
    
    plugin->typeCode =  (struct RTICdrTypeCode *)::QueryConditionTest_get_typecode();
    
    plugin->languageKind = PRES_TYPEPLUGIN_DDS_TYPE; 

    /* Serialized buffer */
    plugin->getBuffer = 
        (PRESTypePluginGetBufferFunction)
        QueryConditionTestPlugin_get_buffer;
    plugin->returnBuffer = 
        (PRESTypePluginReturnBufferFunction)
        QueryConditionTestPlugin_return_buffer;
    plugin->getSerializedSampleSizeFnc =
        (PRESTypePluginGetSerializedSampleSizeFunction)
        ::QueryConditionTestPlugin_get_serialized_sample_size;

    plugin->endpointTypeName = QueryConditionTestTYPENAME;

    return plugin;
}

void
QueryConditionTestPlugin_delete(struct PRESTypePlugin *plugin)
{
    RTIOsapiHeap_freeStructure(plugin);
} 
