// Naming_Context.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Get_Opt.h"
#include "ace/Naming_Context.h"
#include "ace/Remote_Name_Space.h"
#include "ace/Local_Name_Space.h"
//#include "ace/Registry_Name_Space.h"

// Make life easier later on...

typedef ACE_Local_Name_Space <ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> LOCAL_NAME_SPACE;
typedef ACE_Local_Name_Space <ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> LITE_LOCAL_NAME_SPACE;

// The following Factory is used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Name
// Server client.

ACE_SVC_FACTORY_DEFINE (ACE_Naming_Context)

// Define the object that describes the service statically.
ACE_STATIC_SVC_DEFINE (ACE_Naming_Context,
		       "ACE_Naming_Context", ACE_SVC_OBJ_T, &ACE_SVC_NAME (ACE_Naming_Context),
		       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ, 0)

ACE_STATIC_SVC_REQUIRE (ACE_Naming_Context)

int
ACE_Naming_Context::info (char **strp, 
			  size_t length) const
{
  ACE_TRACE ("ACE_Naming_Context::info");
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%s\t#%s\n",
		   "ACE_Naming_Context",
		   "Proxy for making calls to a Name Server");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

int
ACE_Naming_Context::local (void)
{
  ACE_TRACE ("ACE_Naming_Context::local");
  return ACE_OS::strcmp (this->netnameserver_host_, "localhost") == 0
    || ACE_OS::strcmp (this->netnameserver_host_, this->hostname_) == 0;
}    

int
ACE_Naming_Context::open (Context_Scope_Type scope_in, int lite)
{
  ACE_TRACE ("ACE_Naming_Context::open");
  ACE_OS::hostname (this->hostname_, sizeof this->hostname_);

  this->netnameserver_host_ = this->name_options_->nameserver_host ();
  this->netnameserver_port_ = this->name_options_->nameserver_port ();

  // Perform factory operation to select appropriate type of
  // Name_Space subclass.

#if 0
//#if defined (ACE_WIN32)
  // Use ACE_Registry
  ACE_NEW_RETURN (this->name_space_, ACE_Registry_Name_Space (this->name_options_), -1);
#else
  if (scope_in == ACE_Naming_Context::NET_LOCAL && this->local () == 0)
    {
      // Use NET_LOCAL name space, set up connection with remote server.
      ACE_NEW_RETURN (this->name_space_, 
		      ACE_Remote_Name_Space (this->netnameserver_host_,
					     this->netnameserver_port_),
		      -1);
    }
  else // Use NODE_LOCAL or PROC_LOCAL name space.
    {
      if (lite)
	ACE_NEW_RETURN (this->name_space_, LITE_LOCAL_NAME_SPACE (scope_in, this->name_options_), -1);
      else
	ACE_NEW_RETURN (this->name_space_, LOCAL_NAME_SPACE (scope_in, this->name_options_), -1);
    }
#endif /* ACE_WIN32 */

  if (ACE_LOG_MSG->op_status ())
    ACE_ERROR_RETURN ((LM_ERROR, "NAME_SPACE::NAME_SPACE\n"), -1);          
  return 0;
}

int
ACE_Naming_Context::close (void)
{
  ACE_TRACE ("ACE_Naming_Context::close");

  delete this->name_space_;

  return 0;
}

ACE_Naming_Context::ACE_Naming_Context (void)
{
  ACE_TRACE ("ACE_Naming_Context::ACE_Naming_Context");
  ACE_NEW (this->name_options_, ACE_Name_Options);
}

