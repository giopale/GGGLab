


void LangmuirGraphs(){
	auto inl = new TGraph("I_n_L.txt");
	auto inr = new TGraph("I_n_R.txt");

	auto c1 = new TCanvas("c1","MyPrecious");

	inl->SetMarkerStyle(22);
	inr->SetMarkerStyle(20);
	inl->SetMarkerColor(2);
	auto mg0 = new TMultiGraph();
	mg0->Add(inl,"AP");
	mg0->Add(inr,"AP");

	mg0->GetXaxis()->SetTitle("Current [A]");
	mg0->GetYaxis()->SetTitle("Plasma density [m^{-3}]");
	mg0->GetXaxis()->SetLimits(6,6.8);
	mg0->GetYaxis()->SetRangeUser(1e10,280e15);

	mg0->Draw("AP");
	auto leg0 = new TLegend(0.1,0.9,0.4,0.8);
	leg0->AddEntry(inl,"Far probe","ap");
	leg0->AddEntry(inr,"Close probe","ap");
	leg0->Draw();

	c1->Print("I_n.eps");

// *************************

	auto itel = new TGraph("I_Te_L.txt");
	auto iter = new TGraph("I_Te_R.txt");

	auto c2 = new TCanvas("c2","OnionRing");

	itel->SetMarkerStyle(22);
	iter->SetMarkerStyle(20);
	itel->SetMarkerColor(2);
	auto mg1 = new TMultiGraph();
	mg1->Add(itel,"AP");
	mg1->Add(iter,"AP");

	mg1->GetXaxis()->SetTitle("Current [A]");
	mg1->GetYaxis()->SetTitle("Temperature [eV]");
	mg1->GetXaxis()->SetLimits(6,6.8);
	mg1->GetYaxis()->SetRangeUser(0,2.1);

	mg1->Draw("AP");
	auto leg1 = new TLegend(0.1,0.9,0.4,0.8);
	leg1->AddEntry(itel,"Far probe","ap");
	leg1->AddEntry(iter,"Close probe","ap");
	leg1->Draw();

	c2->Print("I_Te.eps");

	// *************************

	auto ivpl = new TGraph("I_Vp_L.txt");
	auto ivpr = new TGraph("I_Vp_R.txt");

	auto c3 = new TCanvas("c3","ThinAir");

	ivpl->SetMarkerStyle(22);
	ivpr->SetMarkerStyle(20);
	ivpl->SetMarkerColor(2);
	auto mg2 = new TMultiGraph();
	mg2->Add(ivpl,"AP");
	mg2->Add(ivpr,"AP");

	mg2->GetXaxis()->SetTitle("Current [A]");
	mg2->GetYaxis()->SetTitle("Plasma potential [eV]");
	mg2->GetXaxis()->SetLimits(6,6.8);
	mg2->GetYaxis()->SetRangeUser(0,5.2);

	mg2->Draw("AP");
	auto leg2 = new TLegend(0.1,0.9,0.4,0.8);
	leg2->AddEntry(ivpl,"Far probe","ap");
	leg2->AddEntry(ivpr,"Close probe","ap");
	leg2->Draw();

	c2->Print("I_Vp.eps");


}