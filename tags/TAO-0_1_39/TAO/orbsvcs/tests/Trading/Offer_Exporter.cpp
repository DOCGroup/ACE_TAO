// $Id$
#include "Offer_Exporter.h"

TAO_Offer_Exporter::
TAO_Offer_Exporter (CosTrading::Lookup_ptr lookup_if,
                    CORBA::Boolean verbose,
		    CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
    : verbose_ (verbose)
{
  // Initialize the offer sequences and structures.
  this->create_offers ();

  // Obtain the necessary trading service interfaces.
  this->register_ = lookup_if->register_if (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
  this->admin_ = lookup_if->admin_if (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
}

TAO_Offer_Exporter::~TAO_Offer_Exporter (void)
{
  while (! this->clean_up_.is_empty ())
    {
      TAO_Dynamic_Property* dp = 0;
      this->clean_up_.dequeue_head (dp);
      delete dp;
    }
}

void
TAO_Offer_Exporter::export_offers (CORBA::Environment& _env)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Exporting offers.\n"));

  for (int i = 0; i < NUM_OFFERS; i++)
    {
      this->props_plotters_[i][4].value <<= "Default";
      this->props_printers_[i][4].value <<= "Default";
      this->props_fs_[i][4].value <<= "Default";
    }
  
  this->export_to (this->register_.in (), _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
}

void
TAO_Offer_Exporter::export_to (CosTrading::Register_ptr reg,
                               CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
                   CosTrading::Register::InvalidObjectRef, 
                   CosTrading::IllegalServiceType, 
                   CosTrading::UnknownServiceType, 
                   CosTrading::Register::InterfaceTypeMismatch, 
                   CosTrading::IllegalPropertyName, 
                   CosTrading::PropertyTypeMismatch, 
                   CosTrading::ReadonlyDynamicProperty, 
                   CosTrading::MissingMandatoryProperty, 
                   CosTrading::DuplicatePropertyName))
{
  TAO_TRY
    {
      for (int i = 0; i < NUM_OFFERS; i++)
	{
	  CosTrading::OfferId_var offer_id = 
	    reg->export (this->plotter_[i]._this (TAO_TRY_ENV),
                         TT_Info::INTERFACE_NAMES[1],
                         this->props_plotters_[i],
                         TAO_TRY_ENV);
	  TAO_CHECK_ENV;

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Registered offer id: %s.\n", offer_id.in ()));
            }
          
	  offer_id = reg->export (this->printer_[i]._this (TAO_TRY_ENV),
                                  TT_Info::INTERFACE_NAMES[2],
                                  this->props_printers_[i],
                                  TAO_TRY_ENV);
	  TAO_CHECK_ENV;
          
          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Registered offer id: %s.\n", offer_id.in ()));
            }
          
	  offer_id = reg->export (this->fs_[i]._this (TAO_TRY_ENV),
                                  TT_Info::INTERFACE_NAMES[3],
                                  this->props_fs_[i],
                                  TAO_TRY_ENV);
	  TAO_CHECK_ENV;

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Registered offer id: %s.\n", offer_id.in ()));
            }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Exporter::export_offers");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Offer_Exporter::export_offers_to_all (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
                   CosTrading::Register::InvalidObjectRef, 
                   CosTrading::IllegalServiceType, 
                   CosTrading::UnknownServiceType, 
                   CosTrading::Register::InterfaceTypeMismatch, 
                   CosTrading::IllegalPropertyName, 
                   CosTrading::PropertyTypeMismatch, 
                   CosTrading::ReadonlyDynamicProperty, 
                   CosTrading::MissingMandatoryProperty, 
                   CosTrading::DuplicatePropertyName))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Exporting to all.\n"));

  if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG, "Obtaining link interface.\n"));
    }
  
  CosTrading::Link_var link_if = this->register_->link_if (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG, "Obtaining references to traders directly"
                  " linked to the root trader.\n"));
    }
  
  CosTrading::LinkNameSeq_var link_name_seq = link_if->list_links (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG, "Registering offers with each of the linked"
                    " traders.\n"));
      }
        
  for (int i = link_name_seq->length () - 1; i >= 0; i--)
    {
      TAO_TRY
        {
          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Getting link information for %s\n",
                          ACE_static_cast (const char*, link_name_seq[i])));
            }
          
          CosTrading::Link::LinkInfo_var link_info =
            link_if->describe_link (link_name_seq[i], _env);

          for (int j = 0; j < NUM_OFFERS; j++)
            {
              this->props_plotters_[j][4].value <<= link_name_seq[i];
              this->props_printers_[j][4].value <<= link_name_seq[i];
              this->props_fs_[j][4].value <<= link_name_seq[i];
            }

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Exporting offers to %s\n",
                          ACE_static_cast (const char*, link_name_seq[i])));
            }

          CosTrading::Register_var remote_reg;
