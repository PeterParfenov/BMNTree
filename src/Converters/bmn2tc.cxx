#include <iostream>

#include "Event.h"
#include "Track.h"
#include "EventHeader.h"

#include <TChain.h>
#include <TTree.h>
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TClonesArray.h>
#include <TStopwatch.h>
#include <TDatabasePDG.h>
#include <TVector3.h>

#include "CbmMCTrack.h"
#include "CbmStsPoint.h"
#include "FairMCEventHeader.h"
#include "BmnZdcPoint.h"


#ifdef _ISCINT_
#include <Rtypes.h>
R__ADD_INCLUDE_PATH($VMCWORKDIR)
#include "macro/run/bmnloadlibs.C"
#endif

int main(int argc, char **argv)
{
   Int_t nevent = 400;   // by default create 400 events
   Int_t comp = 1;       // by default file is compressed
   Int_t split = 1;      // by default, split Event in sub branches
   Int_t enable_imt = 0; // Whether to enable IMT mode.
   Int_t verbosity = -0; // Verbosity level
#ifdef R__HAS_DEFAULT_LZ4
   Int_t compAlg = 4; // Allow user to specify underlying compression algorithm.
#else
   Int_t compAlg = 1;
#endif
   Int_t branchStyle = 1; //new style by default
   TString inFileName, outFileName;

   TDatabasePDG* pDB = new TDatabasePDG();
   TVector3 pPSDMomentum, pPSDPosition;

   if (argc < 4)
   {
      std::cerr << "./bmn2tc -i INPUTFILE -o OUTPUTFILE [OPTIONAL: -v 0,1,2 (verbosity level)]" << std::endl;
      return 10;
   }
   for (int i = 1; i < argc; i++)
   {
      if (std::string(argv[i]) != "-i" &&
          std::string(argv[i]) != "-o" &&
          std::string(argv[i]) != "-v")
      {
         std::cerr << "\nbmn2tc: Unknown parameter: " << argv[i] << std::endl;
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
            std::cerr << "\nbmn2tc: Input file name was not specified!" << std::endl;
            return 20;
         }
         if (std::string(argv[i]) == "-o" && i != argc - 1)
         {
            outFileName = argv[++i];
         }
         if (std::string(argv[i]) == "-o" && i == argc - 1)
         {
            std::cerr << "\nbmn2tc: Output file name was not specified!" << std::endl;
            return 21;
         }
         if (std::string(argv[i]) == "-v" && i != argc - 1)
         {
            verbosity = atof(argv[++i]);
         }
         if (std::string(argv[i]) == "-v" && i == argc - 1)
         {
            std::cerr << "\nbmn2tc: Verbosity!" << std::endl;
            return 20;
         }
      }
   }

   if (verbosity != 0 && verbosity != 1 && verbosity != 2){
      std::cerr << "\nbmn2tc: Incorrect verbosity level. Take it by default." << std::endl;
      verbosity = 0;
   }

   std::cout << "bmn2tc: Processing." << std::endl;

#ifdef _ISCINT_
    bmnloadlibs(); // load libraries
#endif

#ifdef R__USE_IMT
   if (enable_imt)
   {
      ROOT::EnableImplicitMT();
   }
#else
   if (enable_imt)
   {
      std::cerr << "IMT mode requested, but this version of ROOT "
                   "is built without IMT support."
                << std::endl;
      return 1;
   }
