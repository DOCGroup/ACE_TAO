#include "ace/Log_Msg.h"
// $Id$

#include "ace/TLI_Connector.h"

#if defined (ACE_HAS_TLI)
const int MAXLINE = 255;
  
int
main (int argc, char *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR, "Usage: %s employee_id [server-host port-number]\n",
		      argv[0]), -1);

  const char *emp_num   = argv[1];
  const char *host_name = argc < 3 ? ACE_DEFAULT_SERVER_HOST : argv[2];
  unsigned   short port	= argc < 4 ? ACE_DEFAULT_SERVER_PORT : ACE_OS::atoi (argv[3]);
  int	     n;
  char	     buf[MAXLINE];

  ACE_TLI_Stream client;
  ACE_TLI_Connector con;

  if (con.connect (client, ACE_INET_Addr (port, host_name)) == -1)
    ACE_OS::t_error ((char *)host_name), ACE_OS::exit (1);

  ACE_OS::strcpy (buf, emp_num);
  n = ACE_OS::strlen (buf);

  if (client.send_n (buf, n) != n)
    ACE_OS::t_error ("client.send error");

  if (client.recv (buf, MAXLINE) == -1 && t_errno != TLOOK && client.look () != T_DISCONNECT)
    ACE_OS::t_error ("client.recv error");

  if (ACE_OS::strcmp (buf, "ERROR") == 0)
    ACE_OS::printf ("Employee ID %s not in database\n", emp_num);
  else
    ACE_OS::printf ("Employee name requested is: %s\n", buf);

  if (client.close () == -1)
    ACE_OS::t_error ("cli_close"), ACE_OS::exit (1);

  return 0;
}
#else
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support ACE_TLI\n"), 1);
}
#endif /* ACE_HAS_TLI */
