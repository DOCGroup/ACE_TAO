// $Id$
#include "TT_Info.h"
#include "orbsvcs/Trader/Trader_Utils.h"

const char* TT_Info::INTERFACE_NAMES[] = 
{
  "Remote_IO",
  "Plotter",
  "Printer",
  "File_System",
  "PostScript_Printer"
};

const char* TT_Info::REMOTE_IO_NAME = "Remote_IO";

const char* TT_Info::REMOTE_IO_PROPERTY_NAMES[] = 
{
  "Name",
  "Location",
  "Description",
  "Host_Name",
  "Trader_Name"
};

const char* TT_Info::PLOTTER_NAME = "Plotter";

const char* TT_Info::PLOTTER_PROPERTY_NAMES[] = 
{
  "Num_Colors",
  "Auto_Loading",
  "Cost_Per_Page",
  "Model_Number",
  "User_Queue",
  "File_Sizes_Pending"
};

const char* TT_Info::PRINTER_NAME = "Printer";

const char* TT_Info::PRINTER_PROPERTY_NAMES[] = 
{
  "Color",
  "Double_Sided",
  "Cost_Per_Page",
  "Model_Number",
  "Pages_Per_Sec",
  "User_Queue",
  "File_Sizes_Pending"
};

const char* TT_Info::FILESYSTEM_NAME = "File_System";

const char* TT_Info::FILESYSTEM_PROPERTY_NAMES[] =
{
  "Disk_Space",
  "Space_Remaining",
  "Permission_Level"
};

const char* TT_Info::PS_PRINTER_PROPERTY_NAMES[] = 
{
  "Version"
};

const char* TT_Info::PS_PRINTER_NAME = "PostScript_Printer";

const char* TT_Info::LOCATIONS[] =
{
  "Bryan 509",
  "Bryan 503",
  "Dunker 102",
  "Jolley 408",
  "Lopata 401",
  "January 110",
  "Cupples I 30",
  "Cupples II 201",
  "Urbauer 321",
  "Umrath 302",
  "Wilson 110",
  "Olin 239",
  "Simon 238",
  "Rebstock 232",
  "McMillan 123"
};

const char* TT_Info::USERS[] = 
{
  "sbw1",
  "schmidt",
  "naga",
  "alex",
  "mk1",
  "marina",
  "sumedh",
  "sergio",
  "coryan",
  "cdgill",
  "levine",
  "jxh",
  "irfan",
  "nanbor",
  "eea1",
  "brunsch"
};

const char* TT_Info::MODEL_NUMBERS[] =
{
  "HP238293892bn2398",
  "PK92839nkse092309",
  "8328323n293kljs98",
  "29309sjdkq34jksd2",
  "09q834efjhqw834q3",
  "q9834fdkjwq0934kf",
  "q834jfda09q834djk",
  "kajsd09q384kjda90",
  "9p83q4jklsdj098q3",
  "09a834ojfda09wu3r",
  "09234kejdfq934jed",
  "0q83kjefd09384jda",
  "asdfj0283jkdf0923",
  "kladfp72wu34j230f",
  "9q834jidlj234ujof"
};

const int TT_Info::NUM_QUERIES = 7;
const char* TT_Info::QUERIES[][3] =
{
  {INTERFACE_NAMES[REMOTE_IO], "", ""},
  {INTERFACE_NAMES[REMOTE_IO], "Location ~ 'Cupples'", ""},
  {INTERFACE_NAMES[PLOTTER], "'sbw1' in User_Queue", "min Cost_Per_Page"},
  {INTERFACE_NAMES[PLOTTER], "Num_Colors > 1 and Location ~ 'Cupples'", "min Cost_Per_Page"},
  {INTERFACE_NAMES[PRINTER], "Pages_Per_Sec > 3 and Color == TRUE", "with 'sbw1' in User_Queue"},
  {INTERFACE_NAMES[PRINTER], "Color == TRUE or Double_Sided == TRUE", "random"},
  {INTERFACE_NAMES[PRINTER], "(Color or Double_Sided) and 'sbw1' in User_Queue", "with Location ~ 'Cupples'"}
};

void
TT_Info::dump_properties (const CosTrading::PropertySeq& prop_seq)
{
  CORBA::Environment env;
  TAO_Property_Evaluator prop_eval (prop_seq);
  
  for (int length = prop_seq.length (), k = 0; k < length; k++)
    {
      CORBA::Any* value = 0;
      CORBA::TypeCode_ptr tc = 0;
      ACE_DEBUG ((LM_DEBUG, "%-15s: ", prop_seq[k].name.in ()));
      TAO_TRY
	{
	  value = prop_eval.property_value(k, env);
	  TAO_CHECK_ENV;

	  tc = value->type ();
	}
      TAO_CATCHANY
	{
	  ACE_DEBUG ((LM_DEBUG, "Error retrieving property value.\n"));	  
	}
      TAO_ENDTRY;

      if (tc == 0)
	continue;
      else if (tc->equal (TAO_Trader_Test::_tc_StringSeq, env))
	{
	  TAO_Trader_Test::StringSeq* str_seq_ptr;
	  (*value) >>= str_seq_ptr;

	  TAO_Trader_Test::StringSeq_var str_seq (str_seq_ptr);
	  for (int length = str_seq->length (), i = 0; i < length; i++)
	    ACE_DEBUG ((LM_DEBUG, "%s ", (const char *) str_seq[i]));

	  ACE_DEBUG ((LM_DEBUG, "\n"));
	}
      else if (tc->equal (TAO_Trader_Test::_tc_ULongSeq, env))
	{
	  TAO_Trader_Test::ULongSeq* ulong_seq_ptr;
	  (*value) >>= ulong_seq_ptr;

	  TAO_Trader_Test::ULongSeq_var ulong_seq (ulong_seq_ptr);
	  for (int length = ulong_seq->length (), i = 0; i < length; i++)
	    ACE_DEBUG ((LM_DEBUG, "%d ", ulong_seq[i]));

	  ACE_DEBUG ((LM_DEBUG, "\n"));
	}
      else
	CORBA::Any::dump (*value);
    }
}

void
TT_Info::serialize_offer (const CosTrading::Offer& offer)
{
  //TAO_OutputCDR stream;
  //CORBA::Environment env;

  //stream.encode (CosTrading::_tc_Offer, &offer, 0, env);
  //  ACE_Message_Block cdr = ACE_Message_Block::duplicate (stream.begin ());
}
