#include "../MYCODES.h"
#define size_spe 1000
#define size_signal 6000
#define size_total 12000

void my_moving_average(Double_t* signal_f,Double_t* signal, int size, int N)
{
	for(int i=0;i<N;i++)
	{
	
		signal_f[i]=0.0;
		if(i>=size)
		{
			
			for(int j=0;j<size;j++)
			{
				signal_f[i]+=signal[i-j];
			}
			signal_f[i]=signal_f[i]/size;
		}
		else
		{
			for(int j=0;j<(i+1);j++)
			{
				signal_f[i]+=signal[i-j];
			}
			signal_f[i]=signal_f[i]/(i+1);
		}
	
	
	}
	
}


void my_fourier_transform(Double_t* real, Double_t* imag, Double_t* signal, int N) 
{	
	//Double_t divider = (Double_t)sqrt(N);
	for(int k=0;k<N;k++)
	{
		real[k]=0.0;
		imag[k]=0.0;
	
		for(int n=0;n<N;n++)
		{
			real[k]+=signal[n]*cos(2*M_PI*k*n/N);
			imag[k]+=-signal[n]*sin(2*M_PI*k*n/N);
		}
		real[k]=real[k];
		imag[k]=imag[k];
	}
}


void my_inverse_fourier_transform(Double_t* signal,Double_t* real, Double_t* imag, int N) 
{	
	Double_t divider = N;
	for(int k=0;k<N;k++)
	{
		signal[k]=0.0;
		for(int n=0;n<N;n++)
		{
			signal[k]+=real[n]*cos(2*M_PI*k*n/N)-imag[n]*sin(2*M_PI*k*n/N);
			
		}
		signal[k]=signal[k]/divider;
	}
}

void my_convolution(Double_t* c, Double_t* s1,Double_t* s2,int N1,int N2) //N1>=N2
{
	for(int n=0;n<(N1+N2);n++)
	{
		c[n]=0;
		if(n<N2)
		{
			for(int m=0;m<(n+1);m++)
			{
				c[n]+=s2[m]*s1[n-m];
			}
		}
		else if(n>=N1)
		{
			for(int m=n-N1+1;m<N2;m++)
			{
				c[n]+=s2[m]*s1[n-m];
			}
		}
		else
		{
			for(int m=0;m<N2;m++)
			{
				c[n]+=s2[m]*s1[n-m];
			}
		}
	}

}



