GNU Radio Web Scraper

1. Good curl reference:
	http://www.yilmazhuseyin.com/blog/dev/curl-tutorial-examples-usage/
	
2. curl the main archive of the mailing list like this:
	curl --request GET http://lists.gnu.org/archive/html/discuss-gnuradio/
	
3. now try to do the same thing by wrapping these curl commands in a c++
library that links against libcurl

NOTE: I need to add a comment to my tips & hints page.  Read the man page for
'date' and 'strftime'.  Then from textMate I can hit 'ctrl r' to run a date command
in the shell and return the date.

4. forked the repo at: 
	https://github.com/JosephP91/curlcpp
and couldn't build it because of dependencies on '-std=c++11' somewhere in the
make process.  I wasn't going to dig for this so I moved on to look for a simpler
wrapper.

5. forked the repo at:
	https://github.com/gsauthof/ccurl
and it worked great.  ran make in the source directory and it made an executable name
test.  Test takes a single argument on the command line of the url you want to 
curl and returns the result to stdout.  I invoked with:
	./test http://lists.gnu.org/archive/html/discuss-gnuradio/
and it successfully pulled the page for me.  Then I invoked with 
	./test http://lists.gnu.org/archive/html/discuss-gnuradio/ > gnu_front_page.txt
and it successfully saved the page into the gnu_front_page.txt file.

6. Now I have a way to curl the front page and save it to a file.  The next step is
to process that file line by line.  Towards the bottom there is a table that lists 
all the links to each month's worth of discussion topics.  I need to run the file through 
a regex search and build another file with a URL on each line.

7. I can manually scan the front_page and keep the output to five lines with this:
	grep --max-count=5 '<li>' front_page.html
	
8. Now I need to find a regex example to do this in my c++ code.

9. Since my laptop doesn't have c++11 with native regex support I need to use the
Boost.org regex library which requires that I build it since it's not just a header
only supported function.  

10. To get started I need to refresh my memory on using the header only libs.  See
boost_lambda.cpp to see this.

11. Now I need to build against a library.  I haven't done this, so it will be a
learning curve. 