#include "testS.h"

class con_i : public virtual POA_con
{
public:
  con_i (con_i const &) = delete;
  con_i &operator= (con_i const &) = delete;
  con_i (con_i &&) = delete;
  con_i &operator= (con_i &&) = delete;
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}
