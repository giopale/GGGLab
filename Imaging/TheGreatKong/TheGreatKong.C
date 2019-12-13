#include "CalibAnalysis.C"

void TheGreatKong(){

struct H_data day1d1 = {(const char*)"spec0_d1.root",1,0};
struct H_data day1d2 = {(const char*)"spec0_d2.root",0,1};
struct H_data day1d3 = {(const char*)"spec0_d3.root",0,2};
struct H_data day1d4 = {(const char*)"spec0_d4.root",0,3};

H_data day1[8];

day1[0] = {"spec0_d1.root",1,0};
day1[1] = {"spec0_d2.root",0,1};
day1[2] = {"spec0_d3.root",0,2};
day1[3] = {"spec0_d4.root",0,3};
day1[4] = {"spec0_d5.root",1,0};
day1[5] = {"spec0_d6.root",1,1};
day1[6] = {"spec0_d7.root",1,2};
day1[7] = {"spec0_d8.root",1,3};

H_data att0[8];

att0[0]={"att_0.root",0,0};
att0[1]={"att_0.root",0,1};
att0[2]={"att_0.root",0,2};
att0[3]={"att_0.root",0,3};
att0[4]={"att_0.root",1,0};
att0[5]={"att_0.root",1,1};
att0[6]={"att_0.root",1,2};
att0[7]={"att_0.root",1,3};

H_data attAl[8];

attAl[0]={"att_Al.root",0,0};
attAl[1]={"att_Al.root",0,1};
attAl[2]={"att_Al.root",0,2};
attAl[3]={"att_Al.root",0,3};
attAl[4]={"att_Al.root",1,0};
attAl[5]={"att_Al.root",1,1};
attAl[6]={"att_Al.root",1,2};
attAl[7]={"att_Al.root",1,3};

H_data attFe[8];

attFe[0]={"att_Fe.root",0,0};
attFe[1]={"att_Fe.root",0,1};
attFe[2]={"att_Fe.root",0,2};
attFe[3]={"att_Fe.root",0,3};
attFe[4]={"att_Fe.root",1,0};
attFe[5]={"att_Fe.root",1,1};
attFe[6]={"att_Fe.root",1,2};
attFe[7]={"att_Fe.root",1,3};

H_data attGr[8];

attGr[0]={"att_Graphite.root",0,0};
attGr[1]={"att_Graphite.root",0,1};
attGr[2]={"att_Graphite.root",0,2};
attGr[3]={"att_Graphite.root",0,3};
attGr[4]={"att_Graphite.root",1,0};
attGr[5]={"att_Graphite.root",1,1};
attGr[6]={"att_Graphite.root",1,2};
attGr[7]={"att_Graphite.root",1,3};

H_data attPb[8];

attPb[0]={"att_Pb.root",0,0};
attPb[1]={"att_Pb.root",0,1};
attPb[2]={"att_Pb.root",0,2};
attPb[3]={"att_Pb.root",0,3};
attPb[4]={"att_Pb.root",1,0};
attPb[5]={"att_Pb.root",1,1};
attPb[6]={"att_Pb.root",1,2};
attPb[7]={"att_Pb.root",1,3};

H_data attPoly[8];

attPoly[0]={"att_Poly.root",0,0};
attPoly[1]={"att_Poly.root",0,1};
attPoly[2]={"att_Poly.root",0,2};
attPoly[3]={"att_Poly.root",0,3};
attPoly[4]={"att_Poly.root",1,0};
attPoly[5]={"att_Poly.root",1,1};
attPoly[6]={"att_Poly.root",1,2};
attPoly[7]={"att_Poly.root",1,3};

double xAlGrPoly[7];
xAlGrPoly[0]=2.04;
xAlGrPoly[1]=2.018;
xAlGrPoly[2]=2.004;
xAlGrPoly[3]=2.00;
xAlGrPoly[4]=2.004;
xAlGrPoly[5]=2.018;
xAlGrPoly[6]=2.040;

double xFe[7];
xFe[0]=1.02;
xFe[1]=1.009;
xFe[2]=1.002;
xFe[3]=1.00;
xFe[4]=1.002;
xFe[5]=1.009;
xFe[6]=1.020;

double xPb[7];
xPb[0]=0.510;
xPb[1]=0.504;
xPb[2]=0.501;
xPb[3]=0.50;
xPb[4]=0.501;
xPb[5]=0.504;
xPb[6]=0.510;

day1[0] = CalibAnalysis(day1[0],0,400,0); //ok
day1[1] = CalibAnalysis(day1[1],0,0,0);	//ok
day1[2] = CalibAnalysis(day1[2],0,1500,0);	//ok
day1[3] = CalibAnalysis(day1[3],0,1000,0);	//ok
day1[4] = CalibAnalysisMod(day1[4],1,7000,0,1800,9155,24300);	//na_bin = [9155,24300]non va bene -> tirare fuori picchi a mano (ne piglia troppi)
day1[5] = CalibAnalysis(day1[5],0,1000,0);	//ok
day1[6] = CalibAnalysisMod(day1[6],0,5000,0,1800,6775,16833);	//na_bin = [6775,16833]Usare due dei picchi rilevati
day1[7] = CalibAnalysisMod(day1[7],0,5000,0,1800,6711);	//na_bin = [6711]; il picco a 1275 non esiste
H_data tango = JustFill(day1[7],0,1000,0);

/*
for (int i = 0; i < 8; ++i) cout << "Resol: "<< day1[i].resolution << "+/-"<< day1[i].sigmares <<endl;
*/

for(int i = 0; i<8;++i)
{
att0[i]=getHistoForChannelFromTree(att0[i].filename, att0[i].dgtz,att0[i].ch,1000,0,25000);
CalibrateHisto(att0[i].spectrum,day1[i].m,day1[i].q);
//attAl[i].spectrum->Draw();
attAl[i]=getHistoForChannelFromTree(attAl[i].filename, attAl[i].dgtz,attAl[i].ch,1000,0,25000);
CalibrateHisto(attAl[i].spectrum,day1[i].m,day1[i].q);

attFe[i]=getHistoForChannelFromTree(attFe[i].filename, attFe[i].dgtz,attFe[i].ch,1000,0,25000);
CalibrateHisto(attFe[i].spectrum,day1[i].m,day1[i].q);

attGr[i]=getHistoForChannelFromTree(attGr[i].filename, attGr[i].dgtz,attGr[i].ch,1000,0,25000);
CalibrateHisto(attGr[i].spectrum,day1[i].m,day1[i].q);

attPb[i]=getHistoForChannelFromTree(attPb[i].filename, attPb[i].dgtz,attPb[i].ch,1000,0,25000);
CalibrateHisto(attPb[i].spectrum,day1[i].m,day1[i].q);

attPoly[i]=getHistoForChannelFromTree(attPoly[i].filename, attPoly[i].dgtz,attPoly[i].ch,1000,0,25000);
CalibrateHisto(attPoly[i].spectrum,day1[i].m,day1[i].q);
}

int counter=0;
char ans;

cout << " al \t\t fe \t\t gr \t\t poly \t\t pb" << endl;

for (int i=1; i<8; i++)
{
	int p=0, d=0, intg;
p=(511-day1[i].q)/day1[i].m;
d=(250-day1[i].q)/day1[i].m;


int up0 = att0[i].spectrum->FindBin(p-d);
int down0 = att0[i].spectrum->FindBin(p+d);

int upAl = attAl[i].spectrum->FindBin(p-d);
int downAl = attAl[i].spectrum->FindBin(p+d);

int upFe = attFe[i].spectrum->FindBin(p-d);
int downFe = attFe[i].spectrum->FindBin(p+d);

int upGr = attGr[i].spectrum->FindBin(p-d);
int downGr = attGr[i].spectrum->FindBin(p+d);

int upPb = attPb[i].spectrum->FindBin(p-d);
int downPb = attPb[i].spectrum->FindBin(p+d);

int upPoly = attPoly[i].spectrum->FindBin(p-d);
int downPoly = attPoly[i].spectrum->FindBin(p+d);

double intg0= att0[i].spectrum -> Integral(down0,up0);
double intgAl= attAl[i].spectrum -> Integral(downAl,upAl);
int intgFe= attFe[i].spectrum -> Integral(downFe,upFe);
int intgGr= attGr[i].spectrum -> Integral(downGr,upGr);
int intgPoly= attPoly[i].spectrum -> Integral(downPoly,upPoly);
int intgPb= attPb[i].spectrum -> Integral(downPb,upPb);

int err0r = TMath::Sqrt(att0[i].spectrum-> Integral(down0,up0));
int errAl = TMath::Sqrt(attAl[i].spectrum-> Integral(downAl,upAl));
int errFe= TMath::Sqrt(attFe[i].spectrum -> Integral(downFe,upFe));
int errGr= TMath::Sqrt(attGr[i].spectrum -> Integral(downGr,upGr));
int errPoly= TMath::Sqrt(attPoly[i].spectrum -> Integral(downPoly,upPoly));
int errPb= TMath::Sqrt(attPb[i].spectrum -> Integral(downPb,upPb));

double rAl = (intgAl/intg0);
float rFe = intgFe/intg0;
float rGr = intgGr/intg0;
float rPoly = intgPoly/intg0;
float rPb = intgPb/intg0;

float srAl = intgAl/intg0*TMath::Sqrt(1/intgAl + 1/intg0);
float srFe = intgFe/intg0*TMath::Sqrt(1/intgFe + 1/intg0);
float srGr = intgGr/intg0*TMath::Sqrt(1/intgGr + 1/intg0);
float srPoly = intgPoly/intg0*TMath::Sqrt(1/intgPoly + 1/intg0);
float srPb = intgPb/intg0*TMath::Sqrt(1/intgPb + 1/intg0);

//cout<<endl<<p<<"\t"<<d<<"\t "<<intg0 <<"+/-" << err0r << "\t" << intgAl << "+/-" << errAl << "\t" << intgFe << "+/-" << errFe << "\t" << intgGr << "+/-" << errGr<< "\t" << intgPb << "+/-" << errPb<< "\t" << intgPoly << "+/-" << errPoly<<endl;
//cout << intg0 << "pm" << err0r << "\t"<< rAl << "pm" << srAl << "\t" << rFe << "pm" << srFe << "\t" << rGr << "pm" << srGr << "\t" << rPoly << "pm" << srPoly << "\t" << rPb << "pm" << srPb << endl;

double muFe = (-1/xFe[i-1])*TMath::Log(rFe);
double smuFe = TMath::Abs((-1/xFe[i-1])*srFe);

double muAl = (-1/xAlGrPoly[i-1])*TMath::Log(rAl);
double smuAl = TMath::Abs((-1/xAlGrPoly[i-1])*srAl);

double muGr = (-1/xAlGrPoly[i-1])*TMath::Log(rGr);
double smuGr = TMath::Abs((-1/xAlGrPoly[i-1])*srGr);

double muPoly = (-1/xAlGrPoly[i-1])*TMath::Log(rPoly); //0.88–0.96 density
double smuPoly = TMath::Abs((-1/xAlGrPoly[i-1])*srPoly);

double muPb = (-1/xPb[i-1])*TMath::Log(rPb);
double smuPb = TMath::Abs((-1/xPb[i-1])*srPb);


cout << muAl/2.7 << "\t"<< " pm " << "\t"<< smuAl/2.7 << "\t" 
	 << muFe/7.874 << "\t"<< " pm " << "\t" << smuFe/7.874 << "\t" 
	 << muGr/1.7 << "\t"<< " pm " << "\t" << smuGr/1.7 << "\t" 
	 << muPoly/0.94 << "\t"<< " pm " << "\t" << smuPoly/0.94 << "\t"
	 << muPb/11.34 << "\t"<< " pm " << "\t" << smuPb/11.34 << endl;
}


}
/*

char answer;

do
{
	int det;

	cout<<"Wich detector spectrum do you want to draw?"<<endl;
	cin>>det;

	attPb[det].spectrum->Draw();


	cout<<"Do you want to draw another spectrum? (s/n)"<<endl;
	cin>>answer;

} while(answer=='s');

}




H_data bb0[8];
H_data bb[6][8];
UInt_t dgtz=0;
UInt_t chan=0;

string tempname = "att_Al.root";
const char* filename = tempname.c_str();
for(int i=0; i<8; i++){
	if(i<4){dgtz =0; chan = i;}
	else{dgtz =1; chan = i-4;}
	bb[5][i] = {filename,dgtz,chan};
	bb[5][i]= JustFill(bb[5][i],0,400,0);
	CalibrateHisto(bb[5][i].spectrum,day1[i].m,day1[i].q);
}



}

*/