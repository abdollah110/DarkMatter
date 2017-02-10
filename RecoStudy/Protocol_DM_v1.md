Protocol to do the Dark matter search in ‘coannihilation codex’ or ‘MonoLQ + MET’:

The github repository is : https://github.com/abdollah110/DarkMatter

*CodexAnalyzer_Preselection.cc*
------------

This is the code that we do the pre-selection and used for background estimation and closure test studies and make the control plots. Here is how one can run this code;
-First one needs to compile it with:

```shell
./Make.sh CodexAnalyzer_Preselection.cc
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/DYJetsToLL.root ../ROOT80X/DIR/DYJetsToLL_M-50*.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/TTJets.root ../ROOT80X/DIR/Inclusive_TTJets.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/WJetsToLNu.root ../ROOT80X/DIR/WJetsToLNu*.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/VV.root ../ROOT80X/DIR/ZZ.root  ../ROOT80X/DIR/WW.root  ../ROOT80X/DIR/WZ.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/SingleTop.root ../ROOT80X/DIR/ST_*.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/Data.root ../ROOT80X/DIR/SingleMu.root
```


*CodexAnalyzer_FinalSelection.cc*
------------

This code is used for Making the final root files, including all the shape systematics, i.e. JES, Top pt reweighting sys, ….

```shell
./Make.sh CodexAnalyzer_FinalSelection.cc
./CodexAnalyzer_FinalSelection.exe  OutFiles_FullSelection/DYJetsToLL.root ../ROOT80X/DYJetsToLL_M-50*.root
./CodexAnalyzer_FinalSelection.exe  OutFiles_FullSelection/TTJets.root ../ROOT80X/Inclusive_TTJets.root
./CodexAnalyzer_FinalSelection.exe  OutFiles_FullSelection/WJetsToLNu.root ../ROOT80X/WJetsToLNu*.root
./CodexAnalyzer_FinalSelection.exe  OutFiles_FullSelection/VV.root ../ROOT80X/ZZ.root  ../ROOT80X/WW.root  ../ROOT80X/WZ.root
./CodexAnalyzer_FinalSelection.exe  OutFiles_FullSelection/SingleTop.root ../ROOT80X/ST_*.root
./CodexAnalyzer_FinalSelection.exe  OutFiles_FullSelection/Data.root ../ROOT80X/SingleMu.root
```

*CodexAnalyzer_QCDEstim.cc*
------------

This code is used to do the closure test on QCD background estimation



*Step1_JetToMuFR_Data.py*
------------

This macro is used for measuring the jet to mu fake rate from data for qcd background estimation. To run that first one needs to run either CodexAnalyzer_Preselection.cc or CodexAnalyzer_FinalSelection.cc on full samples

*Step2_JetToMuFR_MC.py*
------------

This macro is used for measuring the jet to mu fake rate from mc. To run this one needs first to run CodexAnalyzer_QCDEstim.cc. This is just used for closure test on QCD

*Step3_QCDClosurefromMC.py*
------------

This is for closure test on QCD on MC samples. The output of the Step2_JetToMuFR_MC.py is used as input to this script

*Draw_QCDClosureTestMC.py*
------------

This is the script to make the closure test final plots.

*Step6_PreSelectionPlotter.py*
------------

To make the root files for control plots, this script should be run. This also estimate the qcd estimation as well. The input for this will be the output of the CodexAnalyzer_Preselection.cc

*Step7_FullSelection.py*
------------

This script is used to make the final rootfile for limit computation. All the systematics shape histogram will be created here. The input for this will be the output of the CodexAnalyzer_FinalSelection.cc



*Draw_PreSelection.py*
------------

This is the script to make the control plots. The inout to this macro is the output of the Step6_PreSelectionPlotter.py



