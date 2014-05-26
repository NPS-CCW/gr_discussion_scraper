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
