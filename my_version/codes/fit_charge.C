#include "../MYCODES.h"

void fit_charge()
{
	TFile* _f= new TFile("charge.root","UPDATE");
	
	//TTree *t2 = (TTree*)_f->Get("tc");
	
	TGraph* graph_c = (TGraph*)_f->Get("light_alpha_filtered");
	TGraph* graph_d = (TGraph*)_f->Get("light_alpha_filtered");
	//branch_c = t2->GetBranch(Form("Ch_c"));
	//branch_d = t2->GetBranch(Form("Ch_c"));
	
		
	TCanvas* c = new TCanvas();
	c->SetCanvasSize(1400, 600);
   	c->SetWindowSize(1500, 1500);
   	
	//fit de curvas
	
	Double_t start_time_fit1=3425;
      	Double_t end_time_fit1=3600;	
	
	Double_t start_time_fit2=8000;
      	Double_t end_time_fit2=16000;	
	
	
	Double_t v1[2];
	Double_t v2[2];
	
	TF1 *fit_2 = new TF1("fitt_2","expo",start_time_fit2,end_time_fit2);
	fit_2->SetParameters(1,1600,0);
	graph_c->Fit("fitt_2","MR0+");
	
	fit_2->GetParameters(&v2[0]);
	
	TF1 *fit_1 = new TF1("fitt_1","expo",start_time_fit1,end_time_fit1);
	
	fit_1->SetParameters(100,22,0);
	//fit_1->SetParLimits(0,0,1000);
	//fit_1->SetParLimits(1,1,100);
	//fit_1->FixParameter(2,v2[0]);
	//fit_1->FixParameter(3,v2[1]);
	//fit_1->SetParLimits(4,-5,5);
	graph_c->Fit("fitt_1","0MR+");
	
	
	//TF1 *fit_3 = new TF1("fit_3","[p4]*exp(-x/[p5])+[p6]*exp(-x/[p7])+[c3]",start_time_fit1,end_time_fit2);
	TF1 *fit_3 = new TF1("fit_3","expo(0)+expo(2)",start_time_fit1,end_time_fit2);
	//fit_3->SetParLimits(2,0,10);
	
		
	fit_1->GetParameters(&v1[0]);
	

	fit_2->SetLineColor(2); //red
	fit_2->SetLineColor(3); //green
	fit_3->SetLineColor(4); //blue
	
	
	Double_t v3[4];
	
	v3[0]=v1[0];
	v3[1]=v1[1];
	v3[2]=v2[0];
	v3[3]=v2[1];
	
	fit_3->SetParameters(v3);
	//fit_3->SetParLimits(1,0,800);
	//fit_3->SetParLimits(3,1500,2500);
	
	//fit_3->SetParLimits(0,0,1000);
	//fit_3->SetParLimits(2,0,1e6);
	
	
	//fit_3->FixParameter(2,v2[0]);
	//fit_3->FixParameter(3,v2[1]);
	
	for(int i=0;i<1;i++)
		graph_c->Fit("fit_3","MR+");
	
	graph_c->Draw("");
	//fit_1->Draw("SAME");
	//fit_2->Draw("SAME");
	fit_3->Draw("SAME");
	
	_f->WriteObject(c,Form("canvas_average_ch_c"),"TObject::kOverwrite");	
	
	
	//--------------------------------------------------------------	
}
