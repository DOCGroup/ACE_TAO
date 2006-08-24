#ifndef IBP_MAIN_PARSER_H
#define IBP_MAIN_PARSER_H

#include "ImportanceBinPacker.h"

class IBP_Main_Parser
{
public:
  static ACE_Auto_Ptr<ImportanceBinPacker> ibp;
  static Plan_Seq::_var_type output;

  static int parse(int argC, char** argV);
};

#endif /* IBP_MAIN_PARSER_H */
