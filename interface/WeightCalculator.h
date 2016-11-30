#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TSystem.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include <TLorentzVector.h>
using namespace std;


float WScaleFactor=1.0; // computed on Apr 20th
//float TTScaleFactor=0.91;
//float WScaleFactor=1.22;
//float TTScaleFactor=0.856;


float XSection(std::string OutName) {
    
    
    ////////////////////////////////////////////////////////////////
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
    
    ////////////////////////////////////////////////////////////////
    
    //    https://docs.google.com/spreadsheets/d/1rWM3AlFKO8IJVaeoQkWZYWwSvicQ1QCXYSzH74QyZqE/edit?alt=json#gid=398123591
    
    
    //WJet       float XSection_W[numBG] = {30400, 5400, 1750, 519, 214};
    //     if (OutName.find("WJetsToLNu") != string::npos) return 50690;
    if (OutName.find("WJetsToLNu_Inc") != string::npos) return 50690; //61526.7;
    else if (OutName.find("WJetsToLNu_HT-100To200") != string::npos) return 1345;
    else if (OutName.find("WJetsToLNu_HT-200To400") != string::npos) return 359.7;
    else if (OutName.find("WJetsToLNu_HT-400To600") != string::npos) return 48.91;
    //    else if (OutName.find("WJetsToLNu_HT-600ToInf") != string::npos) return 18.77;
    else if (OutName.find("WJetsToLNu_HT-600To800") != string::npos) return 12.05;
    else if (OutName.find("WJetsToLNu_HT-800To1200") != string::npos) return 5.501;
    else if (OutName.find("WJetsToLNu_HT-1200To2500") != string::npos) return 1.329;
    else if (OutName.find("WJetsToLNu_HT-2500ToInf") != string::npos) return 0.03216;
    
    
    //DYJets   float XSection_DY[numBG] = {2950, 561, 181, 51.1, 23};
    else if (OutName.find("DYJetsToLL_Inc") != string::npos) return 4895 * 1.012; //based on comment from Francesco madgraphMLM it should be 6099 pb
    else if (OutName.find("DYJetsToLL_M-50_HT-100to200") != string::npos) return 147.4;
    else if (OutName.find("DYJetsToLL_M-50_HT-200to400") != string::npos) return 40.99;
    else if (OutName.find("DYJetsToLL_M-50_HT-400to600") != string::npos) return 5.678;
    else if (OutName.find("DYJetsToLL_M-50_HT-600toInf") != string::npos) return 2.198;
    
    //Di-boson
    else if (OutName.find("WW") != string::npos) return 115.0;
    else if (OutName.find("WZ") != string::npos) return 47.13;
    else if (OutName.find("ZZ") != string::npos) return 16.523;
    
    //SingleTop
    else if (OutName.find("ST_t-channel_antitop_4f_leptonDecays") != string::npos) return 80.95 * 0.108*3;
    else if (OutName.find("ST_t-channel_top_4f_leptonDecays") != string::npos) return 136.02 * 0.108*3;
    else if (OutName.find("ST_tW_antitop_5f_inclusiveDecays") != string::npos) return 35.6;
    else if (OutName.find("ST_tW_top_5f_inclusiveDecays") != string::npos) return 35.6;
    
    
    //    else if (OutName.find("TTJets_DiLept") != string::npos) return (89.05);
    //    else if (OutName.find("TTJets_SingleLeptFromT") != string::npos) return (183.46);
    //    else if (OutName.find("TTJets_SingleLeptFromTbar") != string::npos) return (183.46);
    
    //    else if (OutName.find("TTJets_DiLept_Ext") != string::npos) return (89.05);
    //    else if (OutName.find("TTJets_SingleLeptFromT_Ext") != string::npos) return (183.46);
    //    else if (OutName.find("TTJets_SingleLeptFromTbar_Ext") != string::npos) return (183.46);
    
    //    else if (OutName.find("Inclusive_TTJets") != string::npos) return (831.76);
    //    else if (OutName.find("NewTT") != string::npos) return (831.76);
    
    else if (OutName.find("Inclusive_TTJets") != string::npos) return (831.76);
    //    else if (OutName.find("NewTT") != string::npos) return (831.76 * 0.9);
    
    //    //    https://twiki.cern.ch/twiki/bin/view/CMS/Exo2015LQ1AndLQ2Analyses
    //    else if (OutName.find("skimed_lq200") != string::npos) return 60.6;
    //    else if (OutName.find("skimed_lq250") != string::npos) return 20.3;
    //    else if (OutName.find("skimed_lq300") != string::npos) return     8.05E+00;
    //    else if (OutName.find("skimed_lq350") != string::npos) return     3.58E+00;
    //    else if (OutName.find("skimed_lq400") != string::npos) return     1.74E+00;
    //    else if (OutName.find("skimed_lq450") != string::npos) return     9.05E-01;
    //    else if (OutName.find("skimed_lq500") != string::npos) return     4.96E-01;
    //    else if (OutName.find("skimed_lq550") != string::npos) return     2.84E-01;
    //    else if (OutName.find("skimed_lq600") != string::npos) return     1.69E-01;
    //    else if (OutName.find("skimed_lq650") != string::npos) return     1.03E-01;
    //    else if (OutName.find("skimed_lq700") != string::npos) return     6.48E-02;
    //    else if (OutName.find("skimed_lq750") != string::npos) return     4.16E-02;
    //    else if (OutName.find("skimed_lq800") != string::npos) return     2.73E-02;
    //    else if (OutName.find("skimed_lq850") != string::npos) return     1.82E-02;
    //    else if (OutName.find("skimed_lq900") != string::npos) return     1.23E-02;
    //    else if (OutName.find("skimed_lq950") != string::npos) return     8.45E-03;
    //    else if (OutName.find("skimed_lq1000") != string::npos) return     5.86E-03;
    //    else if (OutName.find("skimed_lq1050") != string::npos) return     4.11E-03;
    //    else if (OutName.find("skimed_lq1100") != string::npos) return     2.91E-03;
    //    else if (OutName.find("skimed_lq1150") != string::npos) return     2.08E-03;
    //    else if (OutName.find("skimed_lq1200") != string::npos) return     1.50E-03;
    //    else if (OutName.find("skimed_lq1250") != string::npos) return     1.09E-03;
    //    else if (OutName.find("skimed_lq1300") != string::npos) return     7.95E-04;
    //    else if (OutName.find("skimed_lq1350") != string::npos) return     5.85E-04;
    //    else if (OutName.find("skimed_lq1400") != string::npos) return     4.33E-04;
    //    else if (OutName.find("skimed_lq1450") != string::npos) return     3.21E-04;
    //    else if (OutName.find("skimed_lq1500") != string::npos) return     2.40E-04;
    //
    //
    //
    //
    //    else if (OutName.find("skimed_0_RHNu_1000-500") != string::npos) return      1.692E+00;
    //    else if (OutName.find("skimed_0_RHNu_1500-750") != string::npos) return      2.90E-01;
    //    else if (OutName.find("skimed_0_RHNu_2000-1000") != string::npos) return     6.563E-02;
    //    else if (OutName.find("skimed_0_RHNu_2500-1250") != string::npos) return     1.92E-02;
    //    else if (OutName.find("skimed_0_RHNu_3000-1500") != string::npos) return     6.030E-03;
    
    else if (OutName.find("LQToBTau_M") != string::npos) return     1.0;
    else if (OutName.find("HeavyW") != string::npos ) return      1.0;
    else if (OutName.find("Codex") != string::npos ) return      1.0;
    
    
    else if (OutName.find("QCD_Pt-20toInf_MuEnrichedPt15") != string::npos) return     720648000  * 0.00042 ;
    else if (OutName.find("LooseQCD") != string::npos) return     720648000  * 0.00042 ;
    else if (OutName.find("QCD") != string::npos) return     720648000  * 0.00042 ;
    
    
    //
    //    else if (OutName.find("QCD_Pt-15to20") != string::npos) return  3819570.0   ;  //21580
    //    else if (OutName.find("QCD_Pt-20to30") != string::npos) return  2960198.4   ;  // 1912
    //    else if (OutName.find("QCD_Pt-30to50") != string::npos) return  1652471.46 ;  //  1067
    ////    else if (OutName.find("QCD_Pt-50to80") != string::npos) return 437504.1     ;
    //    else if (OutName.find("QCD_Pt-80to120") != string::npos) return  106033.6648   ;   // event weight 130
    //    else if (OutName.find("QCD_Pt-120to170") != string::npos) return  25190.51514    ;  //event weight 19
    //    else if (OutName.find("QCD_Pt-170to300") != string::npos) return   8654.49315    ; //event weight ~ 8
    //    else if (OutName.find("QCD_Pt-300to470") != string::npos) return    797.35269   ;   // event weight ~ 3
    //    else if (OutName.find("QCD_Pt-470to600") != string::npos) return     79.02553776 ;  // event weight ~ .4
    //    else if (OutName.find("QCD_Pt-600to800") != string::npos) return     25.09505908 ;
    //    else if (OutName.find("QCD_Pt-800to1000") != string::npos) return     4.707368272;
    //    else if (OutName.find("QCD_Pt-1000toInf") != string::npos) return     1.62131692;
    //
    
    
    
    //    W + Jets, W -> lv MG5 (NLO) 	<4 	/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 	SMP-RunIISpring15DR74-00002 	61526.7+497.1-264.6 ±2312.7 pb 	NNLO
    //
    
    
    else if (OutName.find("WJetsToLNu_FXFX") != string::npos) return  61526.7   ;
    else if (OutName.find("WJetsToLNu_Pt-50To100_FXFX") != string::npos) return  8053   ;
    else if (OutName.find("WJetsToLNu_Pt-100To250_FXFX") != string::npos) return  676.3   ;
    else if (OutName.find("WJetsToLNu_Pt-250To400_FXFX") != string::npos) return  23.94   ;
    else if (OutName.find("WJetsToLNu_Pt-400To600_FXFX") != string::npos) return  3.031   ;
    else if (OutName.find("WJetsToLNu_Pt-600ToInf_FXFX") != string::npos) return  0.4524   ;
    
    
    
    
    
    else {
        cout<<"Not Listed in XSection menu !!!! Watch cout    "<<OutName<< "\n";
        return 1;
    }
}




