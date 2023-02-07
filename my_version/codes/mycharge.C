#include "../MYCODES.h"

void mycharge()
{
	//abre arquivo root
	TFile* _f= new TFile("analyzed.root","UPDATE");
	
	//ratio das cargas para filtar
	float I=0.4;
	
	//Abrir arvore t2 ("ambos os canais")
	TTree *t2 = (TTree*)_f->Get("t2");
	
	
	//criar novo TCanvas
	TCanvas *c = new TCanvas();
   	c->SetCanvasSize(1400, 600);
   	c->SetWindowSize(1500, 1500);
	
	//imprimir grafico da razão das cargas em função da carga maior(energia do sinal)
	t2->Draw("Ch_t.charge_m/Ch_t.charge:Ch_t.charge","","ap");
	
	//imprimir linha que separa alpha dos muons
	TLine* l1 = new TLine(-1e5,I,1e8,I);
	l1->SetLineColor(kRed);
	l1->SetLineWidth(2);
	l1->Draw("");
	
	_f->WriteObject(c,Form("charge"),"TObject::kOverwrite");
	
	c->Close();
	
	//----------------------------------------------------------------
	
	//le o numero de entradas na t root: 2 x numeros de triggers pego ( 2 canais )
	Int_t entries=(Int_t)t2->GetEntries();
  	cout<< "Entries = " << entries << "   " << endl;;
	//------------------------------------------------------------------


	
	TTree* tc = new TTree("tc","filtered by charge");
	TTree* td = new TTree("tc","filtered by charge");
	TBranch* branch1;
	ADC_DATA ch_write1;
	ADC_DATA ch_write2;
	ADC_DATA ch_read;
	
	//pega o branch
	branch1 = t2->GetBranch(Form("Ch_t"));
	//seta para esse endereço
	branch1->SetAddress(&ch_read);
	
	//cria o branch em nova ttree
	tc->Branch("Channel_Charge",&ch_write1,ch_write1.tobranch.c_str());
	td->Branch("Channel_Charge",&ch_write2,ch_write2.tobranch.c_str());
	
//----------------------------------------------	
	//sinais medios
	vector<Double_t> avg_c;
  	vector<Double_t> avg_d;
  	vector<Double_t> time;
	
	avg_c.resize(memorydepth,0);
	avg_d.resize(memorydepth,0);
	time.resize(memorydepth,0);
	
	int nc = 0;
	int nd = 0;
//-------------------------------------------	
	
	//varre todas as entradas da ttree
	for(int count=0;count<entries;count++)
	{
		t2->GetEntry(count);
		//cout<<ch_read.charge_m/ch_read.charge<<endl;
		if(ch_read.charge_m/ch_read.charge>=I)
		{
			for(Int_t j = 0; j<memorydepth; j++)
			{
				avg_c[j]+=ch_read.wvf[j];
			}
			nc++;
			ch_write1=ch_read;
			tc->Fill();
		}
		else
		{
			for(Int_t j = 0; j<memorydepth; j++)
			{
				avg_d[j]+=ch_read.wvf[j];
			}
			nd++;
			ch_write2=ch_read;
			td->Fill();
		}	
	}
	
	_f->WriteObject(tc,"tc","TObject::kOverwrite");
	_f->WriteObject(td,"td","TObject::kOverwrite");
	
	//-------------------------------------------------------------------------------
	
	for(Int_t j = 0; j<memorydepth; j++)
	{
      		time[j] = j*4;
    	}
	
	for(Int_t j = 0; j<memorydepth; j++)
	{
        	avg_c[j] = avg_c[j]/nc;
        	avg_d[j] = avg_d[j]/nd;
      	}
	
	TGraph* graph_c;
	graph_c = new TGraph(memorydepth,&time[0],&avg_c[0]);
	
	TGraph* graph_d;
	graph_d = new TGraph(memorydepth,&time[0],&avg_d[0]);
	
	_f->WriteObject(graph_c,Form("average_ch_c"),"TObject::kOverwrite");
	_f->WriteObject(graph_d,Form("average_ch_d"),"TObject::kOverwrite");	
}