void deconvolution_signal()
{
	int dtime=2;//(ns)
	
	
	//abre arquivo charge, onde esta o sinal spe
	TFile* _charge = new TFile("charge.root","UPDATE");
	TGraph* spe = (TGraph*)_charge->Get("average_ch_spe_filtered_1");
	
	
	//abre arquivo alphaxmuon, onde esta o sinal alpha/muon
	TFile* _alphaxmuon = new TFile("alphaxmuon.root","UPDATE");
	TGraph*  alpha = (TGraph*)_alphaxmuon->Get("average_ch_alpha");
	TGraph* muon = (TGraph*)_alphaxmuon->Get("average_ch_muon");

	//pega os sinais	
	Double_t* spe_y=spe->GetY();
	Double_t* alpha_y=alpha->GetY();
	Double_t* muon_y=muon->GetY();
	
	//versoes com zero padding
	Double_t spe_y_zp[size_total];
	Double_t alpha_y_zp[size_total];
	Double_t muon_y_zp[size_total];
	
	
	//vetor de tmepo
	Double_t time[size_signal];
	for(int i =0; i<size_signal;i++)
	{
		time[i]=dtime*i;
	}

	//vetor de tempo ampliado
	Double_t time2[size_total];
	for(int i =0; i<size_total;i++)
	{
		time2[i]=dtime*i;
	}
	
	
	
	Double_t A=13.93;
	Double_t cte= -0.0011502;
	
	//-----------------------------------------
	
	//zero padding
	for(int i=0;i<size_total;i++)
	{
		if(i<size_spe)
		{
			spe_y_zp[i]=spe_y[i];
		}
		else
		{
			spe_y_zp[i]=0.0;
			//spe_y_zp[i]=A*exp(cte*i*4);
		}
	}
	
	A= 2.37212;
	cte=-0.00012231;
	
	for(int i=0;i<size_total;i++)
	{
		if(i<size_signal)
		{
			alpha_y_zp[i]=alpha_y[i];
			muon_y_zp[i]=muon_y[i];
		}
		else
		{
			//alpha_y_zp[i]=A*exp(cte*i*4);
			alpha_y_zp[i]=0.0;
			muon_y_zp[i]=0.0;
		}
	}
	
	
	//filtra mais os sinais
	Double_t alpha_ma[size_total];
	my_moving_average(alpha_ma,alpha_y_zp,1,size_total);
	
	Double_t spe_ma[size_total];
	my_moving_average(spe_ma,spe_y_zp,1,size_total);
	
	Double_t muon_ma[size_total];
	my_moving_average(muon_ma,muon_y_zp,1,size_total);
	
	//------------------------------------------
	//transformada de fourier
	
	Double_t spe_re[size_total];
	Double_t spe_im[size_total];
	
 	my_fourier_transform(spe_re,spe_im,spe_ma,size_total);
 	//my_fourier_transform(spe_re,spe_im,spe_ma,size_total);
 	//------------------------------------------------------------
 
	Double_t alpha_re[size_total];
	Double_t alpha_im[size_total];
	
	Double_t muon_re[size_total];
	Double_t muon_im[size_total];
	
	my_fourier_transform(alpha_re,alpha_im,alpha_ma,size_total);
	my_fourier_transform(muon_re,muon_im,muon_ma,size_total);
 		
	
	//--------------------------------------------------------------------------
	//pegar parte real e imaginaria
	Double_t light_alpha_re[size_total];
	Double_t light_alpha_im[size_total];
	Double_t light_muon_re[size_total];
	Double_t light_muon_im[size_total];
	
	Double_t mod=0.0;
	for(int i=0;i<size_total;i++)
	{
		mod=pow(spe_re[i],2)+pow(spe_im[i],2);
		light_alpha_re[i]=(alpha_re[i]*spe_re[i]+alpha_im[i]*spe_im[i])/mod;
		light_muon_re[i]=(muon_re[i]*spe_re[i]+muon_im[i]*spe_im[i])/mod;
	
		light_alpha_im[i]=(alpha_im[i]*spe_re[i]-alpha_re[i]*spe_im[i])/mod;
		light_muon_im[i]=(muon_im[i]*spe_re[i]-muon_re[i]*spe_im[i])/mod;
	
	}
	
	
	//------------------------------------------------------------------------------
	
	//fazer a trasnformada inversa

	Double_t light_alpha[size_total];
	my_inverse_fourier_transform(light_alpha,light_alpha_re,light_alpha_im,size_total);
	
	Double_t light_muon[size_total];
	my_inverse_fourier_transform(light_muon,light_muon_re,light_muon_im,size_total);
	
	//------------------------------------------------------------------------------
	
	
	int n_ma=1;
	Double_t light_alpha_ma[size_total];
	my_moving_average(light_alpha_ma,light_alpha,n_ma,size_total);
	
	Double_t light_muon_ma[size_total];
	my_moving_average(light_muon_ma,light_muon,n_ma,size_total);
	//----------------------------------------------------
	
	
	TGraph* graph_alpha;
	graph_alpha = new TGraph(size_total,&time2[0],&light_alpha_ma[0]);
	
	TGraph* graph_muon;
	graph_muon = new TGraph(size_total,&time2[0],&light_muon_ma[0]);
	
	
	//--------------------------------
	
	Double_t time3[size_total+size_signal];
	for(int i =0; i<(size_total+size_signal);i++)
	{
		time3[i]=dtime*i;
	}
	
	
	Double_t teste_alpha[size_total+size_signal];
	Double_t teste_muon[size_total+size_signal];
	
	
	my_convolution(teste_alpha,light_alpha_ma,spe_ma,size_total,size_signal);
	my_convolution(teste_muon,light_muon_ma,spe_ma,size_total,size_signal);
	
	
	TGraph* graph_c;
	graph_c = new TGraph(size_total+size_signal,&time3[0],&teste_alpha[0]);
	
	TGraph* graph_d;
	graph_d = new TGraph(size_total+size_signal,&time3[0],&teste_muon[0]);
	graph_d->SetLineColor(3);


	_charge->WriteObject(graph_alpha,Form("alpha_light"),"TObject::kOverwrite");
	_charge->WriteObject(graph_muon,"muon_light","TObject::kOverwrite");

	_charge->WriteObject(alpha,Form("alpha"),"TObject::kOverwrite");
	_charge->WriteObject(muon,Form("muon"),"TObject::kOverwrite");	
	_charge->WriteObject(graph_c,Form("impulse_response_alpha"),"TObject::kOverwrite");
	_charge->WriteObject(graph_d,Form("impulse_responde_muon"),"TObject::kOverwrite");
	
	
}
