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
day1[7] = CalibAnalysisMod(day1[7],0,5000,0,1800,6711);	//na_bin = [6711]; il picco a 1275 non esiste

//day1[4].spectrum->Draw();
cout.clear();

///////////////// Matrix initialization and calibration

H_data bb[5][8];
H_data bb0[8];
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
	bb0[i] = {filename,dgtz,chan};
	bb0[i]= JustFill(bb0[i],0,400,0);
	CalibrateHisto(bb0[i].spectrum,day1[i].m,day1[i].q);
}

cout.clear();





// /////////////// detector stack histograms plot;

int pos = 1;
int index = pos -1 ;
string temptit = "Position " + to_string(pos);
const char* canvastitle = temptit.c_str();

TCanvas *super8 = new TCanvas("super8",canvastitle);
super8->Divide(1,8,0.0001,0.001);
gStyle->SetOptStat(0);

for(int j=0; j<8; j++){
super8->cd(j+1);
//bb[index][j].spectrum->GetXaxis()->SetLabelSize(.1);
bb[index][j].spectrum->GetXaxis()->SetRangeUser(200,1000);
bb[index][j].spectrum->Draw();
}

////////////////// Peak integral calculation

double I[5][8];
double I0[8];
int alpha, bravo; //integration limits in BINS
int low, up;	//integration limits in KEV
low = 400;
up = 700;
for(int i=0; i<5; i++){
	for(int j=0; j<8; j++){
	//cout <<"Indexes "<<i << " " <<j<<": opening "<<bb[i][j].spectrum->GetTitle()<<endl;
		alpha = bb[i][j].spectrum->FindBin(400);
		bravo = bb[i][j].spectrum->FindBin(700);
		I[i][j] = bb[i][j].spectrum->Integral(alpha,bravo);
		cout <<"Integral pos " <<i+1 <<" det " << j+1 <<": " <<I[i][j] <<" low " << (int)low <<" up " << (int)up <<endl;
	}
}

for(int i=0; i<8; i++){
	alpha = bb0[i].spectrum->FindBin(400);
	bravo = bb0[i].spectrum->FindBin(700);
	//I0[i] = bb0[i].spectrum->Integral(alpha,bravo);
	//bb0[i].spectrum->GetXaxis()->SetRangeUser(450,1000);
	super8->cd(i+1);
	//bb0[i].spectrum->Draw();
	//bb0[i].spectrum->GetXaxis()->SetRangeUser(200,1000);
	//cout <<"Integral0: " <<I0[i] <<endl;
}

cout << "det8 m: " <<day1[7].m <<" q: " <<day1[7].q <<endl;



/////////////// 2D histogram 

TH2F* IntMap = new TH2F("IntMap","Intensity [a.u.]", 5, 0, 9, 7, 0, 10.5);

for(int i=0; i<5; i++){
	for(int j=1; j< 8; j++){
	IntMap->SetBinContent(i+1,j,I[i][j]);
	if(i==4){cout << "Integral: "<< I[i][j]<<endl;}
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