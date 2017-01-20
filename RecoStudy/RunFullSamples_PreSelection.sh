./Make.sh CodexAnalyzer_Preselection.cc



./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/DYJetsToLL.root ../ROOT80X/DYJetsToLL_M-50*.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/TTJets.root ../ROOT80X/Inclusive_TTJets.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/WJetsToLNu.root ../ROOT80X/WJetsToLNu*.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/VV.root ../ROOT80X/ZZ.root  ../ROOT80X/WW.root  ../ROOT80X/WZ.root
./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/SingleTop.root ../ROOT80X/ST_*.root

./CodexAnalyzer_Preselection.exe  OutFiles_PreSelection/Data.root ../ROOT80X/SingleMu.root

#./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/Codex1000.root    ../ROOT80X/Codex_1000.root
#./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/Codex1400.root    ../ROOT80X/Codex_1400.root
#./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/Codex1800.root    ../ROOT80X/Codex_1800.root


./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/DM_Codex_1000.root  ../ROOT80X/DM_Codex_1000.root
./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/DM_Codex_1200.root  ../ROOT80X/DM_Codex_1200.root
./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/DM_Codex_1400.root  ../ROOT80X/DM_Codex_1400.root
./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/DM_Codex_1600.root  ../ROOT80X/DM_Codex_1600.root
./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/DM_Codex_1800.root  ../ROOT80X/DM_Codex_1800.root
./CodexAnalyzer_Preselection.exe   OutFiles_PreSelection/DM_Codex_2000.root  ../ROOT80X/DM_Codex_2000.root