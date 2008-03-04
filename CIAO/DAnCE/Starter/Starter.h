#ifndef STARTER_H_
#define STARTER_H_

#include "ace/String_Base.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"

namespace DAnCE
  {

  class Starter
    {
    public:
      Starter (int argc, ACE_TCHAR* argv[]);
      ~Starter();
      void execute();
    private:
      struct Node
        {
          ACE_CString iorfile_;
          ACE_CString ior_;
          CORBA::Object_var obj;

          Node() : obj (CORBA::Object::_nil()) {};
        };

      CORBA::ORB_var orb_;
      int optLogLevel_;
      int & argc_;
      ACE_TCHAR** & argv_;
      bool optNS_;
      ACE_CString optNSFile_;
      ACE_CString optNSOptions_;
      ACE_Map_Manager<ACE_CString, Node, ACE_Null_Mutex> nodes_;
      bool optEM_;
      ACE_CString optEMFile_;
      ACE_CString optGenObjKey_;
      bool optPLB_;
      bool optPL_;

      void parseArgs (int argc, ACE_TCHAR* argv[]);
      void usage ();
      void getLogLevel (int argc, ACE_TCHAR* argv[]);
      void setLogLevel ();
      void generateObjectKey (const char * keyargs);
      void generateObjectKey (const char * node
                              , const char * plan
                              , const char * component
                              , const char * port);
      void write_IOR (const char * ior_file_name, const char* ior);

      void initNaming();
      CORBA::Object_ptr initNodeManager (const char * node);
      CORBA::Object_ptr initExecutionManager();
      void runPlanLauncher();
      void argCopyForNaming (int & c, char **& v);
      void argCopyForNode (const char * node, int & c, char **& v);
      void argCopyForEM (int & c, char **& v);
      void argCopyForPL (int & c, char **& v);
      void releaseArgs (int c, char ** v);
      static bool isPossibleOption(const char* opt);
    };

} // DAnCE

#endif /*STARTER_H_*/
