#include "tao.h"

long GenServerHeader(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  fstream hdr;
  int i;
  ACE_CString *str;

  hdr.open("tao_demuxS.h", ios::out);

  hdr << "#if !defined(TAO_DEMUX_S_H)" << endl;
  hdr << "#define TAO_DEMUX_S_H" << endl << endl;

  hdr << "#include \"tao_demuxC.h\"" << endl;
  hdr << "class _skel_tao_demux: public tao_demux" << endl;
  hdr << "{" << endl;
  hdr << "protected:" << endl;
  hdr << "\t_skel_tao_demux(const char *objname=0);" << endl;
  hdr << "\tvirtual ~_skel_tao_demux() {}" << endl;
  hdr << "public:" << endl;
  hdr << "\t//All methods defined on this interface" << endl;
  for(i=0; i < limit; i++){
    if (arr.get(str, i) == -1)
      {
	return -1;
      }
    //generate skeletons
    hdr << "\tvirtual void " << str->rep() <<
      "(CORBA_Environment &env) = 0;" << endl;
  }
  hdr << endl;
  hdr << "\t// All skeletons used for dispatching" << endl;
  for(i=0; i < limit; i++){
    if (arr.get(str, i) == -1)
      {
	return -1;
      }
    //generate skeletons
    hdr << "\tstatic void " << str->rep() <<
      "_skel(CORBA_ServerRequest &req, CORBA_Object_ptr obj, " <<
      "CORBA_Environment &env);" << endl;
  }
  hdr << "\tstatic void " << 
    "_is_a_skel(CORBA_ServerRequest &req, CORBA_Object_ptr obj, " <<
    "CORBA_Environment &env);" << endl;
  hdr << "};" << endl;

  hdr << "#endif" << endl;
  hdr.close();
}

