// $Id$

#include "ACEXML/common/XML_Types.h"
#include "ACEXML/common/StreamFactory.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/compass/SoftPkgHandler.h"
#include "ACEXML/compass/PropertyFile_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/ARGV.h"
#include "ace/Thread_Manager.h"
#include "ace/DLL.h"
#include "ace/SString.h"

struct ThrCreate
{
  const ACE_TCHAR* name;
  int argc;
  ACE_TCHAR** argv;
};


void*
create_dll (void* name)
{
  typedef int (*dll_func) (int, char**);
  ThrCreate* thr = ACE_reinterpret_cast (ThrCreate*, name);
  const ACE_TCHAR* dllname = thr->name;
  const int argc = thr->argc;
  ACE_TCHAR** argv = thr->argv;

  if (dllname == 0)
    {
      ACE_ERROR ((LM_ERROR, "Trying to create a dll with null name"));
      return (void*)-1;
    }
  ACE_DLL dll;
  if (dll.open (dllname) == -1)
    {
     ACE_ERROR ((LM_ERROR, "Failed to open Service: %m\n"));
     return (void*)-1;
    }

  void *foo;
  const ACE_TCHAR* create = "create";
  ACE_CString entrypoint (create);
  entrypoint += dllname;
  foo = dll.symbol (entrypoint.c_str());

  // Cast the void* to long first.
  long tmp = ACE_reinterpret_cast (long, foo);
  dll_func factory = ACE_reinterpret_cast (dll_func, tmp);
  if (factory == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to resolve factory: %s: %m\n"),
                  dll.error ()));
      return (void*)-1;
    }
  return (void*)factory (argc, argv);
}

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s [-l <filename>]\n")
              ACE_TEXT ("  -l: Install components from <filename>\n"),
              program));
}

static int
error_cleanup (const ACE_TCHAR* dir)
{
  if (rmdir (dir) < 0)
    ACE_ERROR ((LM_ERROR, "Unable to cleanup safe temp directory : %m\n"));
  return -1;
}

