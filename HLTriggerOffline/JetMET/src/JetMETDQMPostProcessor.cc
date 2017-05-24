// Migrated to use DQMEDHarvester by: Jyothsna Rani Komaragiri, Oct 2014

#include "HLTriggerOffline/JetMET/interface/JetMETDQMPostProcessor.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include <iostream>
#include <string.h>
#include <iomanip>
#include<fstream>
#include <math.h>


JetMETDQMPostProcessor::JetMETDQMPostProcessor(const edm::ParameterSet& pset)
{
  subDir_ = pset.getUntrackedParameter<std::string>("subDir");
  patternJetTrg_ = pset.getUntrackedParameter<std::string>("PatternJetTrg","");
  patternMetTrg_ = pset.getUntrackedParameter<std::string>("PatternMetTrg","");
  JetResponse_   = pset.getUntrackedParameter<std::string>("JetResponseTag","");
}

void JetMETDQMPostProcessor::dqmEndJob(DQMStore::IBooker& ibooker, DQMStore::IGetter& igetter)
{
 //////////////////////////////////
  // setup DQM stor               //
  //////////////////////////////////

  bool isJetDir = false;
  bool isMetDir = false;
  bool isJetRespDir = false;

  TPRegexp patternJet(patternJetTrg_);
  TPRegexp patternMet(patternMetTrg_);

  //go to the directory to be processed
  if(igetter.dirExists(subDir_)) ibooker.cd(subDir_);
  else {
    edm::LogWarning("JetMETDQMPostProcessor") << "cannot find directory: " << subDir_ << " , skipping";
    return;
  }
  
  std::vector<std::string> subdirectories = igetter.getSubdirs();
  for(std::vector<std::string>::iterator dir = subdirectories.begin() ;dir!= subdirectories.end(); dir++ ){

    ibooker.cd(*dir);

    isJetDir = false;
    isMetDir = false;
    isJetRespDir = false;

    if (TString(*dir).Contains(patternJet))   isJetDir = true;
    if (TString(*dir).Contains(patternMet))   isMetDir = true;
    if (TString(*dir).Contains(JetResponse_)) isJetRespDir = true;
    
    if (isMetDir) {

      //std::cout << "JetMETDQMPostProcessor - Met paths: " << ibooker.pwd() << " " << *dir << std::endl;
      
      //GenMET
      dividehistos(ibooker, igetter, "_meGenMETTrgMC", "_meGenMET",        "_meTurnOngMET",    "Gen Missing ET",    "Gen Missing ET Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meGenMETTrg",   "_meGenMETTrgLow",  "_meTurnOngMETLow", "Gen Missing ETLow", "Gen Missing ET Turn-On Data");

      //HLTMET
      dividehistos(ibooker, igetter, "_meHLTMETTrgMC", "_meHLTMET",       "_meTurnOnhMET",    "HLT Missing ET",    "HLT Missing ET Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meHLTMETTrg",   "_meHLTMETTrgLow", "_meTurnOnhMETLow", "HLT Missing ETLow", "HLT Missing ET Turn-On Data");
      
    }
    
    if (isJetDir) {

      //std::cout << "JetMETDQMPostProcessor - Jet paths: " << ibooker.pwd() << " " << *dir << std::endl;
      
      //GenJets
      dividehistos(ibooker, igetter, "_meGenJetPtTrgMC", "_meGenJetPt", "_meTurnOngJetPt", "Gen Jet Pt", "Gen Jet Pt Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meGenJetPtTrg", "_meGenJetPtTrgLow", "_meTurnOngJetPt", "Gen Jet PtLow", "Gen Jet Pt Turn-On Data");
      dividehistos(ibooker, igetter, "_meGenJetEtaTrgMC", "_meGenJetEta", "_meTurnOngJetEta", "Gen Jet Eta", "Gen Jet Eta Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meGenJetEtaTrg", "_meGenJetEtaTrgLow", "_meTurnOngJetEta", "Gen Jet EtaLow", "Gen Jet Eta Turn-On Data");
      dividehistos(ibooker, igetter, "_meGenJetPhiTrgMC", "_meGenJetPhi", "_meTurnOngJetPhi", "Gen Jet Phi", "Gen Jet Phi Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meGenJetPhiTrg", "_meGenJetPhiTrgLow", "_meTurnOngJetPhi", "Gen Jet PhiLow", "Gen Jet Phi Turn-On Data");

      //HLTJets
      dividehistos(ibooker, igetter, "_meHLTJetPtTrgMC", "_meHLTJetPt", "_meTurnOnhJetPt", "HLT Jet Pt", "HLT Jet Pt Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meHLTJetPtTrg", "_meHLTJetPtTrgLow", "_meTurnOnhJetPt", "HLT Jet PtLow", "HLT Jet Pt Turn-On Data");
      dividehistos(ibooker, igetter, "_meHLTJetEtaTrgMC", "_meHLTJetEta", "_meTurnOnhJetEta", "HLT Jet Eta", "HLT Jet Eta Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meHLTJetEtaTrg", "_meHLTJetEtaTrgLow", "_meTurnOnhJetEta", "HLT Jet EtaLow", "HLT Jet Eta Turn-On Data");
      dividehistos(ibooker, igetter, "_meHLTJetPhiTrgMC", "_meHLTJetPhi", "_meTurnOnhJetPhi", "HLT Jet Phi", "HLT Jet Phi Turn-On RelVal");
      dividehistos(ibooker, igetter, "_meHLTJetPhiTrg", "_meHLTJetPhiTrgLow", "_meTurnOnhJetPhi", "HLT Jet PhiLow", "HLT Jet Phi Turn-On Data");

    }
    if (isJetRespDir){
      //HLTJets
      profilehistos(ibooker, igetter, "_meHLTGenJetResVsGenJetPt",  "JetResponseProfile", "GenJet Pt[GeV]", "Jet Response");
      profilehistos(ibooker, igetter, "_meHLTGenJetResVsGenJetPtHEP17",  "JetResponseHEP17Profile", "GenJet Pt[GeV]", "Jet Response(HEP17)");
      profilehistos(ibooker, igetter, "_meHLTGenJetResVsGenJetPtHEM17",  "JetResponseHEM17Profile", "GenJet Pt[GeV]", "Jet Response(HEM17)");
      profilehistos(ibooker, igetter, "_meHLTGenJetResVsGenJetPtHEP18",  "JetResponseHEP18Profile", "GenJet Pt[GeV]", "Jet Response(HEP18)");

    }
    
    ibooker.goUp();         
  }
}

