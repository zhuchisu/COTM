/*
 * Copyright (C) 2007 by
 *
 * 	Yang Yang
 *	yyang1988@pku.edu.cn
 * 	School of Electronic Engineering and Computer Science
 * 	Peking University
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "strtokenizer.h"
#include "utils.h"
#include "dataset.h"
#include "model.h"
#include <math.h>
#include <fstream>
#include <iostream>

using namespace std;

model::~model() {
	if (p) {
		delete [] p;
	}
	if (q) {
		delete [] q;
	}
	if (ptrndata) {
		delete ptrndata;
	}
	if (ptrndata) {
		for (int i = 0; i < ptrndata->M; i++) {
			if (ptrndata->contents[i]) {
				delete ptrndata->contents[i];
			}
		}
		delete ptrndata;
	}
    	if (z) {
		for (int m = 0; m < M; m++) {
			if (z[m]) {
				delete [] z[m];
			}
		}
	}
	if (nkv) {
		for (int k = 0; k < K; k++) {
			if (nkv[k]) {
				delete [] nkv[k];
			}
		}
	}
	if (gkv) {
		for (int k = 0; k < K; k++) {
			if (gkv[k]) {
				delete [] gkv[k];
			}
		}
	}
	if (hjv) {
		for (int j = 0; j < J; j++) {
			if (hjv[j]) {
				delete [] hjv[j];
			}
		}
	}
	if (ndk) {
		for (int m = 0; m < M; m++) {
			if (ndk[m]) {
				delete [] ndk[m];
			}
		}
	}
	if (gdk) {
		for (int m = 0; m < M; m++) {
			if (gdk[m]) {
				delete [] gdk[m];
			}
		}
	}
	if (pdcx) {
		for (int m = 0; m < M; m++) {
			if (pdcx[m]) {
				delete pdcx[m];
			}
		}
	}
	if (pdcy) {
		for (int m = 0; m < M; m++) {
			if (pdcy[m]) {
				delete pdcy[m];
			}
		}
	}
	if (nkvsum) {
		delete [] nkvsum;
	}
	if (gkvsum) {
		delete [] gkvsum;
	}
	if (hjvsum) {
		delete [] hjvsum;
	}
	if (ndksum) {
		delete [] ndksum;
	}
	if (gdksum) {
		delete [] gdksum;
	}
	if (hj) {
		delete [] hj;
	}
	if (ksi) {
		delete [] ksi;
	}
	if (theta) {
		for (int m = 0; m < M; m++) {
			if (theta[m]) {
				delete [] theta[m];
			}
		}
	}
	if (phi) {
		for (int k = 0; k < K; k++) {
			if (phi[k]) {
				delete [] phi[k];
			}
		}
	}
	if (psi) {
		for (int j = 0; j < J; j++) {
			if (psi[j]) {
				delete [] psi[j];
			}
		}
	}
    // only for inference
	if (newz) {
		for (int m = 0; m < newM; m++) {
			if (newz[m]) {
				delete newz[m];
			}
		}
	}
	if (newnw) {
		for (int w = 0; w < newV; w++) {
			if (newnw[w]) {
				delete newnw[w];
			}
		}
	}
	if (newnd) {
		for (int m = 0; m < newM; m++) {
			if (newnd[m]) {
				delete newnd[m];
			}
		}
	}
	if (newnwsum) {
		delete newnwsum;
	}
	if (newndsum) {
		delete newndsum;
	}
	if (newtheta) {
		for (int m = 0; m < newM; m++) {
			if (newtheta[m]) {
				delete newtheta[m];
			}
		}
	}
	if (newphi) {
		for (int k = 0; k < K; k++) {
			if (newphi[k]) {
				delete newphi[k];
			}
		}
	}
}

void model::set_default_values() {
	wordmapfile = "wordmap.txt";
	trainlogfile = "trainlog.txt";
	//tassign_suffix = ".tassign";
	tassign_con_suffix = ".contassign";
	tassign_comm_suffix = ".commtassign";
	identifier_suffix = ".identifier";
	theta_suffix = ".theta";
	phi_suffix = ".phi";
	psi_suffix = ".psi";
	ksi_suffix = ".ksi";
	pdc_suffix = ".pdc";
	others_suffix = ".others";
	twords_phi_suffix = ".phitwords";
	twords_psi_suffix = ".psitwords";

	dir = "./";
	//dfile = "trndocs.dat";
	model_name = "model-final";
	model_status = MODEL_STATUS_UNKNOWN;

	ptrndata = NULL;
	pnewdata = NULL;

	M = 0;
	V = 0;
	K = 100;
	J = 50;
	alpha = 50.0 / K;
	beta = 0.1;
	epsilon = 0.1;
	rou = 0.1;
	lamda = 0.1;
	niters = 2000;
	liter = 0;
	savestep = 200;
	twords = 0;
	withrawstrs = 0;

	//alphaM = NULL;
	//betaFM = NULL;
	//betaLM = NULL;
	//epsilonV = NULL;
	//Kalpha = NULL;
	//VbetaF = NULL;
	//VbetaL = NULL;
	Jepsilon = 0;

	p = NULL;
	q = NULL;
	z = NULL;
	nw = NULL;
	nd = NULL;
	nwsum = NULL;
	ndsum = NULL;
	theta = NULL;
	phi = NULL;

	ndk = NULL;
	ndksum = NULL;
	gdk = NULL;
	gdksum = NULL;
	nkv = NULL;
	nkvsum = NULL;
	gkv = NULL;
	gkvsum = NULL;
	hjv = NULL;
	hjvsum = NULL;
	theta = NULL;
	phi = NULL;
	psi = NULL;
	hj = NULL;
	hjsum = 0;

	newM = 0;
	newV = 0;
	newz = NULL;
	newnw = NULL;
	newnd = NULL;
	newnwsum = NULL;
	newndsum = NULL;
	newtheta = NULL;
	newphi = NULL;
}

void model::parameter_print() {
	printf("K: %d\n", K); // # topics
	printf("V: %d\n", V); // # words
	printf("D: %d\n", M); // # documents
	printf("alpha: %f\n", alpha);
	printf("Kalpha: %f\n", Kalpha);
}

int model::parse_args(int argc, char ** argv) {
	return utils::parse_args(argc, argv, this);
}

int model::init(int argc, char ** argv) {
    	// call parse_args
	if (parse_args(argc, argv)) {
		//printf("haha\n");
		return 1;
	}
	if (model_status == MODEL_STATUS_EST) {
		// estimating the model from scratch
		if (init_est()) {
			return 1;
		}
	}
	return 0;
}


int model::save_model(string model_name) {
	if (save_model_tassign(dir + model_name + tassign_con_suffix + ordinal, dir + model_name + tassign_comm_suffix + ordinal)) {
		return 1;
	}
	if (save_model_others(dir + model_name + others_suffix + ordinal)) {
		return 1;
	}
	if (save_model_identifiers(dir + model_name + identifier_suffix + ordinal)) {
		return 1;
	}
	if (save_model_theta(dir + model_name + theta_suffix + ordinal)) {
		return 1;
	}
	if (save_model_phi(dir + model_name + phi_suffix + ordinal)) {
		return 1;
	}
	if (save_model_psi(dir + model_name + psi_suffix + ordinal)) {
		return 1;
	}
	if (save_model_ksi(dir + model_name + ksi_suffix + ordinal)) {
		return 1;
	}
	if (save_model_pdc(dir + model_name + pdc_suffix + ordinal)) {
		return 1;
	}
	if (twords > 0) {
		if (save_model_twords_psi(dir + model_name + twords_psi_suffix + ordinal)) {
			return 1;
		}
	}
	if (twords > 0) {
		if (save_model_twords_phi(dir + model_name + twords_phi_suffix + ordinal)) {
			return 1;
		}
	}
	printf("saving model finished.\n");
	return 0;
}

int model::save_model_tassign(string confile, string commfile) {
	printf("saving model tassign into: %s and %s \n", confile.c_str(), commfile.c_str());
	int i, j, k, C;
	//---------------------confile-------------------------
	FILE * foutcon = fopen(confile.c_str(), "w");

	if (!foutcon) {
		printf("Cannot open file %s to save!\n", confile.c_str());
		return 1;
	}
	// wirte docs with topic assignments for words
	for (i = 0; i < ptrndata->M; i++) {
		for (j = 0; j < ptrndata->contents[i]->length; j++) {
			fprintf(foutcon, "%d:%d ", ptrndata->contents[i]->words[j], z[i][j]);
		}
		fprintf(foutcon, "\n");
	}
	fclose(foutcon);

	//---------------------commfile-------------------------
	FILE * foutcomm = fopen(commfile.c_str(), "w");

	if (!foutcomm) {
		printf("Cannot open file %s to save!\n", commfile.c_str());
		return 1;
	}
	// write docs with topic assignments for words
	fprintf(foutcomm, "%d\n", M);
	for (i = 0; i < ptrndata->M; i++) {
		C = ptrndata->comments[i]->C;
		fprintf(foutcomm, "%d\n", C);
		for (j = 0; j < C; j++) {
			fprintf(foutcomm, "%d th:%f %f\n", j, pdcx[i][j], pdcy[i][j]);
		}
		fprintf(foutcomm, "------------------------------\n");
	}
	//foutcomm.close();
	fclose(foutcomm);
	return 0;
}

int model::save_model_theta(string filename) {
	printf("saving model theta into: %s \n", filename.c_str());
	FILE * fout = fopen(filename.c_str(), "w");
	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < K; j++) {
			fprintf(fout, "%f ", theta[i][j]);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
	return 0;
}

int model::save_model_identifiers(string filename) {
	printf("saving model identifiers into: %s \n", filename.c_str());
	FILE * fout = fopen(filename.c_str(), "w");
	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}
	for (int i = 0; i < ptrndata->M; i++) {
		fprintf(fout, "Document %dth:\n", i);
		int C = ptrndata->comments[i]->C;
		for (int c = 0; c < C; c++) {
			fprintf(fout, "Comment %dth:\n", c);
			int N = ptrndata->comments[i]->commlines[c]->length;
			for (int n = 0; n < N; n++){
				fprintf(fout, "%d ", I[i][c][n]);
			}
			fprintf(fout, "\n");
		}
	}
	fclose(fout);
	return 0;
}

int model::save_model_phi(string filename) {
	printf("saving model phi into: %s \n", filename.c_str());
	//ofstream fout;
    FILE * fout = fopen(filename.c_str(), "w");
	/*
	if (!fout.is_open()) {
		cout << "Cannot open file " << filename << " to save!" << endl;
		return 1;
	}
	*/

    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }

    for (int i = 0; i < K; i++) {
		for (int j = 0; j < V; j++) {
			fprintf(fout, "%f ", phi[i][j]);
		}
	fprintf(fout, "\n");
    }

    fclose(fout);

    return 0;
}

