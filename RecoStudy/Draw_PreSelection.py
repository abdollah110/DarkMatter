#!/usr/bin/env python
import ROOT
import re
from array import array

#isLOG=1


def _GetSF():
    
    return 1

RB_=10
def add_lumi():
    lowX=0.69
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.06)
    lumi.SetTextFont (   42 )
    lumi.AddText("12.9 fb^{-1} (13 TeV)")
    return lumi

def add_CMS():
    lowX=0.21
    lowY=0.70
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(61)
    lumi.SetTextSize(0.08)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("CMS")
    return lumi

def add_Preliminary():
    lowX=0.21
    lowY=0.63
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(52)
    lumi.SetTextSize(0.06)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("Preliminary")
    return lumi

def make_legend():
        output = ROOT.TLegend(0.65, 0.4, 0.92, 0.82, "", "brNDC")
        output.SetLineWidth(0)
        output.SetLineStyle(0)
        output.SetFillStyle(0)
        output.SetBorderSize(0)
        output.SetTextFont(62)
        return output


def MakePlot(FileName,categoriy,HistName,Xaxis,Info,RB_,channel,yMin,isLOG):
    ROOT.gStyle.SetFrameLineWidth(3)
    ROOT.gStyle.SetLineWidth(3)
    ROOT.gStyle.SetOptStat(0)

    c=ROOT.TCanvas("canvas","",0,0,600,600)
    c.cd()

    file=ROOT.TFile(FileName,"r")

    adapt=ROOT.gROOT.GetColor(12)
    new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1


    Data=file.Get(categoriy).Get("data_obs")
    Data.Rebin(RB_)
    
    QCD=file.Get(categoriy).Get("QCD")
    QCD.Rebin(RB_)
#

    W=file.Get(categoriy).Get("W")
    W.Rebin(RB_)
    

    TT=file.Get(categoriy).Get("TT")
    TT.Rebin(RB_)
    



    
    SingleT=file.Get(categoriy).Get("SingleTop")
    SingleT.Rebin(RB_)

    VV=file.Get(categoriy).Get("VV")
#    if not VV: VV=SingleT.Clone()
    VV.Rebin(RB_)
#    VV.Scale(.000001)

    DYS=file.Get(categoriy).Get("ZTT")
    DYS.Rebin(RB_)

    Data.GetXaxis().SetTitle("")
    Data.GetXaxis().SetTitleSize(0)
    Data.GetXaxis().SetNdivisions(505)
    Data.GetYaxis().SetLabelFont(42)
    Data.GetYaxis().SetLabelOffset(0.01)
    Data.GetYaxis().SetLabelSize(0.06)
    Data.GetYaxis().SetTitleSize(0.075)
    Data.GetYaxis().SetTitleOffset(1.04)
    Data.SetTitle("")
    Data.GetYaxis().SetTitle("Events")


    Signal=file.Get(categoriy).Get("DM_Codex_1400")
    Signal.Scale(.1)
    Signal.Rebin(RB_)
    Signal.SetLineStyle(11)
    Signal.SetLineWidth(3)
    Signal.SetLineColor(4)
    Signal.SetMarkerColor(4)


    QCD.SetFillColor(ROOT.TColor.GetColor(408, 106, 154))
    W.SetFillColor(ROOT.TColor.GetColor(200, 2, 285))
    TT.SetFillColor(ROOT.TColor.GetColor(208, 376, 124))
    SingleT.SetFillColor(ROOT.TColor.GetColor(150, 132, 232))
    VV.SetFillColor(ROOT.TColor.GetColor(200, 282, 232))
    DYS.SetFillColor(ROOT.TColor.GetColor(108, 226, 354))
    
    
    
