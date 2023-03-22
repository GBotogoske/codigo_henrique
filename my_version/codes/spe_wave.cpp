#include "../MYCODES.h"
#define n_peaks 10
#define peak 1

void spe_wave()
{
	//le os arquivo que tem as formas de onda
	TFile* _f = new TFile("analyzed.root","UPDATE");
	//le o arquivo com o fit do histograma de carga
	TFile* _charge = new TFile("charge.root","UPDATE");
	//lo o histograma do fit
	TCanvas* c = (TCanvas*)_charge->Get("charge_histogram_fitted");
	//recupera o fit
	TF1 *g = (TF1*)c->GetListOfPrimitives()->FindObject(Form("fit_conj%i",n_peaks-2));
	
	//onde os parametros sao salvos
	Double_t param[n_peaks][3]; //A,mean,std
	
	//recupera os parametros do fit
	for(int j=0;j<n_peaks;j++)
	{
		for(int k=0;k<3;k++)
		{	
				param[j][k]=g->GetParameter(j*3+k);
		}
	}
	//---------------------------------------------
	//plota o fit
	string gaus;
	for(int j=0;j<=n_peaks;j++)
	{
		if(j==0)
		{
			gaus="gaus(0)";
		}
		else
		{
			gaus=gaus+Form("+gaus(%i)",3*j);
		}
	}
	
	
	TF1* graph_charge = new TF1("graph_hist_charge",gaus.c_str(),-100,1000);
	
	for(int j=0;j<=n_peaks;j++)
	{
		for(int k=0;k<3;k++)
		{	
			graph_charge->SetParameter(j*3+k,param[j][k]);
		}
	}
	
	c->Close();
	
	c = new TCanvas();
	c->SetCanvasSize(1400, 600);
   	c->SetWindowSize(1500, 1500);
   	graph_charge->SetNpx(1000);
	graph_charge->Draw("");
		
	
	//minimo entre a primeira e segunda media
	float_t range1 = graph_charge->GetMinimumX(param[peak-1][1],param[peak][1]);
	//minimo entre a segunda e terceira media
	float_t range2 = graph_charge->GetMinimumX(param[peak][1],param[peak+1][1]);
	
	
	TLine* l1 = new TLine(range1,0,range1,1000);
	l1->SetLineColor(kGreen);
	l1->SetLineWidth(2);
	l1->Draw("SAME");

	TLine* l2 = new TLine(range2,0,range2,1000);
	l2->SetLineColor(kGreen);
	l2->SetLineWidth(2);
	l2->Draw("SAME");
	
	
	_charge->WriteObject(c,Form("graph_fit_charge"),"TObject::kOverwrite");
	c->Close();
	
	
	//---------------------------------------------------------------
	
	TTree* t1 = (TTree*)_f->Get("t1");
	TTree* tspe = new TTree("tspe","spe");
	
	ADC_DATA ch_read;
	ADC_DATA ch_write;
	
	TBranch* branch = t1->GetBranch(Form("Ch0"));
	branch->SetAddress(&ch_read);
	
	tspe->Branch("Ch_spe",&ch_write,ch_read.tobranch.c_str());
	
	Int_t entries=(Int_t)t1->GetEntries();
	
	vector<Double_t> avg;
	vector<Double_t> avg_f;
	vector<Double_t> time;
	avg.resize(memorydepth,0);
	avg_f.resize(memorydepth,0);
	time.resize(memorydepth,0);
	int n = 0;
	//----------------------------------------------------------
	
	
	for(int count=0;count<entries;count++)
	{
		t1->GetEntry(count);
		if(ch_read.charge>range1 && ch_read.charge<range2)
		{
			for(Int_t j = 0; j<memorydepth; j++)
			{
				avg[j]+=ch_read.wvf[j];
				avg_f[j]+=ch_read.wvf_filtered[j];
			}
			n++;
			ch_write=ch_read;
			tspe->Fill();
		}
	
	}
	
	
	
	for(Int_t j = 0; j<memorydepth; j++)
	{
      		time[j] = j*2;
    	}
    	

	for(Int_t j = 0; j<memorydepth; j++)
	{
        	avg[j] = avg[j]/n;
        	avg_f[j] = avg_f[j]/n;
      	}
      	
	TGraph* graph_c;
	graph_c = new TGraph(memorydepth,&time[0],&avg[0]);
	
	TGraph* graph_d;
	graph_d = new TGraph(memorydepth,&time[0],&avg_f[0]);
	
	_charge->WriteObject(graph_c,Form("average_ch_%i",peak),"TObject::kOverwrite");
	_charge->WriteObject(graph_d,Form("average_ch_spe_filtered_%i",peak),"TObject::kOverwrite");
	
	
	_charge->WriteObject(tspe,Form("t_%i",peak),"TObject::kOverwrite");
	
}
