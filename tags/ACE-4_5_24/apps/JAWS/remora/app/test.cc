// $Id

#include <unistd.h>
#include <stdlib.h>
#include <iostream.h>
#include "ace/Signal.h"
#include "Remora_Export.h"
#include "Remora_Import.h"

class Shutdown : public ACE_Event_Handler
{
public:
  Shutdown(int& cond) : cond_(cond) {}
  
  virtual int handle_signal(int, siginfo*, ucontext*)
    {
      this->cond_ = 0;
      return 0;
    }

private:

  int& cond_;
};


int
main(int argc, char** argv)
{
  int lcv = 1;
  ACE_Sig_Handler shutdown;

  shutdown.register_handler(SIGINT, new Shutdown(lcv));
  
  try
    {
      Remora_Export cpu("CPU Usage", 0, 100);
      Remora_Export thread("Thread Count", 0, 24);
      Remora_Export through("Throughput", 0, 150);
      Remora_Export energy("Energy Levels", 0, 2084);
      Remora_Export warp("Warp Drive Output", 0, 1023);

      Remora_Import in_threads("Threads", 1, 0, 24);
      Remora_Import in_tachyon("Tachyon Emmissions", 25, 0, 100);
      Remora_Import in_shields("Shield Integrity", 100, 0, 100);
      Remora_Import in_phasers("Phaser Power", 421, 0, 1024);
      
      while (lcv)
	{
	  cpu = rand() % 100;
	  thread = rand() % 24;
	  energy = rand() % 2084;
	  through = rand() % 150;
	  warp = rand() % 1023;

	  cout << "Statistics: " << endl;
	  cout << "CPU: " << CORBA::Long(cpu) << endl;
	  cout << "Thread: " << CORBA::Long(thread) << endl;
	  cout << "Throughput: " << CORBA::Long(through) << endl;
	  cout << "Energy: " << CORBA::Long(energy) << endl;
	  cout << "Warp: " << CORBA::Long(cpu) << endl;

	  cout << "Controls: " << endl;
	  cout << "Threads: " << CORBA::Long(in_threads) << endl;
	  cout << "Tachyon: " << CORBA::Long(in_tachyon) << endl;
	  cout << "Shields: " << CORBA::Long(in_shields) << endl;
	  cout << "Phasers: " << CORBA::Long(in_phasers) << endl;

	  sleep(1);
	}
    }
  catch(CORBA::SystemException& e)
    {

      cerr << e << endl;
    }
}
