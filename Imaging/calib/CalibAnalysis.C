#include "Import.C"




void CalibrateHisto(TH1F *h_uncal, float m, float q) { 
	TAxis *asse1 = h_uncal->GetXaxis();
	double min = (asse1->GetXmin())*m +q;
	double max = (asse1->GetXmax())*m +q;
  	//cout <<"minX =  " <<((asse1->GetXmin())) <<" maxX = " <<(asse1->GetXmax())<<endl; //just to verify the truthness of the calibration
  	//cout <<"min =  " <<min <<" max = " <<max <<endl;
  	asse1->SetLimits(min,max);
	if (m!=1 && q!=0) //This means that I actually changed the calibration!
	    h_uncal->SetXTitle("keV");
		h_uncal->SetYTitle("Counts");
};

//funzione di calibrazione CHANNELS -> KEV
double Calib(double p, float m, float q){
	double x = m*p + q;
	return x;

}

// funzione di calibrazione inversa KEV -> CHANNELS
double invCalib(double y, float m, float q){
	double p = (y - q)/m;
	return p;

}







H_data h1_data = getHistoForChannelFromTree(filename,dgtz,chan,1000,low_cut,26000);		//DETECTOR ;
H_data out_data = in_data;



//Message to the people
if(h1_data.spectrum){cout << "Beginning calibration..." << endl; }

TH1F *h0 = (TH1F*)h1_data.spectrum->Clone();
// search the peaks
	TSpectrum *s = new TSpectrum(30);
	int nPeaks;
	TCanvas *c122 = new TCanvas("c122");
	nPeaks = s->Search(h0,20,"nobackground",0.05);
	double *na_bin = s->GetPositionX();
	//double *na_bin_doubleY = s->GetPositionY();
	if (c122) { c122->Close(); gSystem->ProcessEvents(); }
	

	}









