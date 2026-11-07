#include "client.h"
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <iostream>

int Client_Parameters::tcp_nodelay {};
int Client_Parameters::sockbufsiz {};
std::unique_ptr<Stats> Client_Parameters::stats;

static void *
client_thread (void *data)
{
  std::unique_ptr<Client_Parameters> cp (static_cast<Client_Parameters *> (data));
  float latency {};
  float throughput {};
  URL const &url = *cp->url;

  cp->stats->i_have_started (cp->id);

  // Check for presence of protocol, hostname and filename.

  if (url.get_protocol ().empty ()
      || url.get_hostname ().empty ()
      || url.get_filename ().empty ())
    {
      std::cerr << "Invalid URL" << std::endl;
      cp->stats->i_am_done (cp->id);
      return nullptr;
    }

  // Attempt connection
  connection webserver;

  if (webserver.connect (url.get_hostname ().c_str (), cp->tcp_nodelay, cp->sockbufsiz))
    {
      cp->stats->i_am_done (cp->id);
      return nullptr;
    }
  // Send the request now.


  ACE_Profile_Timer throughput_timer, latency_timer;
  throughput_timer.start();
  latency_timer.start();
  std::string const request = "GET " + url.get_filename () + " HTTP/1.0\r\n\r\n";
  if (webserver.write_n (request.c_str (), request.size ()) != 0)
    {
      cp->stats->i_am_done (cp->id);
      return nullptr;
    }

  char buffer[BUFSIZ];
  ssize_t num_read {};
  ssize_t total_read {};
  bool first_time = true;
  for (;;)
    {
      num_read = webserver.read (buffer, sizeof buffer);
      if (first_time)
        {
          ACE_Profile_Timer::ACE_Elapsed_Time et;
          latency_timer.stop();
          latency_timer.elapsed_time(et);
          latency = et.real_time;
          first_time = false;
        }
      if (num_read <= 0)
        break;
      total_read += num_read;
    }
  cp->stats->i_am_done (cp->id);
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  throughput_timer.stop();
  throughput_timer.elapsed_time(et);
  if (et.real_time > 0)
    throughput = (8 * total_read / et.real_time) / (1000 * 1000);
  cp->stats->log (cp->id, throughput, latency);
  return nullptr;
}

int
driver (char const *id, int total_num, float requests_sec,
        char const *url1, float p1,
        char const *url2, float p2,
        char const *url3, float p3,
        int tcp_nodelay, int sockbufsiz)
{
  // construct the client parameters packet

  Client_Parameters::tcp_nodelay = tcp_nodelay;
  Client_Parameters::sockbufsiz = sockbufsiz;

  Client_Parameters::stats = std::make_unique<Stats> (total_num);

  int missed_deadlines {};
  // sleep_time is in microseconds, and requests_sec is per second, hence the pow(10,6)
  float sleep_time = (1 / requests_sec) * (1000.0 * 1000.0);
  float delta {};
  ACE_OS::srand (ACE_OS::time (nullptr));
  for (int i = 0; i < total_num; i++)
    {
      ACE_Profile_Timer timer;
      if (sleep_time < delta)
        ++missed_deadlines;
      else
        {
          ACE_Time_Value tv (0, static_cast<long> (sleep_time - delta));
          ACE_OS::sleep (tv);
        }
      timer.start ();
      std::unique_ptr<Client_Parameters> cp = std::make_unique<Client_Parameters> (i);

      double const r = static_cast<double> (ACE_OS::rand ()) / RAND_MAX;
      if (r <= p1)
        cp->url = std::make_unique<URL> (url1);
      else if (r <= p1 + p2)
        cp->url = std::make_unique<URL> (url2);
      else if (r <= p1 + p2 + p3)
        cp->url = std::make_unique<URL> (url3);
      else
        cp->url = std::make_unique<URL> (url1);

      // Ownership transfers to client_thread, which wraps this pointer in unique_ptr.
      Client_Parameters *thread_data = cp.release ();
      if (ACE_Thread_Manager::instance ()->spawn (client_thread, thread_data) == -1)
        {
          delete thread_data;
          Client_Parameters::stats->i_have_started (i);
          Client_Parameters::stats->i_am_done (i);
        }
      timer.stop ();
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      timer.elapsed_time (et);
      delta = (0.4 * fabs (et.real_time * (1000 * 1000))) + (0.6 * delta);
    }

  // Join the other threads..
  (ACE_Thread_Manager::instance ())->wait();
  // Now output the data for this test
  std::cout << id;
  Client_Parameters::stats->output();
  std::cout << std::endl;
  if (missed_deadlines != 0)
    {
      std::cout << "missed deadlines " << missed_deadlines << std::endl;
      std::cout << "missed deadlines as a % of total requests: "
                << static_cast<float> (missed_deadlines) / total_num * 100 << std::endl;
    }
  Client_Parameters::stats.reset ();
  return 0;
}


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // This will set the global scale factor if the ACE_SCALE_FACTOR
  // environment variable is set.
  ACE_High_Res_Timer::get_env_global_scale_factor ();

  if (argc < 3)
    {
      std::cerr << "Usage: " << argv[0] << " infile outfile " << std::endl;
      std::cerr << "The input file contains lines, with the following fields: " << std::endl;
      std::cerr << "experiment_id total_number_of_requests request_rate url1 p1 url2 p2 "
                   "url3 p3 TCP_NODELAY SOCKET_RECV_BUFSIZ " << std::endl;

      return 1;
    }

  std::ifstream input (ACE_TEXT_ALWAYS_CHAR (argv[1]));
  if (!input)
    {
      ACE_OS::perror ("fopen");
      return 2;
    }

  std::ofstream output (ACE_TEXT_ALWAYS_CHAR (argv[2]));
  if (!output)
    {
      ACE_OS::perror ("open");
      return 3;
    }
  std::streambuf *original_cout = std::cout.rdbuf (output.rdbuf ());

  std::string line;
  while (std::getline (input, line))
    {
      std::istringstream line_stream (line);
      std::string id;
      std::string url1;
      std::string url2;
      std::string url3;
      int total_num {};
      int tcp {};
      int sock {};
      float rate {};
      float p1 {};
      float p2 {};
      float p3 {};

      if (line.empty () || line[0] == '#')
        continue;
      if (!(line_stream >> id >> total_num >> rate >> url1 >> p1
            >> url2 >> p2 >> url3 >> p3 >> tcp >> sock))
        {
          ACE_ERROR ((LM_ERROR, "Invalid stress test input line: %s\n", line.c_str ()));
          std::cout.rdbuf (original_cout);
          return 4;
        }
      if (total_num < 0 || rate <= 0)
        {
          ACE_ERROR ((LM_ERROR, "Invalid request count or request rate: %s\n", line.c_str ()));
          std::cout.rdbuf (original_cout);
          return 4;
        }

      ACE_OS::fprintf (stderr, "----\n");
      ACE_OS::fprintf (stderr,
                       "\tNow performing experiment:%s\n\tSending %d requests at %f requests/second\n",
                       id.c_str (), total_num, rate);
      driver (id.c_str (), total_num, rate,
              url1.c_str (), p1, url2.c_str (), p2, url3.c_str (), p3, tcp, sock);
    }
  std::cout.rdbuf (original_cout);
  return 0;
}