int model::save_model_ksi(string filename) {
	printf("saving model ksi into: %s \n", filename.c_str());

    FILE * fout = fopen(filename.c_str(), "w");

    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }

    for (int j = 0; j < J; j++) {
	fprintf(fout, "%f ", ksi[j]);
    }

    fclose(fout);

    return 0;
}

int model::save_model_pdc(string filename) {
	printf("saving model pdc into: %s \n", filename.c_str());
    FILE * fout = fopen(filename.c_str(), "w");

    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }

    for (int m = 0; m < M; m++) {
    	fprintf(fout, "Document %dth:\n", m);
    	int C = ptrndata->comments[m]->C;
		for (int c = 0; c < C; c++) {
			fprintf(fout, "%d: %f %f\n", c, pdcx[m][c], pdcy[m][c]);
		}
		fprintf(fout, "\n");
    }

    fclose(fout);
    return 0;
}

int model::save_model_psi(string filename) {
	printf("saving model psi into: %s \n", filename.c_str());
	FILE * fout = fopen(filename.c_str(), "w");

	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}

	for (int i = 0; i < J; i++) {
		for (int j = 0; j < V; j++) {
			fprintf(fout, "%f ", psi[i][j]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);

	return 0;
}

int model::save_model_others(string filename) {
	printf("saving model others into: %s \n", filename.c_str());
	FILE * fout = fopen(filename.c_str(), "w");

	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}

	fprintf(fout, "alpha=%f\n", alpha);
	fprintf(fout, "beta=%f\n", beta);
	fprintf(fout, "ntopics=%d\n", K);
	fprintf(fout, "ndocs=%d\n", M);
	fprintf(fout, "nwords=%d\n", V);
	fprintf(fout, "liter=%d\n", liter);

	fclose(fout);
	return 0;
}

