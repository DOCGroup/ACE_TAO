//$Id$

#include "Time_Date_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// This is the interface program that accesses the remote object

// Constructor.
Time_Date_Client_i::Time_Date_Client_i (void)
{
  // no-op
}

//Destructor.
Time_Date_Client_i::~Time_Date_Client_i (void)
{
  // no-op
}

int
Time_Date_Client_i::parse_args (int argc,
                                char *argv[])
{
  return 0;
}

int
Time_Date_Client_i::run (char *name,
                         int argc,
                         char *argv[])
{
  // Initialize the client.
  if (client.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      Time_Date_ptr time_date;

      CORBA::Long l;

      // Get the time_date.
      time_date->bin_date (l,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Binary time_date = %d\n",
                  l));

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Setting a value for the time_date\n"));

      if (client.shutdown () == 1)
        client->shutdown (ACE_TRY_ENV);
      ACE_UNUSED_ARG (ret_val);
    }
  ACE_CATCH (CORBA::UserException, range_ex)
    {
      ACE_PRINT_EXCEPTION (range_ex,
                           "\tFrom get and set time_date");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, memex)
    {
      ACE_PRINT_EXCEPTION (memex,
                           "Cannot make time_date as Memory exhausted");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Client<Time_Date_Factory,Time_Date_Factory_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Client<Time_Date_Factory,Time_Date_Factory_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
