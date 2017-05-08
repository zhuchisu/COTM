/*
 * Copyright (C) 2007 by
 *
 *  Yang Yang
 *  yyang1988@pku.edu.cn
 *  School of Electronic Engineering and Computer Science
 *  Peking University
 */

#ifndef	_MODEL_H
#define	_MODEL_H

#include "constants.h"
#include "dataset.h"

using namespace std;

// LDA model
class model {
public:
  // fixed options
  string wordmapfile;		// file that contains word map [string -> integer id]
  string trainlogfile;	// training log file
  //string tassign_suffix;	// suffix for topic assignment file
  string tassign_con_suffix;	//suffix for topic assignment file of content
  string tassign_comm_suffix;	//suffix for topic assignment file of comment
  string identifier_suffix;	// suffix for identifiers of correlations
  string theta_suffix;	// suffix for theta file
  string phi_suffix;		// suffix for phi file
  string psi_suffix;		// suffix for phi file
  string ksi_suffix;
  string pdc_suffix;
  string others_suffix;	// suffix for file containing other parameters
  string twords_psi_suffix;	// suffix for file containing words-per-topics
  string twords_phi_suffix;

  string dir;			// model directory
  //--------------------------------------
  //string dfile;		// data file
  string confile; //  data file content
  string commfile; //  data file comment
  string ordinal;
  //--------------------------------------
  string model_name;		// model name
  int model_status;		// model status:
			// MODEL_STATUS_UNKNOWN: unknown status
			// MODEL_STATUS_EST: estimating from scratch
			// MODEL_STATUS_ESTC: continue to estimate the model from a previous one
			// MODEL_STATUS_INF: do inference

  dataset * ptrndata;	// pointer to training dataset object
  dataset * pnewdata; // pointer to new dataset object

  mapid2word id2word; // word map [int => string]
  mapword2id word2id; // word map [string => int]

  // --- model parameters and variables ---
  int M; // dataset size (i.e., number of docs)

  //------------------------------------------
  int * C; // user comments size, M x C.count()
  //------------------------------------------

  int V; // vocabulary size

  //------------------------------------------
  int K; // number of contents topics
  int J; // number of user generated topics
  int T; // number of t update datasets
  double alpha, beta, epsilon, lamda; // LDA hyperparameters
  double rou;
  //double Vbeta;	// V * beta
  //double Kalpha;// K * alpha
  double Jepsilon; // J * epsilon
  double ** betaFM, ** betaLM;
  double Kalpha, * VbetaF, * VbetaL, * epsilonV;
  int oldV;
  double ** betaFMrem; 
  //------------------------------------------

  int niters; // number of Gibbs sampling iterations
  int liter; // the iteration at which the model was saved
  int savestep; // sav
  //------------------------------------------

  //---------ing period
  int twords; // print out top words per each topic
  int withrawstrs;

  double * p; // temp variable for sampling
  double * q;
  int ** z; // topic assignments for words, size M x doc.size()

  //------------------------------------------
  bool *** I; // identifier of the deriviation of each word(either from news topic or UGC topic), size M x C x review.size()
  int ** xdc;
  int ** ydc;
  //------------------------------------------

  int ** ndk;
  int * ndksum;
  int ** gdk;
  int * gdksum;
  int ** nkv;
  int * nkvsum;
  int ** gkv;
  int * gkvsum;
  int ** hjv;
  int * hjvsum;
  int * hj;
  int hjsum;

  int ** sdc;
  int ** rdc;


  int ** nw; // cwt[i][j]: number of instances of word/term i assigned to topic j, size V x K
  int ** nd; // na[i][j]: number of words in document i assigned to topic j, size M x K
  int * nwsum; // nwsum[j]: total number of words assigned to topic j, size K
  int * ndsum; // nasum[i]: total number of words in document i, size M
  //------------------------------------------
  int ** gd; // ga[i][j]: number of correlated comments under document i assigned to topic j, size M x C.count()
  int ** hd; // ha[i][j]: number of uncorrelated comments under document i assigned to topic j, size M x C.count()
  int * hdsum; // ha[j]: number of uncorrelated comments under all documents assigned to topic j, size K
  bool ** idf; // idf[i][j]: identifier of the correlation between the jth comment with ith content, size M x C.count()
  int ** nthwconsum; // nwvsum[i][j]: number of ith(ID) word assigned to topic j, size word2id.size() x K
  int **** nthwcomsum; // nthwconsum[i][j][k][l]: number of kth(ID) word in jth comment of ith content assigned to topic l, size M x C.count() x com.size() x K
  // int ** gnthwcomsum and ** hnthwcomsum can be derived from ****nthwcomsum and **idf
  // --------------------------------------

  double ** theta; // theta: document-topic distributions, size M x K
  double ** thetaContent;
  double ** thetaComment;
  double ** phi; // phi: topic-word distributions, size K x V
  double * ksi;
  double ** psi;
  double ** pdcx;
  double ** pdcy;

  // for inference only
  int inf_liter;
  int newM;
  int newV;
  int ** newz;
  int ** newnw;
  int ** newnd;
  int * newnwsum;
  int * newndsum;
  double ** newtheta;
  double ** newphi;

  model() {
	  set_default_values();
  }
  ~model();

  // set default values for variables
  void set_default_values();

  // parse command line to get options
  int parse_args(int argc, char ** argv);

  // initialize the model
  int init(int argc, char ** argv);

  // load LDA model to continue estimating or to do inference
  int load_model(string model_name);

  // save LDA model to files
  // model_name.tassign: topic assignments for words in docs
  // model_name.theta: document-topic distributions
  // model_name.phi: topic-word distributions
  // model_name.others: containing other parameters of the model (alpha, beta, M, V, K)
  int save_model(string model_name);
  int save_model_tassign(string confile, string commfile);
  int save_model_theta(string filename);
  int save_model_phi(string filename);
  int save_model_others(string filename);
  int save_model_twords_phi(string filename);
  int save_model_psi(string filename);
  int save_model_ksi(string filename);
  int save_model_pdc(string filename);
  int save_model_twords_psi(string filename);
  int save_model_identifiers(string filename);

  // saving inference outputs
  int save_inf_model(string model_name);
  int save_inf_model_tassign(string filename);
  int save_inf_model_newtheta(string filename);
  int save_inf_model_newphi(string filename);
  int save_inf_model_others(string filename);
  int save_inf_model_twords(string filename);

  // init for estimation
  int init_est();
  int init_estc();

  // estimate LDA model using Gibbs sampling
  int update();
  void estimate();
  int sampling(int m, int n);
  int sampling_step_one(int m, int n);
  int sampling_step_two_x(int m, int c);
  int sampling_step_two_y(int m, int c);
  int sampling_step_three(int m, int c);
  void compute_theta();
  void compute_phi();
  void compute_psi();
  void compute_ksi();
  void compute_pdc();
  void update_paras();
  void parameter_print();
  // init for inference
  int init_inf();
  // inference for new (unseen) data based on the estimated LDA model
  void inference();
  int inf_sampling(int m, int n);
  void compute_newtheta();
  void compute_newphi();
};

#endif
