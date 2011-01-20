// $Id$

#include "Offer_Exporter.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_stdio.h"



TAO_Offer_Exporter::
TAO_Offer_Exporter (CosTrading::Lookup_ptr lookup_if,
                    CORBA::Boolean verbose)
    : verbose_ (verbose)
{
  // Initialize the offer sequences and structures.
  this->create_offers ();

  // Obtain the necessary trading service interfaces.
  this->register_ = lookup_if->register_if ();
  this->admin_ = lookup_if->admin_if ();
}

TAO_Offer_Exporter::~TAO_Offer_Exporter (void)
{
  while (! this->clean_up_.is_empty ())
    {
      TAO_Dynamic_Property* dp = 0;
      this->clean_up_.dequeue_head (dp);
      dp->destroy ();
    }
}

void
TAO_Offer_Exporter::export_offers (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Exporting offers.\n"));

  for (int i = 0; i < NUM_OFFERS; i++)
    {
      this->props_plotters_[i][4].value <<= "Default";
      this->props_printers_[i][4].value <<= "Default";
      this->props_fs_[i][4].value <<= "Default";
    }

  this->export_to (this->register_.in ());
}

void
TAO_Offer_Exporter::export_to (CosTrading::Register_ptr reg)
{
  try
    {
      for (int i = 0; i < NUM_OFFERS; i++)
        {
          CORBA::Object_ptr offer_obj= this->plotter_[i]._this ();

          CosTrading::OfferId_var offer_id =
            reg->_cxx_export (offer_obj,
                              TT_Info::INTERFACE_NAMES[1],
                              this->props_plotters_[i]);

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Registered offer id: %C.\n", offer_id.in ()));
            }

          offer_obj = this->printer_[i]._this ();

          offer_id = reg->_cxx_export (offer_obj,
                                       TT_Info::INTERFACE_NAMES[2],
                                       this->props_printers_[i]);

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Registered offer id: %C.\n", offer_id.in ()));
            }

          offer_obj = this->fs_[i]._this ();

          offer_id = reg->_cxx_export (offer_obj,
                                       TT_Info::INTERFACE_NAMES[3],
                                       this->props_fs_[i]);

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Registered offer id: %C.\n", offer_id.in ()));
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Offer_Exporter::export_offers");
      throw;
    }
}

void
TAO_Offer_Exporter::export_offers_to_all (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Exporting to all.\n"));

  if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG, "Obtaining link interface.\n"));
    }

  CosTrading::Link_var link_if = this->register_->link_if ();

  if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG, "Obtaining references to traders directly"
                  " linked to the root trader.\n"));
    }

  CosTrading::LinkNameSeq_var link_name_seq = link_if->list_links ();

  if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG, "Registering offers with each of the linked"
                    " traders.\n"));
      }

  for (CORBA::ULong i = link_name_seq->length () - 1; i > 0; i--)
    {
      try
        {
          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Getting link information for %C\n",
                          static_cast<const char*> (link_name_seq[i])));
            }

          CosTrading::Link::LinkInfo_var link_info =
            link_if->describe_link (link_name_seq[i]);

          for (int j = 0; j < NUM_OFFERS; j++)
            {
              this->props_plotters_[j][4].value <<= link_name_seq[i];
              this->props_printers_[j][4].value <<= link_name_seq[i];
              this->props_fs_[j][4].value <<= link_name_seq[i];
            }

          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "Exporting offers to %C\n",
                          static_cast<const char*> (link_name_seq[i])));
            }

          this->export_to (link_info->target_reg.in ());
        }
      catch (const CORBA::Exception&)
        {
          // @@ IGNORE??
        }
    }
}


