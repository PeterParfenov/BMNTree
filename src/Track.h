#ifndef TRACK_H
#define TRACK_H

#include "TROOT.h"
#include "TObject.h"
#include "TBits.h"
#include "TMath.h"

class Track : public TObject
{

private:
  Int_t fPdg;
  Double32_t fPvect[4]; //[e,px,py,pz] component of the momentum
  Float16_t fMass;      
  Int_t fCharge;        
  Double32_t fVertex[4]; 
  TBits fTriggerBits; //Bits triggered by this track.

public:
  Track() : fTriggerBits(64) {}
  Track(const Track &orig);
  Track(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
        Float16_t mass, Int_t charge);
  virtual ~Track() { Clear(); }
  Track &operator=(const Track &orig);

  void Set(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
           Float16_t mass, Int_t charge);
  void Clear(Option_t *option = "");
  void Print(Option_t *option = "");

  void SetPdg(Int_t a) { fPdg = a; }
  void SetPvect(Int_t i, Float_t a)
  {
    if (i < 4)
      fPvect[i] = a;
  }
  void SetMass(Float16_t a) { fMass = a; }
  void SetCharge(Int_t a) { fCharge = a; }
  void SetVertex(Int_t i, Double32_t a)
  {
    if (i < 4)
      fVertex[i] = a;
  }

  Int_t GetPdg() const { return fPdg; }
  Float_t GetPvect(Int_t i) const { return (i < 4) ? fPvect[i] : 0; }
  Float_t GetPt() const { return TMath::Sqrt(fPvect[1] * fPvect[1] + fPvect[2] * fPvect[2]); }
  Float16_t GetMass() const { return fMass; }
  Int_t GetCharge() const { return fCharge; }
  Double32_t GetVertex(Int_t i = 0) { return (i < 4) ? fVertex[i] : 0; }
  TBits &GetTriggerBits() { return fTriggerBits; }

  ClassDef(Track, 2) //A track segment
};

#endif
