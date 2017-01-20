#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#http://root.cern.ch/viewvc/trunk/tutorials/pyroot/hsimple.py?revision=20881&view=markup
__author__ = "abdollahmohammadi"
__date__ = "$Feb 23, 2013 10:39:33 PM$"

import math
import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gSystem
from ctypes import *
import ROOT as r
import array

##### Get Jet to Tau FR
from Step1_JetToMuFR_Data import Make_Mu_FakeRate
from Step1_JetToMuFR_Data import _FIT_Jet_Function
##### Get Jet to Tau FR

gROOT.Reset()
import os


ROOT.gROOT.SetBatch(True)
SubRootDir = 'OutFiles_FullSelection/'
verbos_ = False
TauScale = [""]
JetScale = ["JetDown", "", "JetUp"]
SystematicTopPtReWeight = ["TopPtRWUp","TopPtRWDown"]
FinalName = ["_mj"]

############################################################################################################
def _FileReturn(Name, channel,cat,HistoName,PostFixJet):

    if not os.path.exists(SubRootDir):
        os.makedirs(SubRootDir)
    myfile = TFile(SubRootDir + Name + '.root')
    print "##### --->>>>>>> File name is ", SubRootDir + Name + '.root'  "   and histo is -->  ", channel+HistoName + cat+PostFixJet
    Histo =  myfile.Get(channel+HistoName + cat+PostFixJet)
    if not os.path.exists("Extra"):
        os.makedirs("Extra")
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(Histo,"XXX")
    myfile.Close()
    return NewFile


####################################################
##   Start Making the Datacard Histograms
####################################################
def MakeTheHistogram(channel,NormMC,NormQCD,ShapeQCD,chl,Binning):
    
    
    signal = ['DM_Codex_']
    signalName = ['Codex_']
    mass = [
    '1000',
    '1200',
    '1400',
    '1600',
    '1800',
    '2000'
    ]


    TOTMASS = ['1000','1200','1400','1600','1800','2000']
    category = [""]

    

    JetScaleOut = ["_CMS_scale_j"+"Down", "", "_CMS_scale_j"+"Up"]
    Signal_Unc_TopPTRW = ["_CMS_top_pt_Reweighting"+"Up","_CMS_top_pt_Reweighting"+"Down"]

    myOut = TFile(FinalName[chl]+NormMC+".root" , 'RECREATE') # Name Of the output file


    for NameCat in category:
        print "starting NameCat and channel and HistoName ", NameCat, channel, NormMC

        tDirectory= myOut.mkdir(channel + str(NameCat))
        tDirectory.cd()
        for tscale in range(len(TauScale)):
            for jscale in range(len(JetScale)):
