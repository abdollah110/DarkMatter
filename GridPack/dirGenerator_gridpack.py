import os
import shutil
import fileinput

generation=1

prodAddress='cards/production/13TeV/DarkMatter_Codex_gen%s/'%(generation)

run_card='Template/_run_card.dat'
proc_card='Template/_proc_card.dat'
extramodels='Template/_extramodels.dat'
customizecards='Template/_customizecards.dat'



def TriName(TriMass):
    DirName= 'Codex_LQ%s_DM_%s_X_%s_gen%s'%(TriMass[0],TriMass[1],TriMass[2],TriMass[3])
    AddressName=prodAddress+DirName
    return DirName,AddressName

def create_Directory(TriMass):
    DirName,AddressName=TriName(TriMass)
    if not os.path.exists(AddressName):
        print '>> file  "%s" does not exits, so it is now created' % AddressName
        os.makedirs(AddressName)


def create_run_card(TriMass):
    DirName,AddressName=TriName(TriMass)
    shutil.copy (run_card, AddressName+'/%s_run_card.dat'%DirName)
    if os.path.isfile (run_card):  print('\x1b[3;30;42m' + 'Success in making run_card%s!'%DirName + '\x1b[0m')

def create_extramodels(TriMass):
    DirName,AddressName=TriName(TriMass)
    shutil.copy (extramodels, AddressName+'/%s_extramodels.dat'%DirName)
    if os.path.isfile (extramodels):  print('\x1b[7;31;42m' + 'Success in making extramodels%s!'%DirName + '\x1b[0m')


def create_proc_card(TriMass):
    DirName,AddressName=TriName(TriMass)
    shutil.copy (proc_card,AddressName+'/%s_proc_card.dat'%DirName)
    if os.path.isfile (proc_card):  print('\x1b[6;31;40m' + 'Success in making proc_card%s!'%DirName + '\x1b[0m')
    ## replace the current directory address to the proper one
    for line in fileinput.input(AddressName+'/%s_proc_card.dat'%DirName,inplace=1):
        if 'Codex' in line:
            line=line.replace('Codex_LQ1000_DM_400_X_440_gen2',DirName)
        print(line.strip())

def create_customizecards_card(TriMass):
    DirName,AddressName=TriName(TriMass)
    shutil.copy (customizecards,AddressName+'/%s_customizecards.dat'%DirName)
    if os.path.isfile (customizecards):  print('\x1b[6;33;40m' + 'Success in making customizecards%s!'%DirName + '\x1b[0m')
    ## replace the current mass parameters to the proper ones
    for line in fileinput.input(AddressName+'/%s_customizecards.dat'%DirName,inplace=1):
        if 'frblock  1' in line:
            line=line.replace('4.000000e+02',TriMass[1])
        if 'frblock  2' in line:
            line=line.replace('4.400000e+02',TriMass[2])
        if 'frblock  3' in line:
            line=line.replace('4.400000e+02',TriMass[2])
        if 'frblock  4' in line:
            line=line.replace('1.000000e+03',TriMass[0])
        if 'frblock  5' in line:
            line=line.replace('1.000000e+03',TriMass[0])
        print(line.strip())


def create_gridpack_generation_script(FullTriMass):
    grdPack_Generate=open("create_gridpack_generation_script.sh","w")
    for TriMass in FullTriMass:
        DirName,AddressName=TriName(TriMass)
        grdPack_Generate.write('./gridpack_generation.sh  %s  %s  8nh\n'%(DirName,AddressName))
    grdPack_Generate.close()


def create_submit_gridpack_generation(FullTriMass):
    grdPack_Submit=open("create_submit_gridpack_generation.sh","w")
    for TriMass in FullTriMass:
        DirName,AddressName=TriName(TriMass)
        grdPack_Submit.write('./submit_gridpack_generation.sh 12000 12000  8nh  %s  %s  8nh\n'%(DirName,AddressName))
    grdPack_Submit.close()


def create_submit_gridpack_generation_lxbatch(FullTriMass):
    location = os.getcwd()
    selfBatchSubmit=open("create_SelfSubmitBatch.sh","w")
    for TriMass in FullTriMass:
        
        DirName,AddressName=TriName(TriMass)
        
        submitName=open('tosubmit_%s.sh'%DirName,"w")
        submitName.write('cd %s\n'%location)
        submitName.write('./gridpack_generation.sh  %s  %s  8nh\n'%(DirName,AddressName))
        submitName.close()

        selfBatchSubmit.write('bsub -q 8nh -J   %s < tosubmit_%s.sh \n'%(DirName,DirName))
    selfBatchSubmit.close()


def CheckList_GridPack(FullTriMass):
    checkList=open("create_ChechList.sh","w")
    for TriMass in FullTriMass:
        DirName,AddressName=TriName(TriMass)
        checkList.write('ls %s_tarball.tar.xz\n'%DirName)
    checkList.close()


FullTriMass=[
['800','300','330'],
['900','350','385'],
['1000','400','440'],
['1100','450','495'],
['1200','500','550'],
['1300','550','605'],
['1400','600','660'],
['1500','650','715'],
['1600','700','770'],
['1700','750','825'],
['1800','800','880'],
['1900','850','935'],
['2000','900','990'],
['1000','400','420'],
['1100','450','475'],
['1200','500','525'],
['1300','550','580'],
['1400','600','630'],
['1500','650','685'],
['1600','700','735'],
['1700','750','790'],
['1800','800','840'],
['1900','850','895'],
['1000','400','460'],
['1100','450','520'],
['1200','500','575'],
['1300','550','635'],
['1400','600','690'],
['1500','650','750'],
['1600','700','805'],
['1700','750','865'],
['1800','800','920'],
['1900','850','975'],
['1200','600','600'],
['1300','600','600'],
['1400','600','600'],
['1500','600','600'],
['1600','600','600'],
['1700','600','600'],
['1800','600','600']
]



for TriMass in FullTriMass:
    TriMass.append(generation)
    create_Directory(TriMass)
    create_run_card(TriMass)
    create_extramodels(TriMass)
    create_proc_card(TriMass)
    create_customizecards_card(TriMass)
create_gridpack_generation_script(FullTriMass)
create_submit_gridpack_generation(FullTriMass)
create_submit_gridpack_generation_lxbatch(FullTriMass)
CheckList_GridPack(FullTriMass)



