The Abstractor is a multithread-powered research tool for scientific literature. It focuses on
delivering fast and accurate results for given queries. The program takes a query as an input,
scans its file collection, finds the most relevant abstracts by measuring the similarities between
abstracts and the given query, then retrieves convenient sentences from the selected abstracts.

Input
• The abstractor.out program takes two command line arguments:
1. input file name.txt: Path of the input file
2. output file name.txt: Path of the output file
• The input file name.txt file contains several parameters, which will be read line-by-line:
1. The first line contains values of T, A, and N, which are separated by spaces.
T: Number of threads (other than the main thread) that will be created for the session
A: Number of abstracts that will be scanned by the threads
N: Number of abstracts that will be returned and summarized
2. The second line contains the words to query. An example query is provided below:
virus pandemic global
3. From lines 3 to A+2, the names of the abstract files will be listed as follows.
abstract 1.txt
abstract 2.txt
...
abstract A.txt
• An input file consists of several lines of sentences that are pre-processed and separated by
dots. An example abstract, which consists of two sentences, all in lowercase, and separated
from punctuation, is provided below.
the covid19 pandemic caused by sarscov2 remains a significant issue for global health economics
and society . a wealth of data has been generated since its emergence in december
2019 and it is vital for clinicians to keep up with this data from across virus the world at a
time of uncertainty and constantly evolving guidelines and clinical practice .

Output
The Abstractor outputs the thread operation logs, scores, and resulting sentences to the given
text file. An example output is provided in Table 4.
• Each thread outputs its name and operation as ”Thread X is calculating abstract Y.txt”.
• You should insert ### after each separate operation, i.e., after finishing threads jobs, listing
result 1, result 2, and so on.

Thread Creation
Since text processing is a costly process and the texts we process are independent of each other,
it is better to use parallelism in order to accelerate the Abstractor program. For that purpose,
it is wise to use threads. Yet, the number of threads must be reasonable, so you should not
create hundreds of them initially, and drain all system resources. You should create T number
of threads and make them work till the end of the program.
Each thread must log its own name and the name of the file it is processing to the output
file. You should name your threads alphabetically and start from A and continue with B, C,
and so on.
You should evenly spread the jobs to the threads. So, some threads should not be idle while
one thread does all the job. When all the abstracts are processed, all of the threads must be
terminated gracefully. You need to use POSIX threads provided by Linux-based systems or
MacOS for the implementation of this project in order to create a multithreaded environment
(Windows is not allowed). To address the synchronization issues, you can use mutex and
semaphores.

Similarity Measurement
In Natural Language Processing, we often need to estimate text similarity between text documents.
There exist several text similarity metrics such as Cosine Similarity, Jaccard Similarity,
and Euclidean Distance Measurement. All these text similarity metrics have different behavior.
In the Abstractor program you should use the Jaccard Similarity metric to determine the
similarity between two texts. This metric finds how close the two texts are to each other in
terms of the number of common words that exist over total words. Let’s calculate the Jaccard
Similarity of Text1 and Text2 :
1. Get two texts to compare:
Text1: the war and the dance
Text2: when the world was at war before we just kept dancing
2. Tokenize texts using space and find the unique words:
WordSet1 = {the, war, and, dance}
WordSet2 = {when, the, world, was, at, war, before, we, just, kept, dancing}
3. Find the intersecting words of two sets:
Intersection WS1 WS2 = {the, war}
4. Find the union of two sets:
Union WS1 WS2 = {the, war, and, dance, when, world, was, at, before, we, just, kept,
dancing}
5. Calculate the Jaccard Similarity using the mathematical formula given below:
J(A,B) =|A ∩ B|/|A ∪ B|
J(Text1, Text2) = |Intersection WS1 WS2|/|Union WS1 WS2|
=2/13=0,1538
6. Note that you are expected to output the Jaccard Similarity score with a precision 4.