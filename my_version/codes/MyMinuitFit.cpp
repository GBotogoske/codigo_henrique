#include "TMinuit.h"
#include <TH1F.h>
#include <TMath.h>


int min_bin=28;
TH1F* hist_g4_original;
TH1F* hist_alpha;


// Define the function to fit and the chi-squared function
TH1F* MyFitFunction(Double_t *par) 
{
	double alpha = par[0];
	double C = par[1];
	double A = par[2];
	double b = par[3];
	
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

	return hist_g4;
}

void MyChi2Function(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag) 
{
	Double_t chi2 = 0.0;
	TH1F* hist_g4 = MyFitFunction(par);
	int nbins = hist_g4_original->GetNbinsX();
	for (Int_t i = 1; i <= nbins; i++)
	{
		Double_t diff = hist_alpha->GetBinContent(i) - hist_g4->GetBinContent(i);
		//chi2 += diff * diff / yErr[i] / yErr[i];
		chi2 = chi2 + diff*diff;
	}
	//hist_g4->Draw("");
	delete hist_g4;
	f = chi2;
}

void MyMinuitFit() 
{
	TFile* _f = new TFile("fit_e.root","UPDATE");
	hist_g4_original = (TH1F*)_f->Get("charge_histogram_g"); 
	hist_alpha = (TH1F*)_f->Get("charge_histogram_a"); 

	//------------------------------------------------------------------------------
	// Create an instance of the TMinuit class
	TMinuit *minuit = new TMinuit(4);

	// Set the function to be minimized
	minuit->SetFCN(MyChi2Function);

	// Set the starting values and step sizes for the fit parameters

	double alpha = 2/100;
	double C = 2;
	double A = 1700;
	double b = 1/58.82;
	
	Int_t ierr;

	Double_t vstart[4] = {alpha,C,A,b};
	Double_t step[4] = {0.001,0.001,1,0.0001};
	minuit->mnparm(0, "par0", vstart[0], step[0], 1/100, 5/100, ierr);
	minuit->mnparm(1, "par1", vstart[1], step[1], 1, 4, ierr);
	minuit->mnparm(2, "par2", vstart[2], step[2], 1000, 2500, ierr);
	minuit->mnparm(3, "par3", vstart[3], step[3], 0.0, 0.02, ierr);

	// Perform the minimization
	minuit->Migrad();

	// Retrieve the results of the fit
	Double_t par[4], err[4];
	minuit->GetParameter(0, par[0], err[0]);
	minuit->GetParameter(1, par[1], err[1]);
	minuit->GetParameter(2, par[2], err[2]);
	minuit->GetParameter(3, par[3], err[3]);

	// Print the results of the fit
	printf("Fit results:\n");
	printf("par0 = %f +/- %f\n", par[0], err[0]);
	printf("par1 = %f +/- %f\n", par[1], err[1]);
	printf("par2 = %f +/- %f\n", par[2], err[2]);
	printf("par3 = %f +/- %f\n", par[3], err[3]);
}

