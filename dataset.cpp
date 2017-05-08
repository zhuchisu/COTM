/*
 * Copyright (C) 2007 by
 *
 * 	Yang Yang
 *	yyang1988@pku.edu.cn
 * 	School of Electronic Engineering and Computer Science
 * 	Peking University
 */

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "constants.h"
#include "strtokenizer.h"
#include "dataset.h"


using namespace std;

int dataset::write_wordmap(string wordmapfile, mapword2id * pword2id) {
	FILE * fout = fopen(wordmapfile.c_str(), "w");
	//ofstream fout(wordmapfile.c_str());
	if (!fout ){
		printf("Cannot open file %s to write!\n", wordmapfile.c_str());
		return 1;
	}
	mapword2id::iterator it;
	fprintf(fout, "%d\n", int(pword2id->size()));
	//fout << pword2id->size() << endl;
	for (it = pword2id->begin(); it != pword2id->end(); it++) {
		fprintf(fout, "%s %d\n", (it->first).c_str(), it->second);
		//fout << (it->first).c_str() << " " << it->second << endl;
	}
	fclose(fout);
	return 0;
}

int dataset::read_wordmap(string wordmapfile, mapword2id * pword2id) {
	pword2id->clear();
	FILE * fin = fopen(wordmapfile.c_str(), "r");
	if (!fin) {
		printf("Cannot open file %s to read!\n", wordmapfile.c_str());
		return 1;
	}

	char buff[BUFF_SIZE_SHORT];
	string line;

	fgets(buff, BUFF_SIZE_SHORT - 1, fin);
	int nwords = atoi(buff);

	for (int i = 0; i < nwords; i++) {
		fgets(buff, BUFF_SIZE_SHORT - 1, fin);
		line = buff;
		strtokenizer strtok(line, " \t\r\n");
		if (strtok.count_tokens() != 2) {
			continue;
		}
		pword2id->insert(pair<string, int>(strtok.token(0), atoi(strtok.token(1).c_str())));
	}

	fclose(fin);
	return 0;
}

int dataset::read_wordmap(string wordmapfile, mapid2word * pid2word) {
	pid2word->clear();

	FILE * fin = fopen(wordmapfile.c_str(), "r");
	if (!fin) {
		printf("Cannot open file %s to read!\n", wordmapfile.c_str());
		return 1;
	}

	char buff[BUFF_SIZE_SHORT];
	string line;

	fgets(buff, BUFF_SIZE_SHORT - 1, fin);
	int nwords = atoi(buff);

	for (int i = 0; i < nwords; i++) {
		fgets(buff, BUFF_SIZE_SHORT - 1, fin);
		line = buff;

		strtokenizer strtok(line, " \t\r\n");
		if (strtok.count_tokens() != 2) {
			continue;
		}

		pid2word->insert(pair<int, string>(atoi(strtok.token(1).c_str()), strtok.token(0)));
	}
	fclose(fin);
	return 0;
}

int dataset::update_wordmap(string confile, string commfile, string wordmapfile, mapword2id word2id) {
	FILE * fincontent = fopen(confile.c_str(), "r");
	if (!fincontent) {
		printf("Cannot open file %s to read!\n", confile.c_str());
		return 1;
	}

	mapword2id::iterator it;
	char buff[BUFF_SIZE_LONG];
	string contentline;

	// get the number of documents
	fgets(buff, BUFF_SIZE_LONG - 1, fincontent);
	M = atoi(buff);
	printf("%d", M);
	if (M <= 0) {
		printf("No document available!\n");
		return 1;
	}

	// allocate memory for contents
	if (contents) {
		consDealloc();
	} else {
		contents = new content*[M];
	}
	V = 0;

	for (int i = 0; i < M; i++) {
		printf("%d", i);
		fgets(buff, BUFF_SIZE_LONG - 1, fincontent);
		contentline = buff;
		strtokenizer strtok(contentline, " \t\r\n");
		int length = strtok.count_tokens();
		if (length <= 0) {
			printf("Invalid (empty) document!\n");
			consDealloc();
			M = V = 0;
			return 1;
		}

		// allocate new document
		content * pcontent = new content(length);

		for (int j = 0; j < length; j++) {
			it = word2id.find(strtok.token(j));
			if (it == word2id.end()) {
				// word not found, i.e., new word
				pcontent->words[j] = word2id.size();
				word2id.insert(pair<string, int>(strtok.token(j), word2id.size()));
			} else {
				pcontent->words[j] = it->second;
			}
		}

		// add new doc to the corpus
		add_content(pcontent, i);
	}

	fclose(fincontent);

	FILE * fincomment = fopen(commfile.c_str(), "r");
	if (!fincomment) {
		printf("Cannot open file %s to read!\n", commfile.c_str());
		return 1;
	}
	string commentline;

	fgets(buff, BUFF_SIZE_LONG - 1, fincomment);
	commentline = buff;
	M = atoi(commentline.c_str());
	if (M <= 0) {
		printf("No news content available!\n");
		//cout << "No news content available!" << endl;
		return 1;
	}

	// allocate memory for contents
	if (comments) {
		commsDealloc();
	} else {
		comments = new comment*[M];
	}

	for (int i = 0; i < M; i++) {
		fgets(buff, BUFF_SIZE_LONG - 1, fincomment);
		//fincomment >> commentline;
		//getline(fincomment, commentline);
		commentline = buff;
		int C = atoi(commentline.c_str());
		//printf("C: %d \n", C);
		if (C <= 0) {
			printf("No comment available\n");
			return 1;
		}

		comments[i] = new comment(C); //each content with C comments.

		for (int j = 0; j < C; j++) {
			//fincomment >> commentline;
			//getline(fincomment, commentline);
			fgets(buff, BUFF_SIZE_LONG - 1, fincomment);
			commentline = buff;
			strtokenizer strtok(commentline, " \t\r\n");
			int length = strtok.count_tokens();

			if (length <= 0) {
  				printf("Invalid (empty) document!\n");
				commsDealloc();
  				//M = V = C = 0;
  				return 1;
  			}

			// allocate new comment
			line * pline = new line(length);
			for (int k = 0; k < length; k++) {
				it = word2id.find(strtok.token(k));
				if (it == word2id.end()) {
					pline->words[k] = word2id.size();
					word2id.insert(pair<string, int>(strtok.token(k), word2id.size()));
				} else {
					pline->words[k] = it->second;
				}
			}
			add_comment(pline, i, j);
			//comments[i]->commlines = pline;
		}

		//fclose(fin);


	}
	// write word map to file
	if (write_wordmap(wordmapfile, &word2id)) {
		return 1;
	}
	//fincomment.close();
	fclose(fincomment);
	// update number of words
	V = word2id.size();
	return 0;
}

