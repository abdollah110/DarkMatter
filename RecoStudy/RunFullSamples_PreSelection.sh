#!/bin/sh

#This script is used to run data(MC) sapmles as local or lxbatch jobs. Please pass the required environmental variables to the script.

#Head: Run_Type: LOCAL or JOB
#Head: Input_Loc: EOS or AFS
#sh RunFullSamples_PreSelection.sh Run_Type Input_Loc
 

export Run_Type=$1
export Input_Loc=$2
export RootName_Prefix=""
alias  list='ls'

source /afs/cern.ch/project/eos/installation/cms/etc/setup.sh


Execute_Date=`date +%m-%d-%y`
printf "Do you know today is: %s\n" $Execute_Date
Date=`printf "${Run_Type}_%s\n" $Execute_Date`  

#Here set the default location of input files for pre selection code.
export Working_Dir=`pwd`
export AFS_SampleLoc=$Working_Dir/../ROOT80X
export EOS_SampleLoc=/eos/cms/store/user/mnaseri/Codex_DK

if [ "${Input_Loc}" == "AFS" ];then
     export SampleLoc=$AFS_SampleLoc
     printf "The default directory for input files is %s, sit in current working directory.\n" $AFS_SampleLoc
elif [ "${Input_Loc}" == "EOS" ];then
     export SampleLoc=$EOS_SampleLoc
     printf "The EOS default directory for input files is %s.\n" $EOS_SampleLoc
     RootName_Prefix=root://eoscms/
   alias  list='eos ls'
else
    echo Please insert the valid second argument for location of input files: EOS or AFS ! 
    exit 1
fi




#Here default output directory is created to store the output files.
export PreResults_DIR=$Working_Dir/OutFiles_PreSelection/${Date}
mkdir -p $PreResults_DIR

# you can set the specific name for output directory.
read -p "Please enter the output directory name, otherwise push Enter key:"


    
if [ -n "${REPLY}" ];then
   PreResults_DIR=$PreResults_DIR/$REPLY
   if [ -d $PreResults_DIR ]
   then
       echo "The $PreResults_DIR directory already exists."
   else
       printf "\nYou are creating the %s directory to store pre selection root files.\n" $PreResults_DIR
       mkdir -p $PreResults_DIR
   fi
else
printf "\nThe pre selection root files are stored in %s as default output directory.\n" $PreResults_DIR 
fi


./Make.sh CodexAnalyzer_Preselection.cc


if [ "${Run_Type}" == "LOCAL" ];then
./CodexAnalyzer_Preselection.exe   $PreResults_DIR/DYJetsToLL.root ` list  $SampleLoc/DYJetsToLL_M-50_*.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/TTJets.root  `list $SampleLoc/Inclusive_TTJets.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/WJetsToLNu.root `list $SampleLoc/WJetsToLNu*.root`	
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/VV.root `list $SampleLoc/ZZ.root  $SampleLoc/WW.root  $SampleLoc/WZ.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/SingleTop.root `list $SampleLoc/ST_*.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/Data.root `list $SampleLoc/SingleMu.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/Codex1000.root    `list $SampleLoc/Codex_1000.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/Codex1400.root    `list $SampleLoc/Codex_1400.root | xargs `
./CodexAnalyzer_Preselection.exe  $PreResults_DIR/Codex1800.root   `list  $SampleLoc/Codex_1800.root | xargs `

elif [ "${Run_Type}" == "JOB" ];then

masterqueue=8nh
memory=12000
diskspace=12000

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]"  " CodexAnalyzer_Preselection.exe $PreResults_DIR/DYJetsToLL.root `list $SampleLoc/DYJetsToLL_M-50_*.root | xargs` "

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/TTJets.root  `list $SampleLoc/Inclusive_TTJets.root | xargs` "

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/WJetsToLNu.root `list $SampleLoc/WJetsToLNu*.root | xargs` "

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/VV.root `list $SampleLoc/ZZ.root  $SampleLoc/WW.root  $SampleLoc/WZ.root | xargs `"

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/SingleTop.root `list $SampleLoc/ST_*.root | xargs `"

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/Data.root `list $SampleLoc/SingleMu.root | xargs`"

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/Codex1000.root    `list $SampleLoc/Codex_1000.root | xargs `"

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/Codex1400.root    `list $SampleLoc/Codex_1400.root | xargs `"

bsub -q ${masterqueue} -C 0  -R "rusage[mem=${memory}:pool=${diskspace}]" " CodexAnalyzer_Preselection.exe $PreResults_DIR/Codex1800.root   `list  $SampleLoc/Codex_1800.root | xargs `"

else
    echo Please insert the valid argument for type of run: LOCAL or JOB ! 
    exit 1
fi
