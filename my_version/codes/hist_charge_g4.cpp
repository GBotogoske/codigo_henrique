void hist_charge_g4()
{
	TFile* _f = new TFile("output_t0.root","UPDATE");
	TTree* ta = (TTree*)_f->Get("alpha");
	TBranch* b = ta->GetBranch("photonsDetected");
	
	TFile* _c = new TFile("fit_e.root","UPDATE");
	
	double maximum0=0;
	double minimum0=+1;
	
	int n = ta->GetEntries();
	int number; 
	
	b->SetAddress(&number);
	
	
	for (int i=0;i<n;i++)
	{ 
		b->GetEntry(i); 
		if(number>maximum0)
		{
			maximum0=number;
		}
		if(number<minimum0)
		{
			minimum0=number;
		}	
	} 
	
	
	TH1F *hist0 = new TH1F("histCharge_a","Charge",100,minimum0*10,maximum0*10);
	
	for (int i=0;i<n;i++)
	{ 
		b->GetEntry(i);
		hist0->Fill(number*10);  	
	} 
	
	_c->WriteObject(hist0,Form("charge_histogram_g"),"TObject::kOverwrite");

}
