// $Id$

#include "ace/Naming_Context.h"

ACE_RCSID(Naming, test_open, "$Id$")

int 
main (int argc, char **argv) 
{
  char *host = argc > 1 ? argv[1] : "-hlocalhost";
  char *port = argc > 2 ? argv[2] : "-p20012";

  ACE_Naming_Context ns;   
  ACE_Name_Options *name_options = ns.name_options ();
  
  char * m_argv[] = { "MyName", 
                      "-cNET_LOCAL", 
                      host, 
                      port,  
                      NULL }; 
  int m_argc = sizeof ( m_argv ) / sizeof ( char * ) -1; 

  name_options->parse_args( m_argc, m_argv );

  int result = 0;
  result = ns.open ( ACE_Naming_Context::NET_LOCAL ); 
  ACE_DEBUG ((LM_DEBUG, "ACE_Naming_Context::open returned %d\n", result));
  if (result != 0)
    return result;
  else
    {
      char Key[128];
      char Val[32];
      char Type[2];
      
      Type[0] = '-';
      Type[1] = '\0';
      
      int i = 0;
      for (int l = 1; l <= 1000 ; l++) 
        {
          ACE_OS::sprintf (Key, "K_%05d_%05d", ACE_OS::getpid(), l);
          ACE_OS::sprintf (Val, "Val%05d", l);
          i = ns.bind (Key, Val, Type);  
          cout << ACE_OS::getpid() << ": bind of " << Key << " :" << i << endl;
          if (i != 0) {
            return -1;
          }          
        }
      
      result = ns.close ();
      ACE_DEBUG ((LM_DEBUG, "ACE_Naming_Context::close returned %d\n", result));
    }

  return result;
}