#    ######  Add OverFlow Bin
#    if not FileName.find("_tmass_MuMet") > 0:
#        QCD.SetBinContent(QCD.GetNbinsX(),QCD.GetBinContent(QCD.GetNbinsX()+1)+QCD.GetBinContent(QCD.GetNbinsX()))
#        W.SetBinContent(W.GetNbinsX(),W.GetBinContent(W.GetNbinsX()+1)+W.GetBinContent(W.GetNbinsX()))
#        TT.SetBinContent(TT.GetNbinsX(),TT.GetBinContent(TT.GetNbinsX()+1)+TT.GetBinContent(TT.GetNbinsX()))
#        SingleT.SetBinContent(SingleT.GetNbinsX(),SingleT.GetBinContent(SingleT.GetNbinsX()+1)+SingleT.GetBinContent(SingleT.GetNbinsX()))
#        VV.SetBinContent(VV.GetNbinsX(),VV.GetBinContent(VV.GetNbinsX()+1)+VV.GetBinContent(VV.GetNbinsX()))
#        DYS.SetBinContent(DYS.GetNbinsX(),DYS.GetBinContent(DYS.GetNbinsX()+1)+DYS.GetBinContent(DYS.GetNbinsX()))
#        Data.SetBinContent(Data.GetNbinsX(),Data.GetBinContent(Data.GetNbinsX()+1)+Data.GetBinContent(Data.GetNbinsX()))







    Data.SetMarkerStyle(20)
    Data.SetMarkerSize(1)
    QCD.SetLineColor(ROOT.kBlack)
    W.SetLineColor(ROOT.kBlack)
    TT.SetLineColor(ROOT.kBlack)
    DYS.SetLineColor(ROOT.kBlack)
    VV.SetLineColor(ROOT.kBlack)
    SingleT.SetLineColor(ROOT.kBlack)
    Data.SetLineColor(ROOT.kBlack)
    Data.SetLineWidth(2)
    
    if FileName.find("LQMass") > 0 :
        print "##################################\n", FileName
        for i in range(Data.GetNbinsX()):
            if i > 15 : Data.SetBinContent(i+1,0)

    stack=ROOT.THStack("stack","stack")
    stack.Add(QCD)
    stack.Add(VV)
    stack.Add(DYS)
    stack.Add(SingleT)
    stack.Add(TT)
    stack.Add(W)

    errorBand = W.Clone()
    errorBand.Add(QCD)
    errorBand.Add(TT)
    errorBand.Add(VV)
    errorBand.Add(SingleT)
    errorBand.Add(DYS)
    errorBand.SetMarkerSize(0)
    errorBand.SetFillColor(16)
    errorBand.SetFillStyle(3001)
    errorBand.SetLineWidth(1)

    pad1 = ROOT.TPad("pad1","pad1",0,0.35,1,1)
    if isLOG: pad1.SetLogy()
    pad1.Draw()
    pad1.cd()
    pad1.SetFillColor(0)
    pad1.SetBorderMode(0)
    pad1.SetBorderSize(10)
    pad1.SetTickx(1)
    pad1.SetTicky(1)
    pad1.SetLeftMargin(0.18)
    pad1.SetRightMargin(0.05)
    pad1.SetTopMargin(0.122)
    pad1.SetBottomMargin(0.026)
    pad1.SetFrameFillStyle(0)
    pad1.SetFrameLineStyle(0)
    pad1.SetFrameLineWidth(3)
    pad1.SetFrameBorderMode(0)
    pad1.SetFrameBorderSize(10)

    Data.GetXaxis().SetLabelSize(0)
    if isLOG: Data.SetMaximum(Data.GetMaximum()*10000)
    else:  Data.SetMaximum(Data.GetMaximum()*3)
    Data.SetMinimum(yMin)
    Data.Draw("e")
    stack.Draw("histsame")
    errorBand.Draw("e2same")
    Data.Draw("esame")
    Signal.Draw("histsame")

    legende=make_legend()
    legende.AddEntry(Data,"Observed","elp")
    legende.AddEntry(Signal,"Codex_1400","l")
    legende.AddEntry(TT,"t#bar{t}+jets","f")
    legende.AddEntry(SingleT,"SingleTop","f")
    legende.AddEntry(DYS,"DY #rightarrowll ","f")
    legende.AddEntry(VV,"Diboson","f")
    legende.AddEntry(W,"W","f")
    legende.AddEntry(QCD,"QCD multijet","f")
    legende.AddEntry(errorBand,"Uncertainty","f")

    legende.Draw()

    l1=add_lumi()
    l1.Draw("same")
    l2=add_CMS()
    l2.Draw("same")
    l3=add_Preliminary()
    l3.Draw("same")

    pad1.RedrawAxis()

    categ  = ROOT.TPaveText(0.18, 0.5+0.013, 0.43, 0.70+0.155, "NDC")
    categ.SetBorderSize(   0 )
    categ.SetFillStyle(    0 )
    categ.SetTextAlign(   12 )
    categ.SetTextSize ( 0.04 )
    categ.SetTextColor(    1 )
    categ.SetTextFont (   41 )
    #       if i==1 or i==3: 
