//-----------------------------------------------------------------------
//
//      Convoluted Landau and Gaussian Fitting Function
//         (using ROOT's Landau and Gauss functions)
//
//  Based on a Fortran code by R.Fruehwirth (fruhwirth@hephy.oeaw.ac.at)
//  Adapted for C++/ROOT by H.Pernegger (Heinz.Pernegger@cern.ch) and
//   Markus Friedl (Markus.Friedl@cern.ch)
//
//  to execute this example, do:
//  root > .x langaus.C
// or
//  root > .x langaus.C++
//
//-----------------------------------------------------------------------

#include "myHeader.h"


using namespace std;


Double_t langaufun(Double_t *x, Double_t *par) {

   //Fit parameters:
   //par[0]=Width (scale) parameter of Landau density
   //par[1]=Most Probable (MP, location) parameter of Landau density
   //par[2]=Total area (integral -inf to inf, normalization constant)
   //par[3]=Width (sigma) of convoluted Gaussian function
   //
   //In the Landau distribution (represented by the CERNLIB approximation), 
   //the maximum is located at x=-0.22278298 with the location parameter=0.
   //This shift is corrected within this function, so that the actual
   //maximum is identical to the MP parameter.

      // Numeric constants
      Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      Double_t mpshift  = -0.22278298;       // Landau maximum location

      // Control constants
      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

      // Variables
      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;


      // MP shift correction
      mpc = par[1] - mpshift * par[0]; 

      // Range of convolution integral
      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];

      step = (xupp-xlow) / np;

      // Convolution integral of Landau and Gaussian by sum
      for(i=1.0; i<=np/2; i++) {
         xx = xlow + (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);

         xx = xupp - (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }

      return (par[2] * step * sum * invsq2pi / par[3]);
}



// This is a much simpler version of what is in fit_helpers.C,
// but seemed to result in poor fits on occasion (e.g. not always smooth curves)


TF1 *langausFit(TH1F *hist, double xlow, double xhigh)
{
  double rms = hist->GetRMS();
  double peakpos = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());

  // attempts at initial guesses at the fit parameters
  double startwidth = rms;
  double startmpv = peakpos;
  double startnorm = hist->Integral();
  double startsigma = rms;

  TF1* fit = new TF1("landaugausfunction", langaufun, xlow, xhigh, 4);
  fit->SetParNames("LWidth","MPV","Area","GSigma");
  fit->SetParameters(startwidth, startmpv, startnorm, startsigma);
  
  hist->Fit(fit, "0L", "", xlow, xhigh);
  return fit;
}

