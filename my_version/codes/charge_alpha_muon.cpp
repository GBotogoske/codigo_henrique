#include "../MYCODES.h"
#define memorydepth_run 6000

void charge_alpha_muon()
{
	//abre arquivo root
	int run=5;
	TFile* _charge= new TFile("charge.root","UPDATE");
	TFile* _f= new TFile(Form("./../../%i_run/codes/analyzed.root",run),"UPDATE");
	
	//ratio das cargas para filtar
	float I=0.6;
	
	//Abrir arvore t2 ("ambos os canais")
	TTree *t1 = (TTree*)_f->Get("t1");
	
	
	//criar novo TCanvas
	TCanvas *c = new TCanvas();
   	c->SetCanvasSize(1400, 600);
   	c->SetWindowSize(1500, 1500);
	
	//imprimir grafico da razão das cargas em função da carga maior(energia do sinal)
	t1->Draw("Ch0.charge_m/Ch0.charge:Ch0.charge","","ap");
	
	//imprimir linha que separa alpha dos muons
	TLine* l1 = new TLine(-1e5,I,1e8,I);
	l1->SetLineColor(kRed);
	l1->SetLineWidth(2);
	l1->Draw("");
	
	_charge->WriteObject(c,Form("charge_ratio"),"TObject::kOverwrite");
	
	c->Close();
	
	//----------------------------------------------------------------
	
	//le o numero de entradas na t root: 
	Int_t entries=(Int_t)t1->GetEntries();
  	cout<< "Entries = " << entries << "   " << endl;
	//------------------------------------------------------------------


	TTree* ta = new TTree("ta","filtered by charge");
	TTree* tm = new TTree("tm","filtered by charge");
	
	Double_t wvf[memorydepth_run];
	Double_t wvf_filtered[memorydepth_run];
	Double_t charge;
	Double_t charge_m;
	
	Double_t wvfa[memorydepth_run];
	Double_t wvf_filtereda[memorydepth_run];
	Double_t chargea;
	Double_t charge_ma;
	
	Double_t wvfm[memorydepth_run];
	Double_t wvf_filteredm[memorydepth_run];
	Double_t chargem;
	Double_t charge_mm;
	
	
	//t1->SetBranchAddress("Ch0->wvf",wvf);
	TBranch* branch1 = t1->GetBranch("Ch0");
	TLeaf* leaf_wvf = branch1->GetLeaf("wvf");
	TLeaf* leaf_wvf_f = branch1->GetLeaf("wvf_filtered");
	TLeaf* leaf_charge = branch1->GetLeaf("charge");
	TLeaf* leaf_charge_m = branch1->GetLeaf("charge_m");
	leaf_wvf->SetAddress(&wvf);
	leaf_wvf_f->SetAddress(&wvf_filtered);
	leaf_charge->SetAddress(&charge);
	leaf_charge_m->SetAddress(&charge_m);
	
	//cria o branch em nova ttree
	ta->Branch("wvf",&wvfa,Form("wvf[%i]/D",memorydepth_run));
	ta->Branch("wvf_f",&wvf_filtereda,Form("wvf_f[%i]/D",memorydepth_run));
	ta->Branch("charge",&chargea,Form("charge"));
	ta->Branch("charge_m",&charge_ma,Form("charge_m/D"));

	tm->Branch("wvf",&wvfm,Form("wvf[%i]/D",memorydepth_run));
	tm->Branch("wvf_f",&wvf_filteredm,Form("wvf_f[%i]/D",memorydepth_run));
	tm->Branch("charge",&chargem,"charge/D");
	tm->Branch("charge_m",&charge_mm,Form("charge_m/D"));
//----------------------------------------------
	
	//sinais medios
	vector<Double_t> avg_a;
  	vector<Double_t> avg_m;
  	vector<Double_t> avg_fa;
  	vector<Double_t> avg_fm;
  	vector<Double_t> time;

	avg_a.resize(memorydepth_run,0);
	avg_m.resize(memorydepth_run,0);
	avg_fa.resize(memorydepth_run,0);
	avg_fm.resize(memorydepth_run,0);
	time.resize(memorydepth_run,0);
	
	int na = 0;
	int nm = 0;
	/*
//-------------------------------------------	
	cout<<"cheguei aki"<<endl;
	*/
	
	//varre todas as entradas da ttree
	for(int count=0;count<100;count++)
	{
		
		t1->GetEntry(count);
		
		//cout<<ch_read.charge_m/ch_read.charge<<endl;
		if(charge_m/charge>=I)
		{
			for(Int_t j = 0; j<memorydepth_run; j++)
			{
				avg_a[j]+=wvf[j];
				avg_fa[j]+=wvf_filtered[j];
				wvf_filtereda[j]=wvf_filtered[j];
				wvfa[j]=wvf[j];
				
			}
			na++;
			
			chargea=charge;
			charge_ma=charge_m;
			
			ta->Fill();
		}
		else
		{
			for(Int_t j = 0; j<memorydepth_run; j++)
			{
				avg_m[j]+=wvf[j];
				avg_fm[j]+=wvf_filtered[j];
				wvf_filteredm[j]=wvf_filtered[j];
				wvfm[j]=wvf[j];
				
			}
			nm++;
			
			chargem=charge;
			charge_mm=charge_m;
			
			tm->Fill();
		}	
	}
	
	_charge->WriteObject(ta,"ta","TObject::kOverwrite");
	_charge->WriteObject(tm,"tm","TObject::kOverwrite");
	
	//-------------------------------------------------------------------------------
	
	
	for(Int_t j = 0; j<memorydepth_run; j++)
	{
      		time[j] = j*4;
    	}
    	

	for(Int_t j = 0; j<memorydepth_run; j++)
	{
        	avg_a[j] = avg_a[j]/na;
        	avg_m[j] = avg_m[j]/nm;
      	}
      	
	TGraph* graph_c;
	graph_c = new TGraph(memorydepth_run,&time[0],&avg_a[0]);
	
	TGraph* graph_d;
	graph_d = new TGraph(memorydepth_run,&time[0],&avg_m[0]);
	
	_charge->WriteObject(graph_c,Form("average_ch_a"),"TObject::kOverwrite");
	_charge->WriteObject(graph_d,Form("average_ch_m"),"TObject::kOverwrite");

}
