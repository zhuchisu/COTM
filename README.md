# COTM
C++ running codes 4 "A Topic Model for Co-occurring Normal Documents and Short Texts". Published in World Wide Web Journal.

## compile
	> make clean
	> make all

## run
	> ./lda -est -alpha 0.5 -beta 0.1 -rou 0.5 -epsilon 0.5 -lamda 0.5 -T 20 -ktopics 100 -jtopics 50 -niters 500 -savestep 1000 -twords 100 -contfile seg_cont.dat -commfile seg_comm.dat

T: number of time slices;

seg_cont.dat: first file of T splitted Normal Documents, located in to root directory;

seg_comm.dat: first file of T splitted Short Texts, located in to root directory.

## format description
If the corpus contains 4 news article and each of them is followed by 4, 3, 2 and 1 comments, then the format of cont.dat is:
	
	4
	contents of news article 1
	contents of news article 2
	contents of news article 3
	contents of news article 4

and the format of comm.dat is:

	4(number of all news articles)
	4(number of comments for the 1st news article)
	comment 1 of news article 1
	comment 2 of news article 1
	comment 3 of news article 1
	comment 4 of news article 1
	3(number of comments for the 2nd news article)
	comment 1 of news article 2
	comment 2 of news article 2
	comment 3 of news article 2
	2
	comment 1 of news article 3
	comment 2 of news article 3
	1
	comment 1 of news article 4
