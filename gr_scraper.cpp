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

void get_front_page(string& s) {
	string url = "http://lists.gnu.org/archive/html/discuss-gnuradio/";
  	curl::global g;
//  	string s;
  	curl::callback::string cb(s);
  	curl::handle h(g, cb);
	curl::tag a;
  	try {
	  	a = h.get(url);
	// 	cb.clear();
  	} catch (const underflow_error &e) {
		cout<<e.what()<<endl;
		cout << "Saved page retrieval.\n";
  	}
}

void write_front_page(const string& s) {
	string front_page = "front_page.html";
	ofstream ofs(front_page.c_str());
	if(!ofs) error("can't open output stream");
	ofs<<s;
}

int main(int argc, char **argv) {
	try {
		
		string s;		//filled by curl
		
		get_front_page(s);
	
		string front_page = "front_page.html";
		ofstream ofs(front_page.c_str());
		if(!ofs) error("can't open output stream");
		ofs << s;
		cout<<"Wrote the string to the file\n";
//		cb.clear();
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