vector <float> W_HTBin(std::string FileLoc){
    
    const int WSize=8;
    std::string W_ROOTFiles[WSize]={"WJetsToLNu_Inc.root", "WJetsToLNu_HT-100To200.root","WJetsToLNu_HT-200To400.root","WJetsToLNu_HT-400To600.root", "WJetsToLNu_HT-600To800.root","WJetsToLNu_HT-800To1200.root","WJetsToLNu_HT-1200To2500.root","WJetsToLNu_HT-2500ToInf.root"};
    
    vector<float> W_events;
    W_events.clear();
    
    for (int i=0; i <WSize;i++){
        
        TFile * File_W = TFile::Open((FileLoc+W_ROOTFiles[i]).c_str());
        TH1F * Histo_W = (TH1F*) File_W->Get("hcount");
        W_events.push_back(Histo_W->GetBinContent(2));
        cout<<"Number of proccessed evenets for "<<W_ROOTFiles[i]<<" = "<<Histo_W->GetBinContent(2)<<"\n";
    }
    
    return W_events ;
    
}

vector <float> DY_HTBin(std::string FileLoc){
    
    const int DYSize=5;
    std::string DY_ROOTFiles[DYSize]={"DYJetsToLL_M-50_Inc.root", "DYJetsToLL_M-50_HT-100to200.root","DYJetsToLL_M-50_HT-200to400.root","DYJetsToLL_M-50_HT-400to600.root", "DYJetsToLL_M-50_HT-600toInf.root"};
    
    vector<float> DY_events;
    DY_events.clear();
    
    for (int i=0; i < DYSize;i++){
        
        TFile * File_DY = TFile::Open((FileLoc+DY_ROOTFiles[i]).c_str());
        TH1F * Histo_DY= (TH1F*) File_DY->Get("hcount");
        DY_events.push_back(Histo_DY->GetBinContent(2));
        cout<<"Number of proccessed evenets for "<<DY_ROOTFiles[i]<<" = "<<Histo_DY->GetBinContent(2)<<"\n";
    }
    
    return DY_events ;
    
}



