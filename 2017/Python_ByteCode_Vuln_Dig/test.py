
import time

a=4
start_time=time.time()
index=1

for index in range(0xFFFFFFFD) :
    a+=1

print 'Using :'+str(time.time()-start_time)
