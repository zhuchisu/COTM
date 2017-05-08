/*
 * Copyright (C) 2007 by
 *
 *  Yang Yang
 *  yyang1988@pku.edu.cn
 *  School of Electronic Engineering and Computer Science
 *  Peking University
 */

#ifndef	_DATASET_H
#define	_DATASET_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// map of words/terms [string => int]
typedef map<string, int> mapword2id;
// map of words/terms [int => string]
typedef map<int, string> mapid2word;

class line {
public:
    int * words;
    string rawstr;
    int length;

    line() {
      words = NULL;
      rawstr = "";
      length = 0;
    }

    line(int length) {
      this->length = length;
      rawstr = "";
      words = new int[length];
    }

    line(int length, int * words) {
      this->length = length;
      rawstr = "";
      this->words = new int[length];
      for (int i = 0; i < length; i++) {
        this->words[i] = words[i];
      }
    }

    line(int length, int * words, string rawstr) {
      this->length = length;
      this->rawstr = rawstr;
      this->words = new int[length];
      for (int i = 0; i < length; i++) {
        this->words[i] = words[i];
      }
    }

    line(vector<int> & doc) {
      this->length = doc.size();
      rawstr = "";
      this->words = new int[length];
      for (int i = 0; i < length; i++) {
        this->words[i] = doc[i];
      }
    }

    line(vector<int> & doc, string rawstr) {
      this->length = doc.size();
      this->rawstr = rawstr;
      this->words = new int[length];
      for (int i = 0; i < length; i++) {
        this->words[i] = doc[i];
      }
    }

    ~line() {
      if (words) {
        delete words;
      }
    }
};

class content {
public:
	int * words;
	string rawstr;
	int length;

	content() {
		words = NULL;
		rawstr = "";
		length = 0;
	}

	content(int length) {
		this->length = length;
		rawstr = "";
		words = new int[length];
	}

	content(int length, int * words) {
		this->length = length;
		rawstr = "";
		this->words = new int[length];
		for (int i = 0; i < length; i++) {
			this->words[i] = words[i];
		}
	}

	content(int length, int * words, string rawstr) {
		this->length = length;
		this->rawstr = rawstr;
		this->words = new int[length];
		for (int i = 0; i < length; i++) {
			this->words[i] = words[i];
		}
	}

	content(vector<int> & doc) {
		this->length = doc.size();
		rawstr = "";
		this->words = new int[length];
		for (int i = 0; i < length; i++) {
			this->words[i] = doc[i];
		}
	}

	content(vector<int> & doc, string rawstr) {
		this->length = doc.size();
		this->rawstr = rawstr;
		this->words = new int[length];
		for (int i = 0; i < length; i++) {
			this->words[i] = doc[i];
		}
	}

	~content() {
		if (words) {
			delete words;
		}
	}
};

//class content: public line
//{

//};

class comment {
public:
	int C;  //number of comments
	//string rawstr;
	line ** commlines;

	comment() {
		this->C = 0;
		this->commlines = NULL;
	}

	comment(int C) {
		this->C = C;
		this->commlines = new line*[C];
	}

	void commDealloc() {
		if (commlines) {
			delete commlines;
		}
	}

  ~comment() {
    if (commlines) {
      delete commlines;
    }
  }
};

class dataset {
public:
  //-------------------------------------
  int V; // number of words
  int M; // number of documents
  content ** contents; // size M x content[i].size()
  comment ** comments; //
  //-------------------------------------

  dataset() {
    this->M = 0;
    this->contents = NULL;
    this->comments = NULL;
  }

  dataset(int M) {
    this->M = M;
    this->contents = new content*[M];
    this->comments = new comment*[M];
  }

  ~dataset() {
    if (contents) {
      for (int i = 0; i < M; i++) {
        delete contents[i];
      }
    }
    delete contents;

    if (comments) {
      for (int i = 0; i < M; i++) {
        delete comments[i];
      }
    }
    delete comments;
  }

  void consDealloc() {
    if (contents) {
      for (int i = 0; i < M; i++) {
        delete contents[i];
      }
    }
    delete contents;
    contents = NULL;
  }

  void commsDealloc() {
    if (comments) {
      for (int i = 0; i < M; i++) {
        delete comments[i];
      }
    }
    delete comments;
    comments = NULL;
  }

  void add_content(content * doc, int idx) {
    if (0 <= idx && idx < M) {
      contents[idx] = doc;
    }
  }

  void comment_init(int idx, int C){
    this->comments[idx]->commlines = new line*[C];
  }

  void add_comment(line * doc, int idx, int idy) {  //idx: index of M,  idy: index of C
    if ((0 <= idx && idx < M) && (0 <= idy && idy < this->comments[idx]->C)) {
      comments[idx]->commlines[idy] = doc;
    }
  }

  static int write_wordmap(string wordmapfile, mapword2id * pword2id);
  static int read_wordmap(string wordmapfile, mapword2id * pword2id);
  static int read_wordmap(string wordmapfile, mapid2word * pid2word);
  int update_wordmap(string confile, string commfile, string wordmapfile, mapword2id word2id);

  int read_trndata(string confile, string commfile, string wordmapfile);
  int read_newdata(string dfile, string wordmapfile);
  int read_newdata_withrawstrs(string dfile, string wordmapfile);
};

#endif