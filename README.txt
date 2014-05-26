GNU Radio Web Scraper

1. Good curl reference:
	http://www.yilmazhuseyin.com/blog/dev/curl-tutorial-examples-usage/
	
2. curl the main archive of the mailing list like this:
	curl --request GET http://lists.gnu.org/archive/html/discuss-gnuradio/
	
3. now try to do the same thing by wrapping these curl commands in a c++
library that implements libcurl

NOTE: I need to add a comment to my tips & hints page.  Read the man page for
'date' and 'strftime'.  Then from textMate I can hit 'ctrl r' to run a date command
in the shell and return the date.