// $Id$

#include "TT_Info.h"
#include "orbsvcs/Trader/Trader_Utils.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"



const char* TT_Info::INTERFACE_NAMES[] =
{
  "TAO_Trader_Test::Remote_IO",
  "TAO_Trader_Test::Plotter",
  "TAO_Trader_Test::Printer",
  "TAO_Trader_Test::File_System",
  "TAO_Trader_Test::PostScript_Printer"
};

const char* TT_Info::REMOTE_IO_NAME = "Remote_IO";

const char* TT_Info::REMOTE_IO_PROPERTY_NAMES[] =
{
  "Name",
  "Location",
  "Description",
  "Host_Name",
  "Trader_Name",
  "Miscellaneous"
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

const int TT_Info::NUM_QUERIES = 9;
const char* TT_Info::QUERIES[][4] =
{
  {INTERFACE_NAMES[REMOTE_IO], "", "", "45"},
  {INTERFACE_NAMES[REMOTE_IO], "'Cupples' ~ Location", "", "6"},
  {INTERFACE_NAMES[PLOTTER], "'sbw1' in User_Queue", "min Cost_Per_Page", "4"},
  {INTERFACE_NAMES[PLOTTER], "Num_Colors > 1 and 'Cupples' ~ Location", "min Cost_Per_Page", "2"},
  {INTERFACE_NAMES[PRINTER], "Pages_Per_Sec < -101215752192 and Color == TRUE", "with 'sbw1' in User_Queue", "0"},
  {INTERFACE_NAMES[PRINTER], "Pages_Per_Sec > 3.0 and Color == TRUE", "with 'sbw1' in User_Queue", "5"},
  {INTERFACE_NAMES[PRINTER], "Pages_Per_Sec > -3.0 and Color == TRUE", "with 'sbw1' in User_Queue", "7"},
  {INTERFACE_NAMES[PRINTER], "Color == TRUE or Double_Sided == TRUE", "random", "15"},
  {INTERFACE_NAMES[PRINTER], "(Color or Double_Sided) and 'sbw1' in User_Queue", "with 'Cupples' ~ Location", "4"}
};

void
TT_Info::dump_properties (const CosTrading::PropertySeq& prop_seq,
                          CORBA::Boolean print_dynamic)
{
  TAO_Property_Evaluator prop_eval (prop_seq);

  for (int length = prop_seq.length (), k = 0; k < length; k++)
    {
      CORBA::ULong seq_length = 0, i= 0;
      CORBA::Any* value = 0;
      CORBA::TypeCode_ptr tc = 0;
      ACE_DEBUG ((LM_DEBUG, "%-15s: ", prop_seq[k].name.in ()));
      try
        {
          CORBA::Boolean is_dynamic = prop_eval.is_dynamic_property (k);

          if (print_dynamic || ! is_dynamic)
            {
              value = prop_eval.property_value(k);

              tc = value->type ();
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "Dynamic Property\n"));
            }
        }
      catch (const CORBA::Exception&)
        {
          // @@ Seth, don't pass the exceptions back?
          ACE_DEBUG ((LM_DEBUG, "Error retrieving property value.\n"));
        }

      if (tc == 0)
        continue;
      int check = tc->equal (TAO_Trader_Test::_tc_StringSeq);

      if (check)
        {
          TAO_Trader_Test::StringSeq* str_seq;
          (*value) >>= str_seq;

          for (seq_length = str_seq->length (), i = 0; i < seq_length; i++)
            ACE_DEBUG ((LM_DEBUG, "%C ", (const char *) (*str_seq)[i]));

          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      else
        {
          check = tc->equal (TAO_Trader_Test::_tc_ULongSeq);

          if (check)
            {
              TAO_Trader_Test::ULongSeq* ulong_seq;
              (*value) >>= ulong_seq;

              for (seq_length = ulong_seq->length (), i = 0; i < seq_length; i++)
                ACE_DEBUG ((LM_DEBUG, "%d ", (*ulong_seq)[i]));

              ACE_DEBUG ((LM_DEBUG, "\n"));
            }
          else
            {
              // @@ Print the any value...
              CORBA::Char       char_val;
              CORBA::Octet     octet_val;
              CORBA::Boolean boolean_val;
              CORBA::Short     short_val;
              CORBA::UShort   ushort_val;
              CORBA::Long       long_val;
              CORBA::ULong     ulong_val;
              CORBA::Float     float_val;
              CORBA::Double   double_val;
              const char *    string_val;

              if ((*value) >>= CORBA::Any::to_char (char_val))
                {
                  ACE_DEBUG ((LM_DEBUG, "%c\n", char_val));
                }
              else if ((*value) >>= CORBA::Any::to_octet (octet_val))
                {
                  ACE_DEBUG ((LM_DEBUG, "%c\n", octet_val));
                }
              else if ((*value) >>= CORBA::Any::to_boolean (boolean_val))
                {
                  ACE_DEBUG ((LM_DEBUG, "%C\n",
                              boolean_val?"TRUE":"FALSE"));
                }
              else if ((*value) >>= short_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%d\n", short_val));
                }
              else if ((*value) >>= ushort_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%ld\n", ushort_val));
                }
              else if ((*value) >>= long_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%d\n", long_val));
                }
              else if ((*value) >>= ulong_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%ld\n", ulong_val));
                }
              else if ((*value) >>= float_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%f\n", long_val));
                }
              else if ((*value) >>= double_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%f\n", double_val));
                }
              else if ((*value) >>= string_val)
                {
                  ACE_DEBUG ((LM_DEBUG, "%C\n", string_val));
                }
            }
        }
    }
}

TT_Parse_Args::TT_Parse_Args (int& argc, ACE_TCHAR** argv)
  : federated_ (0),
    quiet_ (0),
    ior_ (0)
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (current_arg, ACE_TEXT("-f")) == 0 ||
          ACE_OS::strcmp (current_arg, ACE_TEXT("-federate")) == 0)
        {
          arg_shifter.consume_arg ();
          this->federated_ = 1;
        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-q")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-quiet")) == 0)
        {
          arg_shifter.consume_arg ();
          this->quiet_ = 1;
        }
      else if (ACE_OS::strcmp (current_arg, ACE_TEXT("-i")) == 0 ||
               ACE_OS::strcmp (current_arg, ACE_TEXT("-iorfile")) == 0)
        {
          arg_shifter.consume_arg ();
          FILE* ior_file = 0;

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* file_name = arg_shifter.get_current ();
              ior_file = ACE_OS::fopen (file_name, "r");

              if (ior_file == 0)
                ACE_ERROR ((LM_ERROR,
                            "Unable to open %s for reading: %p\n",
                            file_name));

              arg_shifter.consume_arg ();
            }
          else
            ior_file = ACE_OS::fdopen (ACE_STDIN, ACE_TEXT("r"));

          if (ior_file != 0)
            {
              ACE_Read_Buffer read_buffer (ior_file, 1);
              this->ior_ = read_buffer.read ();
            }
          else
            ACE_ERROR ((LM_ERROR, "Couldn't load ior.\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Ignoring argument <%s>\n",
                      current_arg));
          arg_shifter.consume_arg ();
        }
    }
}

TT_Parse_Args::~TT_Parse_Args ()
{
  // Reclaim the ior string's memory.
  ACE_Allocator* alloc = ACE_Allocator::instance ();
  alloc->free (this->ior_);
}

int
TT_Parse_Args::federated () const
{
  return this->federated_;
}

int
TT_Parse_Args::quiet () const
{
  return this->quiet_;
}

char*
TT_Parse_Args::ior () const
{
  return this->ior_;
}