void
TAO_Offer_Exporter::withdraw_offers (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Withdrawing all offers.\n"));

  try
    {
      CORBA::ULong length;

      CosTrading::OfferIdSeq_var offer_id_seq = this->grab_offerids ();

      if (offer_id_seq.ptr () != 0)
        {
          length = offer_id_seq->length ();
          for (CORBA::ULong i = 0; i < length; i++)
            {
              this->register_->withdraw (offer_id_seq[i]);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Offer_Exporter::withdraw_offers");
      throw;
    }
}

void
TAO_Offer_Exporter::describe_offers (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Describing all offers.\n"));

  try
    {
      CORBA::ULong length;
      CosTrading::OfferIdSeq_var offer_id_seq = this->grab_offerids ();

      if (offer_id_seq.ptr () != 0)
        {
          length = offer_id_seq->length ();

          if (this->verbose_)
            ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));

          for (CORBA::ULong i = 0; i < length; i++)
            {
              CosTrading::Register::OfferInfo_var offer_info =
                this->register_->describe (offer_id_seq[i]);

              if (this->verbose_)
                {
                  ACE_DEBUG ((LM_DEBUG, "Offer Id: %C\n", (const char *) offer_id_seq[i]));
                  ACE_DEBUG ((LM_DEBUG, "Service Type: %C\n", offer_info->type.in ()));
                  TT_Info::dump_properties (offer_info->properties, 0);
                  ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
                }
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Offer_Exporter::describe_offers");
      throw;
    }
}

void
TAO_Offer_Exporter::modify_offers (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Modifying all offers.\n"));

  try
    {
      CosTrading::OfferIdSeq_var offer_id_seq = this->grab_offerids ();

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
                                       modify_list);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Offer_Exporter::modify_offers");
      throw;
    }
}

void
TAO_Offer_Exporter::
withdraw_offers_using_constraints (void)
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Offer_Exporter::Withdrawing with constraint.\n"));

  const char* constraint =
    "(not exist Description) and (Location == 'MODIFIED') and (exist Name)";

  if (this->verbose_)
    ACE_DEBUG ((LM_DEBUG, "Constraint: %C\n", constraint));

  try
    {
      this->register_->
        withdraw_using_constraint (TT_Info::INTERFACE_NAMES[TT_Info::PLOTTER],
                                   constraint);

      this->register_->
        withdraw_using_constraint (TT_Info::INTERFACE_NAMES[TT_Info::PRINTER],
                                   constraint);

      this->register_->
        withdraw_using_constraint (TT_Info::INTERFACE_NAMES[TT_Info::FILESYSTEM],
                                   constraint);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Offer_Exporter::withdraw_using_constraint");
      throw;
    }
}

CosTrading::OfferIdSeq*
TAO_Offer_Exporter::grab_offerids (void)
{
  if (this->verbose_)
    ACE_DEBUG ((LM_DEBUG, "TAO_Offer_Exporter::Grabbing all offer ids.\n"));

  CosTrading::OfferIdSeq *offer_id_seq;
  try
    {
      CORBA::ULong length = NUM_OFFERS;
      CosTrading::OfferIdIterator_ptr offer_id_iter;

      this->admin_->list_offers (NUM_OFFERS,
                                 CosTrading::OfferIdSeq_out (offer_id_seq),
                                 CosTrading::OfferIdIterator_out (offer_id_iter));

      if ((! CORBA::is_nil (offer_id_iter)) && offer_id_seq != 0)
        {
          CORBA::Boolean any_left = 0;
          CosTrading::OfferIdSeq *id_seq;
          CosTrading::OfferIdIterator_var offer_id_iter_var (offer_id_iter);

          do
            {
              any_left =
                offer_id_iter->next_n (length,
                                       CosTrading::OfferIdSeq_out (id_seq));

              int offers = id_seq->length ();
              int old_length = offer_id_seq->length ();
              offer_id_seq->length (old_length + offers);

              for (int i = 0; i < offers; i++)
                (*offer_id_seq)[i + old_length] = (*id_seq)[i];

              delete id_seq;
            }
          while (any_left);

          offer_id_iter->destroy ();
        }

      if (this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG, "The following offer ids are registered:\n"));
          for (int len = offer_id_seq->length (), j = 0; j < len; j++)
            ACE_DEBUG ((LM_DEBUG, "Offer Id: %C\n", (const char *)(*offer_id_seq)[j]));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_Offer_Exporter::grab_offerids");
      throw;
    }
  // @@ redundant.

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
      this->props_plotters_[i][9].value <<= dp_user_queue;

      this->props_plotters_[i][10].name = TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_FILE_SIZES_PENDING];
      this->props_plotters_[i][10].value <<= dp_file_queue;
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
      this->props_printers_[i][10].value <<= dp_user_queue;
      this->props_printers_[i][11].name = TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_FILE_SIZES_PENDING];
      this->props_printers_[i][11].value <<= dp_file_queue;
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
      this->props_fs_[i][7].value <<= dp_space_left;
    }
}
