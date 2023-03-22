#include "../MYCODES.h"
#define size_total 12000

 
void moving_average_circular(Double_t* signal_f,Double_t* signal, int size, int N)
{
	for(int i=0;i<N;i++)
	{
		signal_f[i]=0.0;
		for(int j=0;j<size;j++)
		{
			if((i-j)>=0)
			{
				signal_f[i]+=signal[i-j];
			}
			else
			{
				signal_f[i]+=signal[N+i-j];
			}
		}
		signal_f[i]=signal_f[i]/size;
	}
}


void filter_light_signal()
{
	  DENOISE dn;
	//aqui sera aplicado um filtro media movel circular
	int dtime=2;//(ns)
	
	//abre arquivo charge, onde esta o sinal spe
	TFile* _charge = new TFile("charge.root","UPDATE");	
	TGraph* light_alpha = (TGraph*)_charge->Get("alpha_light");
	TGraph* light_muon = (TGraph*)_charge->Get("muon_light");
	
	Double_t* light_alpha_y=light_alpha->GetY();
	Double_t light_alpha_ma[size_total];
	Double_t light_alpha_f2[size_total];
	
	Double_t* light_muon_y=light_muon->GetY();
	Double_t light_muon_ma[size_total];
	Double_t light_muon_f2[size_total];
	
	
	int window=40;
	moving_average_circular(light_alpha_ma,light_alpha_y, window, size_total);
	moving_average_circular(light_muon_ma,light_muon_y, window, size_total);
	dn.TV1D_denoise<Double_t>(&light_alpha_y[0],&light_alpha_f2[0], size_total,3 );
	dn.TV1D_denoise<Double_t>(light_muon_y,light_muon_f2, size_total,3 );
	
	Double_t time[size_total];
	for(int i =0; i<size_total;i++)
	{
		time[i]=dtime*i;
	}
	
	
	TGraph* graph_c;
	graph_c = new TGraph(size_total,&time[0],&light_alpha_ma[0]);
	graph_c->SetTitle("Light Signal ;time[ns]; Intensity[u.a.]");
	
	_charge->WriteObject(graph_c,Form("light_alpha_filtered"),"TObject::kOverwrite");
		
	TGraph* graph_d;
	graph_d = new TGraph(size_total,&time[0],&light_muon_ma[0]);
	graph_d->SetTitle("Light Signal ;time[ns]; Intensity[u.a.]");
	
	_charge->WriteObject(graph_d,Form("light_muon_filtered"),"TObject::kOverwrite");
	
	TGraph* graph_e;
	graph_e = new TGraph(size_total,&time[0],&light_alpha_f2[0]);
	
	_charge->WriteObject(graph_e,Form("light_alpha_filtered2"),"TObject::kOverwrite");
		
	TGraph* graph_f;
	graph_f = new TGraph(size_total,&time[0],&light_muon_f2[0]);
	
	_charge->WriteObject(graph_f,Form("light_muon_filtered2"),"TObject::kOverwrite");
	

}

