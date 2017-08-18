#include <TFile.h> 
#include <TTree.h>
#include <TBranch.h>

//Enter four text files for f001, noise, ped, and status values arguments 
void makeTree(char* file1, char* file2, char* file3, char* file4)
{

	FILE* statusFile = fopen(file1, "r");
		if (!statusFile) {
    		printf ("Cannot open file '%s'.\n", file1);
    	return;
  		}


	FILE* fValueFile = fopen(file2, "r");
		if (!fValueFile) {
    		printf ("Cannot open file '%s'.\n", file2);
    	return;
  		}

	FILE* noiseFile = fopen(file3, "r");
		if (!noiseFile) {
    		printf ("Cannot open file '%s'.\n", file3);
    	return;
  		}

	FILE* pedFile = fopen(file4, "r");
		if (!pedFile) {
    		printf ("Cannot open file '%s'.\n", file4);
    	return;
  		}


	TFile *treeFile = new TFile("coolTree.root","RECREATE"); //output file for tree that will hold all the data 
	TTree *tree = new TTree("coolNtuple","data from all 4 files"); //tree that will hold all the data

	tree->SetEntries(32*4*(192+48)); 
	TBranch *secBranch, *layBranch, *chBranch, *stBranch, *fBranch, *noiseBranch, *pedBranch; 
	
	Int_t sector, layer, channel, status;
	Float_t fvalue, noise, pedestal;

	//create 4 branches on the tree; each branch has sector, layer, and channel 
	secBranch = tree->Branch("sector", &sector, "sector/I");
	layBranch= tree->Branch("layer", &layer, "layer/I");
	chBranch = tree->Branch("channel", &channel, "channel/I"); 
	stBranch = tree->Branch("status", &status, "status/I");
	fBranch = tree->Branch("fvalue", &fvalue, "fvalue/F"); 
	noiseBranch = tree->Branch("noise", &noise, "noise/F"); 
	pedBranch = tree->Branch("pedestal", &pedestal, "pedestal/F"); 



	int maxChar = 80; 
	char line[maxChar]; 
	char dir; //direction means X is eta and Y is phi 

	int sec, lay, ch; 



//loop over the 4 files, one at a time 
	while (fgets(line, maxChar, statusFile)) {
    	int n = sscanf(line, "%d %d %c %d %d", &sector, &layer, &dir, &channel, &status);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

    	if (dir=='Y') channel = -channel;

		secBranch->Fill();
		layBranch->Fill();
		chBranch->Fill();
		stBranch->Fill();

  	}
  	
	fclose(statusFile);

//	fBranch = tree->Branch("fvalue", &fvalue, "fvalue/F"); 

	while (fgets(line, maxChar, fValueFile)) {
    	int n = sscanf(line, "%d %d %c %d %f", &sec, &lay, &dir, &ch, &fvalue);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

		fBranch->Fill();

  	}
  	
	fclose(fValueFile);

//	noiseBranch = tree->Branch("noise", &noise, "noise/F"); 


	while (fgets(line, maxChar, noiseFile)) {
    	int n = sscanf(line, "%d %d %c %d %f", &sec, &lay, &dir, &ch, &noise);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

		noiseBranch->Fill();

  	}
  	
	fclose(noiseFile);

//	pedBranch = tree->Branch("pedestal", &pedestal, "pedestal/F"); 

	while (fgets(line, maxChar, pedFile)) {
    	int n = sscanf(line, "%d %d %c %d %f", &sec, &lay, &dir, &ch, &pedestal);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

		pedBranch->Fill();

  	}
  	
	fclose(pedFile);


	treeFile->Write(); 


}

















	

		
