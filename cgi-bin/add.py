import cgi, cgitb

form = cgi.FieldStorage()

a = int(form.getvalue('a'))
b = int(form.getvalue('b'))
result = a + b

print ("Content-type:text/html")
print()
print ("<html>")
print ('<head>')
print ("<title>Hello - Second CGI Program</title>")
print ('</head>')
print ('<body>')
print ("<h2>%i + %i = %i</h2>" % (a, b, result))
print ('</body>')
print ('</html>')