#    categ.AddText(Info)
    #       else :
    #        categ.AddText("SS")
#    categ.Draw()

    c.cd()
    pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.35);
    pad2.SetTopMargin(0.05);
    pad2.SetBottomMargin(0.35);
    pad2.SetLeftMargin(0.18);
    pad2.SetRightMargin(0.05);
    pad2.SetTickx(1)
    pad2.SetTicky(1)
    pad2.SetFrameLineWidth(3)
    pad2.SetGridx()
    pad2.SetGridy()
    pad2.Draw()
    pad2.cd()
    
    h1=errorBand.Clone()
    h1.SetMaximum(2)
    h1.SetMinimum(0.1)
    h1.SetMarkerStyle(20)

    h3=Data.Clone()
    
    h3.Sumw2()
    h1.Sumw2()
    
    
    h1.SetStats(0)
    h3.SetStats(0)
    h1.SetTitle("")
    
    h1.Divide(errorBand)
    #######  set the bin errors to zero befive divinig data to that
    errorBandZeroErr=errorBand.Clone()
    for ibin in range(errorBandZeroErr.GetXaxis().GetNbins()):
        errorBandZeroErr.SetBinError(ibin+1,0)
    #######
    h3.Divide(errorBandZeroErr)


    h1.GetXaxis().SetTitle(Xaxis)
    h1.GetXaxis().SetLabelSize(0.08)
    h1.GetYaxis().SetLabelSize(0.08)
    h1.GetYaxis().SetTitle("Obs./Exp.")
    h1.GetXaxis().SetNdivisions(505)
    h1.GetYaxis().SetNdivisions(5)
    h1.GetXaxis().SetTitleSize(0.15)
    h1.GetYaxis().SetTitleSize(0.15)
    h1.GetYaxis().SetTitleOffset(0.56)
    h1.GetXaxis().SetTitleOffset(1.04)
    h1.GetXaxis().SetLabelSize(0.11)
    h1.GetYaxis().SetLabelSize(0.11)
    h1.GetXaxis().SetTitleFont(42)
    h1.GetYaxis().SetTitleFont(42)
    
    h1.Draw("e2")
    h3.Draw("E0psame")

    c.cd()
    pad1.Draw()

    ROOT.gPad.RedrawAxis()

    c.Modified()
    c.SaveAs("_plot"+FileName.replace('TotalRootForLimit_PreSelection_MuJet','').replace('.root','')+str(isLOG)+".pdf")
    #       c.SaveAs("mvis"+categoriy+".png")


#channelDirectory = ["MuTau", "EleTau"]
#channelDirectory = ["MuTau"]
#Category = ["_DiJet","_JetBJet"]
#Category = ["_JetBJet"]
#Category = [""]


