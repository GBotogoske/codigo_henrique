// ________________________________________ //
// Author: Henrique Souza
// Filename: adc_read_all_data.C
// Created: 2021
// ________________________________________ //
#define memorydepth 1000
#include "../MYCODES.h"


void adc_read_all_data()
{
    //delete files.log
    system("rm files.log");
    
    //create a new file files.log, with a list of binaries in the folder
    system("ls -1 *.dat| sed -e 's/.dat$//' > files.log");
    
    //create a instance of the class read
    //more information in script: readingCodes.C
    Read r;
         
    //define the sample time in ns    
    r.dtime = 2; // step time in ns
    
    r.nbits = 14; // this is used only for baseline histogram, changing it to 12 might help
    
    //files are saved in binary?
    r.isBinary = true;
    
    //->FALSE: save the data signals in ADC_CHANNEL 
    //->TRUE: save the data signals in mV (set the wavedump baseline)
    r.ADC_channel = false;
    
    //baseline setted in wavedump config file
    r.wavedump_baseline = 10;
   
    //max time to calculate the baseline (ns)
    r.baselineTime = 300; // time limit for baseline
    
    //max time to integrate the sinal(ns), to calculate the charge
    r.chargeTime = 1000; // last time to integrate
    
    //last time (ns) to integrate the fast signal
    r.chargeTime_menor = 1000;
    
    //time to start (ns) to integrate the signal
    r.startCharge = 600;
    
    //last time(ns) to search for amplitude peak
    r.maxRange = 12000; // max range to search for amplitude peak
    
    //time limit to get the fast component (start_charge+fast)
    r.fast = 200; // fprompt fast integration time
    //time limit to get the slow component (start_charge+slow)
    r.slow = 1700; //fprompt slow integration time
    
    //above this, it wont be considerate to calculate the baseline
    r.exclusion_baseline = 35; // filtered waveform, anything above here will do +exclusion window while evaluating the baseline
    // r.exclusion_baseline = {35,15};
    
    //if something above exclusion_baseline is find, it will jump this time (ns)
    r.exclusion_window = 500; // time in ns that it will jump for baseline
    
    //size of denoise filter
    r.filter = 14;//14; // denoise filter.. if filter = 0, no denoise is done.
    
    //save the denoise_filter
    r.saveFilter = true;
    
    //only some events?  
    r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely (you can set stopEvent)
    //how many?
    r.stopEvent = 100; //
    
    //you want to calculate baseline:
    //FALSE -> YES
    //TRUE -> NO
    r.noBaseline = false; // if you dont want to calculate baseline
  
    // r.channels = {1};
    r.channels = {0,1};
      
    //START
    
    r.adc_read_all_data();    
}