int
ACE_TMAIN (int argc, char* argv[])
{
  ACEXML_Char* filename = 0;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("l:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'l':
            filename = get_opt.opt_arg ();
            break;
          default:
            break;
        }
    }
  if (filename == 0)
    {
      usage(argv[0]);
      return -1;
    }
  ACE_ARGV myargv;

  const ACEXML_Char meta_inf[] = {'m', 'e', 't', 'a', '-', 'i', 'n', 'f', 0};
  ACEXML_Char* path = 0;
  size_t pos = ACE_OS::strlen (filename);
  ACEXML_Char* basename = 0;
  if ((basename = ACE_OS::strrchr (filename, '.')) != 0)
    pos = basename - filename;
  size_t len = pos + sizeof (meta_inf) + sizeof (ACE_TCHAR) * 2;
  ACE_NEW_RETURN (path, ACEXML_Char[len], -1);
  ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_path (path);

  ACE_OS::strncpy (path, filename, pos);
  path[pos++] = '/';

  // Store a copy for future use
  ACEXML_Char* dirname = 0;
  ACE_NEW_RETURN (dirname, ACEXML_Char[pos], -1);
  ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_dirname (dirname);
  ACE_OS::memcpy (dirname, path, pos);
  dirname[pos] = 0;

  ACE_OS::strcpy (path + pos, meta_inf);


  ACEXML_StreamFactory factory;
  ACEXML_CharStream* stream = factory.create_stream (path);
  if (!stream)
    {
      ACE_ERROR ((LM_ERROR, "Failed to create input stream from %s\n",
                  path));
      return -1;
    }

  ACEXML_SoftPkgHandler* handler = 0;
  ACE_NEW_RETURN (handler, ACEXML_SoftPkgHandler (path), -1);
  auto_ptr<ACEXML_SoftPkgHandler> cleanup_handler (handler);

  ACEXML_Parser parser;
  ACEXML_InputSource input (stream);

  parser.setContentHandler (handler);
  parser.setDTDHandler (handler);
  parser.setErrorHandler (handler);
  parser.setEntityResolver (handler);

  ACEXML_TRY_NEW_ENV
    {
      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;
    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ex.print();
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
    }
  ACEXML_ENDTRY;
  const ACEXML_SoftPkg* pkg = handler->get_softpkg();
  pkg->dump();

  ACEXML_Char temp[] = "acefileXXXXXX";
  if (mkdtemp (temp) == 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to create safe temp directory\n"));
      return -1;
    }
  char* oldpath = ACE_OS::getenv ("LD_LIBRARY_PATH");
  size_t env_len = 0;
  if (oldpath)
    env_len = ACE_OS::strlen (oldpath);
  env_len += 18;  // For 'LD_LIBRARY_PATH=' + ':' + '\0'
  ACEXML_Char pwd[MAXPATHLEN + 1];
  size_t size = sizeof (pwd);
  if (ACE_OS::getcwd(pwd, size) == 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to set up environment for the Component"
                  "location: %m\n"));
      return error_cleanup (temp);
    }
  ACEXML_String temp_dir (pwd);
  temp_dir += "/";
  temp_dir += temp;
  ACEXML_Char* new_path = 0;
  ACE_NEW_RETURN (new_path, ACEXML_Char[env_len + temp_dir.length()], -1);
  int n = 0;
  n += ACE_OS::sprintf (new_path, "LD_LIBRARY_PATH=%s", temp_dir.c_str());
  if (oldpath)
    ACE_OS::sprintf (new_path + n, ":%s", oldpath);
  ACE_DEBUG ((LM_DEBUG, "Added %s to environment\n", new_path));

  if (ACE_OS::putenv (new_path) < 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to set up environment for the Component"
                  "location: %m\n"));
      return error_cleanup (temp);
    }

  ACE_Thread_Manager* thr_mgr = ACE_Thread_Manager::instance();

  ThrCreate param;

  const ACEXML_SoftPkg::Implementations& impls = pkg->get_impls();
  ACEXML_SoftPkg::Implementation** entry = 0;
  ACEXML_SoftPkg::Implementations::const_iterator iter (impls);
  for (; iter.next (entry); iter.advance())
    {
      ACEXML_SoftPkg::Implementation* impl = *entry;
      ACEXML_String dll_path = dirname + impl->code_.file_;
      ACEXML_CharStream* dll = factory.create_stream (dll_path.c_str());
      if (!dll)
        {
          ACE_ERROR ((LM_ERROR, "Unable to create stream for the Component"
                      "location %s\n", dll_path.c_str()));
          return error_cleanup (temp);
        }
      ACEXML_String dsoname (temp);
      dsoname += "/" + impl->code_.file_;
      ACE_HANDLE dso = ACE_OS::open (dsoname.c_str(),
                                     O_WRONLY| O_CREAT|O_EXCL, S_IRWXU);
      if (dso == -1)
        {
          ACE_ERROR ((LM_ERROR, "Unable to create local copy %s : %m\n",
                      dsoname.c_str()));
          return error_cleanup (temp);
        }
      else if (ACE_OS::unlink (dsoname.c_str()) < 0)
        {
          ACE_ERROR ((LM_ERROR, "Unable to unlink local copy %s : %m\n",
                      dsoname.c_str()));
          return error_cleanup (temp);
        }
      ACEXML_Char buf[65535];
      int bytes = 0;
      while ((bytes = dll->read (buf, sizeof(buf))) > 0)
        {
          if (ACE_OS::write (dso, buf, bytes) != bytes)
            {
              ACE_ERROR ((LM_ERROR, "Unable to create dll %s: %m\n",
                          dsoname.c_str()));
              return error_cleanup (temp);
            }
        }
      ACE_OS::close (dso);

      ACE_ARGV* pArgv = 0;

      if (impl->propfile_.file_.length())
        {
          ACEXML_String path = dirname + impl->propfile_.file_;
          ACEXML_CharStream* pstream = factory.create_stream (path.c_str());
          if (!pstream)
            {
              ACE_ERROR ((LM_ERROR, "Failed to create input stream from %s\n",
                          path.c_str()));
              return error_cleanup (temp);
            }
          ACEXML_PropertyFile_Handler* pHandler = 0;
          ACE_NEW_RETURN (pHandler,
                          ACEXML_PropertyFile_Handler (path.c_str()) , -1);
          auto_ptr<ACEXML_PropertyFile_Handler> cleanup_pHandler (pHandler);
          ACEXML_InputSource property (pstream);

          ACEXML_Parser propParser;
          propParser.setContentHandler (pHandler);
          propParser.setDTDHandler (pHandler);
          propParser.setErrorHandler (pHandler);
          propParser.setEntityResolver (pHandler);

          ACEXML_TRY_NEW_ENV
            {
              propParser.parse (&property ACEXML_ENV_ARG_PARAMETER);
              ACEXML_TRY_CHECK;
            }
          ACEXML_CATCH (ACEXML_SAXException, ex)
            {
              ex.print();
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Parser Exception....\n")));
              return error_cleanup (temp);
            }
          ACEXML_ENDTRY;
          const ACEXML_Property* prop = pHandler->get_property();
          ACEXML_Char* list = prop->dump();
          ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_list (list);
          ACE_DEBUG ((LM_DEBUG, "Property obtained = %s\n", list));
          ACE_NEW_RETURN (pArgv, ACE_ARGV (list), -1);
        }
      if (pArgv)
        {
          myargv.add (pArgv->argv());
          param.argv = myargv.argv();
          param.argc = myargv.argc();
        }
      else
        {
          param.argc = argc;
          param.argv = argv;
        }
      param.name = impl->id_.c_str();
      if (thr_mgr->spawn (create_dll,(void*)&param) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Unable to spawn dll %s\n"),
                             impl->id_.c_str()), -1);
        }
      ACE_OS::sleep (3);
    }
  thr_mgr->wait();
  if (rmdir (temp) < 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to cleanup temporary directory : %m\n"));
      return -1;
    }
  return 0;
}