int model::save_model_twords_phi(string filename) {
	printf("saving model twords phi into: %s \n", filename.c_str());
	FILE * fout = fopen(filename.c_str(), "w");

    if (twords > V) {
		twords = V;
    }
    mapid2word::iterator it;

    for (int k = 0; k < K; k++) {
		vector<pair<int, double> > words_probs;
		pair<int, double> word_prob;
		for (int w = 0; w < V; w++) {
			word_prob.first = w;
			word_prob.second = phi[k][w];
			words_probs.push_back(word_prob);
		}

		// quick sort to sort word-topic probability
		utils::quicksort(words_probs, 0, words_probs.size() - 1);

		fprintf(fout, "Topic %dth:\n", k);
		for (int i = 0; i < twords; i++) {
			it = id2word.find(words_probs[i].first);
			if (it != id2word.end()) {
				fprintf(fout, "\t%s   %f\n", (it->second).c_str(), words_probs[i].second);
			}
		}
		vector<pair<int, double> >().swap(words_probs);
    }
    fclose(fout);

    return 0;
}

int model::save_model_twords_psi(string filename) {
	printf("saving model twrods psi into: %s \n", filename.c_str());
	FILE * fout = fopen(filename.c_str(), "w");

	if (twords > V) {
		twords = V;
	}
	mapid2word::iterator it;

	for (int k = 0; k < J; k++) {
		vector<pair<int, double> > words_probs;
		pair<int, double> word_prob;
		for (int w = 0; w < V; w++) {
			word_prob.first = w;
			word_prob.second = psi[k][w];
			words_probs.push_back(word_prob);
		}

		// quick sort to sort word-topic probability
		utils::quicksort(words_probs, 0, words_probs.size() - 1);

		fprintf(fout, "Topic %dth:\n", k);
		for (int i = 0; i < twords; i++) {
			it = id2word.find(words_probs[i].first);
			if (it != id2word.end()) {
				fprintf(fout, "\t%s   %f\n", (it->second).c_str(), words_probs[i].second);
			}
		}
	}

	return 0;
}

