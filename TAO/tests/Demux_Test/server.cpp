#include "tao_demux_i.h"

#ifdef USE_QUANTIFY
#include <quantify.h>
#endif

#include <iostream.h>
#include <fstream.h>

#include "ace/SString.h"

void print_exception (const CORBA_Exception     *x,
		      const char                  *info,
		      FILE                        *stream
		      );

#if !defined (__cplusplus)
typedef void (*SIG_TYP)(); 
#endif 

#ifdef SVR4
void
sigpipe (int foo)
#else
  void
sigpipe ()
#endif
{
}

//****************** perf hash for obj lookup *************
// for perfect hash
struct object_db 
{
  char *name; // name of method
  CORBA_Object_ptr obj; //fn pointer to obj impl
};

// include the perfect hash code for objects
#include "gperf_object.i"

// Perfect hash table class
class TAO_Perfect_Hash_ObjTable: public TAO_Object_Table
{
public:
  TAO_Perfect_Hash_ObjTable (Object_Hash *oh);

  ~TAO_Perfect_Hash_ObjTable (void);

  virtual int bind (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr obj);
  // Registers a CORBA_Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

private:
  Object_Hash *hash_;
};

TAO_Perfect_Hash_ObjTable::TAO_Perfect_Hash_ObjTable (Object_Hash *oh)
  : hash_ (oh)
{
}

TAO_Perfect_Hash_ObjTable::~TAO_Perfect_Hash_ObjTable (void)
{
}

int
TAO_Perfect_Hash_ObjTable::bind (const CORBA_OctetSeq &key, CORBA_Object_ptr obj)
{
  struct object_db *entry;

  ACE_CString objkey ((char *)key.buffer, key.length);
  entry = this->hash_->in_word_set (objkey.rep (), key.length);
  if (entry != 0)
    {
      // now fill up the entry
      entry->obj = obj;
    }
  else
    {
      return -1; // error
    }
}

int
TAO_Perfect_Hash_ObjTable::find (const CORBA_OctetSeq &key, CORBA_Object_ptr &obj)
{
  struct object_db *entry;

  ACE_CString objkey ((char *)key.buffer, key.length);
  entry = this->hash_->in_word_set (objkey.rep (), key.length);

  if (entry != 0)
    {
      obj = entry->obj;
      return 0;
    }
  else
    {
      return -1;
    }
}

//****************** perf hash for opname lookup *************
struct method_db 
{
  char *name; // name of method
  TAO_Skeleton skel_ptr_; //fn pointer to obj impl
};

#include "gperf_method.i"

// Perfect hash table class
class TAO_Perfect_Hash_Op_Table: public TAO_Operation_Table
{
public:
  TAO_Perfect_Hash_Op_Table (Method_Hash *mh);

  ~TAO_Perfect_Hash_Op_Table (void);

  virtual int find (const CORBA_String &opname,
		    TAO_Skeleton &skel_ptr);
  // Uses <{opname}> to look up the skeleton function and pass it back
  // in <{skelfunc}>.  Returns non-negative integer on success, or -1
  // on failure.

  virtual int bind (const CORBA_String &opname,
		    const TAO_Skeleton skelptr);
  // Associate the skeleton <{skel_ptr}> with an operation named
  // <{opname}>.  Returns -1 on failure, 0 on success, 1 on duplicate.
private:
  Method_Hash *hash_;
};

TAO_Perfect_Hash_Op_Table::TAO_Perfect_Hash_Op_Table (Method_Hash *mh)
  : hash_ (mh)
{
}

TAO_Perfect_Hash_Op_Table::~TAO_Perfect_Hash_Op_Table (void)
{
}

int
TAO_Perfect_Hash_Op_Table::bind (const CORBA_String &opname,
				 const TAO_Skeleton skelptr)
{
  return 0; // nothing to do
}

int
TAO_Perfect_Hash_Op_Table::find (const CORBA_String &opname,
				 TAO_Skeleton &skelptr)
{
  method_db *entry;

  entry = (method_db*) this->hash_->in_word_set (opname, ACE_OS::strlen (opname));
  if (entry != 0)
    {
      skelptr = entry->skel_ptr_;
      return 0;
    }
  else
    {
      return -1;
    }
}

