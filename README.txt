I test my code on my laptop and I am reporting the result on my laptop. I also tested on crunchy1 and it runs there too.

mpirun -np 6 ./int_ring 1000000:

The latency for N=1000000 and 6 processing core is =  0.000056 seconds. Also:
All of the threads added their rank correctly in each iteration: final message = 15000000.

mpirun -np 6 ./int_ring :
I only comunicate one round. The bandwidth that I got on my laptop is : 

The bandwidth  is 9.876744 MB/seconds.