#                if tscale != 1 and jscale!=1 : continue
#               ################################################
#               #   Filling Signal
#    #           ################################################
                for sig in range(len(signal)):
                    for m in range(len(mass)):

                        print "--------------------------------------------------->     Processing  Signal ", signal[sig],mass[m]
                        tDirectory.cd()

                        Name= str(signal[sig])+str(mass[m])
                        NameOut= str(signalName[sig]) +str(TOTMASS[m])+str(JetScaleOut[jscale])

                                
                        NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                        NormHisto=NormFile.Get("XXX")
                
                        RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                        tDirectory.WriteObject(RebinedHist,NameOut)


                ################################################
                #  Filling SingleTop
                ################################################
                print "--------------------------------------------------->     Processing SingleTop"
                tDirectory.cd()
            
                Name= "SingleTop"
                NameOut= "SingleTop"+str(JetScaleOut[jscale])
                
                NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                NormHisto=NormFile.Get("XXX")
                
                RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                tDirectory.WriteObject(RebinedHist,NameOut)
                

                ################################################
                #  Filling VV
                ################################################
                print "--------------------------------------------------->     Processing VV"
                tDirectory.cd()
            
                Name= "VV"
                NameOut= "VV"+str(JetScaleOut[jscale])
                
                NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                NormHisto=NormFile.Get("XXX")
                
                RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                tDirectory.WriteObject(RebinedHist,NameOut)
                



                ################################################
                #  Filling TOP
                ################################################
                print "--------------------------------------------------->     Processing TOP"
                tDirectory.cd()

                Name= "TTJets"
                NameOut= "TT"+str(JetScaleOut[jscale])

                NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                NormHisto=NormFile.Get("XXX")
            
                RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                tDirectory.WriteObject(RebinedHist,NameOut)
                ###############  Systematics on Shape and Norm for  To PT Reweighting ####
                if  jscale==1:
                    for systTopRW in range(len(SystematicTopPtReWeight)):
                        tDirectory.cd()
                        Histogram = NormMC.replace("_LQMass","_LQMass"+SystematicTopPtReWeight[systTopRW])
                        NameOut= "TT"+str(Signal_Unc_TopPTRW[systTopRW])

                        NormFile= _FileReturn(Name, channel,NameCat, Histogram, JetScale[jscale])
                        NormHisto=NormFile.Get("XXX")

                        RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                        tDirectory.WriteObject(RebinedHist,NameOut)
                            
    
                ################################################
                #  Filling ZTT
                ################################################
                print "--------------------------------------------------->     Processing ZTT"
                tDirectory.cd()

                Name= "DYJetsToLL"
                NameOut= "ZTT"+str(JetScaleOut[jscale])
                
                NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                NormHisto=NormFile.Get("XXX")
                
                RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                tDirectory.WriteObject(RebinedHist,NameOut)

                ################################################
                #  Filling W
                ################################################
                print "--------------------------------------------------->     Processing W"
                tDirectory.cd()

                Name="WJetsToLNu"
                NameOut= "W"+str(JetScaleOut[jscale])

                NormFileWNoCor= _FileReturn(Name, channel,NameCat, NormMC.replace("","") +"", JetScale[jscale])
                NormHistoWNoCor=NormFileWNoCor.Get("XXX")
                WNoCorNormaliztaion=NormHistoWNoCor.Integral()
                
            
                NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                NormHisto=NormFile.Get("XXX")
                
                NormHisto.Scale(WNoCorNormaliztaion/NormHisto.Integral())
                RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                tDirectory.WriteObject(RebinedHist,NameOut)



                ################################################
                #  Filling QCD
                ################################################
                if jscale==1:
                    print "--------------------------------------------------->     Processing QCD"
                    tDirectory.cd()
                    
                    Name= "SingleTop"
                    SingleTSampleQCDNorm= _FileReturn(Name, channel,NameCat, NormQCD, JetScale[jscale])
                    SingleTSampleQCDShape= _FileReturn(Name, channel,NameCat, ShapeQCD, JetScale[jscale])
                    
                    Name= "VV"
                    VVSampleQCDNorm= _FileReturn(Name, channel,NameCat, NormQCD, JetScale[jscale])
                    VVSampleQCDShape= _FileReturn(Name, channel,NameCat, ShapeQCD, JetScale[jscale])

                    Name= "TTJets"
                    TTSampleQCDNorm= _FileReturn(Name, channel,NameCat, NormQCD, JetScale[jscale])
                    TTSampleQCDShape= _FileReturn(Name, channel,NameCat, ShapeQCD, JetScale[jscale])

                    Name= "DYJetsToLL"
                    ZTTSampleQCDNorm= _FileReturn(Name, channel,NameCat, NormQCD, JetScale[jscale])
                    ZTTSampleQCDShape= _FileReturn(Name, channel,NameCat, ShapeQCD, JetScale[jscale])

                    Name= "WJetsToLNu"
                    WSampleQCDNorm= _FileReturn(Name, channel,NameCat, NormQCD, JetScale[jscale])
                    WSampleQCDShape= _FileReturn(Name, channel,NameCat, ShapeQCD, JetScale[jscale])
                                
                    Name="Data"
                    DataSampleQCDNorm= _FileReturn(Name, channel,NameCat, NormQCD, JetScale[jscale])
                    DataSampleQCDShape= _FileReturn(Name, channel,NameCat, ShapeQCD, JetScale[jscale])



                    SingleTSampleQCDShapeHist=SingleTSampleQCDShape.Get("XXX")
                    VVSampleQCDShapeHist=VVSampleQCDShape.Get("XXX")
                    TTSampleQCDShapeHist=TTSampleQCDShape.Get("XXX")
                    ZTTSampleQCDShapeHist=ZTTSampleQCDShape.Get("XXX")
                    WSampleQCDShapeHist=WSampleQCDShape.Get("XXX")
                    DataSampleQCDShapeHist=DataSampleQCDShape.Get("XXX")

                    print "\n---->   ##########\nlooseQCDShape before=",    DataSampleQCDShapeHist.Integral()
                    
