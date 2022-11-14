#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
zodiac = form.getvalue('zodiac')

if zodiac == "aquarius":
    img = mpimg.imread('docs/zodiac/aquarius.png')
if zodiac == "aries":
    img = mpimg.imread('docs/zodiac/aries.png')
if zodiac == "cancer":
    img = mpimg.imread('docs/zodiac/cancer.png')
if zodiac == "gemini":
    img = mpimg.imread('docs/zodiac/gemini.png')
if zodiac == "leo":
    img = mpimg.imread('docs/zodiac/leo.png')
if zodiac == "libra":
    img = mpimg.imread('docs/zodiac/libra.png')
if zodiac == "pisces":
    img = mpimg.imread('docs/zodiac/pisces.png')
if zodiac == "sagittarius":
    img = mpimg.imread('docs/zodiac/sagittarius.png')
if zodiac == "scorpio":
    img = mpimg.imread('docs/zodiac/scorpio.png')
imgplot = plt.imshow(img)
plt.show()

print "Content-type:text/html\r\n\r\n"
print "<html>"
print "<head>"
print "<title>Hello - Second CGI Program</title>"
print "</head>"
print "<body>"
print "<h2>This was your cat zodiac sign %s</h2>" % (zodiac)
print "<h3>We hope you enjoyed it!</h3>"
print "<p>Thank you for using catserv.</p>"
print "</body>"

print "</html>"