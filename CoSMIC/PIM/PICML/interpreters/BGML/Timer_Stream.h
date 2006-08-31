#ifndef TIMER_STREAM_H
#define TIMER_STREAM_H

#include "PICML/PICML.h"
#include "BGML/Benchmark_Stream.h"
#include <string>

#include "BGML/BGML_Export.h"

class BGML_Export Timer_Stream
{

public:
    Timer_Stream  (std::ostream& fstream);
    ~Timer_Stream ();

	// Write out the time probe information
	void write_includes ();
	void write_start_time_probe ();
	void write_stop_time_probe ();

	void incr_indent (unsigned short flag = 1);
	// increment the indentation level and by default actually indent the output
	// accordingly

	void decr_indent (unsigned short flag = 1);
	// decrease the indentation level and by default actually indent the output
	// accordingly

	inline void reset (void);
	// reset indentation level to 0

	void indent (void);
	// indent starting next line

	void nl (void);
	// put a newline and indent on the next line

	Timer_Stream &operator<< (const std::string &);
	Timer_Stream &operator<< (const char *);
	Timer_Stream &operator<< (const long &);
	Timer_Stream &operator<< (const unsigned long &);
	Timer_Stream &operator<< (const double &);
	Timer_Stream &operator<< (const char &);

	// = MANIPULATORS
	Timer_Stream &operator<< (const NL &);
	Timer_Stream &operator<< (const INDENT &);
	Timer_Stream &operator<< (const UNINDENT &);

private:
	std::ostream& strm_;
	int indent_level_;
};

#endif // BGML_VISITOR_H
