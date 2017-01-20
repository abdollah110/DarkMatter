#include "../interface/LQ3Analyzer.h"
#include "../interface/WeightCalculator.h"
#include "../interface/Corrector.h"
#include <string>
#include <ostream>


int main(int argc, char** argv) {
    using namespace std;
    
    std::string out = *(argv + 1);
    
    cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;     //PRINTING THE OUTPUT name
    TFile *fout = TFile::Open(out.c_str(), "RECREATE");
    
    myMap1 = new std::map<std::string, TH1F*>();
    myMap2 = new map<string, TH2F*>();
    
    std::vector<string> input;
    for (int f = 2; f < argc; f++) {
        input.push_back(*(argv + f));
        cout << "\n INPUT NAME IS:   " << input[f - 2] << "\n";
    }
    
    
    TFile * PUData= TFile::Open("../interface/pileup-hists/MyDataPileupHistogram2016.root");
    TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
    HistoPUData->Scale(1.0/HistoPUData->Integral());
    
    TFile * PUMC= TFile::Open("../interface/pileup-hists/Sprin16_MC.root");
    TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
    HistoPUMC->Scale(1.0/HistoPUMC->Integral());
    
    TFile * MuCorrId= TFile::Open("../interface/pileup-hists/MuonID_Z_RunBCD_prompt80X_7p65.root");
    //    TFile * MuCorrId= TFile::Open("../interface/pileup-hists/MuonID_Z_2016runB_2p6fb.root");
    TH2F * HistoMuId= (TH2F *) MuCorrId->Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");
    
    TFile * MuCorrIso= TFile::Open("../interface/pileup-hists/MuonIso_Z_RunBCD_prompt80X_7p65.root");
    //    TFile * MuCorrIso= TFile::Open("../interface/pileup-hists/MuonISO_Z_2016runB_2p6fb.root");
    TH2F * HistoMuIso= (TH2F *) MuCorrIso->Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
    
    TFile * MuCorrTrg= TFile::Open("../interface/pileup-hists/SingleMuonTrigger_Z_RunBCD_prompt80X_7p65.root");
    //    TFile * MuCorrTrg= TFile::Open("../interface/pileup-hists/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root");
    TH2F * HistoMuTrg= (TH2F *) MuCorrTrg->Get("Mu45_eta2p1_PtEtaBins_Run274094_to_276097/efficienciesDATA/pt_abseta_DATA");
    
    
    
    TFile * KFactor= TFile::Open("../interface/pileup-hists/kfactors.root");
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
        
        
        std::string ROOTLoc= "/Users/abdollah1/GIT_abdollah110/DarkMatter/ROOT80X/";
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
        Run_Tree->SetBranchAddress("jetHadFlvr",&jetHadFlvr);
        Run_Tree->SetBranchAddress("jetRawPt",&jetRawPt);
        Run_Tree->SetBranchAddress("jetRawEn",&jetRawEn);
        Run_Tree->SetBranchAddress("jetJECUnc",&jetJECUnc);
        Run_Tree->SetBranchAddress("jetHadFlvr",&jetHadFlvr);
        /////////////////////////   MET Info
        
        Run_Tree->SetBranchAddress("pfMET",&pfMET);
        Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
        Run_Tree->SetBranchAddress("metFilters",&metFilters);
        Run_Tree->SetBranchAddress("genHT",&genHT);
        
        
        //###############################################################################################
        //  Weight Calculation
        //###############################################################################################
        Int_t nBin=500;
        Int_t binMin=0;
        Int_t binMax= 5000;
        
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
        
        float jetES[3]={-1,0,1};
        std::string ScaleJet_Cat[3] = {"JetDown", "", "JetUp"};
        
        
        
        
        Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
        cout<<"nentries_wtn===="<<nentries_wtn<<"\n";
        for (Int_t i = 0; i < nentries_wtn; i++) {
            Run_Tree->GetEntry(i);
            if (i % 10000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
            fflush(stdout);
            
            if (isData && (metFilters!=0)) continue;
            
            
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
            
            float TotalWeight = LumiWeight * GetGenWeight * PUWeight * TopPtReweighting ;
            //###############################################################################################
            //  Histogram Filling
            //###############################################################################################
            plotFill("WeightLumi",LumiWeight,10000,0,100);
            plotFill("TopPtReweighting",TopPtReweighting,100,0,10);
            plotFill("WeightPU",PUWeight,100,0,10);
            plotFill("WeightTotal",TotalWeight,50,0,5);
            plotFill("nVtx_NoPUCorr",nVtx,60,0,60);
            plotFill("nVtx_PUCorr",nVtx,60,0,60,PUWeight);
            for (int qq=0; qq < 60;qq++){
                if ((HLTEleMuX >> qq & 1) == 1)
                    plotFill("HLT",qq,60,0,60);
            }
            
            
            //###############################################################################################
            //  Doing MuTau Analysis
            //###############################################################################################
            
            
            //###############################################################################################
            //  Trigger Categorization
            //###############################################################################################
            
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
            
//            bool PassMuonSelections= false;
//            bool PassJetSelections= false;
            for  (int imu=0 ; imu < nMu; imu++){
//                if (PassMuonSelections) continue;
                
                
                float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
                if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
                    IsoMu= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
                
                bool MuPtCut = muPt->at(imu) > LeptonPtCut_ && fabs(muEta->at(imu)) < 2.1 ;
                bool MuIdIso=(muIsTightID->at(imu) > 0  && fabs(muD0->at(imu)) < 0.045 && fabs(muDz->at(imu)) < 0.2);
                
                if (! MuPtCut || !MuIdIso ) continue;
//                PassMuonSelections=true;
                
                
                float LepCor=getCorrFactorMuon74X(isData,  muPt->at(imu), muEta->at(imu) , HistoMuId,HistoMuIso,HistoMuTrg);
                
                
                TLorentzVector Mu4Momentum, Jet4Momentum,KJet4Momentum,NewJet4Collection,LQ;
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
//                    if (PassJetSelections) continue;
                    for (int jetScl=0;jetScl<3;jetScl++){
                    
                    Jet4Momentum.SetPtEtaPhiE(jetPt->at(ijet),jetEta->at(ijet),jetPhi->at(ijet),jetEn->at(ijet));
                    NewJet4Collection.SetPtEtaPhiE(Jet4Momentum.Pt()*(1+ jetES[jetScl]*jetJECUnc->at(ijet)) ,jetEta->at(ijet),jetPhi->at(ijet),Jet4Momentum.E()*(1+ jetES[jetScl]*jetJECUnc->at(ijet)));
                    
                    
                    
                    bool goodJet = (jetPFLooseId->at(ijet) > 0.5 && NewJet4Collection.Pt() > JetPtCut && fabs(NewJet4Collection.Eta() ) < 2.4 && NewJet4Collection.DeltaR(Mu4Momentum) > 0.5);
                    if (! goodJet) continue;
//                    PassJetSelections=true;
                    
                    
                    
                    float jetMET=pfMET;
                    float jetMETPhi=pfMETPhi;
                    
                    float jetMET_x = pfMET * TMath::Cos(pfMETPhi) - (Jet4Momentum.Px()- NewJet4Collection.Px()) ;
                    float jetMET_y = pfMET * TMath::Sin(pfMETPhi) - (Jet4Momentum.Py()- NewJet4Collection.Py()) ;
                    //                                    cout << " jetMET_x="<<jetMET_x<<"   jetMET_y="<<jetMET_y<<"\n";
                    jetMET = sqrt (pow(jetMET_x,2)+ pow(jetMET_y,2));
                    jetMETPhi = atan(jetMET_y / jetMET_x);
                    if (pfMETPhi > (TMath::Pi() / 2)) jetMETPhi += TMath::Pi();
                    if (pfMETPhi < (-TMath::Pi() / 2)) jetMETPhi -= TMath::Pi();
                    
                    
                    
                    LQ=NewJet4Collection + Mu4Momentum;
                     if ((numTau+numElectron +numZboson) > 0 && fabs(LQ.Eta()) > 1.5) continue;
                    //###############################################################################################
                    //  Isolation Categorization
                    //###############################################################################################
                    //###############################################################################################
                    
                    
                    bool LepPassIsolation= IsoMu < LeptonIsoCut;
                    
                    const int size_isoCat = 2;
                    bool Isolation = LepPassIsolation;
                    bool AntiIsolation =  !LepPassIsolation;
                    
                    
                    bool Iso_category[size_isoCat] = {Isolation, AntiIsolation};
                    std::string iso_Cat[size_isoCat] = {"_Iso", "_AntiIso"};
                    //###############################################################################################
                    //  MT Categorization
                    //###############################################################################################
                    float tmass_MuMet= TMass_F(muPt->at(imu), muPt->at(imu)*cos(muPhi->at(imu)),muPt->at(imu)*sin(muPhi->at(imu)) , jetMET, jetMETPhi);
                    const int size_mTCat = 3;
                    bool MT0 = tmass_MuMet > 0;
                    bool MT50 = tmass_MuMet > 50;
                    bool MT100 = tmass_MuMet > 100;
                    
                    bool MT_category[size_mTCat] = {MT0,MT50,MT100};
                    std::string MT_Cat[size_mTCat] = {"_MT0", "_MT50","_MT100"};
                    
//                    float tmass_JetMet= TMass_F(jetPt->at(ijet), jetPt->at(ijet)*cos(jetPhi->at(ijet)),jetPt->at(ijet)*sin(jetPhi->at(ijet)) , pfMET, pfMETPhi);
//                    float tmass_LQMet= TMass_F(LQ.Pt(), LQ.Px(),LQ.Py(), pfMET, pfMETPhi);
                    
                    
                    //###############################################################################################
                    //  Jet Pt Categorization
                    //###############################################################################################
                    
                    const int size_METcut = 5;
                    bool MET200 = jetMET > 200;
                    bool MET250 = jetMET > 250;
                    bool MET300 = jetMET > 300;
                    bool MET350 = jetMET > 350;
                    bool MET400 = jetMET > 400;
                    
                    
                    bool MetCut_category[size_METcut] = {MET200,MET250,MET300,MET350,MET400};
                    std::string MetCut_Cat[size_METcut] = {"_MET200", "_MET250","_MET300", "_MET350","_MET400"};
                    
                    
                    //###############################################################################################
                    
                    std::string CHL="MuJet";

                        plotFill("Weight_Mu", LepCor,200,0,2);
                        plotFill("TotalWeight_Mu",TotalWeight*LepCor,1000,0,10);
                        plotFill("TotalNonLumiWeight_Mu",TotalWeight*LepCor/LumiWeight,1000,0,10);
                        
                        
                        for (int iso = 0; iso < size_isoCat; iso++) {
                            if (Iso_category[iso]) {
                                for (int imt = 0; imt < size_mTCat; imt++) {
                                    if (MT_category[imt]) {
                                        for (int jpt = 0; jpt < size_METcut; jpt++) {
                                            if (MetCut_category[jpt]) {
                                                
                                                        
                                                        //                                                            if (isTTJets!= string::npos) TotalWeight * LepCor=TotalWeight * TTScaleFactor[ist];  // Add TT scale factor
                                                        if (isWJets!= string::npos) WSCALEFACTORE=WSF_mutau;  // Add W scale factor
                                                        
                                                        
                                                        
                                                        float FullWeight = TotalWeight * LepCor  * WSCALEFACTORE;
                                                        std::string FullStringName = MT_Cat[imt] + MetCut_Cat[jpt]+  iso_Cat[iso]+ ScaleJet_Cat[jetScl]; ;
                                                        
                                                        
                                                        plotFill(CHL+"_tmass_MuMet"+FullStringName,tmass_MuMet,200,0,2000,FullWeight);
                                                        plotFill(CHL+"_MET"+FullStringName,pfMET,200,0,2000,FullWeight);
                                                
                                                        plotFill(CHL+"_CloseJetLepPt"+FullStringName,CLoseJetMuPt,1000,0,1000,FullWeight);
                                                        plotFill(CHL+"_LQMass"+FullStringName,LQ.M(),300,0,3000,FullWeight);
                                                
                                                        if (isTTJets!= string::npos) plotFill(CHL+"_LQMassTopPtRWUp"+FullStringName,LQ.M(),nBin,binMin,binMax,FullWeight * TopPtReweighting);
                                                        if (isTTJets!= string::npos) plotFill(CHL+"_LQMassTopPtRWDown"+FullStringName,LQ.M(),nBin,binMin,binMax,FullWeight / TopPtReweighting);
                                                
                                                
                                                        
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    //qcd control region   MET > 100 && MT > 150  && Jet < 100 && jet > 50;
                    //qcd
                    
                    //###############################################################################################
                    //  Doing EleTau Analysis
                    //###############################################################################################
                    
                    
                } //End of Tree
            }//End of file
            //##############  end of dielectron
            
        }
        
    }
    
    fout->cd();
    //    BG_Tree->Write();
    
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
