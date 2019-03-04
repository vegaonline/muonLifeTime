#include "G4TARCPVolumeStore.hh"

void G4TARCPVolumeStore::AddPVolume(const G4GeometryCell& thisCell) {
  G4TARCSetGeometryCell::iterator it = fSetGeometryCell.find(thisCell);
  if (it !=fSetGeometryCell.end()) {
    G4cout << " G4TARCPVolumeStore::AddPVolume: cell " <<  it->GetPhysicalVolume().GetName() << " already store" << G4endl;
    return;
  }
  fSetGeometryCell.insert(thisCell);
}

const G4VPhysicalVolume* G4TARCPVolumeStore::GetPVolume(const G4String& name) const {
  const G4VPhysicalVolume* pvol = 0;
  for (G4TARCSetGeometryCell::const_iterator it = fSetGeometryCell.begin(); it != fSetGeometryCell.end(); ++it) {
    const G4VPhysicalVolume& vol = it->GetPhysicalVolume();
    //G4cout << " Looking at volume: " << vol.GetName() << " compared to: " << name << G4endl;
    if (vol.GetName() == name)
      pvol = &vol;
  }
  if (!pvol) {
    //G4cout << "G4TARCPVolumeStore::GetPVolume: no physical volume named: " << name << ", found" << G4endl;
  }
  return pvol;
}


G4String G4TARCPVolumeStore::GetPNames() const {
  G4String NameString;
  for (G4TARCSetGeometryCell::const_iterator it = fSetGeometryCell.begin(); it !=fSetGeometryCell.end(); ++it) {
    const G4VPhysicalVolume& vol = it->GetPhysicalVolume();
    std::ostringstream os;
    os << vol.GetName() << "_" << it->GetReplicaNumber() << "\n";
    G4String cellname = os.str();
    NameString += cellname;
  }
  return NameString;
}
