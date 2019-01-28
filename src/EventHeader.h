#ifndef EVENTHEADER_H
#define EVENTHEADER_H

#include "TROOT.h"

class EventHeader
{

private:
  Int_t fEvtNum;
  Int_t fRun;
  Int_t fDate;

public:
  EventHeader();
  virtual ~EventHeader();
  void Set(Int_t i, Int_t r, Int_t d)
  {
    fEvtNum = i;
    fRun = r;
    fDate = d;
  }
  void SetEvtNum(Int_t _a) { fEvtNum = _a; }
  void SetRun(Int_t _a) { fRun = _a; }
  void SetDate(Int_t _a) { fDate = _a; }

  Int_t GetEvtNum() const { return fEvtNum; }
  Int_t GetRun() const { return fRun; }
  Int_t GetDate() const { return fDate; }

  ClassDef(EventHeader, 1) //Event Header
};

#endif