int model::update() {
	int m, n, w, k, v, j, d, i, c;

	delete [] p;
	p = new double[K];
	for (k = 0; k < K; k++){
		p[k] = 0;
	}
	delete [] q;
	q = new double[J];
	for (j = 0; j < J; j++){
		q[j] = 0;
	}
	ptrndata = new dataset;
	if (ptrndata->read_wordmap(dir + wordmapfile, &word2id)) {
    	printf("Fail to read wordmap!\n");
	return 1;
	}
	if (ptrndata->update_wordmap(dir + confile, dir + commfile, dir + wordmapfile, word2id)) {
    		printf("Fail to update wordmap!\n");
    		return 1;
	}

	oldV = V;
	betaFMrem = new double*[K];
	for (k = 0; k < K; k++) {
		betaFMrem[k] = new double[V];
		for (v = 0; v < oldV; v++) {
			betaFMrem[k][v] = 0;
		}
	}
	for (k = 0; k < K; k++) {
		for (v = 0; v < oldV; v++) {
			betaFMrem[k][v] = betaFM[k][v];
		}
	}
	//for (k = 0; k < K; k++) {
	//	delete [] betaFM[k];
	//}
	//delete [] betaFM;

	M = ptrndata->M;
	V = ptrndata->V;
	for (k = 0; k < K; k++) {
		betaFM[k] = new double[V];
		for (v = 0; v < V; v++) {
			betaFM[k][v] = 0;
		}
	}
	for (k = 0; k < K; k++) {
		for (v = 0; v < oldV; v++) {
			betaFM[k][v] = betaFMrem[k][v];
		}
	}
	//betaFM
	//--------------------------------
	ndk = new int*[M];
	for (d = 0; d < M; d++) {
		ndk[d] = new int[K];
		for (k = 0; k < K; k++) {
			ndk[d][k] = 0;
		}
	}

	ndksum = new int[M];
	for (d = 0; d < M; d++) {
		ndksum[d] = 0;
	}
	gdk = new int*[M];
	for (d = 0; d < M; d++) {
		gdk[d] = new int[K];
		for (k = 0; k < K; k++) {
			gdk[d][k] = 0;
		}
	}
	gdksum = new int[M];
	for (d = 0; d < M; d++) {
		gdksum[d] = 0;
	}
	nkv = new int*[K];
	for (k = 0; k < K; k++) {
		nkv[k] = new int[V];
		for (v = 0; v < V; v++) {
			nkv[k][v] = 0;
		}
	}
	nkvsum = new int[K];
	for (k = 0; k < K; k++) {
		nkvsum[k] = 0;
	}
	gkv = new int*[K];
	for (k = 0; k < K; k++) {
		gkv[k] = new int[V];
		for (v = 0; v < V; v++) {
			gkv[k][v] = 0;
		}
	}
	gkvsum = new int[K];
	for (k = 0; k < K; k++){
		gkvsum[k] = 0;
	}
	hjv = new int*[J];
	for (int j = 0; j < J; j++) {
		hjv[j] = new int[V];
		for (int v = 0; v < V; v++) {
			hjv[j][v] = 0;
		}
	}
	hjvsum = new int[J];
	for (j = 0; j < J; j++){
		hjvsum[j] = 0;
	}
	hj = new int[J];
	for (j = 0; j < J; j++){
		hj[j] = 0;
	}
	sdc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		sdc[i] = new int[C];
		for (c = 0; c < C; c++){
			sdc[i][c] = 0;
		}
	}
	rdc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		rdc[i] = new int[C];
		for (c = 0; c < C; c++){
			rdc[i][c] = 0;
		}
	}
	xdc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		xdc[i] = new int[C];
		for (c = 0; c < C; c++){
			xdc[i][c] = 0;
		}
	}
	ydc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		ydc[i] = new int[C];
		for (c = 0; c < C; c++){
			ydc[i][c] = 0;
		}
	}
	//-----------------------------------
	// initialize topic assignments of content
	srand((unsigned)time(NULL));
	z = new int*[M];
	for (m = 0; m < ptrndata->M; m++) {
		int N = ptrndata->contents[m]->length;
		z[m] = new int[N];

		// initialize for z
		for (n = 0; n < N; n++) {
			//int topic = (int)(((double)random() / RAND_MAX) * K);
			int topic = (int)(((double)rand() / RAND_MAX) * K);
			if (topic == K)
			{
				topic = K - 1;
			}
			z[m][n] = topic;

			// number of words in document d assigned to topic k
			ndk[m][topic] += 1;
			// number of instances of word i assigned to topic k
			nkv[topic][ptrndata->contents[m]->words[n]] += 1;
			nkvsum[topic] += 1;
		}
		ndksum[m] = N;
	}
	// initialize boolean identifiers.
	I = new bool** [M];
	for (i = 0; i < ptrndata->M; i++) {
		int C = ptrndata->comments[i]->C;
		I[i] = new bool*[C];
		// initialize for I
		for (c = 0; c < C; c++) {
			int N = ptrndata->comments[i]->commlines[c]->length;
			I[i][c] = new bool[N];
			for (n = 0; n < N; n++){
				double temptest = (double)rand() / RAND_MAX;
				if (temptest > 0.5) {
					I[i][c][n] = true;
				} else {
					I[i][c][n] = false;
				}
			}
		}
	}

	for (i = 0; i < ptrndata->M; i++) {
		int C = ptrndata->comments[i]->C;
		for (c = 0; c < C; c++) {
			int N = ptrndata->comments[i]->commlines[c]->length;
			int Topic_x = (int)(((double)rand() / RAND_MAX) * K);
			if (Topic_x == K)
			{
				Topic_x = K - 1;
			}
			xdc[i][c] = Topic_x;
			gdk[i][Topic_x] += 1;
			gdksum[i] += 1;
			int Topic_y = (int)(((double)rand() / RAND_MAX) * J);
			if (Topic_y == J)
			{
				Topic_y = J - 1;
			}
			ydc[i][c] = Topic_y;
			hj[Topic_y] += 1;
			hjsum += 1;
			for (n = 0; n < N; n++){
				if(I[i][c][n]) {
					gkv[Topic_x][ptrndata->comments[i]->commlines[c]->words[n]] += 1;
					gkvsum[Topic_x] += 1;
					sdc[i][c] += 1;
				}else{
					hjv[Topic_y][ptrndata->comments[i]->commlines[c]->words[n]] += 1;
					hjvsum[Topic_y] += 1;
					rdc[i][c] += 1;
				}
			}
		}
	}

	phi = new double*[K];
	for (k = 0; k < K; k++) {
		phi[k] = new double[V];
		for (int v = 0; v < V; v++)
		{
			phi[k][v] = 0.0;
		}
	}
	psi = new double*[J];
	for (j = 0; j < J; j++) {
		psi[j] = new double[V];
		for (int v = 0; v < V; v++)
		{
			psi[j][v] = 0.0;
		}
	}

	theta = new double*[M];
	for (m = 0; m < M; m++) {
		theta[m] = new double[K];
		for (int k = 0; k < K; k++)
		{
			theta[m][k] = 0.0;
		}
	}

	ksi = new double [J];
	for (j = 0; j < J; j++) {
		ksi[j] = 0.0;
	}

	pdcx = new double* [M];
	for (m = 0; m < M; m++) {
		int C = ptrndata->comments[m]->C;
		pdcx[m] = new double[C];
		for (c = 0; c < C; c++) {
			pdcx[m][c] = 0.0;
		}
	}

	pdcy = new double* [M];
	for (m = 0; m < M; m++) {
		int C = ptrndata->comments[m]->C;
		pdcy[m] = new double[C];
		for (c = 0; c < C; c++) {
			pdcy[m][c] = 0.0;
		}
	}

    return 0;
}

