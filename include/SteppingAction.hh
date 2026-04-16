//
// ********************************************************************
// 1.00 IS, FRM2  
// *                                                                  *
// ********************************************************************
//
//
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

// Per-step hook used to capture detector-hit information at geometry boundaries.
class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    virtual ~SteppingAction();
    
    // Called by Geant4 for every tracking step.
    // The implementation filters relevant charged reaction products,
    // extracts detector indexing information, and appends one output row.
    void UserSteppingAction(const G4Step*);
  
};

#endif
