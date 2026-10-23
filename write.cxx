#include "MovingObject.h"
#include <TFile.h>
#include <TTree.h>
#include <cmath>
#include <stdexcept>

void write()
{
    MovingObject measurement;
    MovingObject* object = new MovingObject();

    TFile file("tree_file.root", "RECREATE");
    if (file.IsZombie()) {
        throw std::runtime_error("Cannot create tree_file.root");
    }


    TTree* tree = new TTree("tree", "Velocity measurements");
    tree->Branch("object", "MovingObject", &object);

    gRandom->SetSeed(12345);
    const Int_t nEvents = 10000;
    for (Int_t i{0}; i<nEvents; i++)
    {
        const Double_t vx = gRandom->Gaus(0.0, 0.30);
        const Double_t vy = gRandom->Gaus(0.0, 0.30);
        const Double_t vz = gRandom->Gaus(0.0, 0.30);

        object->SetVelocity(vx, vy, vz);
        tree->Fill();
    }

tree->Write();
file.Close();
delete object;






}