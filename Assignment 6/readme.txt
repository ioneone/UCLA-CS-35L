Issues I ran into:

	After staring at the file for a while, I located
	the block of code, where it's computing the value
	for each pixel. I also located a block of code,
	where it reads the number of threads to use for SRT.
	I guessed that I create "nthreads" threads, and let
	each thread to handle different portion of pixel
	calculations.

	It seems it is computing the pixels column by column.

	for( int px=0; px<width; ++px )
        	for( int py=0; py<height; ++py )

	I needed a way to split the computations using number of threads
	and thread index (0, 1, 2, 3 if running w/ 4 threads).
	
	Naively, I thought I could just divide the computations by 4.
	If there are 20 jobs, I can do 0-4, 5-9, 10-14, 15-19.
	But things get too complicated when the jobs are not divisible
	by number of threads (e.g 23 jobs are not divisible by 4 threads)
	I wonder if there is a simpler way to split the jobs.

	I noticed that columns do not need to be computed in sequence, 
	so I can split the jobs as the follow:
	(if there are 4 threads)
	Thread 0: 0, 4,  8, 12, ...
	Thread 1: 1, 5,  9, 13, ...
	Thread 2: 2, 6, 10, 14, ...
	Thread 3: 3, 7, 11, 15, ...
	
	Thread 0 will compute pixels in column 0, 4, 8, 12, ...
	Thread 1 will compute pixels in column 1, 5, 9, 13, ...
	and so on.
	Notice the numbers are incremented by number of threads.

	Another issue I encountered is that I have no control on
	the order of printf statement being called. This is not good
	because it will result in different image. Therefore, I created
	a global variable called "pixels" to store all the pixel values
	computed by the threads. After making sure all the threads are
	finished, I then printf the values in "pixels" in order.

	I noticed I need "scene" variable to compute the pixels, so I
	made "scene" to global variable as well.

	Initially I tried to pass in two variables "number of threads" and
	"thread index" to my thread function. But it turns out thread function
	only takes one argument. Therefore, I made a struct called "thread_info",
	and pass this struct as an argument, which practically allows me to pass
	more than one arguments to the thread function.

	Unfortunately, some pixels were not computed when I had the
	following code:
 
	pthread_t thread_ids[nthreads];
    	for (int i = 0; i < nthreads; i++) {
		struct thread_info info;
        	info.num_threads = nthreads;
        	info.thread_index = i;

        	pthread_create(&thread_ids[i], NULL, computePixels, &info);        
    	}

	for (int i = 0; i < nthreads; i++)
		pthread_join(thread_ids[i], NULL);

	The problem here is that "struct thread_info info" is out of scope
	each iteration of the loop. For example, when i = 0, I created "info".
	But when i = 1, this "info" is disposed and replaced by a new "info".
	As a result, all my threads are using the same "info" created at last
	iteration. To fix this problem, I created an array of infos outside
	the loop:
	
	pthread_t thread_ids[nthreads];
    	struct thread_info info[nthreads]; 
    	for (int i = 0; i < nthreads; i++) {
        	info[i].num_threads = nthreads;
       		info[i].thread_index = i;
        
        	pthread_create(&thread_ids[i], NULL, computePixels, &info[i]);
    	}

    	for (int i = 0; i < nthreads; i++)
        	int status = pthread_join(thread_ids[i], NULL);

	Now my code runs successfully.

	By the way, I had a small issue when I tried to compile my code
	with make. I knew that when we use threads, we need to compile it
	with -lpthread appended at the end when we do gcc.

	gcc -o thread thread.c -lpthread

	So opened Makefile, and see where I can specify this library option.
	I located that LDLIBS is the variable to store all the library options.
	I appended -lpthread to LDLIBS, and make command works now.

Conclusion about my implementation of SRT performance:

	When I run time command on original implementation of SRT,
	it takes about 41s.

	time ./srt 1 > 1-test.ppm.tmp

	real	0m41.605s
	user	0m41.601s
	sys	0m0.000s

	My implementation of SRT supports multithreading. When I run
	the same command shown above with single thread, it takes 40s,
	which is almost same as the original performance

	However, when I run it with 2, 4, and 8 threads, 
	it takes 20s, 11s, and 5.8s respectively.
	Clearly, my implementation of SRT improves the performance
	as the number of threads increases.
	
	
	