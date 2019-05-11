# Important Notes for Parallel programming class Michael McDermott is teaching Fall semester
[Syllabus](https://grid.cs.gsu.edu/~mmcdermott2/dw/doku.php?id=teaching:4310:home)

# PThreads Examples 

DISCLAIMER: I have had a lot of these examples for a very long time and do not remember where some (really any) of them originally come from. I try to keep them updated so that they can be compiled and run with little to no modification on our various systems. They may require some small modifications to compile and run though.

If they look similar or even exactly like other example code that is probably because its from stock code that originally came from a central repository and I just lost the reference along the way, if you have the original reference then I am more than happy to cite the original author. Another reason for this is that there are only so many ways to write simple illustration or toy examples that are clear and easy to understand so they all tend to be the same. I am not claiming authorship AT ALL here on these samples, I am just posting them here because I think are beneficial learning tools for my the class.

PLEASE CHECK OUT THE CODE IN THIS REPOSITORY FOR EACH OF THE ITEMS BELOW!

## Hello World
This is basically the first thing you ever end up writing in a new language. You can see the invocation of threads, and their joining at the end.
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/Hello%20World

## Simple Mutex Example
This code introduces mutex locks. These ensure that a critical section of code is run 'in isolation'. For instance, if a thread modifies a variable that has global scope it should more than likely be encapsulated in a mutex lock while this modification is taking place. You may also want to lock something that reads in data to ensure the data isn't being modified while its being read.
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/Simple%20Mutex%20Example

## PingPong
We can do slightly more with mutex locks other than just ensuring data or variables are good. We can use them to do coherent communications through the use of a global variable between threads. This program is one example of that sort of communication scheme. A 'ping' is sent from thread A to thread B, and a response 'pong' is sent back. We are of course simply locking a variable and ensuring its 'good' still, but the way we use that variable affords us more power and flexibility with the rest of our program. Notice we aren't actually sending or receiving messages but just writing to a variable and reading out of that same variable over and over.
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/PingPong

## False Sharing and its Simple(ish) fix
This is something you need to be aware of and is important. I am not entirely sure where I saw this originally anymore. I have had notes on this since I took the class you are now taking and the example I was given was very similar. We are not going to deal with this at the low level your book does. The short and simple version is that when your read data you have to be careful about the 'chunk' size of your data. If you overlap those chunks it leads to a performance loss and if you do it a lot with a lot of data the total performance loss can be quite staggering. You can fix this by simply padding your data so that when you read a chunk of data it never overlaps.
### problem code
So this code suffers from false sharing due to it reading a long pointer, which is going to overlap on a data word read on the CDER cluster. Generally these things are fairly consistent across platforms but there can be some differences so its always good to check the machine you are on. In linux you can 'getconf WORD_BIT' to find out the size of a data word. Data sizes for primitive types are fairly standardized and the WORD size is included here as well at least for Windows: https://www.ibm.com/support/knowledgecenter/en/SSFKSJ_9.0.0/com.ibm.mq.ref.dev.doc/q104610_.html
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/False%20Sharing%20and%20its%20Simple(ish)%20fix
## Sample solution (there are other ways)
The fix is to use padding so that our reads do not overlap on a cache line, now we are reading a Data struct pointer. this means that our data size gets inflated by some small amount sometimes. In the below case we create a struct and add a buffer variable to the struct to prevent data reads from overlapping.

We are basically forcing the following behavior instead of thread1 travel all the way to cpu0 for data. for visual reference (from Intel): 
![Visual Reference](https://software.intel.com/sites/default/files/m/d/4/1/d/8/5-4-figure-1.gif)
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/False%20Sharing%20Fix

## Conditional Locks
You use these when you want threads to block and wait on a condition of another thread. In other words, in a simple two thread scenario, thread 1 will sleep until thread 2 signals it, using pthread_cond_signal() to wake up and do something. The following code shows this.
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/Conditional%20Locks

## Barriers
Syncing multiple threads all at once.
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/Barriers

## Prefix Sum with Barriers
### A hopefully clear example of pthreads prefix sum
There are some issues to work out still. Some have been left in on purpose. Some are probably accidental. This should work on CDER but has only been tested in an ubuntu docker instance so far.
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/PThreads/A%20hopefully%20clear%20example%20of%20pthreads%20prefix%20sum

# MPI Examples

## Hello World
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/MPI/Hello%20World

## Send / Recv
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/MPI/Send%20%26%20Receive

## Isend / Irecv
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/MPI/Isend%20%26%20Irecv

## Broadcast
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/MPI/Broadcast

## Scatter and Gather
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/MPI/Scatter%20and%20Gather

# CudaC (GPU) Examples

## Hello World 

### Hello world in standard C99
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/CudaC/Hello%20World%20in%20C99

### Hello World in Cuda-C
### CODE: https://github.com/michaeljeffreymcdermott/parallel-programming/blob/master/CudaC/Hello%20World%20in%20Cuda-C















