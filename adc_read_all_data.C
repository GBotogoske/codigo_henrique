// ________________________________________ //
// Author: Henrique Souza
// Filename: adc_read_all_data.C
// Created: 2021
// ________________________________________ //
#define memorydepth 6000
#include "../MYCODES.h"


void adc_read_all_data(){
    
    system("rm files.log");
    system("ls -1 *.dat| sed -e 's/.dat$//' > files.log");
    
    Read r;
        
    r.dtime = 4; // step time in ns
    r.nbits = 14; // this is used only for baseline histogram, changing it to 12 might help
    r.isBinary = true;
    r.ADC_channel = false;
    r.wavedump_baseline = 10;
    
    r.baselineTime = 4000; // time limit for baseline
    r.chargeTime = 14000; // last time to integrate
    r.chargeTime_menor = 8000;
    r.startCharge = 4000;
    r.maxRange = 24000; // max range to search for amplitude peak
    r.fast = 200; // fprompt fast integration time
    r.slow = 1700; //fprompt slow integration time
    r.exclusion_baseline = 35; // filtered waveform, anything above here will do +exclusion window while evaluating the baseline
    // r.exclusion_baseline = {35,15};
    r.exclusion_window = 1000; // time in ns that it will jump for baseline
    r.filter = 14;//14; // denoise filter.. if filter = 0, no denoise is done.
    r.OnlyOneEvent = false; // Do you want only one event? Choose it wisely (you can set stopEvent)
    r.stopEvent = 100; //
    r.noBaseline = false; // if you dont want to calculate baseline
    r.saveFilter = true;

    // r.channels = {1};
    //r.channels = {1,2,3,4};
      

    
    r.adc_read_all_data();

    
}

