// $Id$

#include "QuoterC.h"

const char *ior = "file://quoter_group.ior";

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize and obtain reference to the Test object.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Quoter::Quote_Receiver_var receiver =
        Quoter::Quote_Receiver::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      Quoter::Stock_Quote quote;

      quote.symbol = CORBA::string_dup ("ACME");
      quote.full_name = CORBA::string_dup ("ACME Labs");
      quote.price = 1.0;

      // Send some stock price updates to the receivers.
      for (int i = 0; i < 10; i++)
        {
          receiver->update_stock_price (quote, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Update the stock price.
          quote.price = quote.price * 1.5;

          // Pause before sampling the stock price at the next
          // interval.
          ACE_Time_Value tv (0, 500000);
          ACE_OS::sleep (tv);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught unexpected exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