ACE_Naming_Context::ACE_Naming_Context (Context_Scope_Type scope_in,
					int lite)
{
  ACE_TRACE ("ACE_Naming_Context::ACE_Naming_Context");

  ACE_NEW (this->name_options_, ACE_Name_Options);

  // Initialize.
  if (this->open (scope_in, lite) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Naming_Context::ACE_Naming_Context"));
}

ACE_Name_Options *
ACE_Naming_Context::name_options (void)
{
  return this->name_options_;
}

int 
ACE_Naming_Context::bind (const ACE_WString &name_in, 
			  const ACE_WString &value_in, 
			  const char *type_in)
{
  ACE_TRACE ("ACE_Naming_Context::bind");
  return this->name_space_->bind (name_in, value_in, type_in);
}

int 
ACE_Naming_Context::bind (const char *name_in, 
			  const char *value_in, 
			  const char *type_in)
{
  ACE_TRACE ("ACE_Naming_Context::bind");
  return this->bind (ACE_WString (name_in), ACE_WString (value_in), type_in);
}

int 
ACE_Naming_Context::rebind (const ACE_WString &name_in, 
			    const ACE_WString &value_in, 
			    const char *type_in)
{
  ACE_TRACE ("ACE_Naming_Context::rebind");
  return this->name_space_->rebind (name_in, value_in, type_in);
}

int 
ACE_Naming_Context::rebind (const char *name_in, 
			    const char *value_in, 
			    const char *type_in)
{
  ACE_TRACE ("ACE_Naming_Context::rebind");
  return rebind (ACE_WString (name_in), ACE_WString (value_in), type_in);
}

int 
ACE_Naming_Context::resolve (const ACE_WString &name_in, 
			     ACE_WString &value_out,
			     char *&type_out)
{
  ACE_TRACE ("ACE_Naming_Context::resolve");
  return this->name_space_->resolve (name_in, value_out, type_out);
}

int 
ACE_Naming_Context::resolve (const char *name_in, 
			     ACE_WString &value_out,
			     char *&type_out)
{
  ACE_TRACE ("ACE_Naming_Context::resolve");
  return this->resolve (ACE_WString (name_in), value_out, type_out);
}

int 
ACE_Naming_Context::resolve (const char *name_in, 
			     char *&value_out, 
			     char *&type_out)
{
  ACE_TRACE ("ACE_Naming_Context::resolve");
  ACE_WString val_str;

  if (this->resolve (ACE_WString (name_in), val_str, type_out) == -1)
    return -1;

  // Note that char_rep() *allocates* the memory!  Thus, caller is
  // responsible for deleting it!
  value_out = val_str.char_rep ();

  return value_out == 0 ? -1 : 0;
}

int  
ACE_Naming_Context::unbind (const ACE_WString &name_in)
{
  ACE_TRACE ("ACE_Naming_Context::unbind");
  return this->name_space_->unbind (name_in);
}

int 
ACE_Naming_Context::unbind (const char *name_in)
{
  ACE_TRACE ("ACE_Naming_Context::unbind");
  return this->unbind (ACE_WString (name_in));
}

int 
ACE_Naming_Context::list_names (ACE_PWSTRING_SET &set_out, 
				const ACE_WString &pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_names");
  return this->name_space_->list_names (set_out, pattern_in);
}

int 
ACE_Naming_Context::list_names (ACE_PWSTRING_SET &set_out, 
				const char *pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_names");
  return this->list_names (set_out, ACE_WString (pattern_in));
}

int 
ACE_Naming_Context::list_values (ACE_PWSTRING_SET &set_out, 
				 const ACE_WString &pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_values");
  return this->name_space_->list_values (set_out, pattern_in);
}

int 
ACE_Naming_Context::list_values (ACE_PWSTRING_SET &set_out, 
				 const char *pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_values");
  return this->list_values (set_out, ACE_WString (pattern_in));
}

int 
ACE_Naming_Context::list_types (ACE_PWSTRING_SET &set_out, 
				 const ACE_WString &pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_types");
  return this->name_space_->list_types (set_out, pattern_in);
}

int 
ACE_Naming_Context::list_types (ACE_PWSTRING_SET &set_out, 
				 const char *pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_types");
  return this->list_types (set_out, ACE_WString (pattern_in));
}

int 
ACE_Naming_Context::list_name_entries (ACE_BINDING_SET &set_out, 
				       const ACE_WString &pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_name_entries");
  return this->name_space_->list_name_entries (set_out, pattern_in);
}

int 
ACE_Naming_Context::list_name_entries (ACE_BINDING_SET &set_out, 
				       const char *pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_name_entries");
  return this->list_name_entries (set_out, ACE_WString (pattern_in));
}

int 
ACE_Naming_Context::list_value_entries (ACE_BINDING_SET &set_out, 
					const ACE_WString &pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_value_entries");
  return this->name_space_->list_value_entries (set_out, pattern_in);
}

int 
ACE_Naming_Context::list_value_entries (ACE_BINDING_SET &set_out, 
					const char *pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_value_entries");
  return this->list_value_entries (set_out, ACE_WString (pattern_in));
}

int 
ACE_Naming_Context::list_type_entries (ACE_BINDING_SET &set_out, 
				       const ACE_WString &pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_type_entries");
  return this->name_space_->list_type_entries (set_out, pattern_in);
}

int 
ACE_Naming_Context::list_type_entries (ACE_BINDING_SET &set_out, 
				       const char *pattern_in)
{
  ACE_TRACE ("ACE_Naming_Context::list_type_entries");
  return this->list_type_entries (set_out, ACE_WString (pattern_in));
}


ACE_Naming_Context::~ACE_Naming_Context (void)
{
  ACE_TRACE ("ACE_Naming_Context::~ACE_Naming_Context");
  delete this->name_space_;
}

void
ACE_Naming_Context::dump ()
{
  ACE_TRACE ("ACE_Naming_Context::dump");
  this->name_space_->dump();
}

int 
ACE_Naming_Context::init (int argc, char *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Naming_Context::init\n"));
  this->name_options_->parse_args (argc, argv);
  return this->open (this->name_options_->context ());
}

int 
ACE_Naming_Context::fini (void)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Naming_Context::fini\n"));
  delete this->name_options_;
  return 0;
}