void Fit_ChargeDist() {

 std::vector<std::string> Angles = { "0", "2", "4", "6", "8", "12", "16", "20", "24", "28", "32", };

  std::vector<std::string> File_Names = {
    "Chewie_Runs45689_45700.root",
    "Chewie_Runs45683_45688.root",
    "Chewie_Runs45675_45682.root",
    "Chewie_Runs45670_45674.root",
    "Chewie_Runs45664_45669.root",
    "Chewie_Runs45657_45663.root",
    "Chewie_Runs45651_45656.root",
    "Chewie_Runs45646_45650.root",
    "Chewie_Runs45639_45645.root",
    "Chewie_Runs45634_45638.root",
    "Chewie_Runs45614_45632.root",
  };

   std::vector<std::string> Histogram_Names = {

     "Charge/Dut0/Landau/hLandauClusterSizeUpToMax_Dut0",
     "Charge/Dut0/Landau/hLandauClusterSize1_Dut0",
     "Charge/Dut0/Landau/hLandauClusterSize2_Dut0",
     "Charge/Dut0/Landau/hLandauClusterSize2sameRow_Dut0",
     "Charge/Dut0/Landau/hLandauClusterSize2sameCol_Dut0",

   };

    std::vector<std::string> Txtfile_Names = {

    "Fit_Clusterize_All_Data.txt",
    "Fit_Clusterize_01_Data.txt",
    "Fit_Clusterize_02_Data.txt",
    "Fit_Clusterize_02_SameRow_Data.txt",
    "Fit_Clusterize_02_SameCol_data.txt",
    
  };
    

printf("Fitting...\n");

   TFile *file_[File_Names.size()];
   TH1F *hist_[Histogram_Names.size()][File_Names.size()];
   TF1 *fit_[Histogram_Names.size()][File_Names.size()];
   TCanvas *can_[Histogram_Names.size()][File_Names.size()];
   cout << "nFiles: " << File_Names.size() << endl;


   for (int i=0; i< File_Names.size(); i++){

       file_[i] = TFile::Open(File_Names[i].c_str());
       //file_[i]->Print();
       
       
     for (int j=0; j < Histogram_Names.size(); j++){
   
       hist_[j][i] = (TH1F*) file_[i]->Get(Histogram_Names[j].c_str());
       fit_[j][i] = langausFit(hist_[j][i],5000,20000);
       hist_[j][i]->GetXaxis()->SetRange(5000,0);
       can_[j][i] = new TCanvas(TString::Format("%i %i", j, i), TString::Format("%i %i", j, i),200,1, 750, 500); 

       hist_[j][i]->Draw();
       fit_[j][i]->Draw("same");
       can_[j][i]->Update();

       // Global style settings
gStyle->SetOptStat(1111);
gStyle->SetOptFit(111);
gStyle->SetLabelSize(0.03,"x");
gStyle->SetLabelSize(0.03,"y");

       // hist_[j][i]->SetLineColor(i+2);

       
     }
   }
 
 
printf("Fitting done\nPlotting results...\n");
	

  ///////////////////////////////////////////////////////////////////////

         for(int m=0; m < Histogram_Names.size();m++){

        std::ofstream fout (Txtfile_Names[m].c_str(),std::ofstream::out);
          
   
        fout << Angles[0].c_str() << "\t" << fit_[m][0]->GetParameter(2) << "\t" << 0 << "\t" << fit_[m][0]-> GetParError(2) << endl; 
	fout << Angles[1].c_str() << "\t" << fit_[m][1]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][1]->GetParError(2) << endl; 
	fout << Angles[2].c_str() << "\t" << fit_[m][2]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][2]->GetParError(2) << endl;
	fout << Angles[3].c_str() << "\t" << fit_[m][3]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][3]->GetParError(2) << endl;
	fout << Angles[4].c_str()  << "\t" << fit_[m][4]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][4]->GetParError(2) << endl;
	fout << Angles[5].c_str()  << "\t" << fit_[m][5]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][5]->GetParError(2) << endl;
	fout << Angles[6].c_str()  << "\t" << fit_[m][6]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][6]->GetParError(2) << endl;
	fout << Angles[7].c_str()  << "\t" << fit_[m][7]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][7]->GetParError(2) << endl;
	fout << Angles[8].c_str()  << "\t" << fit_[m][8]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][8]->GetParError(2) << endl;
	fout << Angles[9].c_str()  << "\t" << fit_[m][9]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][9]->GetParError(2) << endl;
	fout << Angles[10].c_str()  << "\t" << fit_[m][10]->GetParameter(2)  << "\t" << 0 << "\t" << fit_[m][10]->GetParError(2) << endl;

	
	fout.close();
	
        }
	 
   ///////////////////////////////////////////////////////////////////////////////////////////

	 
   TGraphErrors *gr_[Txtfile_Names.size()];

   TCanvas *a_[Txtfile_Names.size()];

   for (int l=0; l < Txtfile_Names.size(); l++){
   
   a_[l] = new TCanvas(TString::Format("txt_%i", l), TString::Format("txt_%i", l),200,1, 700, 500);
   
   a_[l]->SetGrid();
   a_[l]->SetTicks();
   a_[l]->GetFrame()->SetBorderSize(12);

       
   gr_[l] = new TGraphErrors(Txtfile_Names[l].c_str());
   
   gr_[l]->SetMarkerColor(2);
   gr_[l]->SetMarkerSize(1.25);
   gr_[l]->SetMarkerStyle(8);
   gr_[l]->SetTitle("Standard Deviation Vs Angle");
   gr_[l]->GetXaxis()->SetTitle("Angle");
   gr_[l]->GetYaxis()->SetTitle("StdDev");
   //   gr_[l]->GetXaxis()->SetRange(0,20);
   //   gr_[l]->GetYaxis()->SetRange(0,1);
   gr_[l]->GetYaxis()->SetTitleSize(20);
   gr_[l]->GetYaxis()->SetTitleFont(43);
   gr_[l]->GetYaxis()->SetTitleColor(kBlack);
   gr_[l]->GetYaxis()->SetLabelColor(kBlack);
   gr_[l]->GetXaxis()->SetTitleSize(20);
   gr_[l]->GetXaxis()->SetTitleFont(43);
   gr_[l]->GetXaxis()->SetTitleColor(kBlack);
   gr_[l]->GetXaxis()->SetLabelColor(kBlack);
   gr_[l]->Draw("AP");
   //gr_[l]->Draw("PESame");
   
   // a_[l]->Update();
   }

	 
}

