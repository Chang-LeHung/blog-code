import os
# master for pty, slave for tty
m,s = os.openpty()
print (m)
print (s)
# showing terminal name
s = os.ttyname(s)
m = os.ttyname(m)
print (m)
print (s)