ACE_Name_Options::ACE_Name_Options (void)
  : debugging_ (0),
    verbosity_ (0),
    nameserver_port_ (ACE_DEFAULT_SERVER_PORT),
    nameserver_host_ (ACE_OS::strdup (ACE_DEFAULT_SERVER_HOST)),
    namespace_dir_  (ACE_OS::strdup (ACE_DEFAULT_NAMESPACE_DIR)),
    process_name_ (0),
    database_ (0),
    base_address_ (ACE_DEFAULT_BASE_ADDR)
{
  ACE_TRACE ("ACE_Name_Options::ACE_Name_Options");
}

ACE_Name_Options::~ACE_Name_Options (void)
{
  ACE_TRACE ("ACE_Name_Options::~ACE_Name_Options");

  ACE_OS::free ((void *) this->nameserver_host_);
  ACE_OS::free ((void *) this->namespace_dir_ );
  ACE_OS::free ((void *) this->process_name_ );
  ACE_OS::free ((void *) this->database_ );
}

void   
ACE_Name_Options::nameserver_port (int port)
{
  ACE_TRACE ("ACE_Name_Options::nameserver_port");
  this->nameserver_port_ = port;
}

int
ACE_Name_Options::nameserver_port (void)
{
  ACE_TRACE ("ACE_Name_Options::nameserver_port");
  return this->nameserver_port_;
}

void   
ACE_Name_Options::namespace_dir (const char *dir)
{
  ACE_TRACE ("ACE_Name_Options::namespace_dir");
  this->namespace_dir_ = ACE_OS::strdup (dir);
}

void   
ACE_Name_Options::process_name (const char *pname)
{
  ACE_TRACE ("ACE_Name_Options::process_name");
  const char *t = ACE::basename (pname, ACE_DIRECTORY_SEPARATOR_CHAR);
  this->process_name_ = ACE_OS::strdup (t);
}

void   
ACE_Name_Options::nameserver_host (const char *host)
{
  ACE_TRACE ("ACE_Name_Options::nameserver_host");
  this->nameserver_host_ = ACE_OS::strdup (host);
}

const char *
ACE_Name_Options::nameserver_host (void)
{
  ACE_TRACE ("ACE_Name_Options::nameserver_host");
  return this->nameserver_host_;
}

const char *
ACE_Name_Options::database (void)
{
  ACE_TRACE ("ACE_Name_Options::database");
  return this->database_;
}

void   
ACE_Name_Options::database (const char *db)
{
  ACE_TRACE ("ACE_Name_Options::database");
  if (this->database_ != 0)
    ACE_OS::free ((void *) this->database_);
  this->database_ = ACE_OS::strdup (db);
}

