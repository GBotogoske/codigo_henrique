#include "../MYCODES.h"
#define n_peaks 10

Double_t integral_fit(TF1* fit,Double_t max, Double_t min ,Double_t delta)
{
	Double_t integral=0.0;
	for(Double_t i=min;i<=max;i+=delta)
	{
		integral+=fit->Eval(i);
	}
	return integral;

}

Double_t integral_fit_error(TF1* fit,Double_t max, Double_t min ,Double_t delta,Double_t* error)
{
	Double_t error_2;
	error_2=0.0;
	for(Double_t i=min;i<=max;i+=delta)
	{
		error_2+=pow(fit->Eval(i)*error[0]/fit->GetParameter(0),2);
		error_2+=pow(fit->Eval(i)*(i-fit->GetParameter(1))*error[1]/pow(fit->GetParameter(2),2),2);
		error_2+=pow(fit->Eval(i)*pow((i-fit->GetParameter(1)),2)*error[2]/pow(fit->GetParameter(2),3),2);
	}
	return sqrt(error_2);

}


void fit_hist_charge()
{
	Double_t min=-1851.88;
	Double_t max=2110.15;
	Double_t n_bin= 5000;
	Double_t tam_bin= (max-min)/n_bin;
	TFile* _charge = new TFile("charge.root","UPDATE");
	TH1F* hist0 = (TH1F*)_charge->Get("charge_histogram_0");
	
	ofstream MyFile("param.txt");
	MyFile << "integral error"<< endl;
	
	//int n_peaks=2;
	//vector<vector<Double_t>> param(n_peaks); //A,mean,std   //A.e^((x-mean)/2std^2)
	Double_t param[n_peaks][3];
	Double_t param_error[n_peaks][3];
	vector<float> min_fit(n_peaks);
	vector<float> max_fit(n_peaks);
	
	/*param.resize(n_peaks);
	for(Int_t i = 0; i<n_peaks; i++){
	      param[i].resize(3);
    	}*/
	
	vector<TF1*> fit(n_peaks);
	vector<TF1*> fit_conj(n_peaks);
	
	vector<TF1*> fit_f(n_peaks);
	
	
	//baseline
	param[0][0]=100.0; //A
	param[0][1]=0.0; //MEAN
	param[0][2]=5.0; //STD
	
	float delta_fit=5;
	min_fit[0]=-delta_fit;
	max_fit[0]=+delta_fit;
	
	
	//first photo-electron
	param[0][0]=100.0; //A
	param[0][1]=11.0; //MEAN
	param[0][2]=5.0; //STD
	
	min_fit[1]=7.5;
	max_fit[1]=12.5;
	
	//----------------------------------
	//ruido
	fit[0] = new TF1(Form("fit_%i",0),"gaus",min_fit[0],max_fit[0]);
	fit[0]->SetParameters(param[0]);
	hist0->Fit(Form("fit_%i",0),"0R+");
	fit[0]->GetParameters(param[0]);
	//--------------------------
	
	//first photo-electron
	fit[1] = new TF1(Form("fit_%i",1),"gaus",min_fit[1],max_fit[1]);
	fit[1]->SetParameter(0,param[1][0]);
	fit[1]->SetParameter(1,param[1][1]);
	fit[1]->SetParameter(2,param[1][2]);
	hist0->Fit(Form("fit_%i",1),"0R+");
	fit[1]->GetParameters(param[1]);
	
	//first phto-electron plus noise
	fit_conj[0] = new TF1(Form("fit_conj%i",0),"gaus(0)+gaus(3)",min_fit[0],max_fit[1]);
	fit_conj[0]->SetParameter(0,param[0][0]);
	fit_conj[0]->SetParameter(1,param[0][1]);
	fit_conj[0]->SetParameter(2,param[0][2]);
	fit_conj[0]->SetParameter(3,param[1][0]);
	fit_conj[0]->SetParameter(4,param[1][1]);
	fit_conj[0]->SetParameter(5,param[1][2]);
	fit_conj[0]->SetLineColor(3); //green
	hist0->Fit(Form("fit_conj%i",0),"0R+");
	
	TFitResultPtr r;

	string gaus("gaus"); 

	//para todos os picos		
	for(int i=2;i<n_peaks;i++)
	{	
		for(int j=0;j<i;j++)
		{
			for(int k=0;k<3;k++)
			{	
				param[j][k]=fit_conj[i-2]->GetParameter(j*3+k);
			}
		}
		
		
		param[i][0]=param[1][0]/(i+1);
		param[i][1]=param[i-1][1]+(param[i-1][1]-param[i-2][1]);
		param[i][2]=sqrt(i)*param[1][2];
		delta_fit=param[i][2]/2;
		min_fit[i]=param[i][1]-delta_fit;
		max_fit[i]=param[i][1]+delta_fit;
		
		fit[i] = new TF1(Form("fit_%i",i),"gaus",min_fit[i],max_fit[i]);
		fit[i]->SetParameter(0,param[i][0]);
		fit[i]->SetParameter(1,param[i][1]);
		fit[i]->SetParameter(2,param[i][2]);
		
		
		//primeito tenta fitar um gassiana no novo pico
		for(int j=0;j<=5;j++)
				hist0->Fit(Form("fit_%i",i),"0RQ+");
		
		
		//construi a funcao total
		for(int j=0;j<=i;j++)
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
		
		//.q
		fit_conj[i-1] = new TF1(Form("fit_conj%i",i-1),gaus.c_str(),min_fit[0],max_fit[i]);
		for(int j=0;j<=i;j++)
		{
			for(int k=0;k<3;k++)
			{	
				fit_conj[i-1]->SetParameter(j*3+k,param[j][k]);
				if((j*3+k)%3==1 && j!=i)
				{
					//fit_conj[i-1]->FixParameter(j*3+k,param[j][k]);
					fit_conj[i-1]->SetParLimits(j*3+k,param[j][k]-5,param[j][k]+5);
					fit_conj[i-1]->SetParLimits(j*3+k-1,param[j][k-1]-10,param[j][k-1]+10);
				}
			}
		}
		
		
		int n_fit=10;
		for(int l=0;l<n_fit;l++)
		{
			if(l==(n_fit-1))
			{
				for(int j=0;j<=i;j++)
				{
					
					fit_conj[i-1]->SetParName(3*j,Form("A%i",j));
					fit_conj[i-1]->SetParName(3*j+1,Form("mean%i",j));
					fit_conj[i-1]->SetParName(3*j+2,Form("std%i",j));			
				}
				
				r=hist0->Fit(Form("fit_conj%i",i-1),"0RS+");
			}
			else
			{
				hist0->Fit(Form("fit_conj%i",i-1),"0RQ+");

			}
		}
		
	}
	
	
	
	//desenha os picos
	TCanvas* c1 = new TCanvas();
	c1->SetCanvasSize(1400, 600);
   	c1->SetWindowSize(1500, 1500);
	
	TF1* teste =  new TF1("teste","gaus",min_fit[0],max_fit[0]);
	
	//func->IntegralError(x1,x2,r->GetParams(), r->GetCovarianceMatrix()->GetMatrixArray() );
	
	
	hist0->Draw("");
	int color=0;
	Double_t matrix[9];
	
	Double_t mini=-50;
	Double_t maxi=950;

	
	for(int i=0;i<n_peaks;i++)
	{	
		for(int k=0;k<3;k++)
		{	
				param[i][k]=fit_conj[n_peaks-2]->GetParameter(i*3+k);
				param_error[i][k]=fit_conj[n_peaks-2]->GetParError(i*3+k);
		}
		
		for(int m=0;m<3;m++)
		{	
			for(int n=0;n<3;n++)
			{
				matrix[3*m+n]=r->GetCovarianceMatrix()[3*i+m][3*i+n];
			}
		}
		
		fit_f[i] = new TF1(Form("fit_%i",i),"gaus",-50,950);
		for(int k=0;k<3;k++)
		{	
			fit_f[i]->SetParameter(k,param[i][k]);
		}
			
			if(color==0 || color==9)
			{
				color=3;
			}
			else
				color++;
		
		fit_f[i]->SetNpx((int)(maxi-mini)*tam_bin);

		fit_f[i]->SetLineColor(color);
		fit_f[i]->Draw("SAME");
		cout<<"Integral["<<i<<"] = " << integral_fit(fit_f[i],maxi,mini,tam_bin)<<endl;
		
		cout<<"Error["<<i<<"] = " << integral_fit_error(fit_f[i],maxi,mini,tam_bin,param_error[i])<<endl;

		MyFile << integral_fit(fit_f[i],maxi,mini,tam_bin) << " "<<integral_fit_error(fit_f[i],maxi, mini ,tam_bin,param_error[i])<<endl;

	}
	fit_conj[n_peaks-2]->SetLineColor(2);
	fit_conj[n_peaks-2]->Draw("SAME");
	_charge->WriteObject(c1,Form("charge_histogram_fitted"),"TObject::kOverwrite");
	
	cout<<"Integral[ TOTAL] " << fit_conj[n_peaks-2]->Integral(-50,-50+1000)<<endl;
		
	cout<<"Error[Total] = " << fit_conj[n_peaks-2]->IntegralError(-25,-25+1000,r->GetParams(), r->GetCovarianceMatrix().GetMatrixArray() )<<endl;
	
	//c1->Close();
	//fit[0]->Draw("SAME");
	//fit[1]->Draw("SAME");
	//fit[2]->Draw("SAME");
	//fit_conj[n_peaks-2]->SetLineColor(3); //green
	//fit_conj[n_peaks-2]->Draw("SAME");
}

