import pexpect
import time

child_server = pexpect.spawn('./server')

#m=[10,20,30,40,50]

#for n in m:
n=50
l=[]
for i in range(n):
    l.append(pexpect.spawn('./c1ient'))
l[n-1].expect('connected')
start = time.time()
l[n-1].sendline('hello')
l[n-2].expect('hello')
end = time.time()
print(end - start)




print("fine")