vector <float> W_PTBinNLO(std::string FileLoc){
    
    const int WSizeNLO=5;
    std::string W_ROOTFilesNLO[WSizeNLO]={"DIR/WJetsToLNu_FXFX.root", "DIR/WJetsToLNu_Pt-100To250_FXFX.root","DIR/WJetsToLNu_Pt-250To400_FXFX.root","DIR/WJetsToLNu_Pt-400To600_FXFX.root", "DIR/WJetsToLNu_Pt-600ToInf_FXFX.root"};
    
    vector<float> W_eventsNLO;
    W_eventsNLO.clear();
    
    for (int i=0; i <WSizeNLO;i++){
        
        TFile * File_WNLO = TFile::Open((FileLoc+W_ROOTFilesNLO[i]).c_str());
        TH1F * Histo_WNLO = (TH1F*) File_WNLO->Get("hcount");
        W_eventsNLO.push_back(Histo_WNLO->GetBinContent(2));
        cout<<"Number of proccessed evenets for "<<W_ROOTFilesNLO[i]<<" = "<<Histo_WNLO->GetBinContent(2)<<"\n";
    }
    
    return W_eventsNLO ;
    
}











float weightCalc(TH1F *Histo,std::string outputName , float genHT , float WPt, vector<float> W_events, vector<float> DY_events, vector<float> W_eventsNLO) {
    
    //    cout<< "outputName is "<<outputName << "  and histoname is " <<Histo->GetName()<<  " Histo->GetBinContent(1)="<<Histo->GetBinContent(1)<< " XSection(wjet)=" <<XSection("WJets")<<"\n";
    
    
    stringstream ss(outputName);
    
    string token;
    string M;
    while (getline(ss,token, '/'))  M=token;
    
    std::string FirstPart = "";
    std::string LastPart = ".root";
    std::string newOut = M.substr(FirstPart.size());
    newOut = newOut.substr(0, newOut.size() - LastPart.size());
    //    cout<<"--->  Check Name is "<<newOut<<"\n";
    
    
//    float LOtoNLO_DY = 1.230888662;
    float LOtoNLO_DY = 1; // Now we boson have pt dependent SF
//    float LOtoNLO_W = 1.213783784;
    float LOtoNLO_W = 1;  // Now we boson have pt dependent SF
    //    float luminosity=2154;
    //    float luminosity=    3990;
    //    float luminosity=    6260;
    //    float luminosity=    9235;
    float luminosity=    12900;
    
    size_t isSingleMu = outputName.find("SingleMu");
    size_t isSingleEle = outputName.find("SingleEle");
    
    size_t isWjet = outputName.find("WJets");
    size_t isFXFX = outputName.find("FXFX");
    size_t isDYJet = outputName.find("DYJets");
    
    
    if (isSingleMu != string::npos || isSingleEle!= string::npos)   return 1;
    
    else if (isWjet != string::npos && isFXFX==string::npos) {
        
        //        return luminosity*LOtoNLO_W / (W_events[0] / XSection("WJetsToLNu_Inc"));
        
        
        if (genHT <= 100) return luminosity*LOtoNLO_W / (W_events[0] / XSection("WJetsToLNu_Inc"));
        
        else if (genHT > 100 && genHT <= 200) return  luminosity*LOtoNLO_W / (W_events[1] / XSection("WJetsToLNu_HT-100To200") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else if (genHT > 200 && genHT <=400) return   luminosity*LOtoNLO_W / (W_events[2] / XSection("WJetsToLNu_HT-200To400") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else if (genHT > 400 && genHT <=600) return   luminosity*LOtoNLO_W / (W_events[3] / XSection("WJetsToLNu_HT-400To600") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else if (genHT > 600 && genHT <=800) return   luminosity*LOtoNLO_W / (W_events[4] / XSection("WJetsToLNu_HT-600To800") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else if (genHT > 800 && genHT <=1200) return  luminosity*LOtoNLO_W / (W_events[5] / XSection("WJetsToLNu_HT-800To1200") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else if (genHT > 1200 && genHT <=2500) return luminosity*LOtoNLO_W / (W_events[6] / XSection("WJetsToLNu_HT-1200To2500") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else if (genHT > 2500) return                 luminosity*LOtoNLO_W / (W_events[7] / XSection("WJetsToLNu_HT-2500ToInf") + W_events[0] / XSection("WJetsToLNu_Inc"));
        else   {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
        
        
    }
    
    
    
    
//    else if (isWjet != string::npos && isFXFX != string::npos) {
//        
//        if (WPt <= 100)                  return   luminosity / (W_eventsNLO[0] / XSection("WJetsToLNu_FXFX"));
//        else if (WPt > 100 && WPt <= 250)return   luminosity / (W_eventsNLO[1] / XSection("WJetsToLNu_Pt-100To250_FXFX") + W_eventsNLO[0] / XSection("WJetsToLNu_FXFX"));
//        else if (WPt > 250 && WPt <=400) return   luminosity / (W_eventsNLO[2] / XSection("WJetsToLNu_Pt-250To400_FXFX") + W_eventsNLO[0] / XSection("WJetsToLNu_FXFX"));
//        else if (WPt > 400 && WPt <=600) return   luminosity / (W_eventsNLO[3] / XSection("WJetsToLNu_Pt-400To600_FXFX") + W_eventsNLO[0] / XSection("WJetsToLNu_FXFX"));
//        else if (WPt > 600 )               return   luminosity / (W_eventsNLO[4] / XSection("WJetsToLNu_Pt-600ToInf_FXFX") + W_eventsNLO[0] / XSection("WJetsToLNu_FXFX"));
//        
//        else   {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
//        
//    }
    
    
    
    else if (isDYJet != string::npos) {
        
        if (genHT <= 100) return luminosity*LOtoNLO_DY / (DY_events[0] / XSection("DYJetsToLL_Inc"));
        else if (genHT > 100 && genHT <= 200) return  luminosity*LOtoNLO_DY / (DY_events[1] / XSection("DYJetsToLL_M-50_HT-100to200") + DY_events[0] / XSection("DYJetsToLL_Inc"));
        else if (genHT > 200 && genHT <=400) return luminosity*LOtoNLO_DY / (DY_events[2] / XSection("DYJetsToLL_M-50_HT-200to400") + DY_events[0] / XSection("DYJetsToLL_Inc"));
        else if (genHT > 400 && genHT <=600) return luminosity*LOtoNLO_DY / (DY_events[3] / XSection("DYJetsToLL_M-50_HT-400to600") + DY_events[0] / XSection("DYJetsToLL_Inc"));
        else if (genHT > 600) return luminosity*LOtoNLO_DY / (DY_events[4] / XSection("DYJetsToLL_M-50_HT-600toInf") + DY_events[0] / XSection("DYJetsToLL_Inc"));
        else   {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
    }
    
    
    else
        return luminosity * XSection(newOut)*1.0 / Histo->GetBinContent(2);
    
    
}














