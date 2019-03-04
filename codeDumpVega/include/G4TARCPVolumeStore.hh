#ifndef G4TARCPVOLUMESTORE_HH
#define G4TARCPVOLUMESTORE_HH

#include "globals.hh"
#include <set>
#include <sstream>
#include "G4GeometryCell.hh"
#include "G4GeometryCellComp.hh"
#include "G4VPhysicalVolume.hh"

typedef std::set <G4GeometryCell, G4GeometryCellComp> G4TARCSetGeometryCell;

class G4TARCPVolumeStore {
public:
  G4TARCPVolumeStore(){};
  ~G4TARCPVolumeStore(){};

public:
  void AddPVolume(const G4GeometryCell&);
  const G4VPhysicalVolume* GetPVolume(const G4String&) const;
  inline G4int Size() { return fSetGeometryCell.size(); }
  G4String GetPNames() const;
private:
  G4TARCSetGeometryCell fSetGeometryCell;
};


#endif
