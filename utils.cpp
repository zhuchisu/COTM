/*
 * Copyright (C) 2007 by
 *
 *  Yang Yang
 *  yyang1988@pku.edu.cn
 *  School of Electronic Engineering and Computer Science
 *  Peking University
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "strtokenizer.h"
#include "utils.h"
#include "model.h"
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

int utils::parse_args(int argc, char ** argv, model * pmodel) {
    int model_status = MODEL_STATUS_UNKNOWN;
    string dir = "";
    string model_name = "";
    //------------------------
    //string dfile = "";
    string confile = "";
    string commfile = "";
    //------------------------
    double alpha = -1.0;
    double beta = -1.0;
    double epsilon = -1.0;
    double rou = -1.0;
    double lamda = -0.1;
    int K = 0;
    int J = 0;
    int T = 0;
    int niters = 0;
    int savestep = 0;
    int twords = 0;
    int withrawdata = 0;

    int i = 0;
    while (i < argc) {
    string arg = argv[i];

    if (arg == "-est") {
    	model_status = MODEL_STATUS_EST;

    } else if (arg == "-estc") {
    	model_status = MODEL_STATUS_ESTC;

    } else if (arg == "-inf") {
    	model_status = MODEL_STATUS_INF;

    } else if (arg == "-dir") {
    	dir = argv[++i];
    //---------------------------------
    //} else if (arg == "-dfile") {
    //    dfile = argv[++i];
    } else if (arg == "-contfile") {
    	confile = argv[++i];
    } else if (arg == "-commfile") {
    	commfile = argv[++i];
    //---------------------------------
    } else if (arg == "-model") {
    	model_name = argv[++i];
    } else if (arg == "-alpha") {
    	alpha = atof(argv[++i]);
    } else if (arg == "-beta") {
    	beta = atof(argv[++i]);
    } else if (arg == "-lamda") {
    	lamda = atof(argv[++i]);
    } else if (arg == "-rou") {
      rou = atof(argv[++i]);
    } else if (arg == "-epsilon"){
    	epsilon = atof(argv[++i]);
    } else if (arg == "-ktopics") {
    	K = atoi(argv[++i]);
    } else if (arg == "-niters") {
    	niters = atoi(argv[++i]);
    } else if (arg == "-savestep") {
    	savestep = atoi(argv[++i]);
    } else if (arg == "-twords") {
    	twords = atoi(argv[++i]);
    } else if (arg == "-withrawdata") {
    	withrawdata = 1;
    }else if(arg == "-jtopics"){
      J = atoi(argv[++i]);
    }else if(arg == "-T"){
      T = atoi(argv[++i]);
    }
    i++;
}

    if (model_status == MODEL_STATUS_EST) {
      //-----------------------------
      //if (dfile == "") {
      //  printf("Please specify the input data file for model estimation!\n");
      //  return 1;
      //  }
      if (confile == "") {
        printf("Please specify the input content file for model estimation!\n");
        return 1;
      } else if (commfile == "") {
        printf("Please specify the imput comment file for model estimation!\n");
        return 1;
      }
      //-----------------------------
      pmodel->model_status = model_status;

      printf("parameters:\n");
      if (K > 0) {
          pmodel->K = K;
          printf("K: %d ", pmodel->K);
      }

      if (J > 0) {
          pmodel->J = J;
          printf("J: %d ", pmodel->J);
      }

      if (T > 0) {
          pmodel->T = T;
          printf("T: %d ", pmodel->T);
      }

      if (alpha >= 0.0) {
          pmodel->alpha = alpha;
          printf("alpha: %f ", pmodel->alpha);
      } else {
          // default value for alpha
          pmodel->alpha = 50.0 / pmodel->K;
          printf("alpha: %f ", pmodel->alpha);
      }

      if (rou >= 0.0) {
          pmodel->rou = rou;
          printf("rou: %f ", pmodel->rou);
      } else {
          // default value for alpha
          printf("Error in initiating parameter rou !\n");
          return 1;
      }

        if (beta >= 0.0) {
            pmodel->beta = beta;
            printf("beta: %f ", pmodel->beta);
        }else{
            printf("Error in initiating parameter beta !\n");
            return 1;
        }

        if (epsilon > 0.0) {
        	  pmodel->epsilon = epsilon;
                    printf("epsilon: %f ", pmodel->epsilon);
        }else{
            printf("Error in initiating parameter epsilon !\n");
            return 1;
          }
      if (lamda > 0.0) {
    	  pmodel->lamda = lamda;
                  printf("epsilon: %f ", pmodel->lamda);
      }else{
          printf("Error in initiating parameter lamda !\n");
          return 1;
        }

      if (niters > 0) {
          pmodel->niters = niters;
          printf("niters: %d ", pmodel->niters);
      }else{
        printf("Error in initiating parameter niters !\n");
        return 1;
      }

      if (savestep > 0) {
          pmodel->savestep = savestep;
          printf("savestep: %d ", pmodel->savestep);
      }else{
        printf("Error in initiating parameter savestep !\n");
        return 1;
      }

      if (twords > 0) {
          pmodel->twords = twords;
          printf("twords: %d ", pmodel->twords);
      }else{
        printf("Error in initiating parameter twords !\n");
        return 1;
      }
      printf("\n");

      //--------------------------------
      pmodel->confile = confile;
      pmodel->commfile = commfile;
      //--------------------------------
      string::size_type idx = confile.find_last_of("/");
      if (idx == string::npos) {
          pmodel->dir = "./";
      } else {
          pmodel->dir = confile.substr(0, idx + 1);
          pmodel->confile = confile.substr(idx + 1, confile.size() - pmodel->dir.size());
          printf("dir = %s\n", pmodel->dir.c_str());
          printf("confile = %s\n", pmodel->confile.c_str());
      }

      string::size_type idy = commfile.find_last_of("/");
      if (idx == string::npos) {
        pmodel->dir = "./";
      } else {
        if (commfile.substr(0, idx + 1) != pmodel->dir){
          printf("Content file and comment file must be in the same directory!");
          return 1;
        }
          pmodel->commfile = commfile.substr(idx + 1, commfile.size() - pmodel->dir.size());
          printf("commfile = %s\n", pmodel->commfile.c_str());
      }
      //--------------------------------
    }

    if (model_status == MODEL_STATUS_ESTC) {
      if (dir == "") {
          printf("Please specify model directory!\n");
          return 1;
      }

      if (model_name == "") {
          printf("Please specify model name upon that you want to continue estimating!\n");
          return 1;
      }

      pmodel->model_status = model_status;

      if (dir[dir.size() - 1] != '/') {
          dir += "/";
      }
      pmodel->dir = dir;

      pmodel->model_name = model_name;

      if (niters > 0) {
          pmodel->niters = niters;
      }

      if (savestep > 0) {
          pmodel->savestep = savestep;
      }

      if (twords > 0) {
          pmodel->twords = twords;
      }

      // read <model>.others file to assign values for ntopics, alpha, beta, etc.
      if (read_and_parse(pmodel->dir + pmodel->model_name + pmodel->others_suffix, pmodel)) {
          return 1;
      }
        }

        if (model_status == MODEL_STATUS_INF) {
      if (dir == "") {
          printf("Please specify model directory please!\n");
          return 1;
      }

      if (model_name == "") {
          printf("Please specify model name for inference!\n");
          return 1;
      }

      if (confile == "") {
          printf("Please specify the new content file for inference!\n");
          return 1;
      }

    if (commfile == "") {
  	  printf("Please specify the new comment file for inference!\n");
  	  return 1;
    }

      pmodel->model_status = model_status;

      if (dir[dir.size() - 1] != '/') {
          dir += "/";
      }
      pmodel->dir = dir;

      pmodel->model_name = model_name;

    //--------------------------------
    //pmodel->dfile = dfile;
    pmodel->confile = confile;
    pmodel->commfile = commfile;
    //--------------------------------

      if (niters > 0) {
          pmodel->niters = niters;
      } else {
          // default number of Gibbs sampling iterations for doing inference
          pmodel->niters = 20;
      }

      if (twords > 0) {
          pmodel->twords = twords;
      }

      if (withrawdata > 0) {
          pmodel->withrawstrs = withrawdata;
      }

      // read <model>.others file to assign values for ntopics, alpha, beta, etc.
      if (read_and_parse(pmodel->dir + pmodel->model_name + pmodel->others_suffix, pmodel)) {
          return 1;
      }
        }

    if (model_status == MODEL_STATUS_UNKNOWN) {
      printf("Please specify the task you would like to perform (-est/-estc/-inf)!\n");
      return 1;
        }

    return 0;
  }

  int utils::read_and_parse(string filename, model * pmodel) {
  // open file <model>.others to read:
  // alpha=?
  // beta=?
  // ntopics=?
  // ndocs=?
  // nwords=?
  // citer=? // current iteration (when the model was saved)
  //ifstream fin(filename);
  FILE * fin = fopen(filename.c_str(), "r");
  /*
  if (!fin.is_open()) {
  cout << "Cannot open file: " << filename << endl;
  return 1;
  }*/

  if (!fin) {
  printf("Cannot open file: %s\n", filename.c_str());
  return 1;
  }

  char buff[BUFF_SIZE_SHORT];
  string line;

  while (fgets(buff, BUFF_SIZE_SHORT - 1, fin)) {
  line = buff;
  strtokenizer strtok(line, "= \t\r\n");
  int count = strtok.count_tokens();

  if (count != 2) {
    // invalid, ignore this line
    continue;
  }

  string optstr = strtok.token(0);
  string optval = strtok.token(1);

  if (optstr == "alpha") {
      pmodel->alpha = atof(optval.c_str());

    } else if (optstr == "beta") {
      pmodel->beta = atof(optval.c_str());

    } else if (optstr == "ntopics") {
      pmodel->K = atoi(optval.c_str());

    } else if (optstr == "ndocs") {
      pmodel->M = atoi(optval.c_str());

    } else if (optstr == "nwords") {
      pmodel->V = atoi(optval.c_str());

    } else if (optstr == "liter") {
      pmodel->liter = atoi(optval.c_str());

    } else {
      // any more?
    }
  }

  fclose(fin);

  return 0;
    /*
    string line;
  while (fin >> line) {
  	strtokenizer strtok(line, "= \t\r\n");
  	int count = strtok.count_tokens();

  	if (count != 2) {
  		// invalid, ignore this line
  		continue;
  	}

  	string optstr = strtok.token(0);
  	string optval = strtok.token(1);

  	if (optstr == "alpha") {
  		pmodel->alpha = atof(optval.c_str());

  	}
  	else if (optstr == "beta") {
  		pmodel->beta = atof(optval.c_str());

  	}
  	else if (optstr == "ntopics") {
  		pmodel->K = atoi(optval.c_str());

  	}
  	else if (optstr == "ndocs") {
  		pmodel->M = atoi(optval.c_str());

  	}
  	else if (optstr == "nwords") {
  		pmodel->V = atoi(optval.c_str());

  	}
  	else if (optstr == "liter") {
  		pmodel->liter = atoi(optval.c_str());

  	}
  	else {
  		// any more?
  	}
  }
  fin.close();
  return 0;*/

  }


  string utils::generate_model_name(int iter) {
  string model_name = "model-";

  char buff[BUFF_SIZE_SHORT];

  /*
  if (0 <= iter && iter < 10) {
  //sprintf_s(buff, "0000%d", iter);
    snprintf(buff, "0000%d", iter);
  } else if (10 <= iter && iter < 100) {
  sprintf_s(buff, "000%d", iter);
  } else if (100 <= iter && iter < 1000) {
  sprintf_s(buff, "00%d", iter);
  } else if (1000 <= iter && iter < 10000) {
  sprintf_s(buff, "0%d", iter);
  } else {
  sprintf_s(buff, "%d", iter);
  }*/

  if (iter >= 0) {
  model_name += buff;
  } else {
  model_name += "final";
  }

  return model_name;
  }

  void utils::sort(vector<double> & probs, vector<int> & words) {
    for (int i = 0; i < probs.size() - 1; i++) {
  for (int j = i + 1; j < probs.size(); j++) {
      if (probs[i] < probs[j]) {
  	double tempprob = probs[i];
  	int tempword = words[i];
  	probs[i] = probs[j];
  	words[i] = words[j];
  	probs[j] = tempprob;
  	words[j] = tempword;
      }
  }
    }
  }

  void utils::quicksort(vector<pair<int, double> > & vect, int left, int right) {
    int l_hold, r_hold;
    pair<int, double> pivot;

    l_hold = left;
    r_hold = right;
    int pivotidx = left;
    pivot = vect[pivotidx];

    while (left < right) {
  while (vect[right].second <= pivot.second && left < right) {
      right--;
  }
  if (left != right) {
      vect[left] = vect[right];
      left++;
  }
  while (vect[left].second >= pivot.second && left < right) {
      left++;
  }
  if (left != right) {
      vect[right] = vect[left];
      right--;
  }
    }

    vect[left] = pivot;
    pivotidx = left;
    left = l_hold;
    right = r_hold;

    if (left < pivotidx) {
  quicksort(vect, left, pivotidx - 1);
    }
    if (right > pivotidx) {
  quicksort(vect, pivotidx + 1, right);
    }
}
