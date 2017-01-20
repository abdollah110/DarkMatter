To compuet the limit here are the procedure:

1) Run the code in the /Users/abdollah1/GIT_abdollah110/LQ2016/RecoAnalysis to get the two following root files (one for mutau and the oter for eletau)
scp TotalRootForLimit_MuTau_ST_JetBJetFinal_NoMT_OS.root abdollah@cmslpc27:/uscms_data/d3/abdollah/Analysis/Limit/CMSSW_7_1_5/src/auxiliaries/shapes/lq_mt.inputs-sm-13TeV.root
scp TotalRootForLimit_EleTau_ST_JetBJetFinal_NoMT_OS.root  abdollah@cmslpc27:/uscms_data/d3/abdollah/Analysis/Limit/CMSSW_7_1_5/src/auxiliaries/shapes/lq_et.inputs-sm-13TeV.root

scp TotalRootForLimit_MuTau_ST_JetBJetFinal_NoMT_OS.root abdollah@cmslpc27:/uscms_data/d3/abdollah/Analysis/Limit/CMSSW_7_1_5/src/auxiliaries/shapes/RHW_mt.inputs-sm-13TeV.root
scp TotalRootForLimit_EleTau_ST_JetBJetFinal_NoMT_OS.root  abdollah@cmslpc27:/uscms_data/d3/abdollah/Analysis/Limit/CMSSW_7_1_5/src/auxiliaries/shapes/RHW_et.inputs-sm-13TeV.root


2) Update the ExampleLQFull.cpp code with systematics and channels ....

3) scram b

4) ExampleLQ    ../../../../bin/slc6_amd64_gcc481/ExampleRHW

5) cd output/lq6_cards/

6)
python ../../../../../HiggsAnalysis/HiggsToTauTau/scripts/limit.py --asymptotic LIMITS/*
python ../../../scripts/combineTool.py -M CollectLimits -i */*/higgsCo* -o rhw.json
python ../../../scripts/plotBSMxsBRLimit_RHW.py --file=rhw.json

get the postfir plots: Mine
limit.py --max-likelihood --stable --rMin -5 --rMax 5 outputLQ/lq3_cards_ST_MET_HighPtLep50/LIMITS/1000
PostFitShapes -d outputLQ/lq3_cards_ST_MET_HighPtLep50/LIMITS/1000/lq_mt_1_13TeV.txt -o lq_mt.inputs-sm-13TeV.root -m 1000 -f mlfit.root:fit_s --postfit --sampling --print



from cecile:
[13/04/16 10:57:24] Cécile Caillol: I use combine directly, and not limit.py anymore


text2workspace.py mydatacard.txt
combine -M MaxLikelihoodFit mydatacard.root --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"  --rMin 0.5 --rMax 1.5
PostFitShapesFromWorkspace -o ztt_mt_shapes.root -m 125 -f mlfit.root:fit_s --postfit --sampling --print -d mydatacard.txt -w mydatacard.root
python HiggsAnalysis/CombinedLimit/test/diffNuisances.py mlfit.root -A -a --stol 0.99 --stol 0.99 --vtol 99. --vtol2 99. -f text mlfit.root > mlfit.txt


[13/04/16 10:58:03] Cécile Caillol: this is what I do

[13/04/16 10:58:24] Cécile Caillol: PostFitShapesFromWorkspace scales the signal by the signal strength




step1:   limit.py --max-likelihood --stable --rMin -5 --rMax 5 LIMITS/120/
=combineCards.py -S lq_et_1_13TeV=lq_et_1_13TeV.txt lq_mt_1_13TeV=lq_mt_1_13TeV.txt
=text2workspace.py -b /tmp/tmpWWHo59 -o ./tmp.root -m 700 --default-morphing shape2
=combine -M MaxLikelihoodFit -m 700  --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.01 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.001 --cminFallbackAlgo "Minuit,0:0.001" --keepFailures --rMin -5 --rMax 5   --saveNormalizations --saveShapes --saveWithUncertainties  ./tmp.root --out=out


