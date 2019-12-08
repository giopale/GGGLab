#include "CalibAnalysis.C"
#include "TError.h"

struct calib_data
{
	double m;
	double q;
};

///////////////////////// H_data structure reminder:
// struct H_data
// {
// 	const char* filename;
// 	UInt_t dgtz;
// 	UInt_t ch;
// 	ULong64_t acqtime;
// 	ULong64_t resolution;
// 	TF1* calibfun;
// 	double m;
// 	double q;
// 	TGraphErrors* calibgraph;
// 	TH1F* spectrum; 
// };


void BlackBox(){

H_data day1[8];

///////////  Structures initialization

day1[0] = {"spec0_d1.root",1,0};
day1[1] = {"spec0_d2.root",0,1};
day1[2] = {"spec0_d3.root",0,2};
day1[3] = {"spec0_d4.root",0,3};
day1[4] = {"spec0_d5.root",1,0};
day1[5] = {"spec0_d6.root",1,1};
day1[6] = {"spec0_d7.root",1,2};
day1[7] = {"spec0_d8.root",1,3};


//////////////// Calibration analysis
//H_data CalibAnalysis(H_data in_data, int graphs = 0, int low_cut = 400, int scale = 1, int basetime = 1800, float peak1 = -1, float peak2 = -1){

cout.setstate(std::ios_base::failbit);
day1[0] = CalibAnalysis(day1[0],0,400,0); //ok

day1[1] = CalibAnalysis(day1[1],0,0,0);	//ok
day1[2] = CalibAnalysis(day1[2],0,1500,0);	//ok
day1[3] = CalibAnalysis(day1[3],0,1000,0);	//ok
day1[4] = CalibAnalysisMod(day1[4],0,7000,0,1800,9155,24300);	//na_bin = [9155,24300]non va bene -> tirare fuori picchi a mano (ne piglia troppi)
day1[5] = CalibAnalysis(day1[5],0,1000,0);	//ok
day1[6] = CalibAnalysisMod(day1[6],0,5000,0,1800,6775,16833);	//na_bin = [6775,16833]Usare due dei picchi rilevati
H_data tango = JustFill(day1[7],0,1000,0);
day1[7] = CalibAnalysisMod(day1[7],0,5000,0,1800,6711);	//na_bin = [6711]; il picco a 1275 non esiste

//day1[4].spectrum->Draw();
cout.clear();

///////////////// Matrix initialization and calibration

H_data bb[6][8];
//H_data bb0[8];
string tempname = "pos_";
UInt_t dgtz=0;
UInt_t chan=0;

//cout.setstate(std::ios_base::failbit);
for(int i=0; i<5; i++){
	tempname = tempname + to_string(i+1) +".root";
	const char* filename =tempname.c_str();
	//cout <<"Filling from file " <<tempname <<endl;
	for(int j=0; j<8; j++){
		if(j<4){dgtz = 0; chan = j;}
		else{dgtz =1; chan = j-4;}
		bb[i][j] = {filename,dgtz,chan};
		cout.setstate(std::ios_base::failbit);
		bb[i][j]= JustFill(bb[i][j],0,400,0);
		CalibrateHisto(bb[i][j].spectrum,day1[j].m,day1[j].q);
		cout.clear();

		bb[i][j].m = day1[j].m;
		bb[i][j].q = day1[j].q;
		//cout <<"Indexes "<<i <<" " <<j <<": Spectrum "<<bb[i][j].spectrum->GetTitle()<< " acquired"<<endl;
	}
	tempname = "pos_";
}

//		Intensity with no sample
cout.setstate(std::ios_base::failbit);
tempname = "att_0.root";
const char* filename = tempname.c_str();
for(int i=0; i<8; i++){
	if(i<4){dgtz =0; chan = i;}
	else{dgtz =1; chan = i-4;}
	bb[5][i] = {filename,dgtz,chan};
	bb[5][i]= JustFill(bb[5][i],0,400,0);
	CalibrateHisto(bb[5][i].spectrum,day1[i].m,day1[i].q);
}

cout.clear();

//////////////// Cerco l'intervallo di integrazione su tango = day1[7] non calibrato 
// e poi lo applico all'histo di della I0_det8 per la black box 
// nota: se cerco lo stesso interv. su day1[7] calibrato 
// con FindBin(400,700) ottengo risultati strani tipo bin1 13 bin2 200.
// tango.spectrum->SetLineColor(2);
// tango.spectrum->Draw();
int bin1 = tango.spectrum->FindBin(2000);
int bin2 = tango.spectrum->FindBin(3500);
int egral = bb[5][7].spectrum->Integral(bin1,bin2);
cout << "Interessant integral: " <<egral <<" bins: "<< bin1 <<" " <<bin2 <<endl;
//bb[5][7].spectrum->Draw("same");





// /////////////// detector stack histograms plot;

int pos = 1;
int index = pos -1 ;
string temptit = "Position " + to_string(pos);
const char* canvastitle = temptit.c_str();

TCanvas *super8 = new TCanvas("super8",canvastitle);
super8->Divide(1,8,0.0001,0.001);
gStyle->SetOptStat("i");

for(int j=0; j<8; j++){
super8->cd(j+1);
//bb[index][j].spectrum->GetXaxis()->SetLabelSize(.1);
bb[index][j].spectrum->GetXaxis()->SetRangeUser(200,1000);
bb[index][j].spectrum->Draw();
}

////////////////// Peak integral calculation

double I[6][8];
double sigI[6][8]; //calcolo direttamente sigma(log(I/I0))
double I0[8];
int alpha, bravo; //integration limits in BINS
int low, up;	//integration limits in KEV
low = 400;
up = 700;
// Intensity calculation;
for(int i=0; i<6; i++){
	for(int j=0; j<8; j++){
	//cout <<"Indexes "<<i << " " <<j<<": opening "<<bb[i][j].spectrum->GetTitle()<<endl;
		if(i==5){
			// alpha = bb[i][j].spectrum->FindBin(400);
			// bravo = bb[i][j].spectrum->FindBin(700);
			I[i][j] = bb[i][j].spectrum->Integral(bin1,bin2);
		}
		else{
			alpha = day1[j].spectrum->GetXaxis()->FindBin(low);
			bravo = day1[j].spectrum->GetXaxis()->FindBin(up);
			I[i][j] = bb[i][j].spectrum->Integral(alpha,bravo);
		}
		//cout <<"Integral pos " <<i+1 <<" det " << j+1 <<": " <<I[i][j] <<" low " << (int)alpha <<" up " << (int)bravo <<endl;
	}
	//cout << endl;
}

//int ManualIntegral[8] = {24190,160,414,4744,4430,4579,3314,741};
for(int j=0; j<8; j++){
	//I[5][j] = ManualIntegral[j];
}


// Intensity calculation;
for(int i=0; i<6; i++){
	for(int j=0; j<8; j++){
	//cout <<"Indexes "<<i << " " <<j<<": opening "<<bb[i][j].spectrum->GetTitle()<<endl;
				double trial = I[i][j];
				I[5][4] = 2400;
				I[5][6] = 3600;
				I[5][2] = 960;
				I[5][7] = 1255;
				I[5][5] = 5234;
				sigI[i][j] = TMath::Sqrt(1/I[i][j] + 1/I[5][j]);
 				I[i][j] = I[i][j]/I[5][j];
		//cout <<"Intensity pos " <<i+1 <<" det " << j+1 <<": " <<I[i][j] << "  -- dividing " << trial <<" by " <<I[5][j] <<endl;
	}
	//cout << endl;
}


/////// Attenuation coefficient calculation
double ignoto1[7], ignoto1_sig[7];
double ignoto2[3], ignoto2_sig[7];

for(int i=1; i<7; i++){		//striscia verticale
	ignoto1[i] = I[2][i];
	ignoto1_sig[i] = sigI[2][i];
	ignoto1[i] = TMath::Log(ignoto1[i]);
}
for(int i=0; i<3; i++){		//striscia orizzontale
	ignoto2[i] = I[i+1][7];
	ignoto2_sig[i] = sigI[i+1][7];
	ignoto2[i] = TMath::Log(ignoto2[i]);
}

double beta[8] = {0.,0.261,0.197,0.133,0.067,0.067,0.133,0.197};
double x1Fe[8], sigx1Fe[8];
double x1Pb[8], sigx1Pb[8];
double x2Fe[3];
double x2Pb[3];
double muFe = 0.656;
double muPb = 1.7835;
double mean1Fe, mean1Pb, mean2Fe, mean2Pb;

ofstream myfile;
  myfile.open ("00vertical.txt");
  myfile <<"// This file contains x*mu; sig_x*mu for the vertical stripe" <<endl;
for(int i=1; i<7; i++){
	x1Fe[i] = -1* TMath::Cos(beta[i])*ignoto1[i]/muFe;
	x1Pb[i] = -1* TMath::Cos(beta[i])*ignoto1[i]/muPb;
	mean1Fe += x1Fe[i];
	mean1Pb += x1Pb[i];
	//cout<< "x1Fe: " <<x1Fe[i] <<" x1Pb: " <<x1Pb[i] <<endl;

	x1Fe[i] = x1Fe[i]*muFe;
	ignoto1_sig[i] = ignoto1_sig[i]*TMath::Cos(beta[i]);
	myfile <<x1Fe[i] <<"	" << ignoto1_sig[i]<<endl;
}
mean1Fe = mean1Fe/6;
mean1Pb = mean1Pb/6;
myfile.close(); 

cout << endl;

ofstream myfile1;
myfile1.open ("00horizontal.txt");
myfile1 <<"// This file contains x*mu; sig_x*mu for the horizontal stripe" <<endl;

for(int i=0; i<3; i++){
	x2Fe[i] = -1* TMath::Cos(beta[7])*ignoto2[i]/muFe;
	x2Pb[i] = -1* TMath::Cos(beta[7])*ignoto2[i]/muPb;
	mean2Fe += x2Fe[i];
	mean2Pb += x2Pb[i];
	cout<< "x2Fe: " <<x2Fe[i] <<" x2Pb: " <<x2Pb[i] <<endl;

	x2Fe[i] = x2Fe[i]*muFe;
	ignoto2_sig[i] = ignoto2_sig[i]*TMath::Cos(beta[i]);
	myfile1 << x2Fe[i] <<"	"<<ignoto2_sig[i] <<endl;
}
myfile1.close();
mean2Fe = mean2Fe/3;
mean2Pb = mean2Pb/3;


cout << endl <<"Spessori stimati per il materiale nella striscia verticale: " <<endl;
cout << "Fe: " <<mean1Fe <<" Pb: " << mean1Pb  <<endl;

cout << endl <<"Spessori stimati per il materiale nella striscia orizzontale: " <<endl;
cout << "Fe: " <<mean2Fe <<" Pb: " << mean2Pb  <<endl;




for(int i=0; i<8; i++){
	alpha = bb[5][i].spectrum->GetXaxis()->FindBin(400);
	bravo = bb[5][i].spectrum->GetXaxis()->FindBin(700);
	//I0[i] = bb[5][i].spectrum->Integral(alpha,bravo);
	//bb[5][i].spectrum->GetXaxis()->SetRangeUser(450,1000);
	super8->cd(i+1);
	//bb[5][i].spectrum->Draw();
	//bb[5][i].spectrum->GetXaxis()->SetRangeUser(200,1000);
	//cout <<"Integral0: " <<I0[i] <<endl;
}

//cout << "det8 m: " <<day1[7].m <<" q: " <<day1[7].q <<endl;





/////////////// 2D histogram 

TH2F* IntMap = new TH2F("IntMap","Intensity [a.u.]", 5, 0, 5, 7, 1, 8);

for(int i=0; i<5; i++){
	for(int j=1; j< 8; j++){
	IntMap->SetBinContent(i+1,j,I[i][j]);
	//if(j==4){cout << "Integral: "<< I[i][j]<<endl;}
	}
}

TCanvas *c123 = new TCanvas("c123");
IntMap->Draw("colz");





}



////////////// OLD STUFF




/*

UInt_t dgtz=0;
UInt_t chan = 0;
string tempname;

for(int i=0; i<8; i++){
	
	tempname = "spec0_d" + to_string(i+1) + ".root";
	cout <<tempname<<endl;
	//const char* filename = tempname.c_str();
	if(i<4){
		dgtz = 0;
		chan = i;
	}
	if(i>=4){
		dgtz = 1;
		chan = i-4;
	}
	day1[i] = {tempname.c_str(),dgtz,chan};

	cout <<"filename: "<<day1[i].filename << " dgtz: " <<day1[i].dgtz <<" ch: " <<day1[i].ch <<endl;

}








*/