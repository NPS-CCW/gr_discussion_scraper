/*
	GNU_RADIO WEB SCRAPER
		-scrapes the GNU-Radio discussion pages for work on CDMA
		 implementation.

	initiated: 26 May 2014.  Memorial day!  Thinking of both my
						grandfathers today who gave their youth to our 
						country, and the long line of wonderful grandchildren
						they never had the opportunity to meet.
						
	author:   Z. H. Staples, zacstaples (at) mac (dot) com

	license:  This software is free and released into the public
						domain.  If you use it then need to buy me a beer..
						or wine...or dinner..or at least tell me an interesting
						story...or something!  You just don't owe me any
						money.
	
	dependencies: 1. uses a fork of the ccurl project on github.com to
								wrap the curl utility.
								2. uses boost.regex because I'm building it on a 
								system without c++11 native regex support.
								NOTE: both of these dependencies have long open
								source kind of licenses that don't include my 
								requirement for a beer, but I'm guessing you won't
								read their gnu/mit/bsf/yada/yada/yada licenses 
								either.
*/
#include "gr_scraper.h"
#include "ccurl/curl.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

using namespace std;

// error() disguises throws:
inline void error(const string& s){ throw runtime_error(s);}

inline void error(const string& s, const string& s2) { error(s+s2); }

inline void error(const string& s, int i){
	ostringstream os;
	os << s <<": " << i;
	error(os.str());
}

void write_to_file(const string& file_name, const string& data ) {
	ofstream ofs(file_name.c_str());
	if(!ofs) error("can't open output stream");
	ofs<<data;
	cout<<"Wrote "<<file_name<<endl;
}

void curl_get(const string url, const string file) {
  	curl::global g;
  	string s;
  	curl::callback::string cb(s);
  	curl::handle h(g, cb);
	curl::tag a;
  	try {
	  	a = h.get(url);
		write_to_file(file, s);
		cb.clear();
  	} catch (const underflow_error &e) {
		cout<<e.what()<<endl;
		cout << "Saved page retrieval.\n";
  	}
}

void process_front_page(const string in_file, const string out_file){
	//read the file in one line at a time and write it to the output file
	//open the input file
	ifstream ifs(in_file.c_str());
	if(!ifs) error("can't open the input file to process the front page");
	//open the output file
	ofstream ofs(out_file.c_str());
	if(!ofs) error("can't open the output file to process the front page");
	
	//read each line and write it to output...later we will only write it
	//if there is a regex match in the line
	string line;
	while(ifs) {
		getline(ifs, line);
		ofs<<line<<endl;
	}
}

int main(int argc, char **argv) {
	try {	
		// curl the front page and store results in a file
		curl_get("http://lists.gnu.org/archive/html/discuss-gnuradio/",
				 "front_page.html"
				);
				
		// iterate over the front page saving every list item in the page 
		// to another file
		process_front_page("front_page.html", "list_items.html");
	}
	catch(runtime_error& e){
		cerr<<"runtime_error: "<<e.what()<<endl;
		return 5;
	}
	catch(...){
		cerr<<"don't know what broke it."<<endl;
		return 4;
	}
  	return 0;
}
