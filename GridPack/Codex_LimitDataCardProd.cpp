#include <string>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include "boost/filesystem.hpp"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/HttSystematics.h"
#include "CombineHarvester/CombineTools/interface/CardWriter.h"
#include "CombineHarvester/CombineTools/interface/CopyTools.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"


using namespace std;

int main() {
//! [part1]
// First define the location of the "auxiliaries" directory where we can
// source the input files containing the datacard shapes
string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/shapes/";

// Create an empty CombineHarvester instance that will hold all of the
// datacard configuration and histograms etc.
ch::CombineHarvester cb;

typedef vector<pair<int, string>> Categories;
typedef vector<string> VString;
// Uncomment this next line to see a *lot* of debug information
// cb.SetVerbosity(3);

// Here we will just define two categories for an 8TeV analysis. Each entry in
// the vector below specifies a bin name and corresponding bin_id.

VString chns = { "mj"};

map<string, string> input_folders = {
//   {"et", "."},
{"mj", "."}
};

map<string, VString> bkg_procs;
// bkg_procs["et"] = {"ZTT", "W", "QCD", "TT","VV","SingleTop"};
bkg_procs["mj"] = {"ZTT", "W", "QCD", "TT","VV","SingleTop"};

VString sig_procs = {"Codex_"};

map<string, Categories> cats;
//cats["et_13TeV"] = {
//  {1, "EleTau_DiJet"}};

cats["mj_13TeV"] = {
{1, "MuJet"}};

// ch::Categories is just a typedef of vector<pair<int, string>>
//! [part1]


//! [part2]
vector<string> masses = ch::MassesFromRange("1000-1800:400");
//vector<string> masses = ch::MassesFromRange("1-54:1");
// Or equivalently, specify the mass points explicitly:
//    vector<string> masses = {"120", "125", "130", "135"};
//! [part2]


//! [part4]

for (auto chn : chns) {
cb.AddObservations(
{"*"}, {"Codex_"}, {"13TeV"}, {chn}, cats[chn+"_13TeV"]);
cb.AddProcesses(
{"*"}, {"Codex_"}, {"13TeV"}, {chn}, bkg_procs[chn], cats[chn+"_13TeV"], false);
cb.AddProcesses(
masses, {"Codex_"}, {"13TeV"}, {chn}, sig_procs, cats[chn+"_13TeV"], true);
}




cout << ">> Extracting histograms from input root files...\n";
for (string era : {"13TeV"}) {
for (string chn : chns) {

string file = aux_shapes + input_folders[chn] + "/_mj_LQMass_MT200_MET400_Iso.root";
cb.cp().channel({chn}).era({era}).backgrounds().ExtractShapes(
file, "$BIN/$PROCESS", "$BIN/$PROCESS_$SYSTEMATIC");
cb.cp().channel({chn}).era({era}).signals().ExtractShapes(
file, "$BIN/$PROCESS$MASS", "$BIN/$PROCESS$MASS_$SYSTEMATIC");
}
}


//Some of the code for this is in a nested namespace, so
// we'll make some using declarations first to simplify things a bit.
using ch::syst::SystMap;
using ch::syst::era;
using ch::syst::bin_id;
using ch::syst::process;
using ch::JoinStr;


//! [part5]
cb.cp().process(ch::JoinStr({sig_procs, {"ZTT", "W", "TT","VV","SingleTop"}}))
.AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
({"7TeV"}, 1.022)
({"13TeV"}, 1.062));
//! [part5]

//! [part6]
//  cb.cp().process({sig_procs})
//      .AddSyst(cb, "pdf_RHW", "lnN", SystMap<>::init(1.10));
//cb.cp().signals()
//    .AddSyst(cb, "pdf_RHW", "lnN", SystMap<>::init(1.10));


cb.cp().channel({"mt"}).process(ch::JoinStr({sig_procs, {"ZTT", "W", "TT","VV","SingleTop"}}))
.AddSyst(cb, "CMS_trg_m", "lnN", SystMap<>::init(1.05));


cb.cp().channel({"mt"}).process(ch::JoinStr({sig_procs, {"ZTT", "W", "TT","VV","SingleTop"}}))
.AddSyst(cb, "CMS_eff_m", "lnN", SystMap<>::init(1.02));




cb.cp().channel({"mt"}).process(JoinStr({sig_procs, {"ZTT", "W", "TT","VV","SingleTop"}}))
.AddSyst(cb, "CMS_scale_j", "shape", SystMap<>::init(1.00));


cb.cp().channel({"mt"}).process(JoinStr({ { "TT"}}))
.AddSyst(cb, "CMS_top_pt_Reweighting", "shape", SystMap<>::init(1.00));

