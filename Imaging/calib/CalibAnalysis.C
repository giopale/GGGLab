#include <iostream>

struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UInt_t		baseline;
	UShort_t	qshort; //integration with shorter time
	UShort_t	qlong; //integration with longer time
	UShort_t	pur;
	UShort_t	samples[4096];
};


void CalibrateHisto(TH1F *h_uncal, float m, float q) { //Re-scaling of axis, as in the slides

	//int max_bin = h_uncal->GetNbinsX(); // This method returns the number of bins in x of the histogram
	//float max_kev = h_uncal->GetBinCenter(max_bin)*m + q;
	//h_uncal->GetXaxis()->SetLimits(0,max_kev);
	TAxis *asse1 = h_uncal->GetXaxis();
	double min = (asse1->GetXmin())*m +q;
	double max = (asse1->GetXmax())*m +q;
  	cout <<"minX =  " <<((asse1->GetXmin())) <<" maxX = " <<(asse1->GetXmax())<<endl;
  	cout <<"min =  " <<min <<" max = " <<max <<endl;
  	asse1->SetLimits(min,max);
	if (m!=1 && q!=0) //This means that I actually changed the calibration!
	    h_uncal->SetXTitle("keV");
};

double Calib(double p, float m, float q){
	double x = m*p + q;
	return x;

}




TH1F* getHistoForChannelFromTree(char *name_file, short dgtz, short chan, int numBins, double minX, double maxX) {
	// variables
	slimport_data_t indata;
	TFile *infile = new TFile(name_file);
	TTree *intree = (TTree*)infile->Get(Form("acq_tree_%d",dgtz));
	TBranch *inbranch = intree->GetBranch(Form("acq_ch%d",chan));
	inbranch->SetAddress(&indata.timetag);
	TH1F *h_spectrum = new TH1F("h_spectrum","Total spectrum",numBins,minX,maxX);
	// histogram filling
	for (int i=0; i<inbranch->GetEntries(); i++) {
		inbranch->GetEntry(i);
		h_spectrum->Fill(indata.qlong);
		
	}
	// return
	return h_spectrum;
}


void CalibAnalysis(){


TH1F *h1 = getHistoForChannelFromTree((char *)"spec0_d1.root",1,0,1000,400,26000);
TCanvas *c1 = new TCanvas("c1");


// search the peaks
	TSpectrum *s = new TSpectrum(30);
	int nPeaks;
	float *na_bin;
	nPeaks = s->Search(h1,20,"nobackground",0.05);
	double *na_bin_double = s->GetPositionX();
	double *na_bin_doubleY = s->GetPositionY();

	h1->Draw();

	for(int i=0 ; i<nPeaks; i++){
	na_bin[i]=na_bin_double[i];
	
	}



cout <<na_bin[0] << "  " <<na_bin[1]  <<endl;

	

	// fit the peaks with the Na energies
	float na_kev[2]; 
	na_kev[0] = 511.; 
	na_kev[1] = 1275.;
	TGraphErrors *graphErr = new TGraphErrors(nPeaks,na_bin,na_kev);
	TF1 *fitfun = new TF1("calfitfun1","pol1",na_bin[0],na_bin[1]);
	graphErr->Fit(fitfun);
	graphErr->SetMarkerStyle(47);
	TCanvas *c2 = new TCanvas("c2");
	graphErr->SetTitle("Calibration");
	graphErr->Draw();


	// get the fit parameters
	float m = fitfun->GetParameter(1);
	float q = fitfun->GetParameter(0);

	cout  <<" m:  " << m <<" q: " <<q  <<endl;

// call the function to calibrate the histogram
	CalibrateHisto(h1,m,q);
	//h1->Draw();

}