// generate server side skeletons
long GenServerSkels(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  fstream skel;
  int i;
  ACE_CString *str;

  skel.open("tao_demuxS.cpp", ios::out);

  skel << "#include \"tao_demuxS.h\"" << endl;
  //  skel << "#include \"iiopobj.h\"" << endl;
  // skel << "#include \"params.h\"" << endl;
  // skel << "#include \"corba/optable.h\"" << endl;
  // skel << "#include \"corba/corbacom.h\"" << endl << endl;

  // table of methods
  skel << "static const TAO_operation_db_entry tao_demux_operations [] = {" << endl;
  for (i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	{
	  return -1;
	}
      skel << "\t{\"" << str->rep() << "\", &_skel_tao_demux::" << str->rep() <<
	"_skel}," << endl;
      
    }
  skel << "\t{\"_is_a\", &_skel_tao_demux::_is_a_skel}" << endl;
  skel << "};" << endl;

  // table of methods
  skel << "static const TAO_operation_db_entry tao_active_demux_operations [] = {" << endl;
  for (i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	{
	  return -1;
	}
      skel << "\t{\"" << i << "\", &_skel_tao_demux::" << str->rep() <<
	"_skel}," << endl;
      
    }
  skel << "\t{\"" << limit << "\", &_skel_tao_demux::_is_a_skel}" << endl;
  skel << "};" << endl;

  // instantiate operation lookup tables
  skel << "// instantiate a dynamic hash operation table" << endl;
  skel << "TAO_Dynamic_Hash_OpTable dyn_hash_tao_demux_op_tbl(" <<
    "tao_demux_operations, " << (limit+1) << ", " << (limit+1)*2 << ");" << endl;

  skel << "// instantiate a linear search based operation table" << endl;
  skel << "TAO_Linear_OpTable linear_tao_demux_op_tbl(" <<
    "tao_demux_operations, " << limit+1 << ");" << endl;

  skel << "// instantiate a active demux operation table" << endl;
  skel << "TAO_Active_Demux_OpTable active_tao_demux_op_tbl(" <<
    "tao_active_demux_operations, " << limit+1 << ");" << endl;

  skel << "// constructor for skel_tao_demux" << endl;
  skel << "_skel_tao_demux::_skel_tao_demux(const char *obj_name)" << endl;
  //  skel << "\t:optable_(&tao_demux_op_tbl)" << endl;
  skel << "{" << endl;
  skel << "\tconst char* repoID = \"IDL:tao_demux:1.0\";" << endl;
  skel << "\tCORBA_String_var  _tao_typeid = repoID;" << endl;
  skel << "\tIIOP_Object *data; // IIOP object reference" << endl;
  skel << "\tCORBA_BOA_ptr oa = TAO_OA_PARAMS::instance()->oa(); // our OA" << endl;
  skel << "\tTAO_Operation_Table_Parameters *op_params = " << endl;
  skel << "\t\tTAO_OP_TABLE_PARAMETERS::instance();" << endl << endl;
  skel << "\tCORBA_Long i;" << endl;
  
  // set the operation table based on user's choice
  skel << "\tswitch(op_params->lookup_strategy()){" << endl;
  skel << "\tcase TAO_Operation_Table_Parameters::TAO_ACTIVE_DEMUX:" << endl;
  skel << "\t\tthis->optable_ = &active_tao_demux_op_tbl;" << endl;
  skel << "\t\tbreak;" << endl;
  skel << "\tcase TAO_Operation_Table_Parameters::TAO_LINEAR:" << endl;
  skel << "\t\tthis->optable_ = &linear_tao_demux_op_tbl;" << endl;
  skel << "\t\tbreak;" << endl;
  skel << "\tcase TAO_Operation_Table_Parameters::TAO_DYNAMIC_HASH:" << endl;
  skel << "\t\tthis->optable_ = &dyn_hash_tao_demux_op_tbl;" << endl;
  skel << "\t\tbreak;" << endl;
  skel << "\tcase TAO_Operation_Table_Parameters::TAO_PERFECT_HASH:" << endl;
  skel << "\t\tthis->optable_ = op_params->concrete_strategy ();" << endl;
  skel << "\t\tbreak;" << endl;
  skel << "\tcase TAO_Operation_Table_Parameters::TAO_USER_DEFINED:" << endl;
  skel << "\t\tthis->optable_ = op_params->concrete_strategy ();" << endl;
  skel << "\t\tbreak;" << endl;
  skel << "\t}" << endl << endl;

  // code to create IIOP object
  skel << "\tdata = new IIOP_Object(CORBA_string_dup(_tao_typeid));" << endl;
  skel << "\tdata->profile.iiop_version.major = IIOP::MY_MAJOR;" << endl;
  skel << "\tdata->profile.iiop_version.minor = IIOP::MY_MINOR;" << endl;
  skel << "\tdata->profile.host = ACE_OS::strdup(oa->get_addr().get_host_name());" << endl;
  skel << "\tdata->profile.port = oa->get_addr().get_port_number();" << endl;
  skel << "\tdata->profile.object_key.length = ACE_OS::strlen(obj_name);" << endl;
  skel << "\tdata->profile.object_key.maximum = " <<
    "data->profile.object_key.length;" << endl;
  skel << "\tdata->profile.object_key.buffer = " << 
    "new CORBA_Octet [(size_t)data->profile.object_key.length+1];" << endl;
  skel << "\tACE_OS::memcpy(data->profile.object_key.buffer, obj_name, " <<
    "data->profile.object_key.length);" << endl;
  skel << "\tthis->set_parent(data);" << endl;
  skel << "\tthis->sub_ = this;" << endl;
  skel << "\tif (oa) oa->bind(data->profile.object_key, this);" <<
    "// register ourselves" << endl;
  skel << "}" << endl << endl;

  // now the code for each skeleton
  for(i=0; i < limit; i++){
    if (arr.get(str, i) == -1)
      {
	return -1;
      }
    skel << "void _skel_tao_demux::" << str->rep() <<
      "_skel(CORBA_ServerRequest &req, CORBA_Object_ptr obj, " <<
      "CORBA_Environment&env)" << endl;
    skel << "{" << endl;
    skel << "\tCORBA_NVList_ptr nvlist;" << endl;
    skel << "\ttao_demux_ptr impl;" << endl << endl;

    // create a NVlist and populate it
    skel << "\treq.orb()->create_list(0, nvlist);" << endl;
    skel << "\treq.params(nvlist,env); //useless operation, but required "
	 << endl; 
    skel << "\t impl = (tao_demux_ptr) (obj->get_subclass());" << endl;
    skel << "\t// now the magic of dynamic binding" << endl;
    skel << "\timpl->" << str->rep() << "(env);" << endl;
    skel << "}" << endl;
  }

  // finally the code for _is_a
  skel << "void _skel_tao_demux::_is_a_skel" <<
    "(CORBA_ServerRequest &req, CORBA_Object_ptr obj, " <<
    "CORBA_Environment&env)" << endl;
  skel << "{" << endl;
  skel << "\tCORBA_NVList_ptr nvlist;" << endl;
  skel << "\tCORBA_NamedValue_ptr nv;" << endl;
  skel << "\tCORBA_Any  temp_value(_tc_CORBA_String);" << endl;
  skel << "\tchar *_tao_typeid = \"IDL:tao_demux:1.0\";" << endl << endl;

  // create a NVlist and populate it
  skel << "\treq.orb()->create_list(0, nvlist);" << endl;
  skel << "\tnv = nvlist->add_value(0, temp_value, CORBA_ARG_IN, env);" << endl;
  skel << "\treq.params(nvlist,env);" << endl;
  skel << "\tif (env.exception() != 0)" << endl;
  skel << "\t  {" << endl;
  skel << "\t\tdexc(env, \"is_a_skel, params\");" << endl;
  skel << "\t\treturn;" << endl;
  skel << "\t  }" << endl << endl;
  skel << "\tCORBA_Boolean *retval;" << endl;
  skel << "\tCORBA_String value = *(CORBA_String *)nv->value()->value();" << endl;
  skel << "\tif (strcmp((char *)value,(char *)_tao_typeid) == 0" << endl;
  skel << "\t   || strcmp((char *)value, _tc_CORBA_Object->id(env)) == 0)" << endl;
  skel << "\t\tretval = new CORBA_Boolean(CORBA_B_TRUE);" << endl;
  skel << "\telse" << endl;
  skel << "\t\tretval = new CORBA_Boolean(CORBA_B_FALSE);" << endl;
  skel << "\tCORBA_Any *any = new CORBA_Any(_tc_CORBA_Boolean, retval," <<
    "CORBA_B_TRUE);" << endl;
  skel << "\treq.result(any, env);" << endl;
  skel << "\tdexc(env, \"_is_a, result\");" << endl;
  skel << "}" << endl;
  
  skel.close();
}

