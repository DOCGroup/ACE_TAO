#include "ace/Naming_Context.h"

int main ()
{

  static u_long ACE_DEFAULT_BASE_ADDR_1 = (1 * 64 * 1024 * 1024); 
  static u_long ACE_DEFAULT_BASE_ADDR_2 = (2 * 64 * 1024 * 1024);

  int i;
    
  ACE_Naming_Context * ns_ptr = new ACE_Naming_Context();
  ACE_Name_Options *name_options = ns_ptr->name_options ();
     
  ACE_Naming_Context * ns_ptr1 = new ACE_Naming_Context();
  ACE_Name_Options *name_options1 = ns_ptr1->name_options ();
     
  char address_arg1[BUFSIZ];
  char address_arg2[BUFSIZ];
  ACE_OS::sprintf (address_arg1, "-b%d", ACE_DEFAULT_BASE_ADDR_1);

  char * m_argv[] = {"MyName1",
		     "-cNODE_LOCAL" ,
		     address_arg1, 
		     NULL};
  int m_argc = sizeof ( m_argv ) / sizeof ( char * ) -1;

  ACE_OS::sprintf (address_arg2, "-b%d", ACE_DEFAULT_BASE_ADDR_2);
  char * n_argv[] = {"MyName2",
		     "-cNODE_LOCAL" ,
		     address_arg2, 
		     NULL};
  int n_argc = sizeof ( n_argv ) / sizeof ( char * ) -1;

  name_options->parse_args(m_argc,m_argv);
  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL); 
  cout << ::getpid() << ": opened with " << i << endl;
  if ( i != 0 ) return -1;
		
  name_options1->parse_args(n_argc,n_argv);
  i = ns_ptr1->open (ACE_Naming_Context::NODE_LOCAL);  
  cout << ::getpid() << ": 1 opened with " << i << endl;
  if ( i != 0 ) return -1;		

  return 0;
}