ste2:cd outputLQ/lq9_FinalPreTalk/LIMITS/700/out

step 3:  PostFitShapes -o final_lq_mt_700.root -m 700 -f mlfit.root:fit_s --postfit --sampling --print -d ../lq_mt_1_13TeV.txt
         PostFitShapes -o final_lq_et_700.root -m 700 -f mlfit.root:fit_s --postfit --sampling --print -d ../lq_et_1_13TeV.txt




limit.py --max-likelihood --stable --rMin -5 --rMax 5 LIMITS/2000
PostFitShapes -o final_rw_mt_3000.root -m 3000 -f LIMITS/3000/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/3000/RHW__mt_1_13TeV.txt
PostFitShapes -o final_rw_et_3000.root -m 3000 -f LIMITS/3000/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/3000/RHW__et_1_13TeV.txt



limit.py --max-likelihood --stable --rMin -5 --rMax 5 LIMITS/900/
PostFitShapes -o final_lq_mt_900.root -m 900 -f LIMITS/900/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/900/lq_mt_1_13TeV.txt
PostFitShapes -o final_lq_et_900.root -m 900 -f LIMITS/900/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/900/lq_et_1_13TeV.txt

    *************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************

Limit for CLS:
submit.py --CLs --cycles 5 LIMITS/*
Need  to change the mass range from 90 to 150 to  1000 to 3000*/


Using combine twiki:
https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#Signal_Hypothesis_separation
[abdollah@cmslpc27 RHW30_FinalPreTalk_Obs_AN]$ combine -M HybridNew --rMin=0.01 --rMax=5.0 --frequentist --testStat LHC --fork 4 -m 2500 -n TestWprimeRight BothComb.txt  --rRelAcc=0.01 --rAbsAcc=0.0005



***********************************************************
5 cycle(s) to finish
***********************************************************
submit-slave.py --bin combine --method CLs -n 5 --min   0.3  --max   1.5 --toysH 50 -t 500 -j 500 --random --server   LIMITS/1000 LIMITS/1500 LIMITS/2000 LIMITS/2500 LIMITS/3000
submit-slave.py --bin combine --method CLs -n 5 --min   2.0  --max   4.0 --toysH 50 -t 500 -j 500 --random --server   LIMITS/1000 LIMITS/1500 LIMITS/2000 LIMITS/2500 LIMITS/3000

For comaprioson limit
python MakeCompare.py --file=lq.json --file2=../lq15_Mltau200/lq.json --file3=../lq16_Mltau0/lq.json --expected_only --leg=Expected_Mltau_100GeV --leg2=Expected_Mltau_200GeV --leg3=Expected_No_Mltau_cut




making the plots for lq;
python ../../../scripts/plotLQ3.py --file=lq.json --y_axis_min .001 --y_axis_max 100

%%%%%%%   WSF factor



text2workspace.py LIMITS/120/W_et_1_13TeV.txt -m 120 -o myWS.root

PostFitShapesFromWorkspace -o final_W_SF_et.root -m 120 -f LIMITS/120/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/120/W_et_1_13TeV.txt --workspace myWS.root

PostFitShapesFromWorkspace -o final_W_SF_mt.root -m 120 -f LIMITS/120/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/120/W_et_1_13TeV.txt --workspace myWS.root




PostFitShapesFromWorkspace -o final_W_SF_et.root -m 120 -f LIMITS/120/out/mlfit.root:fit_s --postfit --sampling --print -d LIMITS/120/W_et_1_13TeV.txt --workspace myWS.root


######## This is for LQ

text2workspace.py LIMITS/900/lq_mt_1_13TeV.txt -m 900 -o WS_mt.root
combine -M MaxLikelihoodFit LIMITS/900/lq_mt_1_13TeV.txt -m 900 --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"
PostFitShapesFromWorkspace -o final_lq_mt_900.root -m 900 -f mlfit.root:fit_b --postfit --sampling --print -d LIMITS/900/lq_mt_1_13TeV.txt --workspace WS_mt.root
cp mlfit.root mlfit_mt.root



