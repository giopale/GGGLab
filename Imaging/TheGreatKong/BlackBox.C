#include "CalibAnalysis.C"

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

day1[0] = CalibAnalysis(day1[0],0,400,0); //ok

day1[1] = CalibAnalysis(day1[1],0,0,0);	//ok
day1[2] = CalibAnalysis(day1[2],0,1500,0);	//ok
day1[3] = CalibAnalysis(day1[3],0,1000,0);	//ok
day1[4] = CalibAnalysisMod(day1[4],0,7000,0,1800,9155,24300);	//na_bin = [9155,24300]non va bene -> tirare fuori picchi a mano (ne piglia troppi)
day1[5] = CalibAnalysis(day1[5],0,1000,0);	//ok
day1[6] = CalibAnalysisMod(day1[6],0,5000,0,1800,6775,16833);	//na_bin = [6775,16833]Usare due dei picchi rilevati
day1[7] = CalibAnalysisMod(day1[7],0,5000,0,1800,6711);	//na_bin = [6711]; il picco a 1275 non esiste

//day1[4].spectrum->Draw();
cout <<"day1 det 1 m: "<<day1[0].m <<" q: " <<day1[0].q <<endl;






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