int dataset::read_trndata(string confile, string commfile, string wordmapfile) {
	mapword2id word2id;
	//---------------confile----------------
	FILE * fincontent = fopen(confile.c_str(), "r");
	if (!fincontent) {
		printf("Cannot open file %s to read!\n", confile.c_str());
		return 1;
	}

	mapword2id::iterator it;
	char buff[BUFF_SIZE_LONG];
	string contentline;

	// get the number of documents
	fgets(buff, BUFF_SIZE_LONG - 1, fincontent);
	M = atoi(buff);
	if (M <= 0) {
		printf("No document available!\n");
		return 1;
	}

	// allocate memory for contents
	if (contents) {
		consDealloc();
	} else {
		contents = new content*[M];
	}
	// set number of words to zero
	// initiation only need ones
	V = 0;

	for (int i = 0; i < M; i++) {
		fgets(buff, BUFF_SIZE_LONG - 1, fincontent);
		contentline = buff;
		strtokenizer strtok(contentline, " \t\r\n");
		int length = strtok.count_tokens();
		if (length <= 0) {
			printf("Invalid (empty) document!\n");
			consDealloc();
			M = V = 0;
			return 1;
		}

		// allocate new document
		content * pcontent = new content(length);

		for (int j = 0; j < length; j++) {
			it = word2id.find(strtok.token(j));
			if (it == word2id.end()) {
				// word not found, i.e., new word
				pcontent->words[j] = word2id.size();
				word2id.insert(pair<string, int>(strtok.token(j), word2id.size()));
			} else {
				pcontent->words[j] = it->second;
			}
		}

		// add new doc to the corpus
		add_content(pcontent, i);
	}

	fclose(fincontent);

	//---------------commfile----------------
	//ifstream fincomment(commfile);
	FILE * fincomment = fopen(commfile.c_str(), "r");
	/*
	if (!fincomment.is_open()){
		cout << "Cannot open file " << commfile << " to read!" << endl;
		return 1;
	}*/

	if (!fincomment) {
		printf("Cannot open file %s to read!\n", commfile.c_str());
		return 1;
	}

	//mapword2id::iterator it;
	//char buff[BUFF_SIZE_LONG];
	string commentline;

	// get the number of documents
	//fincomment >> commentline;
	//getline(fincomment, commentline);
	fgets(buff, BUFF_SIZE_LONG - 1, fincomment);
	commentline = buff;
	M = atoi(commentline.c_str());
	if (M <= 0) {
		printf("No news content available!\n");
		//cout << "No news content available!" << endl;
		return 1;
	}

	// allocate memory for contents
	if (comments) {
		commsDealloc();
	} else {
		comments = new comment*[M];
	}

	for (int i = 0; i < M; i++) {
		//printf("M: %d\n", i);
		fgets(buff, BUFF_SIZE_LONG - 1, fincomment);
		//fincomment >> commentline;
		//getline(fincomment, commentline);
		commentline = buff;
		int C = atoi(commentline.c_str());
		if (C <= 0) {
			printf("No comment available\n");
			return 1;
		}

		comments[i] = new comment(C); //each content with C comments.

		for (int j = 0; j < C; j++) {
			//printf("C: %d\n", j);
			//fincomment >> commentline;
			//getline(fincomment, commentline);
			fgets(buff, BUFF_SIZE_LONG - 1, fincomment);
			commentline = buff;
			strtokenizer strtok(commentline, " \t\r\n");
			int length = strtok.count_tokens();

			if (length <= 0) {
				printf("hehe\n");
  				printf("Invalid (empty) document!\n");
				commsDealloc();
  				//M = V = C = 0;
  				return 1;
  			}

			// allocate new comment
			line * pline = new line(length);

			for (int k = 0; k < length; k++) {
				it = word2id.find(strtok.token(k));
				if (it == word2id.end()) {
					pline->words[k] = word2id.size();
					word2id.insert(pair<string, int>(strtok.token(k), word2id.size()));
				} else {
					pline->words[k] = it->second;
				}
			}
			add_comment(pline, i, j);
			//comments[i]->commlines = pline;
		}

		//fclose(fin);


	}
	// write word map to file
	if (write_wordmap(wordmapfile, &word2id)) {
		return 1;
	}
	//fincomment.close();
	fclose(fincomment);
	// update number of words
	V = word2id.size();
	return 0;
}
