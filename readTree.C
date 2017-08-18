#include <TFile.h> 
#include <TTree.h>
#include <TH2.h>
#include <TCanvas.h> 


void readTree(char* fileName)

{

	TFile *f = new TFile(fileName);
	TTree *t = (TTree*)f->Get("coolNtuple");

	Int_t sector, layer, channel, status;
	Float_t fvalue, noise, pedestal, ratio;

	t->SetBranchAddress("sector", &sector);
	t->SetBranchAddress("layer", &layer);
	t->SetBranchAddress("channel", &channel);
	t->SetBranchAddress("status", &status);
	t->SetBranchAddress("fvalue", &fvalue);
	t->SetBranchAddress("noise", &noise);
	t->SetBranchAddress("pedestal", &pedestal);

	TCanvas *myc = new TCanvas("myc", "myc", 1200, 600);	

	TH2F *h2_status = new TH2F("h2_status", "Cool Status bits",  192+48, -48, 192, 33*5, -16, 17);
	TH2F *h2_f001 = new TH2F("h2_f001", "Cool F001",  192+48, -48, 192, 33*5, -16, 17);
	TH2F *h2_noise = new TH2F("h2_noise", "Cool Noise",  192+48, -48, 192, 33*5, -16, 17);
	TH2F *h2_ped = new TH2F("h2_ped", "Cool Pedestal",  192+48, -48, 192, 33*5, -16, 17);

	TH2F *h2_ratio = new TH2F("h2_ratio", "(F001-pedestal)/noise <= 4.0",  192+48, -48, 192, 33*5, -16, 17);

	TH2F *h2_ratio_high = new TH2F("h2_ratio_high", "(F001-pedestal)/noise > 4.0",  192+48, -48, 192, 33*5, -16, 17);

	Long64_t nentries = t->GetEntries();

	for(Long64_t i=0; i<nentries; i++)
	{
		t->GetEntry(i);
		
		ratio = (fvalue-pedestal)/noise; 

		h2_status-> Fill(channel+0.1, sector+0.2*(layer-1)+0.1, status);
		h2_f001-> Fill(channel+0.1, sector+0.2*(layer-1)+0.1, fvalue);
		h2_noise-> Fill(channel+0.1, sector+0.2*(layer-1)+0.1, noise);
		h2_ped-> Fill(channel+0.1, sector+0.2*(layer-1)+0.1, pedestal);

		if(ratio <= 4.0)
		{
			h2_ratio-> Fill(channel+0.1, sector+0.2*(layer-1)+0.1, ratio);
		}
		
		else
		{
			h2_ratio_high-> Fill(channel+0.1, sector+0.2*(layer-1)+0.1, ratio);

		}

//		printf("%f\t%f\t%f\t%f\n", fvalue, pedestal, noise, ((fvalue-pedestal)/noise)); 
//		printf("%d\t%d\t%d\t%d\t%f\t%f\t%f\n", sector, layer, channel, status, fvalue, noise, pedestal); 

	}


  	h2_status->SetStats(kFALSE); 
  	h2_status->Draw("COLZ0");
	myc->SaveAs("CoolStatus.pdf");

  	h2_f001->SetStats(kFALSE); 
  	h2_f001->Draw("COLZ0");
	myc->SaveAs("CoolF001.pdf");
	myc->Clear();

  	h2_noise->SetStats(kFALSE); 
  	h2_noise->Draw("COLZ0");
	myc->SaveAs("CoolNoise.pdf");
	myc->Clear();

  	h2_ped->SetStats(kFALSE); 
  	h2_ped->Draw("COLZ0");
	myc->SaveAs("CoolPedestal.pdf");
	myc->Clear();

  	h2_ratio->SetStats(kFALSE); 
  	h2_ratio->Draw("COLZ0");
	myc->SaveAs("CoolRatioNormal.pdf");
	myc->Clear();

  	h2_ratio_high->SetStats(kFALSE); 
  	h2_ratio_high->Draw("COLZ0");
	myc->SaveAs("CoolRatioHigh.pdf");
	myc->Close();




}

