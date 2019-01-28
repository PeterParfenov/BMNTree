////////////////////////////////////////////////////////////////////////
// Standard root-build/test/Event.* files were used as an example
////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "RVersion.h"
#include "TRandom.h"
#include "TDirectory.h"
#include "TProcessID.h"

#include "Event.h"

ClassImp(Event);

TClonesArray *Event::fgTracks = 0;

////////////////////////////////////////////////////////////////////////////////
/// Create an Event object.
/// When the constructor is invoked for the first time, the class static
/// variable fgTracks is 0 and the TClonesArray fgTracks is created.

Event::Event()
{
   if (!fgTracks)
      fgTracks = new TClonesArray("Track", 1000);
   fTracks = fgTracks;
   fHighPt = new TRefArray;
   fMuons = new TRefArray;
   fNtrack = 0;
}

////////////////////////////////////////////////////////////////////////////////

Event::~Event()
{
   Clear();
   delete fHighPt;
   fHighPt = 0;
   delete fMuons;
   fMuons = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// Add a new track to the list of tracks for this event.
/// To avoid calling the very time consuming operator new for each track,
/// the standard but not well know C++ operator "new with placement"
/// is called. If tracks[i] is 0, a new Track object will be created
/// otherwise the previous Track[i] will be overwritten.

Track *Event::AddTrack(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
                       Float16_t mass, Int_t charge, Float_t ptmin)
{
#if ROOT_VERSION_CODE >= ROOT_VERSION(5, 32, 0)
   Track *track = (Track *)fTracks->ConstructedAt(fNtrack++);
   track->Set(pdg, E, px, py, pz, mass, charge);
#else
   TClonesArray &tracks = *fTracks;
   Track *track = new (tracks[fNtrack++]) Track(pdg, px, py, pz, mass, charge);
#endif
   //Save reference to last Track in the collection of Tracks
   fLastTrack = track;
   //Save reference in fHighPt if track is a high Pt track
   if (track->GetPt() > ptmin)
      fHighPt->Add(track);
   //Save reference in fMuons if track is a muon candidate
   if (track->GetMass() < TMath::Sqrt(0.11))
      fMuons->Add(track);
   return track;
}

////////////////////////////////////////////////////////////////////////////////

void Event::Clear(Option_t * /*option*/)
{
   fTracks->Clear("C"); //will also call Track::Clear
   fHighPt->Delete();
   fMuons->Delete();
   fTriggerBits.Clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Static function to reset all static objects for this event
///   fgTracks->Delete(option);

void Event::Reset(Option_t * /*option*/)
{
   delete fgTracks;
   fgTracks = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Event::SetHeader(Int_t i, Int_t run, Int_t date)
{
   fNtrack = 0;
   fEvtHdr.Set(i, run, date);
}

////////////////////////////////////////////////////////////////////////////////

void Event::Print(Option_t *option)
{
   // Print data members to the standard output
   std::cout << "________________________________________________" << std::endl
             << "-I-                  Event                   -I-" << std::endl
             << "Event number               : " << fEvtHdr.GetEvtNum() << std::endl
             << "Impact parameter (fm)      : " << fB << std::endl
             << "Number of particles        : " << fNtrack << std::endl;
   TString opt = option;
   if (opt.Contains("all"))
   {
      Track *track;
      for (Int_t iPa = 0; iPa < fNtrack; iPa++)
      {
         track = (Track *)fTracks->UncheckedAt(iPa);
         track->Print(option);
         std::cout << "Track number in TClonesArray  : " << iPa << std::endl;
      }
      std::cout << "------------------------------------------------" << std::endl;
   }
   std::cout << "________________________________________________" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
