struct slimport_data_t {
	ULong64_t	timetag; //time stamp
	UInt_t		baseline;
	UShort_t	qshort; //integration with shorter time
	UShort_t	qlong; //integration with longer time
	UShort_t	pur;
	UShort_t	samples[4096];
};

struct histo{
	TH1F* h1;
	TH1F* h2;
};

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

void Import(){
	
	TH1F *h1 = getHistoForChannelFromTree((char *)"spec0_d1.root",1,0,1000,400,26000);		//DETECTOR 1
	TH1F *h2 = getHistoForChannelFromTree((char *)"spec0_d2.root",0,1,1000,0,26000);		//DETECTOR 2
	TH1F *h3 = getHistoForChannelFromTree((char *)"spec0_d3.root",0,2,1000,0,26000);		//DETECTOR 3
	TH1F *h4 = getHistoForChannelFromTree((char *)"spec0_d4.root",0,3,1000,1200,26000);		//DETECTOR 3

	TCanvas *c1 = new TCanvas("c1");
int num;
 cout <<"Quale istogramma vuoi visualizzare? (premi da 1 a 8)\n"<<endl;
 cin >>num;
  

switch(num)
{
	case 1:
	h1->SetTitle("h1");
	h1->Draw();
	break;
	case 2:
	h2->SetTitle("h2");
	h2->Draw();
	break;
	case 3:
	h3->SetTitle("h3");
	h3->Draw();
	break;
	case 4:
	h4->SetTitle("h4");
	h4->Draw();
	break;

}


	//h2->Draw();
}