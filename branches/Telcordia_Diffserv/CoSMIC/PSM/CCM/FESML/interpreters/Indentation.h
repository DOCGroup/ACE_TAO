#include <string>

class Indentation
{
public:
  Indentation();
  void operator-- ();
  void operator++ ();

private:
  std::string indent;
};