cb.cp().channel({"et"}).process(JoinStr({ { "TT"}}))
.AddSyst(cb, "CMS_top_pt_Reweighting", "shape", SystMap<>::init(1.00));



cb.cp().process({"ZTT"})
.AddSyst(cb, "CMS_htt_ZTTNorm", "lnN", SystMap<>::init(1.10));

cb.cp().process({"TT"})
.AddSyst(cb, "CMS_htt_TTNorm", "lnN", SystMap<>::init(1.05));

cb.cp().process({"VV"})
.AddSyst(cb, "CMS_htt_DibosonNorm", "lnN", SystMap<>::init(1.15));

cb.cp().process({"SingleTop"})
.AddSyst(cb, "CMS_htt_SingleTopNorm", "lnN", SystMap<>::init(1.15));

cb.cp().process({"W"})
.AddSyst(cb, "CMS_htt_WNorm", "lnN", SystMap<>::init(1.20));

cb.cp().process({"QCD"})
.AddSyst(cb, "CMS_htt_QCDNorm", "lnN", SystMap<>::init(1.20));

cb.cp().process(sig_procs)
.AddSyst(cb, "CMS_signal_PDF", "lnN", SystMap<>::init(1.05));



cb.cp()
.AddSyst(cb,
"CMS_scale_met_$ERA", "lnN", SystMap<era, bin_id, process>::init
({"13TeV"}, {1},     {"Codex_"},        1.05)
({"13TeV"}, {1},     {"ZTT"},        1.05)
({"13TeV"}, {1},     {"TT"},         1.05)
({"13TeV"}, {1},     {"W"},        1.05)
({"13TeV"}, {1},     {"VV"},        1.05)
({"13TeV"}, {1},     {"SingleTop"},        1.05));





cout << ">> Adding systematic uncertainties...\n";
// ch::AddSystematics_et_mt(cb);

//cb.cp().process(ch::JoinStr({sig_procs, {"ZTT"}}))
//    .AddSyst(cb, "CMS_scale_t_mutau_$ERA", "shape", SystMap<>::init(1.00));
//! [part6]

//! [part7]
cb.cp().channel({"mt"}).backgrounds().ExtractShapes(
aux_shapes + "/_mj_LQMass_MT200_MET400_Iso.root",
"$BIN/$PROCESS",
"$BIN/$PROCESS_$SYSTEMATIC");
cb.cp().channel({"mt"}).signals().ExtractShapes(
aux_shapes + "/_mj_LQMass_MT200_MET400_Iso.root",
"$BIN/$PROCESS$MASS",
"$BIN/$PROCESS$MASS_$SYSTEMATIC");




//! [part7]

//! [part8]
auto bbb = ch::BinByBinFactory()
.SetAddThreshold(0.1)
.SetFixNorm(true);

bbb.AddBinByBin(cb.cp().backgrounds(), cb);

// This function modifies every entry to have a standardised bin name of
// the form: {analysis}_{channel}_{bin_id}_{era}
// which is commonly used in the htt analyses
ch::SetStandardBinNames(cb);
//! [part8]

//! [part9]
// First we generate a set of bin names:
set<string> bins = cb.bin_set();
// This method will produce a set of unique bin names by considering all
// Observation, Process and Systematic entries in the CombineHarvester
// instance.

// We create the output root file that will contain all the shapes.
//TFile output("RHW_mt.inputs.root", "RECREATE");

// Finally we iterate through each bin,mass combination and write a


auto pre_drop = cb.syst_name_set();
//cb.syst_name(droplist, false);
auto post_drop = cb.syst_name_set();
cout << ">> Systematics dropped: " << pre_drop.size() - post_drop.size()
<< "\n";


string folder = "outputCodex/Codex_MT200_MET400/LIMITS";
boost::filesystem::create_directories(folder);
boost::filesystem::create_directories(folder + "/common");
for (auto m : masses) {
boost::filesystem::create_directories(folder + "/" + m);
}

for (string chn : chns) {
TFile output((folder + "/common/Codex_" + chn + ".input.root").c_str(),
"RECREATE");
auto bins = cb.cp().channel({chn}).bin_set();
for (auto b : bins) {
for (auto m : masses) {
cout << ">> Writing datacard for bin: " << b << " and mass: " << m
<< "\r" << flush;
cb.cp().channel({chn}).bin({b}).mass({m, "*"}).WriteDatacard(
folder + "/" + m + "/" + b + ".txt", output);
}
}
output.Close();
}


//! [part9]

}