//***********************************************************
char Usage[] = "\
Usage: server [options] \n\
Common options:\n\
-t demux_strategy: l(linear), g(GPERF), a (delayered active demux), d (dynamic hash)\n\
-i iter: number of iterations to expect from client \n\
";

extern CORBA_Double TAO_Avg;
extern CORBA_Long TAO_HowMany;
extern CORBA_Long TAO_Loop;
int
main (int argc, char *const *argv)
{
  unsigned long addr_tmp;
  int c;
  unsigned short done = 0;
  CORBA_Environment env;
  CORBA_ORB_ptr orb_ptr;
  CORBA_POA_ptr oa_ptr;
  int iter = 1;
  TAO_OA_Parameters    *params = TAO_OA_PARAMS::instance();
  char                *orb_name = "ior";
  int                 idle = -1;
  int use_ior = 0;
  int terminationStatus = 0;
  tao_demux_i **mymux;  // array of objects implementing the "demux" interface
  int  numObjs = 5, numMethods = 4;
  Object_Hash oh;
  TAO_Object_Table *objt = new TAO_Perfect_Hash_ObjTable (&oh);
  Method_Hash mh;
  TAO_Operation_Table *optbl = new TAO_Perfect_Hash_Op_Table (&mh);

  TAO_Operation_Table_Parameters *op_params =
     TAO_OP_TABLE_PARAMETERS::instance(); 
			 
  fstream iorfile;  // stores the object references of all the objects
  fstream outfile;
  CORBA_String str; // scratch area

  params->userdef_lookup_strategy (objt);

  orb_ptr = CORBA_ORB_init (argc, argv, orb_name, env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "ORB init", stdout);
    return 1;
  }

  oa_ptr = orb_ptr->BOA_init (argc, argv, "ROA");
  if (env.exception () != 0) {
    print_exception (env.exception (), "OA init", stdout);
    return 1;
  }

  TAO_debug_level = 0;
  // Now process the options other than Orbix specific options
  while ((c = getopt (argc, argv, "ui:t:o:m:")) != -1)
    {
      switch (c)
	{
	case 't':
	  switch (*optarg)
	    {
	    case 'l':
	      op_params->lookup_strategy
		(TAO_Operation_Table_Parameters::TAO_LINEAR); 
	      break;
	    case 'a':
	      op_params->lookup_strategy
		(TAO_Operation_Table_Parameters::TAO_ACTIVE_DEMUX);
	      break;
	    case 'g':
	      op_params->lookup_strategy
		(TAO_Operation_Table_Parameters::TAO_PERFECT_HASH); 
	      op_params->concrete_strategy (optbl);
	      break;
	    case 'd':
	    default:
	      op_params->lookup_strategy
		(TAO_Operation_Table_Parameters::TAO_DYNAMIC_HASH);
	      break;
	    }
	  break;
	case 'u':
	  use_ior = 1;
	  orb_name = "";
	  break;
	case 'i':
	  iter = atoi (optarg);
	  break;
	case 'o':
	  numObjs = atoi(optarg);
	  break;
	case 'm':
	  numMethods = atoi(optarg);
	  break;
	default:
	  goto usage;
	}
    }

  TAO_Avg = 0;
  TAO_Loop = 0;
  TAO_HowMany = iter*numObjs*numMethods;
    
  //
  // Receiver
  //

  iorfile.open ("ior.txt", ios::out);
  mymux = new tao_demux_i* [numObjs];

  // instantiate objects
#include "obj_create.i"
  iorfile.close();
  cout << "Server ready to handle events" << endl;

#if defined (USE_QUANTIFY)
  quantify_clear_data();
  quantify_start_recording_data();
#endif

  ACE_Service_Config::run_reactor_event_loop();

  outfile.open ("muxcost.dat", ios::out|ios::app);
  outfile << numObjs << "\t" << numMethods << "\t" << TAO_Avg << endl;
  outfile.close ();
  //
  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).
  //
  oa_ptr->clean_shutdown (env);
  return terminationStatus;
usage:
  fprintf (stderr, Usage);
  return(1);
}

