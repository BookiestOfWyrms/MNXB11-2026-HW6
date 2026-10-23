#include "MovingObject.h"
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TROOT.h>
#include <iostream>



void read()
{
    // Open the tree file:
    TFile file("tree_file.root", "READ");
    if (file.IsZombie()) {
        std::cerr << "Could not open tree_file.root\n";
        return;
    }

    TTree* tree = nullptr;
    file.GetObject("tree", tree);
    if (tree == nullptr) {
        std::cerr << "Could not find the tree named tree\n";
        return;
    }

    tree->Print()

    MovingObject measurement;
    MovingObject* object = &measurement;
    if (tree->SetBranchAddress("object", &object) < 0) {
        std::cerr << "Could not connect the object branch\n";
        return;
    }

    const Long64_t nEntries = tree->GetEntries();
    std::cout << "Entries read: " << nEntries << '\n';


    TH2D components("hComponents",
                    "Velocity components;v_{x} (m/s);v_{y} (m/s);Measurements",
                    100, -1.5, 1.5, 100, -1.5, 1.5);
    components.SetDirectory(nullptr); // The input file must not own it.
    components.SetStats(false);

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        components.Fill(object->Vx(), object->Vy());
    }

    TCanvas* componentCanvas = new TCanvas("cComponents",
                                         "Velocity components", 900, 750);
    componentCanvas->SetRightMargin(0.16); // Room for the colour scale.
    components.DrawCopy("COLZ"); // The displayed copy survives this function.
    componentCanvas->SaveAs("velocity_components.png");
    componentCanvas->SaveAs("velocity_components.pdf");

   
    TCanvas* selectionCanvas = new TCanvas("cSelected",
                                         "Selected measurements", 900, 750);
    tree->SetMarkerStyle(7);

    const Long64_t nSelected = tree->Draw(
        "sqrt(fVx*fVx + fVy*fVy):fVz",
        "sqrt(fVx*fVx + fVy*fVy + fVz*fVz) > 0.50",
        "AP"); // A: axes; P: individual points, rather than a density histogram.

    if (nSelected > 0) {
        TGraph* graph = static_cast<TGraph*>(
            selectionCanvas->GetPrimitive("Graph"));
        if (graph != nullptr) {
            graph->SetTitle(
                "Measurements with v > 0.50 m/s;v_{z} (m/s);v_{xy} (m/s)");
            selectionCanvas->Modified();
            selectionCanvas->Update();
            selectionCanvas->SaveAs("selected_measurements.png");
            selectionCanvas->SaveAs("selected_measurements.pdf");
        }
        std::cout << "Measurements with speed > 0.50 m/s: "
                  << nSelected << '\n';
    } else {
        std::cerr << "No points selected, or the Draw expression failed\n";
    }

    tree->ResetBranchAddresses();
    file.Close();

}