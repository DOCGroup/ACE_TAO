#include "tao.h"

int main (int argc, char *argv[])
{
  // define the two arrays that will hold generated object keys and method
  // names of the hypothetical "tao_demux" interface
  ACE_Unbounded_Queue<ACE_CString> *objectArr, *methodArr;

  // provide default values to the limit on the number of object keys and
  // method names
  long objectLimit = 5, 
    methodLimit = 4;

  // for parsing the arguments
  ACE_Get_Opt get_opt (argc, argv, "hm:o:");

  for (int c; (c = get_opt ()) != EOF;)
    {
      switch (c)
	{
	case 'm': // number of methods
	  methodLimit = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'o': // number of object keys
	  objectLimit = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'h': // usage
	default:
	  Usage ();
	  return 1;
	}
    }

  cout << "Generating code for " << objectLimit << " objects and " <<
    methodLimit << " methods" << endl;
  objectArr = new ACE_Unbounded_Queue<ACE_CString> [objectLimit];
  methodArr = new ACE_Unbounded_Queue<ACE_CString> [methodLimit];

  // generate the object keys
  if (GenObjectTable (*objectArr, objectLimit) == -1)
    {
    }

  // generate method names
  if (GenMethodTable (*methodArr, methodLimit) == -1)
    {
    }

  // generate client side stub headers
  if (GenClientHeader (*methodArr, methodLimit) == -1)
    {
    }

  // generate client stubs
  if (GenClientStubs (*methodArr, methodLimit) == -1)
    {
    }

  // generate include code for client-side main program
  if (GenClientCode (*methodArr, methodLimit) == -1)
    {
    }

  // generate server skeleton header
  if (GenServerHeader (*methodArr, methodLimit) == -1)
    {
    }

  // generate server skeletons
  if (GenServerSkels (*methodArr, methodLimit) == -1)
    {
    }

  // generate th eimpl header
  if (GenServantHeader (*methodArr, methodLimit) == -1)
    {
    }

  // generate impl 
  if (GenServantImpl (*methodArr, methodLimit) == -1)
    {
    }

  // generate code that creates instances of objects
  if (GenObjectCreateCode (*objectArr, objectLimit) == -1)
    {
    }

  GenGPERFCode ();
}

void Usage (void)
{
  cerr << "Usage: tao [-m <#methods>] [-o <#objects>] [-h]" << endl;
  return;
}
