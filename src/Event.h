#ifndef Event_H
#define Event_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Event                                                                //
//                                                                      //
// Description of the event and track parameters                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TRef.h"
#include "TBits.h"
#include "TMath.h"

#include "Track.h"
#include "EventHeader.h"

class TDirectory;

class Event : public TObject
{

private:
  Double_t fB;
  Int_t fNtrack; //Number of tracks
  EventHeader fEvtHdr;
  TClonesArray *fTracks; //->array with all tracks
  TRefArray *fHighPt;    //array of High Pt tracks only
  TRefArray *fMuons;     //array of Muon tracks only
  TRefArray *fGEMS;      //array of GEM tracks only
  TRef fLastTrack;       //reference pointer to last track
  TBits fTriggerBits;    //Bits triggered by this event.

  static TClonesArray *fgTracks;

  Event(const Event &) = delete;
  Event &operator=(const Event &) = delete;

public:
  Event();
  virtual ~Event();
  // void Build(Int_t ev, Int_t arg5 = 600, Float_t ptmin = 1);
  void Clear(Option_t *option = "");
  static void Reset(Option_t *option = "");
  void Print(Option_t *option = "");

  void SetB(Double_t a) { fB = a; };
  void SetHeader(Int_t i, Int_t run, Int_t date);
  Track *AddTrack(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
                  Float16_t mass, Int_t charge, Float_t ptmin);

  Track *GetTrack(Int_t i) const { return ((Track *)fTracks->UncheckedAt(i)); }
  Int_t GetNtrack() const { return fNtrack; }
  Double_t GetB() const { return fB; }
  EventHeader *GetHeader() { return &fEvtHdr; }
  TClonesArray *GetTracks() const { return fTracks; }
  TRefArray *GetHighPt() const { return fHighPt; }
  TRefArray *GetMuons() const { return fMuons; }
  Track *GetLastTrack() const { return (Track *)fLastTrack.GetObject(); }
  TBits &GetTriggerBits() { return fTriggerBits; }

  ClassDef(Event, 1) //Event structure
};

#endif
