#ifndef ANNOTEST_IDL_BE_GLOBAL_HEADER
#define ANNOTEST_IDL_BE_GLOBAL_HEADER

#include <string>

class BE_GlobalData {
public:
  BE_GlobalData ();

  void destroy ();

  void gen_source_info (bool value)
  {
    this->gen_source_info_ = value;
  }

  /**
   * Spawn Command Line Options
   */
  std::string spawn_options ();

  /**
   * Parse an argument that the frontend did not recognize.
   */
  void parse_args (long &i, char **av);

private:
  bool gen_source_info_ {};
};

#endif
