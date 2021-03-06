
/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                    AliGeoCave  file                               -----
// -----                Created 26/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------


#ifndef ALICEO2_PASSIVE_GEOCAVE_H
#define ALICEO2_PASSIVE_GEOCAVE_H

#include "FairGeoSet.h"  // for FairGeoSet
#include "Rtypes.h"      // for GeoCave::Class, Bool_t, ClassDef, etc
#include "TString.h"     // for TString
#include <fstream>                      // for fstream

class FairGeoMedia;
namespace AliceO2 {
namespace Passive {

class  GeoCave : public FairGeoSet
{
  protected:
    TString name;
  public:
    GeoCave();
    ~GeoCave() {}
    const char* getModuleName(Int_t) {return name.Data();}
    Bool_t read(std::fstream&,FairGeoMedia*);
    void addRefNodes();
    void write(std::fstream&);
    void print();
    ClassDef(AliceO2::Passive::GeoCave,0) // Class for the geometry of CAVE
};
}
}
#endif  /* !PNDGEOCAVE_H */
