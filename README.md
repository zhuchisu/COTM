# COTM
C++ running codes 4 "A Topic Model for Co-occurring Normal Documents and Short Texts". Published in World Wide Web Journal.

## compile
	> make clean
	> make all

## data 
seg_cont(x).dat:file of Normal Documents, located in to root directory, splitted into 10 slices
	> seg_cont.dat seg_cont1.dat seg_cont2.dat seg_cont3.dat seg_cont4.dat seg_cont5.dat seg_cont6.dat seg_cont7.dat seg_cont8.dat seg_cont10.dat

seg_comm(x).dat:file of Short Texts, located in to root directory, splitted into 10 slices
	> seg_comm.dat seg_comm1.dat seg_comm2.dat seg_comm3.dat seg_comm4.dat seg_comm5.dat seg_comm6.dat seg_comm7.dat seg_comm8.dat seg_comm10.dat

## run
	> ./lda -est -alpha 0.5 -beta 0.1 -rou 0.5 -epsilon 0.5 -lamda 0.5 -T 20 -ktopics 100 -jtopics 50 -niters 500 -savestep 1000 -twords 100 -contfile seg_cont.dat -commfile seg_comm.dat

cont.dat: file of news articles, located in to root directory;

comm.dat: file containing comments for all news articles, located in to root directory.

##format description
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
