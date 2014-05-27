/*
	GNU_RADIO WEB SCRAPER
	-scrapes the GNU-Radio discussion pages for work on CDMA
	 implementation.

	initiated: 26 May 2014. Memorial day!  Thinking of both my
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
#include "ccurl/curl.h"
#include <boost/regex.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>

using namespace std;

// initiate libcurl globally
curl::global g;


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


void append_to_file(const string& file_name, const string& data ) 
{
	static bool first_write = true;
	ofstream ofs;
	if(first_write) {
		ofs.open(file_name.c_str());
		first_write = false;
	}else {
		ofs.open(file_name.c_str(), ofstream::app );
	}	
	if(!ofs) error("can't open the output file to write cdma_lines stream");
	ofs<<data<<endl;
	cout<<"appended to "<<file_name<<endl;
}

void add_headers(ofstream& ofs)
{
	ofs<<"<html>\n";
	ofs<<"<head>\n";
	ofs<<"<title>Zac's GNU-Radio scraper</title>\n";
	ofs<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n";
	ofs<<"</head><body><h2>Zac's GNU-Radio Scraper</h2>\n";
	ofs<<"<h4>All discussion of CDMA since 2001</h4>\n";	
}

void add_footers(ofstream& ofs) 
{
	ofs<<"<hr>\n";
	ofs<<"<em>This software is provided free of charge under the beer license."
	   <<" Under the terms of the beer license you owe me a beer at the appropriate"
	   <<" point in the not too distant future.</em>\n";
	ofs<<"</body></html>\n";
	
}

void process_front_page(const string in_file, const string out_file, const string base_url)
{
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
		string p = "^<li>.+=\"(.+)\">.+</li>$";
		boost::regex pat(p);
		boost::smatch matches;
		if(boost::regex_match(line, matches, pat)) {
			if(matches.size() == 2){		//we have a match for the capture
				ofs<<base_url<<matches[1]<<endl;	//add the capture to the links file
			}
		}
	}
}

void process_tmp_file(const string in_file, const string out_file) 
{
	//step through each line of the in_file and then append strings
	//to the cdma_links when you get a match
	
	//open the input file
	ifstream ifs(in_file.c_str());
	if(!ifs) error("can't open the tmp_file from list_items");
	
	//read each line and write it to output if it mentions CDMA
	string line;
	while(ifs) {
		getline(ifs, line);	
		string p = ".*(cdma|CDMA).*";
		boost::regex pat(p);
		boost::smatch matches;
		if(boost::regex_match(line, matches, pat)) {
			cout<<"cdma match\n";
			append_to_file(out_file, line);
		}
	}
}



void process_cdma_links(const string in_file, const string out_file) 
{
	cout<<"Processing final html doc\n";
	
	string base_url = "http://lists.gnu.org/archive/html/discuss-gnuradio/";
	string subj_url;
	string msg_url;
	string year_url;
	string month_url;
	string closing_url;	
	//read the file in one line at a time and write it to the output file
	//open the input file
	ifstream ifs(in_file.c_str());
	if(!ifs) error("can't open the input file to process the front page");
	//open the output file
	ofstream ofs(out_file.c_str());
	if(!ofs) error("can't open the output file to process the front page");
	
	add_headers(ofs);
	
	//read each line, reformat the href links and write to output
	//default line looks like this 
	//<li><b><a name="00180" href="msg00180.html">Re: [Discuss-gnuradio] CDMA-UMTS protocol implementation with USRP1</a></b>, <i>Marcus Müller</i>, <tt>2014/05/09</tt>
	//we want to grab the date at the end and reformat like this
	//<li><b><a name="00180" href="http://lists.gnu.org/archive/html/discuss-gnuradio/2014-05/msg00180.html">Re: [Discuss-gnuradio] CDMA-UMTS protocol implementation with USRP1</a></b>, <i>Marcus Müller</i>, <tt>2014/05/09</tt>
	
	string line;
	while(ifs) {
		getline(ifs, line);	
		string p = ".*(msg\\d+.html)(\".*<tt>)(\\d{4})/(\\d{2})(.*)";
		boost::regex pat(p);
		boost::smatch matches;
		if(boost::regex_match(line, matches, pat)) {
			if(matches.size() == 6 ){ 		//we have matches for message, year and month
				msg_url = matches[1];
				subj_url = matches[2];
				year_url = matches[3];
				month_url = matches[4];
				closing_url = matches[5];
			}
			ofs<<"<li><b><a href=\""<<base_url<<year_url<<"-"<<month_url
				<<'/'<<msg_url<<subj_url<<year_url<<'/'<<month_url<<closing_url;
		}
	}
	add_footers(ofs);
}

void curl_get_then_append(const string url, const string tmp_file,
						  const string cdma_file) 
{
  	static string s;
  	static curl::callback::string cb(s);
  	static curl::handle h(g, cb);
  	try {
	  	h.get(url);
		write_to_file(tmp_file, s);
		process_tmp_file(tmp_file, cdma_file);
		cb.clear();
  	} catch (const underflow_error &e) {
		cout<<e.what()<<endl;
		cout << "error caught in curl_get_then_append\n";
  	}
}

bool is_URL(const string line){
	/*
		TODO rudimentary test right now on length...need regex here too 
	*/
	
	if (line.size() > 5) return true;
	return false;
}

int main(int argc, char **argv) {
	try {
	  		
		// curl the front page and store results in a file
		curl_get("http://lists.gnu.org/archive/html/discuss-gnuradio/",
				 "front_page.html"
				);
				
		// iterate over the front page saving every list item in the page 
		// to another file
		process_front_page("front_page.html", "list_items.html", 
						   "http://lists.gnu.org/archive/html/discuss-gnuradio/"
						   );
						
		//now I need to build a curl loop 
		ifstream ifs("list_items.html");
		if(!ifs) error("can't open the input file to process first curl loop");
		string line;
		while(ifs) {
			getline(ifs, line);
			if(is_URL(line))
				curl_get_then_append(line, "tmp.html", "cdma.html");
		}
		
		// now that I have all the CDMA related articles I just need to 
		// reformat the cdma.html page with correct fully defined links in href line
		process_cdma_links("cdma.html", "cdma_final.html");
		
		//finally open the web page we build and remove all the temp files with
		//shell script
		int i;
		if (system(NULL)) puts("Opening cdma_final.html");
		else error("system is unabailable to open cdma_final.html");
		i=system("open cdma_final.html");
		cout<<"Cleaning up\n";
		i=system("./clean.sh");
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