#ifdef TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
          CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
          CORBA::Object_var obj = orb->string_to_object (link_info->target_reg, TAO_TRY_ENV);
          TAO_CHECK_ENV;
          remote_reg = CosTrading::Register::_narrow (obj.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;
#else 
          remote_reg = link_info->target_reg;
#endif /* TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG */
          
          this->export_to (remote_reg.in (), _env);
          TAO_CHECK_ENV_RETURN_VOID (_env);
        }
      TAO_CATCHANY
        {
        }
      TAO_ENDTRY;
    }
}


void
TAO_Offer_Exporter::withdraw_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::IllegalOfferId, 
		   CosTrading::UnknownOfferId, 
		   CosTrading::Register::ProxyOfferId))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Withdrawing all offers.\n"));
  
  TAO_TRY
    {
      CORBA::ULong length;

      CosTrading::OfferIdSeq_var offer_id_seq = this->grab_offerids (_env);
      TAO_CHECK_ENV;
      
      if (offer_id_seq.ptr () != 0)
	{
	  length = offer_id_seq->length ();
	  for (CORBA::ULong i = 0; i < length; i++)
	    {
	      this->register_->withdraw (offer_id_seq[i], TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	    }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Exporter::withdraw_offers");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Offer_Exporter::describe_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::IllegalOfferId, 
		   CosTrading::UnknownOfferId, 
		   CosTrading::Register::ProxyOfferId))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Describing all offers.\n"));
  
  TAO_TRY
    {
      CORBA::ULong length;
      CosTrading::OfferIdSeq_var offer_id_seq = this->grab_offerids (_env);
      TAO_CHECK_ENV;

      if (offer_id_seq.ptr () != 0)
	{          
	  length = offer_id_seq->length ();

          if (this->verbose_)
            ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
          
	  for (CORBA::ULong i = 0; i < length; i++)
	    {
	      CosTrading::Register::OfferInfo_var offer_info =
		this->register_->describe (offer_id_seq[i], TAO_TRY_ENV);
	      TAO_CHECK_ENV;

              if (this->verbose_)
                {
                  ACE_DEBUG ((LM_DEBUG, "Offer Id: %s\n", (const char *) offer_id_seq[i]));
                  ACE_DEBUG ((LM_DEBUG, "Service Type: %s\n", offer_info->type.in ()));	      
                  TT_Info::dump_properties (offer_info->properties, CORBA::B_FALSE);
                  ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
                }
	    }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Exporter::describe_offers");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Offer_Exporter::modify_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::NotImplemented, 
		   CosTrading::IllegalOfferId, 
		   CosTrading::UnknownOfferId, 
		   CosTrading::Register::ProxyOfferId, 
		   CosTrading::IllegalPropertyName, 
		   CosTrading::Register::UnknownPropertyName, 
		   CosTrading::PropertyTypeMismatch, 
		   CosTrading::ReadonlyDynamicProperty, 
		   CosTrading::Register::MandatoryProperty, 
		   CosTrading::Register::ReadonlyProperty, 
		   CosTrading::DuplicatePropertyName))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Modifying all offers.\n"));
  
  TAO_TRY
    {
      CosTrading::OfferIdSeq_var offer_id_seq = this->grab_offerids (_env);
      TAO_CHECK_ENV;

      if (offer_id_seq.ptr () != 0)
	{          
	  CORBA::ULong length = offer_id_seq->length ();
          CosTrading::PropertyNameSeq del_list;
          CosTrading::PropertySeq modify_list;

          del_list.length (1);
          modify_list.length (2);
          del_list[0] = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION];
          modify_list[0].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::LOCATION];
          modify_list[0].value <<= "MODIFIED";
          modify_list[1].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::MISCELLANEOUS];
          modify_list[1].value <<= "MODIFIED";
          
	  for (CORBA::ULong i = 0; i < length; i++)
	    {
              this->register_->modify (offer_id_seq[i],
                                       del_list,
                                       modify_list,
                                       TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	    }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Exporter::modify_offers");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Offer_Exporter::
withdraw_offers_using_constraints (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType, 
		   CosTrading::IllegalConstraint, 
		   CosTrading::Register::NoMatchingOffers))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Withdrawing with constraint.\n"));

  const char* constraint =
    "(not exist Description) and (Location == 'MODIFIED') and (exist Name)";
  
  if (this->verbose_)
    ACE_DEBUG ((LM_DEBUG, "Constraint: %s\n", constraint));

  TAO_TRY
    {
      this->register_->
        withdraw_using_constraint (TT_Info::INTERFACE_NAMES[TT_Info::PLOTTER],
                                   constraint,
                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->register_->
        withdraw_using_constraint (TT_Info::INTERFACE_NAMES[TT_Info::PRINTER],
                                   constraint,
                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->register_->
        withdraw_using_constraint (TT_Info::INTERFACE_NAMES[TT_Info::FILESYSTEM],
                                   constraint,
                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Exporter::withdraw_using_constraint");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

CosTrading::OfferIdSeq*
TAO_Offer_Exporter::grab_offerids (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::NotImplemented))
{
  if (this->verbose_)
    ACE_DEBUG ((LM_DEBUG, "TAO_Offer_Exporter::Grabbing all offer ids.\n"));

  CosTrading::OfferIdSeq_ptr offer_id_seq;
  TAO_TRY
    {
      CORBA::ULong length = NUM_OFFERS;
      CosTrading::OfferIdIterator_ptr offer_id_iter;

      this->admin_->list_offers (NUM_OFFERS,
				 CosTrading::OfferIdSeq_out (offer_id_seq),
				 CosTrading::OfferIdIterator_out (offer_id_iter),
				 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if ((! CORBA::is_nil (offer_id_iter)) && offer_id_seq != 0)
	{
	  CORBA::Boolean any_left = CORBA::B_FALSE;
	  CosTrading::OfferIdSeq_ptr id_seq;
	  CosTrading::OfferIdIterator_var offer_id_iter_var (offer_id_iter);
	  
	  do
	    {
	      any_left =
		offer_id_iter->next_n (length,
				       CosTrading::OfferIdSeq_out (id_seq),
				       TAO_TRY_ENV);
	      TAO_CHECK_ENV;

	      int offers = id_seq->length ();
	      int old_length = offer_id_seq->length ();
	      offer_id_seq->length (old_length + offers);

	      for (int i = 0; i < offers; i++)
		(*offer_id_seq)[i + old_length] = (*id_seq)[i];

	      delete id_seq;
	    }
          while (any_left);

          offer_id_iter->destroy (TAO_TRY_ENV);
          TAO_CHECK_ENV;
	}

      if (this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG, "The following offer ids are registered:\n"));
          for (int len = offer_id_seq->length (), j = 0; j < len; j++)
            ACE_DEBUG ((LM_DEBUG, "Offer Id: %s\n", (const char *)(*offer_id_seq)[j]));
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Offer_Exporter::grab_offerids");
      TAO_RETHROW_RETURN (offer_id_seq);
    }
  TAO_ENDTRY;
      
  return offer_id_seq;
}

void
TAO_Offer_Exporter::create_offers (void)
{
  const int QUEUE_SIZE = 4;
  
  int counter = 0, i = 0;
  char name[BUFSIZ];
  char description[BUFSIZ];
  CORBA::Any extra_info;
  TAO_Trader_Test::StringSeq string_seq (QUEUE_SIZE);
  TAO_Trader_Test::ULongSeq ulong_seq (QUEUE_SIZE);

  CORBA::Environment _env;
  CosTradingDynamic::DynamicProp* dp_user_queue;
  CosTradingDynamic::DynamicProp* dp_file_queue;
  CosTradingDynamic::DynamicProp* dp_space_left;

  ACE_INET_Addr addr ((u_short) 0);
  const char* hostname = addr.get_host_name ();
  
  // Initialize plotters
  string_seq.length (QUEUE_SIZE);
  ulong_seq.length (QUEUE_SIZE);
  for (i = 0; i < NUM_OFFERS; i++)
    {      
      ACE_OS::sprintf (name, "Plotter #%d", i);
      ACE_OS::sprintf (description,
		       "%s is a plotter. It plots stuff.",
		       name);

      for (int j = 0; j < QUEUE_SIZE; j++, counter = (counter + 1) % NUM_OFFERS)
	{
	  string_seq[j] = TT_Info::USERS [counter];
	  ulong_seq[j] = counter * 10000;
	}
      
      TAO_Dynamic_Property* user_queue =
        new TAO_Simple_Dynamic_Property<TAO_Trader_Test::StringSeq> (string_seq);
      TAO_Dynamic_Property* file_sizes =
        new TAO_Simple_Dynamic_Property<TAO_Trader_Test::ULongSeq> (ulong_seq);

      this->clean_up_.enqueue_head (user_queue);
      this->clean_up_.enqueue_head (file_sizes);

      dp_user_queue = user_queue->construct_dynamic_prop
	(TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_USER_QUEUE],
	 TAO_Trader_Test::_tc_StringSeq,
	 extra_info);

      dp_file_queue = file_sizes->construct_dynamic_prop
	(TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_FILE_SIZES_PENDING],
	 TAO_Trader_Test::_tc_ULongSeq,
	 extra_info);
      
      this->props_plotters_[i].length (11);
      this->props_plotters_[i][0].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME];
      this->props_plotters_[i][0].value <<= name;
      this->props_plotters_[i][1].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::LOCATION];
      this->props_plotters_[i][1].value <<= TT_Info::LOCATIONS[i];
      this->props_plotters_[i][2].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION];
      this->props_plotters_[i][2].value <<= description;
      this->props_plotters_[i][3].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::HOST_NAME];
      this->props_plotters_[i][3].value <<= hostname;
      this->props_plotters_[i][4].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::TRADER_NAME];
      this->props_plotters_[i][4].value <<= "Default";
      this->props_plotters_[i][5].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_NUM_COLORS];
      this->props_plotters_[i][5].value <<= (CORBA::Long)(i * 2);
      this->props_plotters_[i][6].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_AUTO_LOADING];
      this->props_plotters_[i][6].value <<= CORBA::Any::from_boolean ((CORBA::Boolean) (i % 2));
      this->props_plotters_[i][7].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_COST_PER_PAGE];
      this->props_plotters_[i][7].value <<= (CORBA::Float) i;
      this->props_plotters_[i][8].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_MODEL_NUMBER];
      this->props_plotters_[i][8].value <<= TT_Info::MODEL_NUMBERS[i];
      this->props_plotters_[i][9].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_USER_QUEUE];
      this->props_plotters_[i][9].
	value.replace (CosTradingDynamic::_tc_DynamicProp, dp_user_queue, CORBA::B_TRUE, _env);
      this->props_plotters_[i][10].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_FILE_SIZES_PENDING];
      this->props_plotters_[i][10].
	value.replace (CosTradingDynamic::_tc_DynamicProp, dp_file_queue, CORBA::B_TRUE, _env);
    }

  // Initialize printers
  for (i = 0; i < NUM_OFFERS; i++)
    {
      ACE_OS::sprintf (name, "Printer #%d", i);
      ACE_OS::sprintf (description,
		       "%s is a printer. It prints stuff.",
		       name);

      for (int j = 0; j < QUEUE_SIZE; j++, counter = (counter + 1) % NUM_OFFERS)
	{
	  string_seq[j] = TT_Info::USERS [counter];
	  ulong_seq[j] = counter * 10000;
	}

      TAO_Dynamic_Property* user_queue =
        new TAO_Simple_Dynamic_Property<TAO_Trader_Test::StringSeq> (string_seq);
      TAO_Dynamic_Property* file_sizes =
        new TAO_Simple_Dynamic_Property<TAO_Trader_Test::ULongSeq> (ulong_seq);

      this->clean_up_.enqueue_head (user_queue);
      this->clean_up_.enqueue_head (file_sizes);

      dp_user_queue = user_queue->construct_dynamic_prop
	(TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PLOTTER_USER_QUEUE],
	 TAO_Trader_Test::_tc_StringSeq,
	 extra_info);

      dp_file_queue = file_sizes->construct_dynamic_prop
	(TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PLOTTER_FILE_SIZES_PENDING],
	 TAO_Trader_Test::_tc_ULongSeq,
	 extra_info);

      this->props_printers_[i].length (12);
      this->props_printers_[i][0].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME];
      this->props_printers_[i][0].value <<= name;
      this->props_printers_[i][1].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::LOCATION];
      this->props_printers_[i][1].value <<= TT_Info::LOCATIONS[i];      
      this->props_printers_[i][2].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION];
      this->props_printers_[i][2].value <<= description;
      this->props_printers_[i][3].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::HOST_NAME];
      this->props_printers_[i][3].value <<= hostname;
      this->props_printers_[i][4].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::TRADER_NAME];
      this->props_printers_[i][4].value <<= "Default";
      this->props_printers_[i][5].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_COLOR];
      this->props_printers_[i][5].value <<= CORBA::Any::from_boolean ((CORBA::Boolean) (i % 2));
      this->props_printers_[i][6].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_DOUBLE_SIDED];
      this->props_printers_[i][6].value <<= CORBA::Any::from_boolean ((CORBA::Boolean) ((i + 1) % 2));
      this->props_printers_[i][7].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_COST_PER_PAGE];
      this->props_printers_[i][7].value <<= (CORBA::Float) i;
      this->props_printers_[i][8].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_MODEL_NUMBER];
      this->props_printers_[i][8].value <<= TT_Info::MODEL_NUMBERS[i];
      this->props_printers_[i][9].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_PAGES_PER_SEC];
      this->props_printers_[i][9].value <<= (CORBA::UShort) i;
      this->props_printers_[i][10].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_USER_QUEUE];
      this->props_printers_[i][10].
	value.replace (CosTradingDynamic::_tc_DynamicProp, dp_user_queue, CORBA::B_TRUE, _env);
      this->props_printers_[i][11].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_FILE_SIZES_PENDING];
      this->props_printers_[i][11].
	value.replace (CosTradingDynamic::_tc_DynamicProp, dp_file_queue, CORBA::B_TRUE, _env);
    }

  // Initialize FileSystem
  for (i = 0; i < NUM_OFFERS; i++)
    {
      ACE_OS::sprintf (name, "File System #%d", i);
      ACE_OS::sprintf (description,
		       "%s is a File System. It stores stuff.",
		       name);

      TAO_Dynamic_Property* space_left =
        new TAO_Simple_Dynamic_Property<CORBA::ULong> (i * 4434343);

      this->clean_up_.enqueue_head (space_left);

      dp_space_left = space_left->construct_dynamic_prop
        (TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::SPACE_REMAINING],
	 CORBA::_tc_ulong,
	 extra_info);

      this->props_fs_[i].length (8);
      this->props_fs_[i][0].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME];
      this->props_fs_[i][0].value <<= name;
      this->props_fs_[i][1].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::LOCATION];
      this->props_fs_[i][1].value <<= TT_Info::LOCATIONS[i];
      this->props_fs_[i][2].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION];
      this->props_fs_[i][2].value <<= description;
      this->props_fs_[i][3].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::HOST_NAME];
      this->props_fs_[i][3].value <<= hostname;
      this->props_fs_[i][4].name = TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::TRADER_NAME];
      this->props_fs_[i][4].value <<= "Default";
      this->props_fs_[i][5].name = TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::DISK_SIZE];
      this->props_fs_[i][5].value <<= (CORBA::ULong) (i * 2000000);
      this->props_fs_[i][6].name = TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::PERMISSION_LEVEL];
      this->props_fs_[i][6].value <<= (CORBA::UShort) (i + 1);
      this->props_fs_[i][7].name = TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::SPACE_REMAINING];
      this->props_fs_[i][7].
	value.replace (CosTradingDynamic::_tc_DynamicProp, dp_space_left, CORBA::B_TRUE, _env);
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<TAO_Dynamic_Property*>;
template class ACE_Unbounded_Queue<TAO_Dynamic_Property*>;
template class ACE_Unbounded_Queue_Iterator<TAO_Dynamic_Property*>;
template class TAO_Simple_Dynamic_Property<TAO_Trader_Test::StringSeq>;
template class TAO_Simple_Dynamic_Property<TAO_Trader_Test::ULongSeq>;
template class TAO_Simple_Dynamic_Property<CORBA::ULong>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<TAO_Dynamic_Property*>
#pragma instantiate ACE_Unbounded_Queue<TAO_Dynamic_Property*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<TAO_Dynamic_Property*>
#pragma instantiate TAO_Simple_Dynamic_Property<TAO_Trader_Test::StringSeq>
#pragma instantiate TAO_Simple_Dynamic_Property<TAO_Trader_Test::ULongSeq>
#pragma instantiate TAO_Simple_Dynamic_Property<CORBA::ULong>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


