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


H_data CalibAnalysis(H_data in_data, int graphs = 0, int low_cut = 400){
gErrorIgnoreLevel = kPrint, kInfo, kWarning;
//Retrieving all H_data structures from files

int dgtz = in_data.dgtz;
int chan =in_data.ch;
const char* filename = in_data.filename;


H_data h1_data = getHistoForChannelFromTree(filename,dgtz,chan,1000,low_cut,26000);		//DETECTOR ;
H_data out_data = in_data;

//setting the calibration graph title:
string tempTitle(filename);
string Tree = " * Tree=" + to_string(dgtz)+ " * ";
string Bran = " Ch=" + to_string(chan);
tempTitle = tempTitle + Tree + Bran +" * calibration";
const char* calibTitle = tempTitle.c_str();

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
	
	if(nPeaks!=0){
		if( nPeaks >2 ){
			cout << "Calibration error: more than two peaks detected." <<endl;
		}
	}
	else{
		if(na_bin[1]<na_bin[2]){
			cout<< "Program terminated due to a calibration problem: wrong peak order\n" <<endl;
		}	
	}
	

	// fit the peaks with the Na energies
	double na_kev[2]; 
	na_kev[0] = 511.; 
	na_kev[1] = 1275.;
	TGraphErrors *graphErr = new TGraphErrors(nPeaks,na_bin,na_kev);
	TF1 *fitfun = new TF1("calfitfun1","pol1",na_bin[0],na_bin[1]);
	TFitResultPtr r = graphErr->Fit(fitfun,"Q");
	Int_t status = r;
	if(r!=0){
		cout<<"Calibration status: FAILED"<< endl;
		cout<<"Calibration terminated" <<endl;
		}
	if(r==0){	
		graphErr->SetMarkerStyle(47);
		graphErr->SetTitle(calibTitle);


		// get the fit parameters
		float m = fitfun->GetParameter(1);
		float q = fitfun->GetParameter(0);
		cout<<"Calibration status: CONVERGED; " <<"Exit parameters: "<<" m:  " << m <<" q: " <<q  <<endl;
		if(m>0){	
		// call the function to calibrate the histogram
			CalibrateHisto(h0,m,q);
		// second call to Search to plot the peaks over the calibrated histogram
		TSpectrum *t = new TSpectrum(30);
		TCanvas *c125 = new TCanvas("c125");
		nPeaks = t->Search(h0,20,"nobackground",0.05);
		if (c125) { c125->Close(); gSystem->ProcessEvents(); }
		// Commit changes into the structure:
		h1_data.spectrum = h0;
		h1_data.calibfun = fitfun;
		h1_data.calibgraph = graphErr;

		
		//gaussian fit for resolution determination
		TCanvas *c34 = new TCanvas("c34");
		double p1 = Calib(na_bin[0],m,q);
		double up = 1.15*p1;
		double down = 0.8*p1;
		TF1* f1 = new TF1("gaussiana","gaus",down,up);
		TFitResultPtr fp1 = h1_data.spectrum->Fit(f1,"RQ");
		if (c34) { c34->Close(); gSystem->ProcessEvents(); }
		double sigma = f1->GetParameter(2);
		double resol = 2.355*sigma;
		h1_data.resolution =resol;


		out_data = h1_data;
		}

		else{cout<<"m is negative - calibration aborted"<<endl;}
	
	// Draw the resulting histogram and calibration function
		if(graphs!=0){
			TCanvas *c123 = new TCanvas("c123");
			gStyle->SetOptStat("i");
			h0->Draw();
			TCanvas *c124 = new TCanvas("c124");
			graphErr->Draw();
		}

	
	cout<<"End of calibration" <<endl;

	}	

return out_data;
}
















	//TH1F* h1=Import(); //questo funziona ma non ti serve usarla 
	// basta direttamente chiamare gethisto
	//se invece vuoi usare anche tutto il resto di import devi farti
	//ritornare un doppio puntator o un array di puntatori
	//alla peggio puoi semplicemente rimettere lo swith di import qui 
	//volendo ce un modo per linkare i ptr tra una funzione e l'altra
	//ma è "rischioso " perchè dipende come lo interpreta in compiler
	//ultima alternativa è chiamre import dandogli in pasto i parametri
	//che ti danno il particolare istogramma o quello che vuoi fare poi

	// TH1F *h1 = getHistoForChannelFromTree((char *)"spec0_d1.root",1,0,1000,400,26000);		//DETECTOR 1
	// TH1F *h2 = getHistoForChannelFromTree((char *)"spec0_d2.root",0,1,1000,0,26000);		//DETECTOR 2
	// TH1F *h3 = getHistoForChannelFromTree((char *)"spec0_d3.root",0,2,1000,0,26000);		//DETECTOR 3
	// TH1F *h4 = getHistoForChannelFromTree((char *)"spec0_d4.root",0,3,1000,1200,26000);		//DETECTOR 3

	// h1->SetTitle("sugo estremo");
	// h1->Draw();





/*TH1F* getHistoForChannelFromTree(char *name_file, short dgtz, short chan, int numBins, double minX, double maxX) {
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
*/

/*string calibNames[8];
for(int i =0; i<8 ;i++)
{
	calibNames[i]= "spec0_d" + to_string(i+1) +".root";
}*/