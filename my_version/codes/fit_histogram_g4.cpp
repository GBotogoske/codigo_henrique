#include <TH1F.h>
#include <TMath.h>

void fit_histogram_g4() 
{
//---------------------------------------------------
	//Abrir histograma g4
	TFile* _f = new TFile("fit_e.root","UPDATE");
	TH1F* hist_g4_original = (TH1F*)_f->Get("charge_histogram_g"); 

	TH1F* hist_alpha = (TH1F*)_f->Get("charge_histogram_a"); 
	
	
	//reescale
	// Define the scaling factor
		
	
	double alpha =  0.017677;
	double C = 2.165039;
	double A = 2248.07212;
	double b = 0.017486;
	
	// Define the new binning with the rescaled x-axis
	int nbins = hist_g4_original->GetNbinsX();
	TH1F* hist_g4 = new TH1F("charge_histogram_g_cp", "Charge", nbins, 0, 700);
	
	// Loop over all the bins of the original histogram and copy the contents to the new histogram
	
	
	double min=0.0;
	double max=0.0;
	double center=0.0;
	double center_fit=0.0;
	
	int min_bin=70;
	
	double content=0.0;
	
	
	for (int i = 1; i <= nbins; i++)
	{
		min= hist_g4->GetBinLowEdge(i);
		center_fit=hist_g4->GetBinCenter(i);
		max=min+2*(center_fit-min);
		//cout<<min<<" "<<max<<endl;
		if(max>700)
		{
			break;
		}
		content=0.0;
		for(int j = 1; j <= nbins; j++)
		{
			center = hist_g4_original->GetBinCenter(j) * alpha;
			if(center>=min && center<max)
			{
				content=content+hist_g4_original->GetBinContent(j);
			
			}
			if(center>700)
			{
				break;
			}
			//break se passar do max, para ser mais rapido
		}
		
		if(center_fit>=min_bin)
		{
			content = C*content + A*exp(-b*center_fit); 	
			hist_g4->SetBinContent(i, content);	
		}
		else
		{
			hist_g4->SetBinContent(i, 0.0);	
		}
		
	}


	
	//-----------------------------------------
 	
 	
 	//hist_g4->Add(hist_exp);
 	
 	// draw the histogram with the rescaled x-axis
	
	
	hist_g4->SetTitle("Histogram fitted ;photo-electrons;entries");
	
	//hist_g4->SetTitle("Signal ;time[ns];Voltage[mV]");
	hist_alpha->SetLineColor(2);

	hist_g4->Draw("");
	hist_g4->SetFillColor(kBlue);
	hist_alpha->SetLineWidth(2);
	
	hist_alpha->Draw("SAME");
	
	
	auto legend = new TLegend(0.1,0.7,0.48,0.9);
   	//legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
   	legend->AddEntry(hist_g4,"MC fit","f");
   	legend->AddEntry(hist_alpha,"Experimental","f");
	legend->Draw();
	//hist_exp->Draw();
	
 
}

