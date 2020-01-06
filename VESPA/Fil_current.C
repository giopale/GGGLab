

// Filament current analysis

vector<double> range(double min, double max, size_t N) {
    vector<double> range;
    double delta = (max-min)/double(N-1);
    for(int i=0; i<N; i++) {
        range.push_back(min + i*delta);
    }
    return range;
}

double Curr(double *x){
	double xx = x[0];
	double epsilon, alpha, rho, r, L, pi;
	epsilon = .3;
	alpha = 5.64e-8;
	rho = 6.2e-11;
	r = 1.25e-4;
	L = 0.1;
	pi = TMath::Pi();
	double A =  (TMath::Pi() * TMath::Power(2* epsilon* alpha, 3./13) * TMath::Power(r,23./13))/(TMath::Power(rho,10./13)*TMath::Power(L,7./13));
	double f = (A*TMath::Power(xx,0.7/1.3));
	return f;

}


void Fil_current(){

	

	vector<double> v = range(0,12,10000);
	vector<double> ith;
	for(int i=0; i<v.size(); i++){
		ith.push_back(Curr(&v[i]));
	}

	TGraph *c = new TGraph(v.size(),&v[0],&ith[0]);
	c->SetLineColor(4);

	double epsilon, alpha, rho, r, L, pi;
	epsilon = .3;
	alpha = 5.64e-8;
	rho = 6.2e-11;
	r = 1.25e-4;
	L = 0.1;
	double A =  (TMath::Pi() * TMath::Power(2* epsilon* alpha, 3./13) * TMath::Power(r,23./13))/(TMath::Power(rho,10./13)*TMath::Power(L,7./13));

	auto c1 = new TCanvas("c1","MyCanvas");
	TGraph *g = new TGraph("Current.txt");
	g->SetTitle("");
	g->SetMarkerStyle(20);
	g->SetMarkerColor(1);

	g->GetXaxis()->SetTitle("Voltage [V]");
	g->GetYaxis()->SetTitle("Current [A]");
	g->GetXaxis()->SetTitleSize(.045);
	g->GetYaxis()->SetTitleSize(.045);

	TF1 *f0 = new TF1("f0","[0]*TMath::Power(x,7./13.)",0,12);
	g->Fit(f0,"R");
	double Aexp = f0->GetParameter(0);
	double Aexp_sig = f0->GetParError(0);
	cout << "Aexp = " << Aexp <<endl;

	double B = TMath::Power((TMath::Power(rho,10) * TMath::Power(L,7))/(TMath::Power(TMath::Pi() ,13) * TMath::Power(r,23) * TMath::Power(2*alpha,3)), 1./3.);
	double eps = B* TMath::Power(Aexp,13./3.);
	double eps_sigma = 13/3 *eps * Aexp_sig / Aexp;
	cout <<"eps exp = " << eps << " ± " <<eps_sigma <<endl;	
	g->Draw("AP");
	c->Draw("same");

	auto leg = new TLegend(0.6,0.1,0.9,0.3);
	leg->AddEntry(g,"Experimental data");
	leg->AddEntry(c, "Theoretical trend");
	leg->AddEntry(f0, "Experimental trend");
	gStyle->SetLegendTextSize(0.04);

	leg->Draw();
	c1->Print("VoltCurrent.pdf");



	// >>>>>>>>>>>>>>>> Temperature Curve

	vector<double> i0 = range(0,13,10000);
	double i00[2] = {0,13};
	double melting[2] = {3695,3695};
	vector<double> temps, temps0;

	double g0 = 459.74695;
	double C0 = g0 * TMath::Power(epsilon,-10./28.);
	double C = g0 * TMath::Power(eps, -10./28.);
	// double C0 = TMath::Power(rho/(epsilon*alpha*TMath::Power(TMath::Pi(),2)* TMath::Power(r,3)),1./2.8);
	// double C = TMath::Power(rho/(eps*alpha*TMath::Power(TMath::Pi(),2)* TMath::Power(r,3)),10./28.);

	cout << "Lambda constant - Theoretical: " << C0 << " Experimental: " <<C <<endl;
	double actualTemp0 = C*TMath::Power(6.5,2./2.8);
	cout << "Actual wire temperature [K]: " <<actualTemp0 <<endl;
	for(int i=0; i<i0.size(); i++){
			temps0.push_back(C0*TMath::Power(i0[i],2./2.8));
			temps.push_back(C*TMath::Power(i0[i],2./2.8));

		}

	auto c20 = new TCanvas("c20","MyPrecious");
	auto d00 = new TGraph(i0.size(), &i0[0],&temps0[0]);
	auto d0 = new TGraph(i0.size(), &i0[0],&temps[0]);

	// auto d1 = new TGraph(i0.size(), &i0[0],&melting[0]);
	auto d1 = new TGraph(2, i00,melting);
	d1->SetLineStyle(9);

	d0->SetTitle();
	d1->SetTitle();

	d0->GetXaxis()->SetTitle("Current [A]");
	d0->GetYaxis()->SetTitle("Temperature [K]");
	d0->GetXaxis()->SetTitleSize(.045);
	d0->GetYaxis()->SetTitleSize(.045);
	d0->GetXaxis()->SetRangeUser(0,11.5);

	d0->SetLineColor(2);
	d0->GetYaxis()->SetTitleOffset(.98);

	d0->Draw();
	d1->Draw("same");
	d00->Draw("same");

	auto leg1 = new TLegend(0.55,0.25,0.9,0.1);
	leg1->AddEntry(d0,"Temperature, exp. trend");
	leg1->AddEntry(d00,"Temperature, th. trend");
	leg1->AddEntry(d1, "Tungsten melting point");
	
	gStyle->SetLegendTextSize(0.04);

	leg1->Draw();
	c20->Print("TempCurrent.eps");
}





