int model::init_est() {
    int m, n, w, k, v, j, d, i, c;

	p = new double[K];
	for (k = 0; k < K; k++){
		p[k] = 0;
	}
	q = new double[J];
	for (j = 0; j < J; j++){
		q[j] = 0;
	}
	ptrndata = new dataset;
	if (ptrndata->read_trndata(dir + confile, dir + commfile, dir + wordmapfile)) {
		printf("Fail to read training data!\n");
		return 1;
	}

    // + allocate memory and assign values for variables
	M = ptrndata->M;
	V = ptrndata->V;
    //--------------------------------
    // K: from command line or default value
    // J: from command line or default value
    // alpha, beta: from command line or default values
    // niters, savestep: from command line or default values
    //---------alpha----------
    /*
    alphaM = new double*[M];
    for (d = 0; d < M; d++) {
    	alphaM[d] = new double[K];
    	for (k = 0; k < K; k++) {
    		alphaM[d][k] = alpha;
    	}
    }
    Kalpha = new double*[M];
    for (d = 0; d < M; d++) {
    	Kalpha[d] = K * alpha;
    }*/

    //---------beta----------
	betaFM = new double*[K];
	for (k = 0; k < K; k++) {
		betaFM[k] = new double[V];
		for (v = 0; v < V; v++) {
			betaFM[k][v] = beta;
		}
	}
	VbetaF = new double[K];
	for (k = 0; k < K; k++) {
		VbetaF[k] = 0.0;
		for (v = 0; v < V; v ++) {
			VbetaF[k] = VbetaF[k] + betaFM[k][v];
		}
	}

	betaLM = new double*[J];
	for (j = 0; j < J; j++) {
		betaLM[j] = new double[V];
		for (v = 0; v < V; v++) {
			betaLM[j][v] = beta;
		}
	}
	VbetaL = new double[J];
	for (j = 0; j < J; j++) {
		VbetaL[j] = 0.0;
		for (v = 0; v < V; v++) {
			VbetaL[j] = VbetaL[j] + betaLM[j][v];
		}
	}

    //---------epsilon----------
	epsilonV = new double[J];
	for (j = 0; j < J; j++) {
		epsilonV[j] = epsilon;
	}
	Jepsilon = 0.0;
	for (j = 0; j < J; j++) {
		Jepsilon = Jepsilon + epsilonV[j];
	}

    //--------------------------------
	ndk = new int*[M];
	for (d = 0; d < M; d++) {
		ndk[d] = new int[K];
		for (k = 0; k < K; k++) {
			ndk[d][k] = 0;
		}
	}
	ndksum = new int[M];
	for (d = 0; d < M; d++) {
		ndksum[d] = 0;
	}
	gdk = new int*[M];
	for (d = 0; d < M; d++) {
		gdk[d] = new int[K];
		for (k = 0; k < K; k++) {
			gdk[d][k] = 0;
		}
	}
	gdksum = new int[M];
	for (d = 0; d < M; d++) {
		gdksum[d] = 0;
	}
	nkv = new int*[K];
	for (k = 0; k < K; k++) {
		nkv[k] = new int[V];
		for (v = 0; v < V; v++) {
			nkv[k][v] = 0;
		}
	}
	nkvsum = new int[K];
	for (k = 0; k < K; k++) {
		nkvsum[k] = 0;
	}
	gkv = new int*[K];
	for (k = 0; k < K; k++) {
		gkv[k] = new int[V];
		for (v = 0; v < V; v++) {
			gkv[k][v] = 0;
		}
	}
	gkvsum = new int[K];
	for (k = 0; k < K; k++){
		gkvsum[k] = 0;
	}
	hjv = new int*[J];
	for (int j = 0; j < J; j++) {
		hjv[j] = new int[V];
		for (int v = 0; v < V; v++) {
			hjv[j][v] = 0;
		}
	}
	hjvsum = new int[J];
	for (j = 0; j < J; j++){
		hjvsum[j] = 0;
	}
	hj = new int[J];
	for (j = 0; j < J; j++){
		hj[j] = 0;
	}
	sdc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		sdc[i] = new int[C];
		for (c = 0; c < C; c++){
			sdc[i][c] = 0;
		}
	}
	rdc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		rdc[i] = new int[C];
		for (c = 0; c < C; c++){
			rdc[i][c] = 0;
		}
	}
	xdc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		xdc[i] = new int[C];
		for (c = 0; c < C; c++){
			xdc[i][c] = 0;
		}
	}
	ydc = new int* [M];
	for (i = 0; i < ptrndata->M; i++){
		int C = ptrndata->comments[i]->C;
		ydc[i] = new int[C];
		for (c = 0; c < C; c++){
			ydc[i][c] = 0;
		}
	}
	//-----------------------------------
	// initialize topic assignments of content
	srand((unsigned)time(NULL));
	z = new int*[M];
	for (m = 0; m < ptrndata->M; m++) {
		int N = ptrndata->contents[m]->length;
		z[m] = new int[N];

		// initialize for z
		for (n = 0; n < N; n++) {
			//int topic = (int)(((double)random() / RAND_MAX) * K);
			int topic = (int)(((double)rand() / RAND_MAX) * K);
			if (topic == K)
			{
				topic = K - 1;
			}
			z[m][n] = topic;

			// number of words in document d assigned to topic k
			ndk[m][topic] += 1;
			// number of instances of word i assigned to topic k
			nkv[topic][ptrndata->contents[m]->words[n]] += 1;
			nkvsum[topic] += 1;
		}
		ndksum[m] = N;
	}
	// initialize boolean identifiers.
	I = new bool** [M];
	for (i = 0; i < ptrndata->M; i++) {
		int C = ptrndata->comments[i]->C;
		I[i] = new bool*[C];
		// initialize for I
		for (c = 0; c < C; c++) {
			int N = ptrndata->comments[i]->commlines[c]->length;
			I[i][c] = new bool[N];
			for (n = 0; n < N; n++){
				double temptest = (double)rand() / RAND_MAX;
				if (temptest > 0.5) {
					I[i][c][n] = true;
				} else {
					I[i][c][n] = false;
				}
			}
		}
	}

	for (i = 0; i < ptrndata->M; i++) {
		int C = ptrndata->comments[i]->C;
		for (c = 0; c < C; c++) {
			int N = ptrndata->comments[i]->commlines[c]->length;
			int Topic_x = (int)(((double)rand() / RAND_MAX) * K);
			if (Topic_x == K)
			{
				Topic_x = K - 1;
			}
			xdc[i][c] = Topic_x;
			gdk[i][Topic_x] += 1;
			gdksum[i] += 1;
			int Topic_y = (int)(((double)rand() / RAND_MAX) * J);
			if (Topic_y == J)
			{
				Topic_y = J - 1;
			}
			ydc[i][c] = Topic_y;
			hj[Topic_y] += 1;
			hjsum += 1;
			for (n = 0; n < N; n++){
				if(I[i][c][n]) {
					gkv[Topic_x][ptrndata->comments[i]->commlines[c]->words[n]] += 1;
					gkvsum[Topic_x] += 1;
					sdc[i][c] += 1;
				}else{
					hjv[Topic_y][ptrndata->comments[i]->commlines[c]->words[n]] += 1;
					hjvsum[Topic_y] += 1;
					rdc[i][c] += 1;
				}
			}
		}
	}

	phi = new double*[K];
	for (k = 0; k < K; k++) {
		phi[k] = new double[V];
		for (int v = 0; v < V; v++)
		{
			phi[k][v] = 0.0;
		}
	}
	psi = new double*[J];
	for (j = 0; j < J; j++) {
		psi[j] = new double[V];
		for (int v = 0; v < V; v++)
		{
			psi[j][v] = 0.0;
		}
	}

	theta = new double*[M];
	for (m = 0; m < M; m++) {
		theta[m] = new double[K];
		for (int k = 0; k < K; k++)
		{
			theta[m][k] = 0.0;
		}
	}

	ksi = new double [J];
	for (j = 0; j < J; j++) {
		ksi[j] = 0.0;
	}

	pdcx = new double* [M];
	for (m = 0; m < M; m++) {
		int C = ptrndata->comments[m]->C;
		pdcx[m] = new double[C];
		for (c = 0; c < C; c++) {
			pdcx[m][c] = 0.0;
		}
	}

	pdcy = new double* [M];
	for (m = 0; m < M; m++) {
		int C = ptrndata->comments[m]->C;
		pdcy[m] = new double[C];
		for (c = 0; c < C; c++) {
			pdcy[m][c] = 0.0;
		}
	}

    return 0;
}

