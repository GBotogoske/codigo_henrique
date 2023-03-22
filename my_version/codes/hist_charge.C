#include "../MYCODES.h"

void hist_charge()
{
	//lembre de deixar em zero esse valor
	Double_t baseline_charge=-2.03405/2;
	
	//abre arquivo root
	TFile* _f= new TFile("analyzed.root","UPDATE");
	TFile* _charge = new TFile("charge.root","RECREATE");
		
	//Abrir arvore t1 ("cada um dos canais")
	TTree *t1 = (TTree*)_f->Get("t1");
	//Abrir arvore t2 ("ambos os canais")
	TTree *t2 = (TTree*)_f->Get("t2");
	
	TLeaf *leaf0 = t1->GetBranch("Ch0")->GetLeaf("charge"); 
	TLeaf *leaf1 = t1->GetBranch("Ch1")->GetLeaf("charge"); 
	TLeaf *leaft = t2->GetBranch("Ch_t")->GetLeaf("charge"); 
	
	int n = (int) t1->GetEntries();
	int nt = (int) t1->GetEntries();
	

	
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
	TH1F *hist0 = new TH1F("histCharge0","Charge",5000,minimum0,maximum0);
	TH1F *hist0_norm = new TH1F("histCharge0_norm","Charge",5000,minimum0,maximum0);
	
	for (int i=0;i<n;i++)
	{ 
		leaf0->GetBranch()->GetEntry(i); 
		hist0->Fill(leaf0->GetValue()-baseline_charge); 	
		hist0_norm->Fill(leaf0->GetValue()-baseline_charge); 
	} 
	hist0_norm->Scale((Double_t)1/hist0_norm->Integral());	
	_charge->WriteObject(hist0,Form("charge_histogram_0"),"TObject::kOverwrite");
	_charge->WriteObject(hist0_norm,Form("charge_histogram_0_norm"),"TObject::kOverwrite");
//------------------------------------------------------------------------------------
	
	double maximum1=-1;
	double minimum1=+1;
	
	for (int i=0;i<n;i++)
	{ 
		leaf1->GetBranch()->GetEntry(i); 
		if(leaf1->GetValue()>maximum1)
		{
			maximum1=leaf1->GetValue();
		}
		if(leaf1->GetValue()<minimum1)
		{
			minimum1=leaf1->GetValue();
		}	
	} 
	
	
	TH1F *hist1 = new TH1F("histCharge1","Charge",10000,minimum1,maximum1);
	TH1F *hist1_n = new TH1F("histCharge1_norm","Charge",10000,minimum1,maximum1);
	
	for (int i=0;i<n;i++)
	{ 
		leaf1->GetBranch()->GetEntry(i); 
		hist1->Fill(leaf1->GetValue()); 	
	} 
		
	hist1_n->Scale(1/n);	
	_charge->WriteObject(hist1,Form("charge_histogram_1"),"TObject::kOverwrite");
	_charge->WriteObject(hist1_n,Form("charge_histogram_1"),"TObject::kOverwrite");
//-----------------------------------------------------------------------------------------

	double maximumt=-1;
	double minimumt=+1;
	
	for (int i=0;i<nt;i++)
	{ 
		leaft->GetBranch()->GetEntry(i); 
		if(leaft->GetValue()>maximum1)
		{
			maximumt=leaft->GetValue();
		}
		if(leaft->GetValue()<minimumt)
		{
			minimumt=leaft->GetValue();
		}	
	} 
	
	
	TH1F *histt = new TH1F("histCharget","Charge",1000,minimumt,maximumt);
	
	for (int i=0;i<nt;i++)
	{ 
		leaft->GetBranch()->GetEntry(i); 
		histt->Fill(leaft->GetValue()); 	
	} 
		
	_charge->WriteObject(histt,Form("charge_histogram_t"),"TObject::kOverwrite");
	
	
}
