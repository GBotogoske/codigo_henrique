void histogram2txt()
{
	TFile* _f = new TFile("alphaxmuon.root","UPDATE");
	TTree* ta = (TTree*)_f->Get("ta");
	TLeaf *leaf = ta->GetBranch("Cha")->GetLeaf("charge");
	
	Double_t charge_pe=1.05039e+01+5.60252e-03;
	
	double maximum0=-1;
	double minimum0=+1;
	
	int n = (int) ta->GetEntries();
	
	
	for (int i=0;i<n;i++)
	{ 
		leaf->GetBranch()->GetEntry(i); 
		if(leaf->GetValue()>maximum0)
		{
			maximum0=leaf->GetValue();
		}
		if(leaf->GetValue()<minimum0)
		{
			minimum0=leaf->GetValue();
		}	
	} 
	int n_bin=100;
	minimum0=65;
	maximum0=620;
	TH1F *hist0 = new TH1F("histCharge","Charge_alpha",n_bin,minimum0,maximum0);
	
	for (int i=0;i<n;i++)
	{ 
		leaf->GetBranch()->GetEntry(i); 
		hist0->Fill(leaf->GetValue()/charge_pe); 	
	} 
	hist0->Draw("");
	n=hist0->GetNbinsX();
	
	ofstream MyFile("hist_alpha.txt");
	MyFile << "bin_number charge(photoelectron) amplitude"<< endl;
	
	
	Double_t delta_q=(maximum0-minimum0)/n_bin;
	cout<<delta_q<<endl;
	for (int i=0;i<n;i++)
	{ 
		MyFile	<< i << " " << i*delta_q + minimum0 << " " <<  hist0->GetBinContent(i+1) << endl;
	} 
	
}
