#include "tao.h"


// Generate the client side include file "client.i". This file has a table of
// function pointers to the the different operations of the
// interface. Depending on the strategy chosen, we make appropriate calls

long GenClientCode(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  fstream client_incl;
  int i;
  ACE_CString *str;

  client_incl.open("client.i", ios::out);

  for(i=0; i < limit; i++){
    if (arr.get(str, i) == -1)
      {
	return -1;
      }
    client_incl << "void " << str->rep() << "(tao_demux_ptr obj)" << endl;
    client_incl << "{" << endl;
    client_incl << "\tCORBA_Environment env;" << endl << endl;
    client_incl << "\tobj->" << str->rep() << "(env);" << endl;
    client_incl << "}" << endl;
  }
  client_incl << endl;
  client_incl << "static const method_db mtbl[] = {" << endl;
  for (i=0; i < limit; i++){
    if (arr.get(str, i) == -1)
      {
	return -1;
      }
    client_incl << "\t" << str->rep() << "," << endl;
  }
  client_incl << "};" << endl << endl;
  client_incl.close();
}