#endif

   TFile *hfile;
   TTree *tree;
   // TTreePerfStats *ioperf = nullptr;
   Event *event = 0;

   // Fill event, header and tracks with some random numbers
   //   Create a timer object to benchmark this loop
   TStopwatch timer;
   timer.Start();
   Long64_t nb = 0;
   Int_t ev;
   Int_t bufsize;
   Double_t told = 0;
   Double_t tnew = 0;
   Int_t printev = 100;

   //         Write case
   // Create a new ROOT binary machine independent file.
   // Note that this file may contain any kind of ROOT objects, histograms,
   // pictures, graphics objects, detector geometries, tracks, events, etc..
   // This file is now becoming the current directory.
   hfile = new TFile(outFileName.Data(), "RECREATE", "TTree benchmark ROOT file");
   hfile->SetCompressionLevel(comp);
   hfile->SetCompressionAlgorithm(compAlg);

   // Create histogram to show write_time in function of time
   Float_t curtime = -0.5;
   Int_t ntime = nevent / printev;

   // Create a ROOT Tree and one superbranch
   tree = new TTree("events", "An example of a ROOT tree");
   tree->SetAutoSave(1000000000); // autosave when 1 Gbyte written
   tree->SetCacheSize(10000000);  // set a 10 MBytes cache (useless when writing local files)
   bufsize = 64000;
   if (split)
      bufsize /= 4;
   event = new Event(); // By setting the value, we own the pointer and must delete it.
   TTree::SetBranchStyle(branchStyle);
   TBranch *branch = tree->Branch("event", &event, bufsize, split);
   branch->SetAutoDelete(kFALSE);
   if (split >= 0 && branchStyle)
      tree->BranchRef();
   Float_t ptmin = 1;

   Int_t ObjectNumber = 0;

   Int_t nMultPrim, nMultALL, ntracks,
          nMultPart, nMultSp, PDG, Charge, nModule;

   Double_t P, Pz, Pt, eta, Etot, Energy;

   TChain* inFile = new TChain("cbmsim");
   inFile->Add(inFileName.Data());
   std::cout << "#recorded entries = " << inFile->GetEntries() << std::endl;


   TClonesArray* stsTracks = nullptr;
   inFile->SetBranchAddress("MCTrack", &stsTracks);

   FairMCEventHeader* fairEvent = nullptr;
   inFile->SetBranchAddress("MCEventHeader.", &fairEvent);

   TClonesArray* listPSDpoints = nullptr;
   TBranch *bPSD = inFile->GetBranch("ZdcPoint");
   if (!bPSD) cout << "pb with branch PsdPoint ... " << endl;
   bPSD->SetAddress(&listPSDpoints);

   CbmMCTrack* mcTrack = nullptr;
   BmnZdcPoint* psdPoint = nullptr;

    for (Int_t iEv = 0; iEv < inFile->GetEntries(); iEv++) {
        inFile->GetEntry(iEv);

        std::cout << "\tEvent: " << iEv << "\t"
                  << timer.RealTime() << std::endl;
        timer.Continue();


        ntracks = stsTracks->GetEntriesFast();

        // hMultALL->Fill(ntracks);
        nMultPrim = 0;
        nMultALL  = 0;
        nMultPart = 0;
        nMultSp   = 0;

        for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
            mcTrack = (CbmMCTrack*) stsTracks->UncheckedAt(iTrack);
            PDG = mcTrack->GetPdgCode();

            // std::cout << "Track" << iTrack << "|" << PDG << " ";
            if (pDB->GetParticle(PDG))
              Charge = pDB->GetParticle(PDG)->Charge()/3;
            else
              Charge = 0;
            P = mcTrack->GetP();
            Pt = mcTrack->GetPt();
            Pz = mcTrack->GetPz();
            eta = 0.5*TMath::Log((P+Pz)/(P-Pz));
        }//Track loop

        bPSD->GetEntry(iEv);
        Etot = 0;
        for (Int_t ihit=0; ihit<listPSDpoints->GetEntries(); ihit++) {
	  psdPoint = (BmnZdcPoint*) listPSDpoints->At(ihit);
          if (!psdPoint) continue;
          psdPoint->Momentum(pPSDMomentum);
          psdPoint->Position(pPSDPosition);
          Energy = psdPoint->GetEnergyLoss();
          nModule = psdPoint->GetCopyMother();
          Etot += Energy;
        }//PSD loop
    }    //Event loop

   return 0;
}