FileNamesInfo=[
               ["_tmass_JetMet","M_{T}(jet,MET) (GeV)","",5,1],
               ["_tmass_LQMet","M_{T}(LQ,MET)  (GeV)","",5,1],
               ["_LepPt","lepton p_{T} (GeV)","",5,1],
               ["_LepEta","lepton #eta ","",5,10],
               ["_JetPt","jet p_{T} (GeV)","",5,1],
               ["_JetEta","jet #eta ","",5,10],
#               ["_nVtx","# of vertex","",1,10],
#               ["_nVtx_NoPU","# of vertex before PU reweighting","",1,10],
               ["_MET","MET  (GeV)","",5,1],
               ["_LQMass","M_{LQ}   (GeV)","",5,1],
               ["_tmass_MuMet","M_{T}(#mu,MET) (GeV)","",5,1],
               ["_dPhi_Jet_Met","#Delta#phi (jet,MET)","",5,1],
               ["_dPhi_Mu_Met","#Delta#phi (#mu,MET)","",5,1],
#               ["_LQEta","#eta_{LQ}","",10,10],
#               ["_numTau","_numTau","",1],
#               ["_numElectron","_numElectron","",1],
#               ["_numBJet","_numBJet","",1],
#               ["_numZboson","_numZboson","",1],
               
               ]




#def MakePlot(FileName,categoriy,HistName,Xaxis,Info,RB_,channel):

#PlotName= ["_tmass_MuMet","_tmass_JetMet","_tmass_LQMet","_LepEta","_LepPt","_JetPt","_JetEta","_MET","_LQMass","_dPhi_Jet_Met","_dPhi_Mu_Met","_nVtx","_nVtx_NoPU"]

#PlotName= ["_tmass_MuMet","_tmass_JetMet","_tmass_LQMet","_LepEta","_LepPt","_LepIso","_JetPt","_JetEta","_MET","_LQMass","_dPhi_Jet_Met","_dPhi_Mu_Met","_nVtx","_nVtx_NoPU"]

#    Isolation=["_Iso", "_AntiIso","_Total"]
Isolation=["_Iso"]
#    MT=["_NoMT"]
#MT= ["_NoMT","_LowMT","_HighMT"]
MT= ["_NoMT","_HighMT"]
#    JPT=["_LowDPhi", "_HighDPhi"];
JPT=[ "_HighDPhi"]
lqEta= ["_Barrel", "_Endcap","_TotEta"]
#    lqEta= ["_TotEta"]
region= ["", "_ttbarCR","_DYCR"]
#    region= ["", "_DYCR"]
#logStat=[0,1]
logStat=[1]


#for NormMC in PlotName:
for i in range(0,len(FileNamesInfo)):
    
    NormMC=FileNamesInfo[i][0]
    axisName=FileNamesInfo[i][1]
    nothing=FileNamesInfo[i][2]
    Bin=FileNamesInfo[i][3]
    yMin=FileNamesInfo[i][4]
    
    for iso in Isolation:
        for mt in MT:
            for jpt in JPT:
                for etalq in lqEta:
                    for reg in region:
                        for isLOG in logStat:
                    
                            FileName="TotalRootForLimit_PreSelection_"+"MuJet"+NormMC+mt+jpt+etalq+reg+iso+".root"
                            Info=NormMC+mt+jpt+etalq+reg+iso
                            print "---->", FileName
                            MakePlot(FileName,"MuJet","",axisName,Info,Bin,"",yMin,isLOG)



#MakeTheHistogram("MuJet",NormMC+mt+jpt+dr+iso,NormMC+mt+jpt+dr+iso,"",1)
#
#
#
#
#
#for ch in channelDirectory:
#    for cat in Category:
#        for i in range(0,len(FileNamesInfo)):
#
##            FileName="TotalRootForLimit_PreSelection_"+ch+FileNamesInfo[i][0]+"_OS.root"
#            FileName="TotalRootForLimit_PreSelection_"+ch+FileNamesInfo[i][0]+".root"
#            MakePlot(FileName,ch+cat,FileNamesInfo[i][0],FileNamesInfo[i][1],FileNamesInfo[i][2],FileNamesInfo[i][3],ch+cat)
#