void model::estimate() {
	if (twords > 0) {
	    // print out top words per topic
	    dataset::read_wordmap(dir + wordmapfile, &id2word);
	}
	Kalpha = K * alpha;
	//Vbeta = V * beta;

	printf("Sampling %d iterations!\n", niters);
	int last_iter = liter;
	for (liter = last_iter + 1; liter <= niters + last_iter; liter++) {
		printf("Iteration %d ...\n", liter);
		// for all z_i
		for (int m = 0; m < M; m++) {
			for (int n = 0; n < ptrndata->contents[m]->length; n++) {
				//printf("m: %d, n: %d ...\n", m,n);
				// (z_i = z[m][n])
				// sample from p(z_i|z_-i, w)
				int topic = sampling_step_one(m, n);
				z[m][n] = topic;
			}
		}
		//printf("step one finished.");

		for (int m = 0; m < M; m++) {
			for (int c = 0; c < ptrndata->comments[m]->C; c++) {
				xdc[m][c] = sampling_step_two_x(m, c);
				ydc[m][c] = sampling_step_two_y(m, c);
			}
		}
		//printf("step two finished.");

		for (int m = 0; m < M; m++) {
			for (int c = 0; c < ptrndata->comments[m]->C; c++) {
				sampling_step_three(m, c);
			}
		}
		//printf("step three finished.");
	}

    printf("Gibbs sampling completed!\n");
    printf("Saving the final model!\n");
    compute_theta();
    compute_phi();
    compute_psi();
    compute_ksi();
    compute_pdc();
    update_paras();
    liter--;
    save_model(utils::generate_model_name(-1));
}

