// $Id$

#include "QuoterS.h"
#include "tao/PortableGroup/miopC.h"
#include "tao/PortableGroup/PortableGroupC.h"
#include "ace/Get_Opt.h"

class Quote_Receiver_i : public POA_Quoter::Quote_Receiver
{
  // = TITLE
  //   An implementation of the Quote_Receiver interface
  //
public:
  /// Constructor.
  Quote_Receiver_i (CORBA::ORB_ptr orb, 
                    int num_updates);

  // = 
  virtual void update_stock_price (
      const Quoter::Stock_Quote & quote,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  // The ORB.
  CORBA::ORB_var orb_;

  // Quit after receiving a specified number of stock updates.
  int quote_updates_left_;
};

Quote_Receiver_i::Quote_Receiver_i (CORBA::ORB_ptr orb, 
                                    int num_updates)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    quote_updates_left_ (num_updates)
{
}

void
Quote_Receiver_i::update_stock_price (const Quoter::Stock_Quote & quote,
                                      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))

{
  printf ("%s is now at $%.2f!\n",
          quote.symbol.in (),
          quote.price);

  this->quote_updates_left_--;

  // Check if we've received enough quotes.
  if (this->quote_updates_left_ <= 0)
    {
      // Shutdown.
      this->orb_->shutdown (0, ACE_TRY_ENV);
    }
}


int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Standard initialization:
      // parse arguments and get all the references (ORB,
      // RootPOA, RTORB, RTCurrent, POAManager).
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create an instance of our servant.
      Quote_Receiver_i quote_receiver (orb.in (), 10);

#if 1
      CORBA::Object_var MIOP =
        orb->resolve_initial_references (TAO_OBJID_MIOP,
                                         0,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      MIOP::MulticastObjectGroupFactory_var mogf =
               MIOP::MulticastObjectGroupFactory::_narrow (MIOP.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create a multicast group IOR
      CORBA::Octet class_d_address[4]={225,1,1,1};

      CORBA::Object_var group1 = mogf->create_group (quote_receiver._interface_repository_id (),
                                                     class_d_address,
                                                     1234, 
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Print Object IOR.
      CORBA::String_var ior =
        orb->object_to_string (group1.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      FILE *output_file= ACE_OS::fopen ("quoter_group.ior", "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           "quoter_group.ior"),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
#else
      CORBA::String_var ior = CORBA::string_dup ("corbaloc:miop:1.0@1.0-Acme_Stock_Quotes_Domain-1/225.1.1.8:5000");
      CORBA::Object_var group1 = orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;
#endif

      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (group1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->activate_object_with_id (id, &quote_receiver, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Run ORB Event loop.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

