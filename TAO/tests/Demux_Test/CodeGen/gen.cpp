#include "tao.h"

static long GenDistinctString(ACE_Unbounded_Queue<ACE_CString> &, int);
static long GenRand(int, int);
static long IsPresent(ACE_Unbounded_Queue<ACE_CString> &, int, ACE_CString &);
static ACE_CString	CreateString(long);
static char	GetVarChar(int);

// object table generator
long GenObjectTable(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  int i,j,k;	// indices
  long	rnd;	// random number
  int opt;
  fstream arrfp;
  ACE_CString *str;


  // Generate "limit" distinct strings
  for (i=0; i < limit; i++)
    {
      if (GenDistinctString(arr, i) == -1) // side effect: inserts a new string
	 return -1;                        // at locn i in array "arr" 
    }
	
  // write the strings into a file whose format will be understood by GPERF
  arrfp.open("gperf_objects.dat",ios::out);
  
  arrfp << "struct object_db {" << endl;
  arrfp << "\tchar *name; // name of method" << endl;
  arrfp << "\tCORBA_Object_ptr obj; //fn pointer to obj impl" << endl;
  arrfp << "};" << endl;
  arrfp << "%%" << endl;

  for(i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	{
	  // error
	  return -1;
	}
      arrfp << str->rep() << ", " << "0" << endl;
      // we do not know the addresses of the objects. We shall insert them
      // later. 
    }
  arrfp.close();
  return 0;
}

// method table generator
long GenMethodTable(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  int i,j,k;	// indices
  long	rnd;	// random number
  int opt;
  fstream arrfp;
  ACE_CString *str;


  // Generate "limit" distinct strings
  for (i=0; i < limit; i++){
    if (GenDistinctString(arr, i) == -1) // side effect: inserts a new string at locn i
	return -1;			 // in array "arr" 
  }
	
  // write the strings into a file whose format will be understood by GPERF
  arrfp.open("gperf_methods.dat",ios::out);
  
  arrfp << "struct method_db {" << endl;
  arrfp << "\tchar *name; // name of method" << endl;
  arrfp << "\tTAO_Skeleton skel_ptr; //fn pointer to skeleton" << endl;
  arrfp << "};" << endl;
  arrfp << "%%" << endl;

  for(i=0; i < limit; i++)
    {
      if (arr.get(str, i) == -1)
	{
	  // error
	  return -1;
	}
      arrfp << (str->rep()) << ", " << "_skel_tao_demux::" <<  str->rep() <<
	"_skel" << endl;
    }
  arrfp.close();
  return 0;
}


// generate a distinct string and insert it at location "limit"
long GenDistinctString(ACE_Unbounded_Queue<ACE_CString> &arr, int limit)
{
  int i, j;	// indices
  long rnd;
  ACE_CString s;
  long status;

  rnd = GenRand(3,32); // get a random number between 3 and 32 which will be
  // used as a string length of the distinct string to be
  // generated. We use 3-32 because certain function
  // names such as _N, _C cause name conflicts.
  // Continue generating strings until a distinct one not generated before
  // is formed
  do {
    s = CreateString(rnd);
    status = IsPresent (arr, limit, s);
    if (status == -1)
      return -1;
  } while (status > 0);

  // save it at this location
  if (arr.enqueue_tail(s) == -1)
    {
      return -1;
    }  
  return 0;
}

// generate a random number in the given limits
long GenRand(int low, int up)
{
  return (lrand48() % (up-low+1)) + low; // rnd will be the size of the 
}

// check if the given string is present in the array
long IsPresent(ACE_Unbounded_Queue<ACE_CString> &arr, int limit, ACE_CString &s)
{
  ACE_CString *str;

  if (!strcasecmp(s.rep(), "_is_a"))
    return 1;

  for (int i=0; i < limit; i++){
    if (arr.get(str, i) == -1)
      {
	return -1;
      }
    if (!strcasecmp(str->rep(), s.rep()))
      return 1;  // they are same
  }
  return 0; 	// not present
}

// create a string of length = len
ACE_CString	CreateString(long len)
{
  int i;
  ACE_CString s;
  char ch;

  for (i=0; i < len; i++){
    ch = GetVarChar(i);
    s += ACE_CString(&ch);
  }
  return s;
}

// Generate a valid character for a C++ variable
char GetVarChar(int i)
{
  // for i == 0, we cannot return a digit as the first character
  char ch;
  while(1) {
    ch = GenRand(48, 122);
    if (isdigit(ch) || isupper(ch) || islower(ch) || (ch == '_')){
      if ((i == 0) && (isdigit(ch) || (ch == '_')))
	continue;
      break;	//otherwise
    }
  }
  return ch;
}