#                    if (SingleTSampleQCDShapeHist) : DataSampleQCDShapeHist.Add(SingleTSampleQCDShapeHist, -1)
#                    if (VVSampleQCDShapeHist): DataSampleQCDShapeHist.Add(VVSampleQCDShapeHist, -1)
#                    DataSampleQCDShapeHist.Add(TTSampleQCDShapeHist, -1)
#                    DataSampleQCDShapeHist.Add(ZTTSampleQCDShapeHist, -1)
#                    DataSampleQCDShapeHist.Add(WSampleQCDShapeHist, -1)
                    print "\n---->   ##########\nlooseQCDShaoe after=",    DataSampleQCDShapeHist.Integral()


                    SingleTSampleQCDNormHist=SingleTSampleQCDNorm.Get("XXX")
                    VVSampleQCDNormHist=VVSampleQCDNorm.Get("XXX")
                    TTSampleQCDNormHist=TTSampleQCDNorm.Get("XXX")
                    ZTTSampleQCDNormHist=ZTTSampleQCDNorm.Get("XXX")
                    WSampleQCDNormHist=WSampleQCDNorm.Get("XXX")
                    DataSampleQCDNormHist=DataSampleQCDNorm.Get("XXX")
                    

                    if (SingleTSampleQCDNormHist) : DataSampleQCDNormHist.Add(SingleTSampleQCDNormHist, -1)
                    if (VVSampleQCDNormHist): DataSampleQCDNormHist.Add(VVSampleQCDNormHist, -1)
                    DataSampleQCDNormHist.Add(TTSampleQCDNormHist, -1)
                    DataSampleQCDNormHist.Add(ZTTSampleQCDNormHist, -1)
                    DataSampleQCDNormHist.Add(WSampleQCDNormHist, -1)
                    
                    print "\n##########\nlooseQCDNORM after=",    DataSampleQCDNormHist.Integral()
                    FR_FitMaram=Make_Mu_FakeRate(channel)
                    QCDEstimation=0
                    for bin in xrange(50,500):
                        value=DataSampleQCDNormHist.GetBinContent(bin)
                        if value < 0 : value=0
                        FR= _FIT_Jet_Function(bin+1.5,FR_FitMaram)
                        QCDEstimation += value * FR/(1-FR)
                    print "\n##########\n QCDEstimation",    QCDEstimation

        #            QCDEstimation= (DataSampleQCDNormHist.Integral()- (TT_qcd+ZTT_qcd+W_qcd+SingleT_qcd+VV_qcd)) * OS_SS_Ratio

                    NameOut= "QCD"+str(JetScaleOut[jscale])
                    DataSampleQCDShapeHist.Scale(QCDEstimation/DataSampleQCDShapeHist.Integral())  # The shape is from btag-Loose Need get back norm
                    RebinedHist= DataSampleQCDShapeHist.Rebin(len(Binning)-1,"",Binning)
                    tDirectory.WriteObject(RebinedHist,NameOut)

                ################################################
                #  Filling Data
                ################################################
                if jscale==1:
                    print "--------------------------------------------------->     Processing Data"
                    tDirectory.cd()

                    Name='Data'
                    NameOut='data_obs'

                    NormFile= _FileReturn(Name, channel,NameCat, NormMC, JetScale[jscale])
                    NormHisto=NormFile.Get("XXX")
                
                    RebinedHist= NormHisto.Rebin(len(Binning)-1,"",Binning)
                    tDirectory.WriteObject(RebinedHist,NameOut)




    myOut.Close()

if __name__ == "__main__":

    Binning = array.array("d",[0,100,200,300,400,500,600,700,800,900,1000,1150,1300,1450,1600,1800,2000])


    Met_Cat= ["_MET200", "_MET250","_MET300", "_MET350","_MET400"]
    MT_Cat = ["_MT0", "_MT50","_MT100"]

    for met in Met_Cat:
        for mt in MT_Cat:


            NormMC="_LQMass"+mt+met
            MakeTheHistogram("MuJet",NormMC+"_Iso","_CloseJetLepPt"+mt+met+"_AntiIso",NormMC+"_AntiIso",0,Binning)
