// ________________________________________ //
// Author: Henrique Souza
// Filename: variables.C
// Created: 2021
// ________________________________________ //
class ADC_DATA{
  public:
    Double_t peak;
    Double_t peakpos;
    Double_t charge;
    //Int_t channel;
    Double_t charge_m;
    Double_t fprompt;
    Double_t event;
    Double_t time;
    Double_t wvf[memorydepth];
    Int_t selection;
    Int_t chan;
    
    ADC_DATA();
    string tobranch = "peak/D:peakpos/D:charge/D:charge_m/D:fprompt/D:event/D:time/D:wvf[" + to_string(memorydepth) + "]/D:selection/I:chan/I";

    void setBranchName(Int_t n = memorydepth){
      tobranch = "peak/D:peakpos/D:charge/D:charge_m/D:fprompt/D:event/D:time/D:wvf[" + to_string(n) + "]/D:selection/I:chan/I";
    }

    // const char *tobranch = Form("peak/D:peakpos/D:charge/D:fprompt/D:event/D:time/D:wvf[%i]/D:selection/I",memorydepth);
};


ADC_DATA::ADC_DATA(){
  
  for(Int_t i = 0; i<memorydepth; i++){
    wvf[i]=0;
  }
  peak = 0;
  peakpos = 0;
  charge = 0;
  //channel=0;
  charge_m;
  fprompt = 0;
  event = 0; 
  time = 0;
  selection = 0; 
  chan =0;
}
