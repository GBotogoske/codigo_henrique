#define memorydepth 6000
//#include "../MYCODES.h"


void plot_mean_signals()
{

	TFile* _f1 = new TFile("charge.root","UPDATE");
	TGraph* g1 = (TGraph*)_f1->Get("average_ch_spe_filtered_1");
	TGraph* g2 = (TGraph*)_f1->Get("average_ch_spe_filtered_2");
	TGraph* g3 = (TGraph*)_f1->Get("average_ch_spe_filtered_3");
	g3->SetTitle("Signal;time[ns];Voltage[mV]");
	g3->SetLineColor(3);
	g2->SetTitle("Signal;time[ns];Voltage[mV]");
	g2->SetLineColor(2);
	g1->SetTitle("Signal;time[ns];Voltage[mV]");
	g1->SetLineColor(1);
	g3->Draw("");
	g2->Draw("SAME");
	g1->Draw("SAME");
	
	auto legend = new TLegend(0.1,0.7,0.48,0.9);
   	//legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
   	legend->AddEntry(g1,"One photo-electron","f");
   	legend->AddEntry(g2,"Two photo-electron","f");
   	legend->AddEntry(g3,"Three photo-electron","f");
   	//legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
   	//legend->AddEntry("gr","Graph with error bars","lep");
   	legend->Draw();
}
