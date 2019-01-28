#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStopwatch.h"

#include "Event.h"
#include "Track.h"

int main(int argc, char** argv)
{
  TStopwatch timer;
  timer.Start();

  TString inFileName, outFileName;
  std::cout << "ReadEvent: Processing." << std::endl;

  if (argc < 4)
   {
      std::cerr << "./urqmd2tc -i INPUTFILE -o OUTPUTFILE" << std::endl;
      return 10;
   }
   for (int i = 1; i < argc; i++)
   {
      if (std::string(argv[i]) != "-i" &&
          std::string(argv[i]) != "-o")
      {
         std::cerr << "\nurqmd2tc: Unknown parameter: " << argv[i] << std::endl;
         return 11;
      }
      else
      {
         if (std::string(argv[i]) == "-i" && i != argc - 1)
         {
            inFileName = argv[++i];
         }
         if (std::string(argv[i]) == "-i" && i == argc - 1)
         {
            std::cerr << "\nurqmd2tc: Input file name was not specified!" << std::endl;
            return 20;
         }
         if (std::string(argv[i]) == "-o" && i != argc - 1)
         {
            outFileName = argv[++i];
         }
         if (std::string(argv[i]) == "-o" && i == argc - 1)
         {
            std::cerr << "\nurqmd2tc: Output file name was not specified!" << std::endl;
            return 21;
         }
      }
   }


  const int diag_name[] = {0, 12, 17, 27, 37, 47, 57, 67, 77};
  const int Ndiags = 3;

  TH1F *hMult = new TH1F("hMult", "Charge particle multiplicity", 1000, 0, 1000.); hMult->Sumw2();

  TH1F *hRapidity[Ndiags];
  TH1F *hEta[Ndiags];
  TH1F *hPt[Ndiags];
  TH1F* hMultF = new TH1F("hMultF","Forward charged particle multiplicity",150,0,150); hMultF->Sumw2();
  TH1F* hMultB = new TH1F("hMultB","Backward charged particle multiplicity",150,0,150); hMultB->Sumw2();
  TH1F* hMultNormF = new TH1F("hMultNormF","Normalized forward charged particle multiplicity",150,0,150); hMultF->Sumw2();

  Int_t n[Ndiags], nMultF, nMultB, QF, QB;
  Int_t nMultF1, nMultB1, QF1, QB1;

  for (int i = 0; i < Ndiags; i++)
  {
    hRapidity[i] = new TH1F(Form("hRapidity%i", i),
                            Form("rapidity yild of all particles %i;y;#frac{1}{N}#frac{dN}{dy}", diag_name[i]),
                            200, -10., 10.);
    hEta[i] = new TH1F(Form("hEta%i", i),
                       Form("#eta yild of all particles %i;#eta;#frac{1}{N}#frac{dN}{d#eta}", diag_name[i]),
                       200, -10., 10.);
    hPt[i] = new TH1F(Form("hPt%i", i),
                      Form("p_{T}-differential yild of charged hadrons %i;p_{T}, [GeV/c];#frac{1}{2#pi p_{T}} #frac{dN}{dp_{T}}", diag_name[i]), 100, 0., 5.);
    n[i] = 0;
  }

  TFile *fi = new TFile(inFileName.Data(), "read");
  TTree *tree = (TTree *)fi->Get("events");

  Event *ev = new Event();
  tree->SetBranchAddress("event", &ev);

  Int_t nentries = tree->GetEntriesFast();
  Int_t ntracks;
  Double_t rapidity, eta, mom, massT;
  Int_t nChargeCounter;

  for (int i = 0; i < nentries; i++)
  {
    nChargeCounter = 0;
    nMultF = 0; nMultF1 = 0;
    nMultB = 0; nMultB1 = 0;
    QF = 0; QF1 = 0;
    QB = 0; QB1 = 0;
    tree->GetEntry(i);
    if (i % 1000 == 0)
    {
      std::cout << "Real Time: " << timer.RealTime() << std::endl;
      timer.Continue();
      ev->Print();
    }

    ntracks = ev->GetNtrack();
    if (ntracks == 0) continue;

    for (int j = 0; j < ntracks; j++)
    {
      mom = TMath::Sqrt(ev->GetTrack(j)->GetPvect(3) * ev->GetTrack(j)->GetPvect(3) + ev->GetTrack(j)->GetPt() * ev->GetTrack(j)->GetPt());
      massT = TMath::Sqrt(ev->GetTrack(j)->GetPt()*ev->GetTrack(j)->GetPt() + ev->GetTrack(j)->GetMass()*ev->GetTrack(j)->GetMass());
      rapidity = 0.5 * TMath::Log((ev->GetTrack(j)->GetPvect(0) + ev->GetTrack(j)->GetPvect(3)) / (ev->GetTrack(j)->GetPvect(0) - ev->GetTrack(j)->GetPvect(3)));
      // rapidity = TMath::Log(ev->GetTrack(j)->GetPvect(0)+ev->GetTrack(j)->GetPvect(3)) - TMath::Log(massT);
      // eta = 0.5 * TMath::Log((mom + ev->GetTrack(j)->GetPvect(3)) / (mom - ev->GetTrack(j)->GetPvect(3)));
      eta = TMath::Log(mom+ev->GetTrack(j)->GetPvect(3)) - TMath::Log(ev->GetTrack(j)->GetPt());

      if (ev->GetTrack(j)->GetCharge() != 0){
        nChargeCounter++;
        if (eta>0 /*&& eta<4*/) { nMultF++; QF+= ev->GetTrack(j)->GetCharge(); }
        if (eta<0 /*&& eta>-4*/) { nMultB++; QB += ev->GetTrack(j)->GetCharge(); }
      }
      // if (TMath::Abs(eta) >= 2.4)
      //  continue;
      hEta[0]->Fill(eta);
      hRapidity[0]->Fill(rapidity);
      // if (TMath::Abs(eta) < 2.4)
      if (TMath::Abs(eta) < 2.4 && ev->GetTrack(j)->GetCharge() != 0) hPt[0]->Fill(ev->GetTrack(j)->GetPt(), 1. / (2 * TMath::Pi() * ev->GetTrack(j)->GetPt()*(2.4 + 2.4)));
      // for (int iDiag = 1; iDiag < Ndiags; iDiag++)
      // {
      // }
    }
    hMult->Fill(nChargeCounter);
    hMultF->Fill(nMultF);
    hMultNormF->Fill(nMultF,nMultB);
    hMultB->Fill(nMultB);
  }

  TFile *fo = new TFile(outFileName.Data(), "recreate");
  fo->cd();
  hMult->Write();
  hMultF->Write();
  hMultNormF->Write();
  hMultB->Write();
  for (int i = 0; i < Ndiags; i++)
  {
    hEta[i]->Write();
    hRapidity[i]->Write();
    hPt[i]->Write();
  }

  fo->Close();
  fi->Close();

  timer.Stop();
  timer.Print();

  return 0;
}
