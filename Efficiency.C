#include "myHeader.h"

using namespace std;


void Efficiency(){


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

    "Efficiency/Dut0/Efficiency/Efficiency_Dut0",
    "Efficiency/Dut0/Efficiency/EfficiencyRef_Dut0",
  };

  std::vector<std::string> Txtfile_Names = {
     "Efficiency_Overall_Data.txt",
     "Efficiency_Reference_Data.txt"
  };


  /////////////////////////////////////////////////////


  
   TFile *file_[File_Names.size()];
   TH1F *hist_[Histogram_Names.size()][File_Names.size()];
   cout << "nFiles: " << File_Names.size() << endl;
 
   for (int i=0; i< File_Names.size(); i++){

       file_[i] = TFile::Open(File_Names[i].c_str());
       //     file_[i]->Print();
       
       
     for (int j=0; j < Histogram_Names.size(); j++){
   
       hist_[j][i] = (TH1F*) file_[i]->Get(Histogram_Names[j].c_str());
 
     }
   }

   /////////////////////////////////////////////////////

       
       for(int m=0; m < Histogram_Names.size();m++){

        std::ofstream fout (Txtfile_Names[m].c_str(),std::ofstream::out);
          
   
        fout << Angles[0].c_str() << "\t" << hist_[m][0]->GetSum() << "\t" << 0 << "\t" << hist_[m][0]->GetSumOfWeights() << endl;
	fout << Angles[1].c_str() << "\t" << hist_[m][1]->GetSum()  << "\t" << 0 << "\t" << hist_[m][1]->GetSumOfWeights() << endl; 
	fout << Angles[2].c_str() << "\t" << hist_[m][2]->GetSum()  << "\t" << 0 << "\t" << hist_[m][2]->GetSumOfWeights() << endl;
	fout << Angles[3].c_str() << "\t" << hist_[m][3]->GetSum()  << "\t" << 0 << "\t" << hist_[m][3]->GetSumOfWeights() << endl;
	fout << Angles[4].c_str()  << "\t" << hist_[m][4]->GetSum()  << "\t" << 0 << "\t" << hist_[m][4]->GetSumOfWeights() << endl;
	fout << Angles[5].c_str()  << "\t" << hist_[m][5]->GetSum()  << "\t" << 0 << "\t" << hist_[m][5]->GetSumOfWeights() << endl;
	fout << Angles[6].c_str()  << "\t" << hist_[m][6]->GetSum()  << "\t" << 0 << "\t" << hist_[m][6]->GetSumOfWeights() << endl;
	fout << Angles[7].c_str()  << "\t" << hist_[m][7]->GetSum()  << "\t" << 0 << "\t" << hist_[m][7]->GetSumOfWeights() << endl;
	fout << Angles[8].c_str()  << "\t" << hist_[m][8]->GetSum()  << "\t" << 0 << "\t" << hist_[m][8]->GetSumOfWeights() << endl;
	fout << Angles[9].c_str()  << "\t" << hist_[m][9]->GetSum()  << "\t" << 0 << "\t" << hist_[m][9]->GetSumOfWeights() << endl;			
	fout << Angles[10].c_str()  << "\t" << hist_[m][10]->GetSum()  << "\t" << 0 << "\t" << hist_[m][10]->GetSumOfWeights() << endl;
		

	fout.close();
	
        }

  ///////////////////////////////////////////////////////
	
  
   TGraph *gr_[Txtfile_Names.size()];

   TCanvas *a_[Txtfile_Names.size()];

   for (int l=0; l < Txtfile_Names.size(); l++){
   
   a_[l] = new TCanvas(TString::Format("txt_%i", l), TString::Format("txt_%i", l),100,1, 1000, 500);
   
   a_[l]->SetGrid();
   a_[l]->SetTicks();
   a_[l]->GetFrame()->SetBorderSize(12);

       
   gr_[l] = new TGraph(Txtfile_Names[l].c_str());
   
   gr_[l]->SetMarkerColor(2);
   gr_[l]->SetMarkerSize(1.25);
   gr_[l]->SetMarkerStyle(8);
   gr_[l]->SetTitle("Efficiency Vs Angle");
   gr_[l]->GetXaxis()->SetTitle("Angle");
   //   gr_[l]->GetXaxis()->SetRange(0,20);
   //   gr_[l]->GetYaxis()->SetRange(0,1);
   // gr_[l]->GetYaxis()->SetRangeUser(0.0,1.0);
   gr_[l]->GetYaxis()->SetTitleSize(20);
   gr_[l]->GetYaxis()->SetTitleFont(43);
   gr_[l]->GetYaxis()->SetTitleColor(kBlack);
   gr_[l]->GetYaxis()->SetLabelColor(kBlack);
   gr_[l]->GetXaxis()->SetTitleSize(20);
   gr_[l]->GetXaxis()->SetTitleFont(43);
   gr_[l]->GetXaxis()->SetTitleColor(kBlack);
   gr_[l]->GetXaxis()->SetLabelColor(kBlack);
   gr_[l]->Draw("AP");
   
   }

   gr_[0]->GetYaxis()->SetTitle("Overall Efficiency");
   gr_[1]->GetYaxis()->SetTitle("Efficiency Ref");
   
}



