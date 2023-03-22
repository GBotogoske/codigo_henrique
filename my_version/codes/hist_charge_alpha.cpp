#include "../MYCODES.h"

void hist_charge_alpha()
{
	
	//abre arquivo root
	TFile* _f= new TFile("alphaxmuon.root","UPDATE");
	TFile* _c = new TFile("fit_e.root","RECREATE");
		
	//Abrir arvore t1 ("cada um dos canais")
	TTree *ta = (TTree*)_f->Get("ta");
	Double_t charge_pe=1.05039e+01+5.60252e-03;
	
	int n = (int) ta->GetEntries();
	TLeaf *leaf0 = ta->GetBranch("Cha")->GetLeaf("charge"); 
		
	double maximum0=-1;
	double minimum0=+1;
	
	for (int i=0;i<n;i++)
	{ 
		leaf0->GetBranch()->GetEntry(i); 
		if(leaf0->GetValue()>maximum0)
		{
			maximum0=leaf0->GetValue();
		}
		if(leaf0->GetValue()<minimum0)
		{
			minimum0=leaf0->GetValue();
		}	
	} 
	
	cout<<minimum0<<endl;
	cout<<maximum0<<endl;
	minimum0=0;
	maximum0=700;
	TH1F *hist0 = new TH1F("histCharge_a","Charge",100,minimum0,maximum0);
	
	double value;
	for (int i=0;i<n;i++)
	{ 
		leaf0->GetBranch()->GetEntry(i); 
		value=leaf0->GetValue()/charge_pe;
		if(value>=70)
			hist0->Fill(value); 	
	} 
	
	_c->WriteObject(hist0,Form("charge_histogram_a"),"TObject::kOverwrite");

//------------------------------------------------------------------------------------
	
}
