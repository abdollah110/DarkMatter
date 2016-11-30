#include "../interface/LQ3Analyzer.h"
#include "../interface/WeightCalculator.h"
#include "../interface/Corrector.h"
#include <string>
#include <ostream>


int main(int argc, char** argv) {
    using namespace std;
    
    std::string out = *(argv + 1);
 
    std::string SampleLoc=getenv("SampleLoc");
    std::string RootName_Prefix=getenv("RootName_Prefix"); 
    std::string Working_Dir=getenv("Working_Dir");
    std::string Input_Loc=getenv("Input_Loc");

   
    cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;     //PRINTING THE OUTPUT name
    TFile *fout = TFile::Open(out.c_str(), "RECREATE");
    
    myMap1 = new std::map<std::string, TH1F*>();
    myMap2 = new map<string, TH2F*>();
    
    std::vector<string> input;
    std::string tmp_name="";

    for (int f = 2; f < argc; f++) {
    
        if(Input_Loc=="AFS")
        tmp_name=RootName_Prefix + *(argv+f);
        else if(Input_Loc=="EOS")
        tmp_name=RootName_Prefix+SampleLoc+"/"+ *(argv+f);
        else std::cout<<"There is no valid name for location of input files"<<std::endl;

        input.push_back(tmp_name);
        cout << "\n INPUT NAME IS:   " << input[f - 2] << "\n";
    }
    
    
    TFile * PUData= TFile::Open((Working_Dir+"/"+"../interface/pileup-hists/MyDataPileupHistogram2016.root").c_str());
    TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
    HistoPUData->Scale(1.0/HistoPUData->Integral());
    
    TFile * PUMC= TFile::Open((Working_Dir+"/"+"../interface/pileup-hists/Sprin16_MC.root").c_str());
    TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
    HistoPUMC->Scale(1.0/HistoPUMC->Integral());
    
    TFile * MuCorrId= TFile::Open((Working_Dir+"/"+"../interface/pileup-hists/MuonID_Z_RunBCD_prompt80X_7p65.root").c_str());
    //    TFile * MuCorrId= TFile::Open("../interface/pileup-hists/MuonID_Z_2016runB_2p6fb.root");
    TH2F * HistoMuId= (TH2F *) MuCorrId->Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
    
    TFile * MuCorrIso= TFile::Open((Working_Dir+"/"+"../interface/pileup-hists/MuonIso_Z_RunBCD_prompt80X_7p65.root").c_str());
    //    TFile * MuCorrIso= TFile::Open("../interface/pileup-hists/MuonISO_Z_2016runB_2p6fb.root");
    TH2F * HistoMuIso= (TH2F *) MuCorrIso->Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
    
    TFile * MuCorrTrg= TFile::Open((Working_Dir+"/"+"../interface/pileup-hists/SingleMuonTrigger_Z_RunBCD_prompt80X_7p65.root").c_str());
    //    TFile * MuCorrTrg= TFile::Open("../interface/pileup-hists/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root");
    TH2F * HistoMuTrg= (TH2F *) MuCorrTrg->Get("Mu45_eta2p1_PtEtaBins_Run274094_to_276097/efficienciesDATA/pt_abseta_DATA");
    
    
    
    TFile * KFactor= TFile::Open((Working_Dir+"/"+"../interface/pileup-hists/kfactors.root").c_str());
    TH1F * WLO= (TH1F *) KFactor->Get("WJets_LO/inv_pt");
    TH1F * WNLO= (TH1F *) KFactor->Get("EWKcorr/W");
    TH1F * ZLO= (TH1F *) KFactor->Get("ZJets_LO/inv_pt");
    TH1F * ZNLO= (TH1F *) KFactor->Get("EWKcorr/Z");
    
    
    
    for (int k = 0; k < input.size(); k++) {
        
        //std::string input = *(argv + 2);
        TFile *f_Double = TFile::Open(input[k].c_str());
        cout << "\n  Now is running on ------->   " << std::string(f_Double->GetName()) << "\n";
        
        std::string InputROOT= std::string(f_Double->GetName());
        TFile * myFile = TFile::Open(f_Double->GetName());
        TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
        
        //        TTree *Run_Tree = (TTree*) f_Double->Get("ggNtuplizer/EventTree");
        TTree *Run_Tree = (TTree*) f_Double->Get("EventTree");
        
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(6);
        
        
        std::string ROOTLoc= RootName_Prefix+SampleLoc+"/";
        vector<float> DY_Events = DY_HTBin(ROOTLoc);
        vector<float> W_Events = W_HTBin(ROOTLoc);
        //        vector<float> W_EventsNLO = W_PTBinNLO(ROOTLoc); //This is for the NLO samples (as the stat is too low we do not use them)
        vector<float> W_EventsNLO = W_HTBin(ROOTLoc);
        
        /////////////////////////   General Info
        Run_Tree->SetBranchAddress("isData", &isData);
        Run_Tree->SetBranchAddress("run", &run);
        Run_Tree->SetBranchAddress("lumis", &lumis);
        Run_Tree->SetBranchAddress("event", &event);
        Run_Tree->SetBranchAddress("genWeight",&genWeight);
        Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
        Run_Tree->SetBranchAddress("puTrue", &puTrue);
        Run_Tree->SetBranchAddress("nVtx",&nVtx);
        
        /////////////////////////   MC Info
        Run_Tree->SetBranchAddress("nMC", &nMC);
        Run_Tree->SetBranchAddress("mcPID", &mcPID);
        Run_Tree->SetBranchAddress("mcStatus", &mcStatus);
        Run_Tree->SetBranchAddress("mcPt", &mcPt );
        Run_Tree->SetBranchAddress("mcEta", &mcEta );
        Run_Tree->SetBranchAddress("mcPhi", &mcPhi );
        Run_Tree->SetBranchAddress("mcE", &mcE );
        Run_Tree->SetBranchAddress("mcMass", &mcMass );
        Run_Tree->SetBranchAddress("mcMomPID", &mcMomPID );
        Run_Tree->SetBranchAddress("mcGMomPID", &mcGMomPID );
        
        
        
        /////////////////////////   Tau Info
        Run_Tree->SetBranchAddress("nTau", &nTau);
        Run_Tree->SetBranchAddress("tauPt"  ,&tauPt);
        Run_Tree->SetBranchAddress("tauEta"  ,&tauEta);
        Run_Tree->SetBranchAddress("tauPhi"  ,&tauPhi);
        Run_Tree->SetBranchAddress("tauMass"  ,&tauMass);
        Run_Tree->SetBranchAddress("tauCharge"  ,&tauCharge);
        Run_Tree->SetBranchAddress("taupfTausDiscriminationByDecayModeFinding", &taupfTausDiscriminationByDecayModeFinding);
        Run_Tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
        Run_Tree->SetBranchAddress("tauByLooseMuonRejection3", &tauByLooseMuonRejection3);
        Run_Tree->SetBranchAddress("tauByMVA6MediumElectronRejection"  ,&tauByMVA6MediumElectronRejection);
        Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
        Run_Tree->SetBranchAddress("tauByMediumCombinedIsolationDeltaBetaCorr3Hits",&tauByMediumCombinedIsolationDeltaBetaCorr3Hits);
        Run_Tree->SetBranchAddress("tauByMVA6LooseElectronRejection", &tauByMVA6LooseElectronRejection);
        Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
        Run_Tree->SetBranchAddress("tauDecayMode",&tauDecayMode);
        Run_Tree->SetBranchAddress("tauByLooseIsolationMVArun2v1DBoldDMwLT",&tauByLooseIsolationMVArun2v1DBoldDMwLT);
        Run_Tree->SetBranchAddress("tauByVLooseIsolationMVArun2v1DBoldDMwLT",&tauByVLooseIsolationMVArun2v1DBoldDMwLT);
        /////////////////////////   Mu Info
        Run_Tree->SetBranchAddress("nMu", &nMu);
        Run_Tree->SetBranchAddress("muPt"  ,&muPt);
        Run_Tree->SetBranchAddress("muEta"  ,&muEta);
        Run_Tree->SetBranchAddress("muPhi"  ,&muPhi);
        Run_Tree->SetBranchAddress("muIsoTrk", &muIsoTrk);
        Run_Tree->SetBranchAddress("muCharge",&muCharge);
        Run_Tree->SetBranchAddress("muIsTightID",&muIsTightID);
        Run_Tree->SetBranchAddress("muIsMediumID",&muIsMediumID);
        Run_Tree->SetBranchAddress("muIsLooseID",&muIsLooseID);
        Run_Tree->SetBranchAddress("muPFChIso", &muPFChIso);
        Run_Tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
        Run_Tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
        Run_Tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
        Run_Tree->SetBranchAddress("muD0",&muD0);
        Run_Tree->SetBranchAddress("muDz",&muDz);
        
        /////////////////////////   Ele Info
        Run_Tree->SetBranchAddress("nEle", &nEle);
        Run_Tree->SetBranchAddress("elePt"  ,&elePt);
        Run_Tree->SetBranchAddress("eleEta"  ,&eleEta);
        Run_Tree->SetBranchAddress("elePhi"  ,&elePhi);
        Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
        Run_Tree->SetBranchAddress("eleIDMVANonTrg", &eleIDMVANonTrg);
        Run_Tree->SetBranchAddress("eleCharge",&eleCharge);
        Run_Tree->SetBranchAddress("eleSCEta",&eleSCEta);
        Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
        Run_Tree->SetBranchAddress("elePFPhoIso", &elePFPhoIso);
        Run_Tree->SetBranchAddress("elePFNeuIso", &elePFNeuIso);
        Run_Tree->SetBranchAddress("elePFPUIso", &elePFPUIso);
        Run_Tree->SetBranchAddress("eleD0",&eleD0);
        Run_Tree->SetBranchAddress("eleDz",&eleDz);
        Run_Tree->SetBranchAddress("eleMissHits", &eleMissHits);
        Run_Tree->SetBranchAddress("eleConvVeto", &eleConvVeto);
        Run_Tree->SetBranchAddress("eleSCEta", &eleSCEta );
        
        /////////////////////////   Jet Info
        Run_Tree->SetBranchAddress("nJet",&nJet);
        Run_Tree->SetBranchAddress("jetPt",&jetPt);
        Run_Tree->SetBranchAddress("jetEta",&jetEta);
        Run_Tree->SetBranchAddress("jetPhi",&jetPhi);
        Run_Tree->SetBranchAddress("jetEn",&jetEn);
        Run_Tree->SetBranchAddress("jetpfCombinedInclusiveSecondaryVertexV2BJetTags",&jetpfCombinedInclusiveSecondaryVertexV2BJetTags);
        Run_Tree->SetBranchAddress("jetPFLooseId",&jetPFLooseId);
        Run_Tree->SetBranchAddress("jetPUidFullDiscriminant",&jetPUidFullDiscriminant);
        Run_Tree->SetBranchAddress("jetRawPt",&jetRawPt);
        Run_Tree->SetBranchAddress("jetJECUnc",&jetJECUnc);
        Run_Tree->SetBranchAddress("jetRawEn",&jetRawEn);
        Run_Tree->SetBranchAddress("jetHadFlvr",&jetHadFlvr);
        /////////////////////////   MET Info
        Run_Tree->SetBranchAddress("pfMET",&pfMET);
        Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
        Run_Tree->SetBranchAddress("metFilters",&metFilters);
        Run_Tree->SetBranchAddress("genHT",&genHT);
        
        
        //###############################################################################################
        //  Weight Calculation
        //###############################################################################################
        float WSCALEFACTORE=1.00;  //measured July 4th from WEstimaOutPut/_16_80X
        float WSF_mutau=1.0;
        float WSF_etau=1.0;
        float MuMass= 0.10565837;
        float eleMass= 0.000511;
        float LeptonPtCut_=60;
        float TauPtCut_=20;
        float JetPtCut=100; // used to be 50
        float BJetPtCut=20;
        float ElectronPtCut_=15;
        float CSVCut= 0.935 ;    // loose is 0.460                 //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation74X
        float LeptonIsoCut=0.15;
        
        
        Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
        cout<<"nentries_wtn===="<<nentries_wtn<<"\n";
        for (Int_t i = 0; i < nentries_wtn; i++) {
            Run_Tree->GetEntry(i);
            if (i % 10000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
            fflush(stdout);
            
            if (isData && (metFilters!=0)) continue;
            std::vector<string> HistNamesFilled;
            HistNamesFilled.clear();
            //###############################################################################################
            //  Weight Calculation
            //###############################################################################################
            //###############################################################################################
            //############ Top Reweighting & W boson SCale Factor
            
            float GenTopPt=0;
            float GenAntiTopPt=0;
            float TopPtReweighting = 1;
            float WBosonPt=0;
            float WBosonKFactor=1;
            float ZBosonPt=0;
            float ZBosonKFactor=1;
            
            for (int igen=0;igen < nMC; igen++){
                if (mcPID->at(igen) == 6 && mcStatus->at(igen) ==62) GenTopPt=mcPt->at(igen) ;
                if (mcPID->at(igen) == -6 && mcStatus->at(igen) ==62) GenAntiTopPt=mcPt->at(igen);
                if (fabs(mcPID->at(igen)) ==24   && mcStatus->at(igen) ==22)  WBosonPt= mcPt->at(igen); // In inclusive we have status 62||22||44 while in HTbins we have just 22
                if (fabs(mcPID->at(igen)) ==23   && mcStatus->at(igen) ==22)  ZBosonPt= mcPt->at(igen);
            }
            size_t isTTJets = InputROOT.find("TTJets");
            if (isTTJets!= string::npos) TopPtReweighting = compTopPtWeight(GenTopPt, GenAntiTopPt);
            size_t isWJets = InputROOT.find("WJets");
            if (isWJets!= string::npos) WBosonKFactor=Get_W_Z_BosonKFactor(WBosonPt,WLO,WNLO);  //Swtch ON only for LO Madgraph sample
            size_t isDYJets = InputROOT.find("DYJets");
            if (isDYJets!= string::npos) ZBosonKFactor=Get_W_Z_BosonKFactor(ZBosonPt,ZLO,ZNLO);  //Swtch ON only for LO Madgraph sample
            
            //###############################################################################################
            float LumiWeight = 1;
            float GetGenWeight=1;
            float PUWeight = 1;
           

            if (!isData){
                
                if (HistoTot) LumiWeight = weightCalc(HistoTot, InputROOT, genHT,WBosonPt, W_Events, DY_Events,W_EventsNLO);
                GetGenWeight=genWeight;
                
                int puNUmmc=int(puTrue->at(0)*10);
                int puNUmdata=int(puTrue->at(0)*10);
                float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
                float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
                PUWeight= PUData_/PUMC_;
            }
            
            float TotalWeight = LumiWeight * GetGenWeight * PUWeight * TopPtReweighting * WBosonKFactor * ZBosonKFactor;
            //###############################################################################################
            //  Some Histogram Filling
            //###############################################################################################
            plotFill("WeightLumi",LumiWeight,10000,0,100);
            plotFill("TopPtReweighting",TopPtReweighting,100,0,10);
            plotFill("WeightPU",PUWeight,100,0,10);
            plotFill("WeightTotal",TotalWeight,50,0,5);
            plotFill("nVtx_NoPUCorr",nVtx,60,0,60);
            plotFill("nVtx_PUCorr",nVtx,60,0,60,PUWeight);
            plotFill("WBosonPt",WBosonPt,150,0,1500,PUWeight);
            
            for (int qq=0; qq < 60;qq++){
                if ((HLTEleMuX >> qq & 1) == 1)
                    plotFill("HLT",qq,60,0,60);
            }
            
           
            
            //###############################################################################################
            //  Doing MuTau Analysis
            //###############################################################################################
            //###########       Trigger Requirement ###########################################################
            const int size_trgCat = 1;
            bool PassTrigger=1;
            if (isData) PassTrigger = (HLTEleMuX >> 26 & 1) == 1; //  else if (name.find("HLT_Mu45_eta2p1_v") != string::npos) bitEleMuX = 26;
            
            if (! PassTrigger) continue;
            
            
            
            
            //###########       tau Veto   ###########################################################
            int numTau=0;
            for  (int itau=0 ; itau < nTau; itau++){
                
                if (tauPt->at(itau) < TauPtCut_  || fabs(tauEta->at(itau)) > 2.3 ) continue;
                
                bool TauIdIso =  taupfTausDiscriminationByDecayModeFinding->at(itau) > 0.5 && tauByLooseMuonRejection3->at(itau) > 0 && tauByMVA6LooseElectronRejection->at(itau) > 0 && tauByLooseIsolationMVArun2v1DBoldDMwLT->at(itau) > 0;
                
                if (!TauIdIso) continue;
                numTau++;
            }
            //###########       Ele Veto   ###########################################################
            int numElectron=0;
            for  (int jele=0 ; jele < nEle; jele++){
                
                if ( elePt->at(jele) < 15 || fabs(eleEta->at(jele)) > 2.5) continue;
                
                bool eleMVAIdExtra= false;
                if (fabs (eleSCEta->at(jele)) < 0.8 && eleIDMVANonTrg->at(jele) >  0.913286 ) eleMVAIdExtra= true;
                else if (fabs (eleSCEta->at(jele)) >  0.8 &&fabs (eleSCEta->at(jele)) <  1.5 && eleIDMVANonTrg->at(jele) >  0.805013 ) eleMVAIdExtra= true;
                else if ( fabs (eleSCEta->at(jele)) >  1.5 && eleIDMVANonTrg->at(jele) >  0.358969  ) eleMVAIdExtra= true;
                else eleMVAIdExtra= false;
                
                
                if (!(eleMVAIdExtra )) continue;
                numElectron++;
                
            }
            //###########       bJet Veto   ###########################################################
            int numBJet=0;
            for (int ijet= 0 ; ijet < nJet ; ijet++){
                if (jetPFLooseId->at(ijet) > 0.5 && jetPt->at(ijet) > BJetPtCut && fabs(jetEta->at(ijet)) < 2.4 && jetpfCombinedInclusiveSecondaryVertexV2BJetTags->at(ijet) >  CSVCut )
                    numBJet++;
            }
            
          
            
            //###########       Z boson Veto   ###########################################################
            int numZboson=0;
            if (nMu > 1){
                TLorentzVector Mu4Momentum_0,Mu4Momentum_1,Z4Momentum;
                Mu4Momentum_0.SetPtEtaPhiM(muPt->at(0),muEta->at(0),muPhi->at(0),MuMass);
                Mu4Momentum_1.SetPtEtaPhiM(muPt->at(1),muEta->at(1),muPhi->at(1),MuMass);
                Z4Momentum=Mu4Momentum_1+Mu4Momentum_0;
                
                float IsoMu1=muPFChIso->at(0)/muPt->at(0);
                if ( (muPFNeuIso->at(0) + muPFPhoIso->at(0) - 0.5* muPFPUIso->at(0) )  > 0.0)
                    IsoMu1= ( muPFChIso->at(0)/muPt->at(0) + muPFNeuIso->at(0) + muPFPhoIso->at(0) - 0.5* muPFPUIso->at(0))/muPt->at(0);
                
                float IsoMu2=muPFChIso->at(1)/muPt->at(1);
                if ( (muPFNeuIso->at(1) + muPFPhoIso->at(1) - 0.5* muPFPUIso->at(1) )  > 0.0)
                    IsoMu2= ( muPFChIso->at(1)/muPt->at(1) + muPFNeuIso->at(1) + muPFPhoIso->at(1) - 0.5* muPFPUIso->at(1))/muPt->at(1);
                
                if ( IsoMu1 < 0.25  && IsoMu2 < 0.25 && Z4Momentum.M() > 80 && Z4Momentum.M()< 100 ) numZboson++;
            }
            
            
            //############################################################################################
            //###########       Loop over MuJet events   #################################################
            //############################################################################################
            
            for  (int imu=0 ; imu < nMu; imu++){
                
                float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
                if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
                    IsoMu= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
                
                bool MuPtCut = muPt->at(imu) > LeptonPtCut_ && fabs(muEta->at(imu)) < 2.1 ;
                bool MuIdIso=(muIsTightID->at(imu) > 0  && fabs(muD0->at(imu)) < 0.045 && fabs(muDz->at(imu)) < 0.2);
                
                if (! MuPtCut || !MuIdIso ) continue;
                
                
                float LepCor=getCorrFactorMuon74X(isData,  muPt->at(imu), muEta->at(imu) , HistoMuId,HistoMuIso,HistoMuTrg);
                
                
                TLorentzVector Mu4Momentum, Tau4Momentum, Jet4Momentum,ExtraMu4Momentum, ExtraEle4Momentum,KJet4Momentum,LQ;
                Mu4Momentum.SetPtEtaPhiM(muPt->at(imu),muEta->at(imu),muPhi->at(imu),MuMass);
                
                
                //###########      Finding the close jet near mu   ###########################################################
                
                float CLoseJetMuPt=muPt->at(imu);
                float CLoseJetMuEta=muEta->at(imu);
                
                if (MuPtCut && MuIdIso ){
                    
                    double Refer_R_jetmu = 5;
                    
                    for (int kjet= 0 ; kjet < nJet ; kjet++){
                        KJet4Momentum.SetPtEtaPhiE(jetPt->at(kjet),jetEta->at(kjet),jetPhi->at(kjet),jetEn->at(kjet));
                        
                        
                        if (KJet4Momentum.DeltaR(Mu4Momentum) < Refer_R_jetmu) {
                            Refer_R_jetmu = KJet4Momentum.DeltaR(Mu4Momentum);
                            if (Refer_R_jetmu < 0.5 && jetPt->at(kjet)  >= muPt->at(imu)) {
                                CLoseJetMuPt = jetPt->at(kjet);
                                CLoseJetMuEta = jetEta->at(kjet);
                                
                            }
                        }
                    }
                }
                
                //###########    loop over  Jet    ###########################################################
                
                for (int ijet= 0 ; ijet < nJet ; ijet++){
                    
                    Jet4Momentum.SetPtEtaPhiE(jetPt->at(ijet),jetEta->at(ijet),jetPhi->at(ijet),jetEn->at(ijet));
                    
                    
                    bool goodJet = (jetPFLooseId->at(ijet) > 0.5 && jetPt->at(ijet) > JetPtCut && fabs(jetEta->at(ijet)) < 2.4 && Jet4Momentum.DeltaR(Mu4Momentum) > 0.5);
                    if (! goodJet) continue;
                    
                    LQ=Jet4Momentum + Mu4Momentum;
                    
                    //###############################################################################################
                    //  Isolation Categorization
                    //###############################################################################################
                    //###############################################################################################
                    bool LepPassIsolation= IsoMu < LeptonIsoCut;
                    
                    const int size_isoCat = 3;
                    bool Isolation = LepPassIsolation;
                    bool AntiIsolation =  !LepPassIsolation;
                    bool Total = 1;
                    
                    bool Iso_category[size_isoCat] = {Isolation, AntiIsolation,Total};
                    std::string iso_Cat[size_isoCat] = {"_Iso", "_AntiIso","_Total"};
                    //###############################################################################################
                    //  MT Categorization
                    //###############################################################################################
                    float tmass_MuMet= TMass_F(muPt->at(imu), muPt->at(imu)*cos(muPhi->at(imu)),muPt->at(imu)*sin(muPhi->at(imu)) , pfMET, pfMETPhi);
                    
                    const int size_mTCat = 3;
                    bool NoMT = 1;
                    bool LoWMT = (tmass_MuMet < 40);
                    bool HighMT = (tmass_MuMet > 100);
                    
                    bool MT_category[size_mTCat] = {NoMT,LoWMT,HighMT};
                    std::string MT_Cat[size_mTCat] = {"_NoMT", "_LowMT","_HighMT"};
                    
                    float tmass_JetMet= TMass_F(jetPt->at(ijet), jetPt->at(ijet)*cos(jetPhi->at(ijet)),jetPt->at(ijet)*sin(jetPhi->at(ijet)) , pfMET, pfMETPhi);
                    float tmass_LQMet= TMass_F(LQ.Pt(), LQ.Px(),LQ.Py(), pfMET, pfMETPhi);
                    
                    //###############################################################################################
                    //  dPhi Jet_MET Categorization
                    //###############################################################################################
                    const int size_jetMetPhi = 2;
                    bool lowDPhi = (deltaPhi(Jet4Momentum.Phi(),pfMETPhi) < 0.5);
                    bool HighDPhi = (deltaPhi(Jet4Momentum.Phi(),pfMETPhi) >= 0.5);
                    
                    bool jetMetPhi_category[size_jetMetPhi] = {lowDPhi,HighDPhi};
                    std::string jetMetPhi_Cat[size_jetMetPhi] = {"_LowDPhi", "_HighDPhi"};
                    
                    //###############################################################################################
                    //  LQ eta Categorization
                    //###############################################################################################
                    const int size_lqEta = 3;
                    bool centralLQ = (fabs(LQ.Eta()) < 1.5);
                    bool BarrelLQ = (fabs(LQ.Eta()) >= 1.5 );
                    bool TotLQ = 1;
                    
                    bool lqEta_category[size_lqEta] = {centralLQ,BarrelLQ,TotLQ};
                    std::string lqEta_Cat[size_lqEta] = {"_Barrel", "_Endcap","_TotEta"};
                    
                    //###############################################################################################
                    //  TTbar control region Categorization
                    //###############################################################################################
                    const int size_CR = 3;
                    bool signalRegion = numTau+numZboson + numElectron  < 1 ;
                    bool TTcontrolRegion = (numTau <1 && numZboson < 1 && numElectron > 0 );
                    bool DYcontrolRegion = (numTau <1 && numZboson > 0 && numElectron < 1 );
//                    bool signalRegion = numTau+numZboson + numElectron + numBJet < 1 ;
//                    bool TTcontrolRegion = (numTau <1 && numZboson < 1 && numElectron > 0 && numBJet > 0);
//                    bool DYcontrolRegion = (numTau <1 && numZboson > 0 && numElectron < 1 && numBJet < 1 );
                    bool region_category[size_CR] = {signalRegion,TTcontrolRegion,DYcontrolRegion};
                    std::string region_Cat[size_CR] = {"", "_ttbarCR","_DYCR"};
                    
                    //###############################################################################################
                    
                    std::string CHL="MuJet";
                    
                    plotFill("Weight_Mu", LepCor,200,0,2);
                    plotFill("TotalWeight_Mu",TotalWeight*LepCor,1000,0,10);
                    plotFill("TotalNonLumiWeight_Mu",TotalWeight*LepCor/LumiWeight,1000,0,10);
                    
                    
                    for (int iso = 0; iso < size_isoCat; iso++) {
                        if (Iso_category[iso]) {
                            for (int imt = 0; imt < size_mTCat; imt++) {
                                if (MT_category[imt]) {
                                    for (int jpt = 0; jpt < size_jetMetPhi; jpt++) {
                                        if (jetMetPhi_category[jpt]) {
                                            for (int ieta = 0; ieta < size_lqEta; ieta++) {
                                                if (lqEta_category[ieta]) {
                                                    for (int iCR = 0; iCR < size_CR; iCR++) {
                                                        if (region_category[iCR]) {
                                                            
                                                            
                                                            if (isWJets!= string::npos) WSCALEFACTORE=WSF_mutau;  // Add W scale factor
                                                            
                                                            
                                                            
                                                            float FullWeight = TotalWeight * LepCor  * WSCALEFACTORE;
                                                            std::string FullStringName = MT_Cat[imt] + jetMetPhi_Cat[jpt] + lqEta_Cat[ieta] + region_Cat[iCR] + iso_Cat[iso]  ;
                                                            
                                                            
                                                            
                                                            //##################
                                                            //This check is used to make sure that each event is just filled once for any of the categories ==> No doube-counting of events  (this is specially important for ttbar events where we have many jets and leptons)
                                                            if (!( std::find(HistNamesFilled.begin(), HistNamesFilled.end(), FullStringName) != HistNamesFilled.end())){
                                                                HistNamesFilled.push_back(FullStringName);
                                                                
                                                                plotFill(CHL+"_tmass_MuMet"+FullStringName,tmass_MuMet,200,0,2000,FullWeight);
                                                                plotFill(CHL+"_tmass_JetMet"+FullStringName,tmass_JetMet,200,0,2000,FullWeight);
                                                                plotFill(CHL+"_tmass_LQMet"+FullStringName,tmass_LQMet,200,0,2000,FullWeight);
                                                                
                                                                plotFill(CHL+"_JetPt"+FullStringName,jetPt->at(ijet) ,200,0,2000,FullWeight);
                                                                plotFill(CHL+"_JetEta"+FullStringName,jetEta->at(ijet),100,-2.5,2.5,FullWeight);
                                                                plotFill(CHL+"_LepPt"+FullStringName,muPt->at(imu),200,0,2000,FullWeight);
                                                                plotFill(CHL+"_LepEta"+FullStringName,muEta->at(imu),100,-2.5,2.5,FullWeight);
                                                                plotFill(CHL+"_CloseJetLepPt"+FullStringName,CLoseJetMuPt,1000,0,1000,FullWeight);
                                                                //                                                            plotFill(CHL+"_CloseJetLepEta"+FullStringName,CLoseJetMuEta,100,-2.5,2.5,FullWeight);
                                                                
                                                                plotFill(CHL+"_nVtx"+FullStringName,nVtx,50,0,50,FullWeight);
                                                                plotFill(CHL+"_nVtx_NoPU"+FullStringName,nVtx,50,0,50,TotalWeight * LepCor / PUWeight);
                                                                
                                                                plotFill(CHL+"_MET"+FullStringName,pfMET,200,0,2000,FullWeight);
                                                                plotFill(CHL+"_LQMass"+FullStringName,LQ.M(),200,0,2000,FullWeight);
                                                                plotFill(CHL+"_LQEta"+FullStringName,LQ.Eta(),300,-3,3,FullWeight);
                                                                plotFill(CHL+"_LQPt"+FullStringName,LQ.Pt(),200,0,2000,FullWeight);
                                                                
                                                                //                                                            plotFill(CHL+"_MuPtOverJetPt"+FullStringName,muPt->at(imu)/jetPt->at(ijet),1000,0,10,FullWeight);
                                                                //                                                            plotFill(CHL+"_LQPtOverMET"+FullStringName,LQ.Pt()/pfMET,1000,0,10,FullWeight);
                                                                
                                                                plotFill(CHL+"_dPhi_Jet_Met"+FullStringName,deltaPhi(Jet4Momentum.Phi(),pfMETPhi),200,0,4,FullWeight);
                                                                plotFill(CHL+"_dPhi_Mu_Met"+FullStringName,deltaPhi(Mu4Momentum.Phi(),pfMETPhi),200,0,4,FullWeight);
                                                                
                                                                plotFill(CHL+"_BosonKFactor"+FullStringName,ZBosonKFactor*WBosonKFactor,200,0,2,FullWeight);
                                                                
                                                                
                                                                
                                                                
                                                            }
                                                            
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    //###############################################################################################
                    //  Doing EleTau Analysis
                    //###############################################################################################
                    
                    
                } //End of Tree
            }//End of file
            //##############  end of dielectron
            
        }
        
    }
    
    fout->cd();
    
    map<string, TH1F*>::const_iterator iMap1 = myMap1->begin();
    map<string, TH1F*>::const_iterator jMap1 = myMap1->end();
    
    for (; iMap1 != jMap1; ++iMap1)
        nplot1(iMap1->first)->Write();
    
    map<string, TH2F*>::const_iterator iMap2 = myMap2->begin();
    map<string, TH2F*>::const_iterator jMap2 = myMap2->end();
    
    for (; iMap2 != jMap2; ++iMap2)
        nplot2(iMap2->first)->Write();
    
    fout->Close();
    
    
}
