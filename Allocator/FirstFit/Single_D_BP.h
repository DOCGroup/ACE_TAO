#ifndef SINGLE_BP
#define SINGLE_BP

//#include <windows.h>

//#include "BinPack.h"
#include <vector>
#include <functional>
#include <map>
#include <fstream>

using namespace std;

struct Item_Type
{
	int id;
	float cpu;
	float memory;
	int rank;
	bool packed;
	int bin_no;
};

/// the Bin structure
struct Bin
{
	vector <int> items;
  Item_Type capacity;
	int current_rank_;
  string name;
};

typedef vector<Bin> Bin_Array;

typedef vector<Item_Type> Item_Array;

class Single_D_BP 
{
public:


	Single_D_BP (int range_max, float slack, int upper_bound, string expt_name,int count = 100);

	~Single_D_BP ();

	void init_new_expt ();

	void first_fit ();

	void print_all_bins ();

	void initialize (int total_items);

	void initialize_bins (int total_bins);

	void analyze ();

	void sort (int sort_type);

	void get_items_from_file (int total_items);

	virtual void report ();


	void worst_fit ();

	void best_fit ();

	void balancing ();

	virtual int add_an_item (Item_Type item);

	virtual void remove_item (int index);

	void clean_bins ();

	int complete_pack ();

//	void print_header ();

	bool current_result (void){return current_trial;}

	void init_new_algo ();

	void generate_items_unsolvable (int bins);

	int back_tracks (){return back_track_count;}

	int back_track_level (){return this->back_track_item;}

	int total_items (){return items.size ();}

	void total_dimension_size (int& cpu, int& mem)
	{cpu = total_cpu;mem = total_mem;}

	void print_filled_bins_char (ofstream& out,int run_num,string heur);

  void set_items_array (Item_Array& item_array);

  void set_bins (Bin_Array& bin_array);

  void get_packed_items (Item_Array& packed_items);


private:

	/// randomly shuffle the items ....
	void shuffle ();

	/// variables required for balancing the items 
	Item_Array packed_items;

	/// Sorts the items ...
	void special_sort ();

	void sort_sub_range (Item_Array& sub_items);

	void save_to_file ();


	/// the typedef vector ...

	Item_Array items;

	Bin_Array bins;

/// Current trial status
	bool current_trial;

	/// the total items ..
	int total_items_;

	/// the total bins	
	int total_bins_;

	/// No. of Successes
	int success;

	/// No. of failures
	int failure;

	/// Total no. of trials
	int trials;

	/// Total no whose size is greater 
	int size_greater;

	/// item size greater than the bins 
	int size_exceeded;

	/// contains the items which could not be packed
	vector<int> failed_items;

	void print_items ();

	/// input file 
	std::ifstream in;

	/// using file
	bool using_file;

	/// type of sorting to be used ...
	int sort_type;

/// 
	int not_allocable;

	/// finds an item confirming to a given rank ...
	int find_item_of_rank (int bin_index, int rank);

	std::map<int, int> res_distr_;

	/// size of the Bin
	int bin_size;

	/// The range from which items will be chosen
	int range_max_;

	/// rejection count
	long long rejection_count;

	/// allowed slack 
	float allowed_slack_;

	/// counter - counts the number of trials
	long counter;

	/// name of the experiment
	string name;

	/// float upper slack bound
	int upper_bound_;

	/// the back track in case of complete bin-pack
	int back_track_count;

	/// back track item 
	int back_track_item;

	/// total cpu dimensions for all items
	int total_cpu;

	/// total cpu dimensions for all items
	int total_mem;

  /// maximum size of the bin
  static const float BINSIZE;
};

class Sort_Scaler : public 
	binary_function <Item_Array::iterator,
	Item_Array::iterator,
	bool>
{
public: 
	bool operator ()(Item_Type front,Item_Type back);
};

class Sort_Large_D : public 
	binary_function <Item_Array::iterator,
	Item_Array::iterator,
	bool>
{
public: 
	Sort_Large_D (bool cpu) {cpu_ = cpu;}
	bool operator ()(Item_Type front,Item_Type back);

	bool cpu_;
};


class Sort_Largest_Val : public 
	binary_function <Item_Array::iterator,
	Item_Array::iterator,
	bool>
{
public: 
	bool operator ()(Item_Type front,Item_Type back);
};


class Sort_Multi : public 
	binary_function <Item_Array::iterator,
	Item_Array::iterator,
	bool>
{
public: 
	bool operator ()(Item_Type front,Item_Type back);
};

class Sort_Single : public 
	binary_function <Item_Array::iterator,
	Item_Array::iterator,
	bool>
{
public: 
	bool operator ()(Item_Type front,Item_Type back);
};

#endif