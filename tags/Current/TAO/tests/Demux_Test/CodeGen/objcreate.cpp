#include "tao.h"

long GenObjectCreateCode(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  // code for creating objects. The strategy will be based on active demux or
  // other strategies
  fstream file;
  int i;
  ACE_CString *str;

  file.open("obj_create.i", ios::out);

  file << "\t  if (params->demux_strategy() == " <<
    "TAO_OA_Parameters::TAO_ACTIVE_DEMUX){" << endl;

  for (i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	return -1;

      file << "\t\tmymux[" << i << "] = new tao_demux_i(\"" << i << "\");" <<
	endl; 
      file << "\t\tstr = orb_ptr->object_to_string(mymux[" << i << "], env);"
	   << endl;
      file << "#ifdef DEBUG" << endl;
      file << "\t\tcout << \"IOR = \" << (char *)str << endl;" << endl;
      file << "#endif" << endl;
      file << "\t\tiorfile << (char *)str << endl; " << endl;
      file << "\t\tCORBA_string_free(str);" << endl;
    }
  file << "\t  } else {" << endl;
  for (i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	return -1;

      file << "\t\tmymux[" << i << "] = new tao_demux_i(CORBA_string_dup (\""
	   << str->rep() << "\"));" << endl; 
      file << "\t\tstr = orb_ptr->object_to_string(mymux[" << i << "], env);"
	   << endl;
      file << "#ifdef DEBUG" << endl;
      file << "\t\tcout << \"IOR = \" << (char *)str << endl;" << endl;
      file << "#endif" << endl;
      file << "\t\tiorfile << (char *)str << endl; " << endl;
      file << "\t\tCORBA_string_free(str);" << endl;
    }
  file << "\t  }" << endl;

  file.close();
  return 0;
}
