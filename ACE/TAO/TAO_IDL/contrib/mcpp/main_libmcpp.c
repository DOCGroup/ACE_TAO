/* $Id$ most simple sample source to use libmcpp */

#include "mcpp_lib.h"
// FUZZ: disable check_for_improper_main_declaration
int
main (int argc, char *argv[])
// FUZZ: disable check_for_improper_main_declaration
{
    return mcpp_lib_main (argc, argv);
}
