#include "testS.h"

class TestImpl : public POA_SimpleStrings
{
public:
  explicit TestImpl (CORBA::ORB_ptr orb);
  TestImpl (TestImpl const &) = delete;
  TestImpl (TestImpl &&) = delete;
  TestImpl & operator= (TestImpl const &) = delete;
  TestImpl & operator= (TestImpl &&) = delete;

  void get_strings (ArrayOfString_out strings) override;

private:
  CORBA::ORB_var const orb_;
};