//----------------------------------------------------------------------
TProfile* 
JetMETDQMPostProcessor::dividehistos(DQMStore::IBooker & ibooker, DQMStore::IGetter & igetter, const std::string& numName, const std::string& denomName, 
				     const std::string& outName, const std::string& label, const std::string& titel)
{
  //ibooker.pwd();
  //std::cout << "In dividehistos: " << ibooker.pwd() << std::endl;
  
  //std::cout << numName <<std::endl;
  TH1F* num = getHistogram(ibooker, igetter, ibooker.pwd()+"/"+numName);

  //std::cout << denomName << std::endl;
  TH1F* denom = getHistogram(ibooker, igetter, ibooker.pwd()+"/"+denomName);

  if (num == NULL)
    edm::LogWarning("JetMETDQMPostProcessor") << "numerator histogram " << ibooker.pwd()+"/"+numName << " does not exist";
  if (denom == NULL)
    edm::LogWarning("JetMETDQMPostProcessor") << "denominator histogram " << ibooker.pwd()+"/"+denomName << " does not exist";

  // Check if histograms actually exist
  if(!num || !denom) return 0;

  MonitorElement* meOut = ibooker.bookProfile(outName, titel, num->GetXaxis()->GetNbins(), num->GetXaxis()->GetXmin(), num->GetXaxis()->GetXmax(),0.,1.2);
  meOut->setEfficiencyFlag();
  TProfile* out = meOut->getTProfile();
  out->GetXaxis()->SetTitle(label.c_str());
  out->SetYTitle("Efficiency");
  out->SetOption("PE");
  out->SetLineColor(2);
  out->SetLineWidth(2);
  out->SetMarkerStyle(20);
  out->SetMarkerSize(0.8);
  out->SetStats(kFALSE);

  for(int i=1;i<=num->GetNbinsX();i++){
    double e, low, high;
    Efficiency( (int)num->GetBinContent(i), (int)denom->GetBinContent(i), 0.683, e, low, high );
    double err = e-low>high-e ? e-low : high-e;
    //here is the trick to store info in TProfile:
    out->SetBinContent( i, e );
    out->SetBinEntries( i, 1 );
    out->SetBinError( i, sqrt(e*e+err*err) );
  }
  return out;
}

TProfile* 
JetMETDQMPostProcessor::profilehistos(DQMStore::IBooker & ibooker, DQMStore::IGetter & igetter, const std::string& histName, const std::string& outName, const std::string& label, const std::string& titel)
{
  //ibooker.pwd();
  //std::cout << "In dividehistos: " << ibooker.pwd() << std::endl;
  

  TH2F* hSrc = get2DHistogram(ibooker, igetter, ibooker.pwd()+"/"+histName);
  if (hSrc == NULL)
    edm::LogWarning("JetMETDQMPostProcessor") << "hSrcerator histogram " << ibooker.pwd()+"/"+histName << " does not exist";
  TProfile* profile(hSrc->ProfileX());  
  profile->GetXaxis()->SetTitle(label.c_str());
//  ibooker.bookProfile(outName, profile->get());
std::unique_ptr<TProfile> test(hSrc->ProfileX());
test->SetTitle(titel.c_str());
//test->SetLabel(label.c_str());
ibooker.bookProfile(outName, test.get());
  return profile;
}
//----------------------------------------------------------------------
TH1F *
JetMETDQMPostProcessor::getHistogram(DQMStore::IBooker & ibooker, DQMStore::IGetter & igetter, const std::string &histoPath)
{
  ibooker.pwd();
  MonitorElement *monElement = igetter.get(histoPath);
  if (monElement != NULL)
    return monElement->getTH1F();
  else
    return NULL;
}
TH2F *
JetMETDQMPostProcessor::get2DHistogram(DQMStore::IBooker & ibooker, DQMStore::IGetter & igetter, const std::string &histoPath)
{
  ibooker.pwd();
  MonitorElement *monElement = igetter.get(histoPath);
  if (monElement != NULL)
    return monElement->getTH2F();
  else
    return NULL;
}
//----------------------------------------------------------------------
void
JetMETDQMPostProcessor::Efficiency(int passing, int total, double level, double &mode, double &lowerBound, double &upperBound)
{
  // protection
  if (total == 0)
    {
      mode = 0.5;
      lowerBound = 0;
      upperBound = 1;
      return;
    }
  mode = passing / ((double) total);

  // see http://root.cern.ch/root/html/TEfficiency.html#compare
  lowerBound = TEfficiency::Wilson(total, passing, level, false);
  upperBound = TEfficiency::Wilson(total, passing, level, true);
}

//----------------------------------------------------------------------

DEFINE_FWK_MODULE(JetMETDQMPostProcessor);
