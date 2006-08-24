#include "ibp_main_parser.h"

#define NUM_BLADES 6
#define BLADE_CAPACITY 0.8

int ACE_TMAIN (int argc,
               ACE_TCHAR *argv[])
{
  char ** cargv = new char*[argc];
  for (int i = 0; i < argc; ++i)
  {
    cargv[i] = new char[ACE_OS::strlen(argv[i])];
    ACE_OS::strcpy (cargv[i], argv[i]);
  }

  char ** bin_names = new char*[NUM_BLADES + 1];
  bin_names[NUM_BLADES] = 0;
  double ** bin_capacities = new double*[NUM_BLADES + 1];
  bin_capacities[NUM_BLADES] = 0;

  for (int i = 0; i < NUM_BLADES; ++i)
  {
    bin_names[i] = new char[BUFSIZ];
    ACE_OS::snprintf(bin_names[i], BUFSIZ, "blade%d", i+2);
    bin_capacities[i] = new double(BLADE_CAPACITY);
  }

  int retval = 0;
  {
    IBP_Main_Parser::ibp.reset(new ImportanceBinPacker(bin_names, bin_capacities));
    IBP_Main_Parser::output = new Plan_Seq(0);
    retval = IBP_Main_Parser::parse (argc, argv);
  }

  for (double** i = bin_capacities; *i != 0; ++i)
    delete *i;
  delete [] bin_capacities;

  for (char** i = bin_names; *i != 0; ++i)
    delete [] *i;
  delete [] bin_names;

  for (char** i = cargv; i < &(cargv[argc]); ++i)
    delete [] *i;
  delete [] cargv;

  return retval;
}