int model::sampling_step_one(int m, int n) {
	// remove z_i from the count variables
	int topic = z[m][n];
	int w = ptrndata->contents[m]->words[n];
	ndk[m][topic] -= 1;
	nkv[topic][w] -= 1;
	ndksum[m] -= 1;
	nkvsum[topic] -= 1;
	for (int k = 0; k < K; k++) {
		/*
		if (m == 52 && n == 241){
			printf("-------------------\n");
			printf("m: %d, n: %d ...\n", m,n);
			printf("k: %d ...\n", k);
			printf("w: %d ...\n", w);
			printf("ndk[m][k]: %d ...\n", ndk[m][k]);
			printf("gdk[m][k]: %d ...\n", gdk[m][k]);
			printf("ndksum[m]: %d ...\n", ndksum[m]);
			printf("gdksum[m]: %d ...\n", gdksum[m]);
			printf("nkv[k][w]: %d ...\n", nkv[k][w]);
			printf("gkv[k][w]: %d ...\n", gkv[k][w]);
			printf("betaFM[k][w]: %f ...\n", betaFM[k][w]);
			printf("nkvsum[k]: %d ...\n", nkvsum[k]);
			printf("gkvsum[k]: %d ...\n", gkvsum[k]);
			printf("VbetaF[k]: %f ...\n", VbetaF[k]);
		}
		*/
		p[k] = exp(log(ndk[m][k] + gdk[m][k] + alpha) - log((ndksum[m] + gdksum[m] + Kalpha)) +
			log((nkv[k][w] + gkv[k][w] + betaFM[k][w])) - log((nkvsum[k] + gkvsum[k] + VbetaF[k])));
	}

	// cumulate multinomial parameters
	for (int k = 1; k < K; k++) {
	    p[k] += p[k - 1];
	}
	// scaled sample because of unnormalized p[]
	double u = ((double)rand() / RAND_MAX) * p[K - 1];

	for (topic = 0; topic < K; topic++) {
		if (p[topic] >= u) {
			break;
		}
	}
	if (topic == K)
	{
		topic = K - 1;
	}

    // add newly estimated z_i to count variables
	ndk[m][topic] += 1;
	nkv[topic][w] += 1;
	ndksum[m] += 1;
	nkvsum[topic] += 1;

	return topic;
}

int model::sampling_step_two_x(int m, int c){
	int topic = 0;
	for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
		if(I[m][c][i]) {
			topic = xdc[m][c];
			gdk[m][topic] -= 1;
			gdksum[m] -= 1;
			gkv[topic][ptrndata->comments[m]->commlines[c]->words[i]] -= 1;
			gkvsum[topic] -= 1;
		}
	}

	for (int k = 0; k < K; k++) {
		p[k] = log(1.0);
		double left = log(ndk[m][k] + gdk[m][k] + alpha) - log(ndksum[m] + gdksum[m] + Kalpha);
		for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
			if (I[m][c][i]){
				int w = ptrndata->comments[m]->commlines[c]->words[i];
				p[k] += (log(nkv[k][w] + gkv[k][w] + betaFM[k][w]) - log(nkvsum[k] + gkvsum[k] + VbetaF[k]));
			}
		}
		p[k] += left;
		p[k] = exp(p[k]);
	}
    // cumulate multinomial parameters
	for (int k = 1; k < K; k++) {
		p[k] += p[k - 1];
	}
    // scaled sample because of unnormalized p[]
	double u = ((double)rand() / RAND_MAX) * p[K - 1];

	for (topic = 0; topic < K; topic++) {
		if (p[topic] >= u) {
			break;
		}
	}
	if (topic == K)
	{
		topic = K - 1;
	}

	for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
		if(I[m][c][i]) {
			gkv[topic][ptrndata->comments[m]->commlines[c]->words[i]] += 1;
			gkvsum[topic] += 1;
			gdk[m][topic] += 1;
			gdksum[m] += 1;
		}
	}
	return topic;
}

int model::sampling_step_two_y(int m, int c) {
	int topic = 0;
	for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
		if(!I[m][c][i]) {
			topic = ydc[m][c];
			hjv[topic][ptrndata->comments[m]->commlines[c]->words[i]] -= 1;
			hjvsum[topic] -= 1;
			hj[topic] -= 1;
			hjsum -= 1;
		}
    }

	for (int j = 0; j < J; j++) {
		q[j] = log(1.0);
		double left = log(hj[j] + epsilonV[j]) - log(hjsum + Jepsilon);
		for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
			if (!I[m][c][i]){
				int w = ptrndata->comments[m]->commlines[c]->words[i];
				q[j] += (log(hjv[j][w] + betaLM[j][w]) - log(hjvsum[j] + VbetaL[j]));
			}
		}
		q[j] += left;
		q[j] = exp(q[j]);
	}
    // cumulate multinomial parameters
    for (int j = 1; j < J; j++) {
        q[j] += q[j - 1];
    }
    // scaled sample because of unnormalized p[]
    double u = ((double)rand() / RAND_MAX) * q[J - 1];

    for (topic = 0; topic < J; topic++) {
        if (q[topic] >= u) {
            break;
        }
    }
	if (topic == J)
	{
		topic = J - 1;
	}

	for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
		if(!I[m][c][i]) {
			hjv[topic][ptrndata->comments[m]->commlines[c]->words[i]] += 1;
			hjvsum[topic] += 1;
			hj[topic] += 1;
			hjsum += 1;
		}
	}
	return topic;
}

