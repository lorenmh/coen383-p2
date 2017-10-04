# Types

* `process_t`: represents a process (arrival time, execution time, estimated run time, process id, etc)
* `process_queue_t`: represents a buffer/array of `process_t`
* `result_t`: represents a test result of the scheduling algorithm (for display / statistics / analysis)
* `history_t`: Can be an array of process ids (In assignment document this is where we record 'A time chart of the 100+ quanta, such as ABCDABCD ...'). We don't need to be picky here, we can make this an over-large array.

# Functions

* `process_queue_t *create_process_queue(int n)`: given n, will malloc a buffer of process\_t's and return its pointer. Will call `randomize_process` for each process. This queue should be sorted by arrival time.
* `process_queue_t *clone_process_queue(process_queue_t *pq, int n)`: used to copy the process queue
* `void free_process_queue(process_queue_t *pq)`: frees the memory of the process queue

* `void fcfs(process_queue_t *pq, history_t *h)`: runs FCFS on the process queue, populates history
* `void sjf(process_queue_t *pq, history_t *h)`: runs shortest job first
* `void srt(process_queue_t *pq, history_t *h)`: runs shortest remaining time
* `void rr(process_queue_t *pq, history_t *h)`: runs round robin
* `void hpf_npe(process_queue_t *pq, history_t *h)`: runs highest priority first, non pre-emptive
* `void hpf_pe(process_queue_t *pq, history_t *h)`: runs highest priority first, pre-emptive

* `void generate_result(process_queue_t *pq, result_t *r)`: given a `process_queue_t` will populate the values in the `result_t`

# Overview

Here's an overview of one run:

We generate a `process_queue_t` by calling `create_process_queue`. This
contains ~ 50 processes or whatever. The process values are randomized and the
queue is sorted by arrival time (this is all done in `create_process_queue`). We
then copy this process queue 5 times using `clone_process_queue`, meaning there
will now be 6 process queues. These 6 identical process queues are going to be
sent to the schedulers (FCFS, SJF, etc). So, we take copy 1 and send it to
FCFS, we take copy 2 and send it to SJF, etc.

We run the schedulers using the process queue. *The scheduler will modify the
process's values in the process queue!* This means to do the analysis we only
need to look at this process queue, and for each process in there look at the
execution time, completion time, arrival time, etc.

To get these results, we pass the process queue to `generate_result` which will
populate a `result_t` struct with the average turnaround time, average waiting
time, average response time, and throughput.

We do this 6 times and then evaluate
