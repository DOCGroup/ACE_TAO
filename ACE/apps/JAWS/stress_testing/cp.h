#include "util.h"
#include "stats.h"
#include <memory>

class Client_Parameters {
public:
  explicit Client_Parameters (int);
  Client_Parameters (Client_Parameters const &) = delete;
  Client_Parameters (Client_Parameters &&) = delete;
  Client_Parameters &operator= (Client_Parameters const &) = delete;
  Client_Parameters &operator= (Client_Parameters &&) = delete;

  std::unique_ptr<URL> url;
  static std::unique_ptr<Stats> stats;
  static int tcp_nodelay;
  static int sockbufsiz;
  int id;
};
