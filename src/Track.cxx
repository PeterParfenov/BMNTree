#include <iostream>

#include "Track.h"
#include "TRandom.h"

ClassImp(Track);

Track::Track(const Track &orig) : TObject(orig), fTriggerBits(orig.fTriggerBits)
{
  fPdg = orig.fPdg;
  fPvect[0] = orig.fPvect[0];
  fPvect[1] = orig.fPvect[1];
  fPvect[2] = orig.fPvect[2];
  fPvect[3] = orig.fPvect[3];
  fMass = orig.fMass;
  fCharge = orig.fCharge;

  fVertex[0] = orig.fVertex[0];
  fVertex[1] = orig.fVertex[1];
  fVertex[2] = orig.fVertex[2];
  fVertex[3] = orig.fVertex[3];
}

////////////////////////////////////////////////////////////////////////////////
/// Create a track object.

Track::Track(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
             Float16_t mass, Int_t charge)
{
  Set(pdg, E, px, py, pz, mass, charge);
}

////////////////////////////////////////////////////////////////////////////////
/// Copy a track

Track &Track::operator=(const Track &orig)
{
  TObject::operator=(orig);
  fPdg = orig.fPdg;
  fPvect[0] = orig.fPvect[0];
  fPvect[1] = orig.fPvect[1];
  fPvect[2] = orig.fPvect[2];
  fPvect[3] = orig.fPvect[3];
  fMass = orig.fMass;
  fCharge = orig.fCharge;

  fVertex[0] = orig.fVertex[0];
  fVertex[1] = orig.fVertex[1];
  fVertex[2] = orig.fVertex[2];
  fVertex[3] = orig.fVertex[3];

  fTriggerBits = orig.fTriggerBits;

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Note that we intend on using TClonesArray::ConstructedAt, so we do not
/// need to delete any of the arrays.

void Track::Clear(Option_t * /*option*/)
{
  TObject::Clear();
  fTriggerBits.Clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Set the values of the Track data members.

void Track::Set(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
                Float16_t mass, Int_t charge)
{
  fPdg = pdg;
  fPvect[0] = E;
  fPvect[1] = px;
  fPvect[2] = py;
  fPvect[3] = pz;
  fMass = mass;
  fCharge = charge;

  // fVertex[0] = t;
  // fVertex[1] = vx;
  // fVertex[2] = vy;
  // fVertex[3] = vz;
}

////////////////////////////////////////////////////////////////////////////////

void Track::Print(Option_t *option)
{
  // Print the data members to the standard output
  std::cout << "------------------------------------------------" << std::endl
            << "-I-                   Track                  -I-" << std::endl
            << "PDG code                         : " << fPdg << std::endl
            << "Charge                           : " << fCharge << std::endl
            << "Mass                             : " << fMass << std::endl
            << "Momentum {E, px, py, pz} (GeV/c) : {" << fPvect[0] << ", " << fPvect[1] << ", " << fPvect[2] << ", " << fPvect[3] << "}" << std::endl
            << "Position {t, x, y, z} (fm)       : {" << fVertex[0] << ", " << fVertex[1] << ", " << fVertex[2] << ", " << fVertex[3] << "}" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