text2workspace.py LIMITS/900/lq_et_1_13TeV.txt -m 900 -o WS_et.root
combine -M MaxLikelihoodFit LIMITS/900/lq_et_1_13TeV.txt -m 900 --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"
PostFitShapesFromWorkspace -o final_lq_et_900.root -m 900 -f mlfit.root:fit_b --postfit --sampling --print -d LIMITS/900/lq_et_1_13TeV.txt --workspace WS_et.root
cp mlfit.root mlfit_et.root


######## This is for RH W

text2workspace.py LIMITS/3000/RHW__mt_1_13TeV.txt -m 3000 -o WS_mt.root
combine -M MaxLikelihoodFit LIMITS/3000/RHW__mt_1_13TeV.txt -m 3000 --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"
PostFitShapesFromWorkspace -o final_rw_mt_3000.root -m 3000 -f mlfit.root:fit_b --postfit --sampling --print -d LIMITS/3000/RHW__mt_1_13TeV.txt --workspace WS_mt.root
cp mlfit.root mlfit_mt.root



text2workspace.py LIMITS/3000/RHW__et_1_13TeV.txt -m 3000 -o WS_et.root
combine -M MaxLikelihoodFit LIMITS/3000/RHW__et_1_13TeV.txt -m 3000 --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"
PostFitShapesFromWorkspace -o final_rw_et_3000.root -m 3000 -f mlfit.root:fit_b --postfit --sampling --print -d LIMITS/3000/RHW__et_1_13TeV.txt --workspace WS_et.root
cp mlfit.root mlfit_et.root


######## This is for ControPlots


text2workspace.py LIMITS/1000/RHW__mt_1_13TeV.txt -m 1000 -o WS_mt.root
combine -M MaxLikelihoodFit LIMITS/1000/RHW__mt_1_13TeV.txt -m 1000 --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"
PostFitShapesFromWorkspace -o final_rw_mt_1000.root -m 1000 -f mlfit.root:fit_s --postfit --sampling --print -d LIMITS/1000/RHW__mt_1_13TeV.txt --workspace WS_mt.root
cp mlfit.root mlfit_mt.root



text2workspace.py LIMITS/1000/RHW__et_1_13TeV.txt -m 1000 -o WS_et.root
combine -M MaxLikelihoodFit LIMITS/1000/RHW__et_1_13TeV.txt -m 1000 --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"
PostFitShapesFromWorkspace -o final_rw_et_1000.root -m 1000 -f mlfit.root:fit_s --postfit --sampling --print -d LIMITS/1000/RHW__et_1_13TeV.txt --workspace WS_et.root
cp mlfit.root mlfit_et.root



Dataset: /DM_Codex_1000_400_440/abdollah-GEN-SIM-8ecf21deb7c86694de43d2ad8e8e2292/USER
Dataset: /DM_Codex_1200_500_550/abdollah-GEN-SIM-8ecf21deb7c86694de43d2ad8e8e2292/USER
Dataset: /DM_Codex_1400_600_660/abdollah-GEN-SIM-8ecf21deb7c86694de43d2ad8e8e2292/USER
Dataset: /DM_Codex_1600_700_770/abdollah-GEN-SIM-8ecf21deb7c86694de43d2ad8e8e2292/USER
Dataset: /DM_Codex_1800_800_880/abdollah-GEN-SIM-8ecf21deb7c86694de43d2ad8e8e2292/USER
Dataset: /DM_Codex_2000_900_990/abdollah-GEN-SIM-8ecf21deb7c86694de43d2ad8e8e2292/USER



hadd DM_Codex_1000.root   crab_DM_Codex_1000*.root
hadd DM_Codex_1200.root   crab_DM_Codex_1200*.root
hadd DM_Codex_1400.root   crab_DM_Codex_1400*.root
hadd DM_Codex_1600.root   crab_DM_Codex_1600*.root
hadd DM_Codex_1800.root   crab_DM_Codex_1800*.root
hadd DM_Codex_2000.root   crab_DM_Codex_2000*.root