char *
ACE_Name_Options::base_address (void)
{
  ACE_TRACE ("ACE_Name_Options::database");
  return this->base_address_;
}

void   
ACE_Name_Options::base_address (char *base_address)
{
  ACE_TRACE ("ACE_Name_Options::base_address");
  this->base_address_ = base_address;
}

ACE_Naming_Context::Context_Scope_Type
ACE_Name_Options::context (void)
{
  ACE_TRACE ("ACE_Name_Options::context");
  return this->context_;
}

void   
ACE_Name_Options::context (ACE_Naming_Context::Context_Scope_Type context)
{
  ACE_TRACE ("ACE_Name_Options::context");
  this->context_ = context;
}

const char *
ACE_Name_Options::process_name (void)
{
  ACE_TRACE ("ACE_Name_Options::process_name");
  return this->process_name_;
}

const char *
ACE_Name_Options::namespace_dir (void)
{
  ACE_TRACE ("ACE_Name_Options::namespace_dir");
  return this->namespace_dir_;
}

int 
ACE_Name_Options::debug (void)
{
  ACE_TRACE ("ACE_Name_Options::debug");
  return this->debugging_;
}

int 
ACE_Name_Options::verbose (void)
{
  ACE_TRACE ("ACE_Name_Options::verbose");
  return this->verbosity_;
}

void
ACE_Name_Options::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Name_Options::parse_args");
  ACE_LOG_MSG->open (argv[0]);
  this->process_name (argv[0]);

  // Default is to use the PROC_LOCAL context...
  this->context (ACE_Naming_Context::PROC_LOCAL);

  // Make the database name the same as the process name by default
  // (note that this makes a copy of the process_name_ so that we can
  // clean it up in the destructor).
  this->database (this->process_name ());

  ACE_Get_Opt get_opt (argc, argv, "b:c:dh:l:P:p:s:T:v");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'c':
	{
	  if (ACE_OS::strcmp (get_opt.optarg, "PROC_LOCAL"))
	    this->context (ACE_Naming_Context::PROC_LOCAL);
	  else if (ACE_OS::strcmp (get_opt.optarg, "NODE_LOCAL"))
	    this->context (ACE_Naming_Context::NODE_LOCAL);
	  else if (ACE_OS::strcmp (get_opt.optarg, "NET_LOCAL"))
	    this->context (ACE_Naming_Context::NET_LOCAL);
	}
	break;
      case 'd':
	this->debugging_ = 1;
	break;
      case 'h':
	this->nameserver_host (get_opt.optarg);
	break;
      case 'l':
	this->namespace_dir (get_opt.optarg);
	break;
      case 'P':
	this->process_name (get_opt.optarg);
	break;
      case 'p':
	this->nameserver_port (ACE_OS::atoi (get_opt.optarg));
	break;
      case 's':
	this->database (get_opt.optarg);
	break;
      case 'b':
	this->base_address ((char *) ACE_OS::atoi (get_opt.optarg));
	break;
      case 'T':
	if (ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0)
	  ACE_Trace::start_tracing ();
	else if (ACE_OS::strcasecmp (get_opt.optarg, "OFF") == 0)
	  ACE_Trace::stop_tracing ();
	break;
      case 'v':
	this->verbosity_ = 1;
	break;
      default:
	ACE_OS::fprintf (stderr, "%s\n"
			 "\t[-d] (enable debugging)\n"
			 "\t[-h nameserver host]\n"
			 "\t[-l namespace directory]\n"
			 "\t[-P processname]\n"
			 "\t[-p nameserver port]\n"
			 "\t[-s database name]\n"
			 "\t[-b base address]\n"
			 "\t[-v] (verbose) \n",
			 argv[0]);
	/* NOTREACHED */
	break;
      }
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Local_Name_Space <ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Local_Name_Space <ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Malloc<ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> >;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> >;
template class ACE_Name_Space_Map <ACE_Allocator_Adapter <ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> > >;
template class ACE_Name_Space_Map <ACE_Allocator_Adapter <ACE_Malloc <ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> > >;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
