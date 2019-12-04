#include <Riostream.h>
#include <stdlib.h>
#include <TROOT.h>
#include <TSystem.h>
#include "TGeoManager.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TView.h"
#include "TF1.h"
#include "TGeometry.h"
#include "TClonesArray.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TGeoPhysicalNode.h"
#include "TParticle.h"
#include "TRandom2.h"
#include "TView3D.h"
#include "TLine.h"
#include "Math/ProbFuncMathCore.h"
#include "TAxis.h"
#include <stdio.h>
#include <cstdio>
#include <string>

struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UInt_t		baseline;
	UShort_t	qshort; //integration with shorter time
	UShort_t	qlong; //integration with longer time
	UShort_t	pur;
	UShort_t	samples[4096];
};

struct H_data
{
	const char* filename;
	UInt_t dgtz;
	UInt_t ch;
	ULong64_t acqtime;
	ULong64_t resolution;
	TF1* calibfun;
	TGraphErrors* calibgraph;
	TH1F* spectrum; 
};


H_data getHistoForChannelFromTree(const char *name_file, short dgtz, short chan, int numBins, double minX, double maxX) {
	
	// variables
	slimport_data_t indata;
	H_data histo;
	//setting title 
	TFile *infile = new TFile(name_file);
	string Tree = " Tree= " + to_string(dgtz)+ " * ";
	string Bran = " Ch= " + to_string(chan);
	string ReadFile0(name_file);
	string ReadFile = "File= " + ReadFile0 + " * ";
	string titleString = ReadFile + Tree +Bran;


	const char *GraphTitle = titleString.c_str();
	//cout <<"Il titolo e` "<<stringax <<endl;
	//string title = name_file + "-" + "dgtz" + std::to_string(dgtz) + "_" + "ch" + std::to_string(chan);
	TTree *intree = (TTree*)infile->Get(Form("acq_tree_%d",dgtz));
	TBranch *inbranch = intree->GetBranch(Form("acq_ch%d",chan));

	TH1F* h_temp = new TH1F("h123",GraphTitle,numBins,minX,maxX);
	inbranch->SetAddress(&indata.timetag);
	ULong64_t time =0;
	// histogram filling
	for (int i=0; i<inbranch->GetEntries(); i++) {
		inbranch->GetEntry(i);
		h_temp->Fill(indata.qlong);
		time = indata.timetag; 
		
	}

	// return
	gStyle->SetTitleFontSize(.08);
	h_temp->GetXaxis()->SetTitleSize(.055);
	h_temp->GetYaxis()->SetTitleSize(.055);
	h_temp->GetXaxis()->SetLabelSize(.045);
	h_temp->GetYaxis()->SetLabelSize(.045);
	histo.acqtime = time * 4e-9;
	string timestr = to_string(histo.acqtime);
	titleString = titleString + " * " + " Time = " +timestr + "s";
	h_temp->SetTitle(titleString.c_str());
	histo.spectrum=h_temp;

	
	if(histo.spectrum){cout <<"\n*** File: "<<ReadFile0 <<", tree: "<< dgtz <<", channel: " << chan <<" *** Spectrum acquired ***"<< endl;}
	return histo;
}

