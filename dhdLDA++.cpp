/*
 * Copyright (C) 2007 by
 *
 * 	Yang Yang
 *	yyang1988@pku.edu.cn
 * 	School of Electronic Engineering and Computer Science
 * 	Peking University
 */

#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void show_help();

int main(int argc, char ** argv) {
	ostringstream ss;
	model lda, *plda;
	plda = &lda;

	if (lda.init(argc, argv)) {
		show_help();
		return 1;
	}

	if (lda.model_status == MODEL_STATUS_EST || lda.model_status == MODEL_STATUS_ESTC) {
		printf("plda->T = %d\n", plda->T);
		cout<< plda->confile << endl;
		cout<< plda->commfile << endl;
		for (int i = 0; i < plda->T; i++){
			//dataset update
			if (i >= 1){
				ss << i;
				cout<<ss.str()<<endl;
				plda->confile = plda->confile.substr(0,8) + ss.str() + plda->confile.substr(plda->confile.size()-4,plda->confile.size());
				plda->commfile = plda->commfile.substr(0,8) + ss.str() + plda->commfile.substr(plda->commfile.size()-4,plda->commfile.size());
				ss.str("");
				plda->ordinal = ss.str();
				cout<< plda->confile << endl;
				cout<< plda->commfile << endl;
				lda.update();
			}
			// parameter estimation
			lda.parameter_print();
			lda.estimate();
		}
	}

	return 0;
}

void show_help() {
    printf("Command line usage:\n");
    printf("\tlda -est -alpha <double> -beta <double> -rou <double> -epsilon <double> -lamda <double> -T <int> -ktopics <int> -jtopics <int> -niters <int> -savestep <int> -twords <int> -contfile <string> -commfile <string>\n");
}
