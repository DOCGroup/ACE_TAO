#include "tao.h"

// generate the implementation header
long GenServantHeader(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  // generate the implementation class
  fstream hdr;
  int i;
  ACE_CString *str;

  hdr.open("tao_demux_i.h", ios::out);

  hdr << "#if !defined(TAO_DEMUX_I_H)" << endl;
  hdr << "#define TAO_DEMUX_I_H" << endl << endl;

  hdr << "#include \"tao_demuxS.h\"" << endl << endl;

  hdr << "class tao_demux_i: public virtual _skel_tao_demux" << endl;
  hdr << "{" << endl;
  hdr << "public:" << endl;
  hdr << "\ttao_demux_i(char *obj_name);" << endl;
  hdr << "\t~tao_demux_i();" << endl;
  hdr << "\t//now all the methods" << endl;

  for(i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	return -1;

      hdr << "\tvoid " << str->rep() <<
	"(CORBA_Environment &env);" << endl; 
    }
  hdr << "};" << endl; 

  hdr << "#endif" << endl;
}


// generate the implementation
long GenServantImpl(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  // generate the implementation class
  fstream impl;
  int i;
  ACE_CString *str;

  impl.open("tao_demux_i.cpp", ios::out);

  impl << "#include <iostream.h>" << endl;
  impl << "#include \"tao_demux_i.h\"" << endl << endl;

  impl << "tao_demux_i::tao_demux_i(char *obj_name)" << endl;
  impl << "  :_skel_tao_demux(obj_name){}" << endl;
  impl << "tao_demux_i::~tao_demux_i(){}" << endl;
  impl << "\t//now all the methods" << endl;

  for(i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	return -1;

      impl << "void tao_demux_i::" << str->rep() <<
	"(CORBA_Environment &env)" << endl;
      impl << "{" << endl;
      impl << "#ifdef DEBUG" << endl;
      impl << "   cout << \"Object name = \" << this->_get_name(env) << " <<
	"\", Operation name = " << str->rep() << "\" << endl;" << endl; 
      impl << "#endif" << endl;
      impl << "}" << endl << endl;
    }
}

