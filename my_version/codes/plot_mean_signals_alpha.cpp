#define memorydepth 6000
//#include "../MYCODES.h"


void plot_mean_signals_alpha()
{

	TFile* _f = new TFile("alphaxmuon.root","UPDATE");

	TGraph* g1 = (TGraph*)_f->Get("average_ch_alpha");
	TGraph* g2 = (TGraph*)_f->Get("average_ch_muon");

	g2->SetTitle("Signal ;time[ns];Voltage[mV]");
	g2->SetLineColor(2);
	g1->SetTitle("Signal ;time[ns];Voltage[mV]");
	g1->SetLineColor(1);

	g1->Draw("");
	g2->Draw("SAME");
	
	
	auto legend = new TLegend(0.1,0.7,0.48,0.9);
   	//legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
   	legend->AddEntry(g1,"alpha","f");
   	legend->AddEntry(g2,"muon","f");
   	
   	//legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
   	//legend->AddEntry("gr","Graph with error bars","lep");
   	legend->Draw();
}