int model::sampling_step_three(int m, int c){
	int topic = 0;
	//printf("length: %d\n", ptrndata->comments[m]->commlines[c]->length);
	for (int i = 0; i < ptrndata->comments[m]->commlines[c]->length; i++) {
		if(I[m][c][i]) {
			topic = xdc[m][c];
			gkv[topic][ptrndata->comments[m]->commlines[c]->words[i]] -= 1;
			gkvsum[topic] -= 1;
			sdc[m][c] -= 1;
		}else{
			topic = ydc[m][c];
			hjv[topic][ptrndata->comments[m]->commlines[c]->words[i]] -= 1;
			hjvsum[topic] -= 1;
			rdc[m][c] -= 1;
		}
		int mdc = ptrndata->comments[m]->commlines[c]->length;

		double * temp = new double [2];
		int Topic_x = xdc[m][c], Topic_y = ydc[m][c];
		int w = ptrndata->comments[m]->commlines[c]->words[i];
		temp[0] = exp(log((nkv[Topic_x][w] + gkv[Topic_x][w] + betaFM[Topic_x][w])) - log((nkvsum[Topic_x] + gkvsum[Topic_x] + VbetaF[Topic_x])) +
			log(sdc[m][c] + rou) - log(mdc + 2*rou - 1));
		temp[1] = exp(log(hjv[Topic_y][w] + betaLM[Topic_y][w]) - log(hjvsum[Topic_y] + VbetaL[Topic_y]) +
			log(rdc[m][c] + rou) - log(mdc + 2*rou - 1));
		// cumulate multinomial parameters
		for (int j = 1; j < 2; j++) {
			temp[j] += temp[j - 1];
		}
		// scaled sample because of unnormalized p[]
		double u = ((double)rand() / RAND_MAX) * temp[2 - 1];

		int flag;
		for (flag = 0; flag < 2; flag++) {
			if (temp[flag] >= u) {
				break;
			}
		}
		if (flag == 2)
		{
			flag = 2 - 1;
		}
		if (flag == 0){
			I[m][c][i] = true;

			topic = xdc[m][c];
			gkv[topic][ptrndata->comments[m]->commlines[c]->words[i]] += 1;
			gkvsum[topic] += 1;
			sdc[m][c] += 1;
		}else{
			I[m][c][i] = false;

			topic = ydc[m][c];
			hjv[topic][ptrndata->comments[m]->commlines[c]->words[i]] += 1;
			hjvsum[topic] += 1;
			rdc[m][c] += 1;
		}
		if (mdc != (sdc[m][c] + rdc[m][c])){
			printf("Error occured in matching mdc: %d, sdc: %d and rdc: %d.\n", mdc, sdc[m][c], rdc[m][c]);
			return 1;
		}
		if ((sdc[m][c] < 0) || (rdc[m][c] < 0)){
			printf("Error occured in value sdc: %d and rdc: %d.\n", sdc[m][c], rdc[m][c]);
			return 1;
		}
		if (temp) {
			delete [] temp;
		}
	}
}

void model::compute_theta() {
    for (int m = 0; m < M; m++) {
		for (int k = 0; k < K; k++) {
			theta[m][k] = (ndk[m][k] + gdk[m][k] + alpha) / (ndksum[m] + gdksum[m] + K * alpha);
		}
    }
}

void model::compute_phi() {
    for (int k = 0; k < K; k++) {
		for (int v = 0; v < V; v++) {
			phi[k][v] = (nkv[k][v] + gkv[k][v] + beta) / (nkvsum[k] + gkvsum[k] + V * beta);
		}
    }
}

void model::compute_psi() {
	for (int j = 0; j < J; j++) {
		for (int v = 0; v < V; v++) {
			psi[j][v] = (hjv[j][v] + beta) / (hjvsum[j] + V * beta);
		}
	}
}

void model::compute_ksi() {
	for (int j = 0; j < J; j++) {
		ksi[j] = (hj[j] + epsilon) / (hjsum + J * epsilon);
 	}
}

void model::compute_pdc() {
	for (int m = 0; m < M; m++) {
		int C = ptrndata->comments[m]->C;
		for (int c = 0; c < C; c++) {
			pdcx[m][c] = (sdc[m][c] + rou) / (sdc[m][c] + rdc[m][c] + 2 * rou);
			pdcy[m][c] = (rdc[m][c] + rou) / (sdc[m][c] + rdc[m][c] + 2 * rou);
			pdcx[m][c] = pdcx[m][c] / (pdcx[m][c] + pdcy[m][c]);
			pdcy[m][c] = pdcy[m][c] / (pdcx[m][c] + pdcy[m][c]);
		}
	}
}

void model::update_paras() {
    liter = 0;
	for (int k = 0; k < K; k++) {
		for (int v = 0; v < V; v++) {
			betaFM[k][v] = betaFM[k][v] + lamda * (nkv[k][v] + gkv[k][v]);
		}
	}
	for (int j = 0; j < J; j++) {
		for (int v = 0; v < V; v++) {
			betaLM[j][v] = betaLM[j][v] + lamda * (hjv[j][v]);
		}
	}
	for (int j = 0; j < J; j++) {
		epsilonV[j] = epsilonV[j] + hj[j];
	}
}
