#ifndef BENCHMARK_STREAM_H
#define BENCHMARK_STREAM_H

#include <ostream>
#include <string>
#include <vector>

#include "BGML/Global_Data.h"
#include "BGML/BGML_Export.h"

// A dummy structure to inform TAO_OutStream's << operator to  put a newline
// and use the current indentation for the succeeding line
struct BGML_Export NL
{
  NL (void);
};

struct BGML_Export INDENT
{
  //   Increase the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  INDENT (int do_now = 0);

  const int do_now_;
};

struct BGML_Export UNINDENT
{
  //   Decrease the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  UNINDENT (int do_now = 0);

  const int do_now_;
};

extern const NL nl;
extern const INDENT idt;
extern const INDENT idt_nl;
extern const UNINDENT uidt;
extern const UNINDENT uidt_nl;

// --- Constants for Warmup and Benchmarking
const int BGML_DEFAULT_WARMUP_ITER = 100;
const int BGML_DEFAULT_BENCH_ITER  = 5000;

class BGML_Export BenchmarkStream
{
public:
  BenchmarkStream (std::string& component_name,
				   std::string& operation_name,
				   std::vector<std::string>& arg_list,
				   std::ostream& strm,
				   BGML_Data &state);
  // constructor.for generating workloads

  ~BenchmarkStream (void);
  // destructor.

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

  BenchmarkStream &operator<< (const std::string &);
  BenchmarkStream &operator<< (const char *);
  BenchmarkStream &operator<< (const long &);
  BenchmarkStream &operator<< (const unsigned long &);
  BenchmarkStream &operator<< (const double &);
  BenchmarkStream &operator<< (const char &);

  // = MANIPULATORS
  BenchmarkStream &operator<< (const NL &);
  BenchmarkStream &operator<< (const INDENT &);
  BenchmarkStream &operator<< (const UNINDENT &);

  void gen_ifdef_macro (const std::string &);
  void gen_ifdef_macro (const char *);
  void gen_ifdefc_macro (const char *name);
  void gen_ifdefc_macro (const std::string& name);
  void gen_endif (const std::string &);
  void gen_endif (const char *);
  void gen_endifc (const std::string &);
  void gen_endifc (const char *);

  // generate a #if !defined, #defined macro

  void generate_task_header (std::string& class_name, bool is_main_class);
  // Generate the header file for the Task

  void gen_include_file (std::string& file_name);
  // Generate the "#include" definitions

  void gen_constructor_decl (std::string& name);
  void gen_destructor_decl  (std::string& name);
  void gen_constructor_defn (std::string& name);
  void gen_destructor_defn (std::string& name);
  // Generate the Constructor/Destructor for the Task classes

  void gen_private_mem_decl (bool is_main);
  // Generate the private members of the Task_Base class

  void generate_task_def (std::string &metric, BGML_Data &data);
  // Generate the .cpp file required by the task

  void gen_template_function_def (const std::string& name,
								  const std::string& class_name,
								  const std::string& return_type,
								  const std::vector<std::string>& param_list);

  void gen_warmup_iterations (__int64 iterations,
							  __int64 benchmark_priority);
  // Generate the warmup iterations

  void gen_bench_def (__int64 iterations);
  // Generate the benchmarking definition

  int gen_barrier (void);
  // Generate barrier information

  void activate_background_tasks (int task_size);
  // Activate background tasks if necessary

  void gen_background_load (std::string& class_name, __int64 start, __int64 finish);
  // Generate Background load

  void generate_workload_def (__int64 interations,
							  BGML_Task_Group_Data &data);
  // Generates the workload definition

  void gen_run_method (std::string &name);
  // Generate the run method to run the tasks

  void create_export_macro (std::string& shared_name, std::string &output_path);
  // Generate the export header file

  void create_build_file ();
  // Generate the mpc file to create the benchmarking library

  void generate_tool_description ();
  // Generate the tool description information

private:
  std::string& component_name_;
  std::string& operation_name_;
  std::vector<std::string>& arg_list_;
  std::ostream& strm_;
  int indent_level_;
  BGML_Data &bgml_state_;

  void upcase (const char *);
  void generate_rate_helper ();
};

#endif // BenchmarkStream_H
