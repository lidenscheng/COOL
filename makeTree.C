#include <TFile.h> 
#include <TTree.h>

//Enter four text files for f001, noise, ped, and status values arguments 
void makeTree(char* file1, char* file2, char* file3, char* file4)
{

	FILE* statusFile = fopen(file1, "r");
		if (!statusFile) {
    		printf ("Cannot open file '%s'.\n", file1);
    	return;
  		}


	FILE* f001File = fopen(file2, "r");
		if (!f001File) {
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
	

	typedef struct{Int_t sector_s, layer_s, channel_s, stat;} STATUS; 

	typedef struct{Int_t sector_f, layer_f, channel_f, f;} F001; 

	typedef struct{
		Int_t sector_n, layer_n, channel_n; 
		Float_t n;
		} NOISE;

	typedef struct{
		Int_t sector_p, layer_p, channel_p;  
		Float_t ped; 
		} PEDESTAL;  


	static STATUS status;
	static F001 f001;
	static NOISE noise;
	static PEDESTAL pedestal; 

	//create 4 branches on the tree; each branch has sector, layer, and channel 
	tree->Branch("status", &status, "sector_s/I:layer_s/I:channel_s/I:stat/I");
	tree->Branch("f001", &f001, "sector_f/I:layer_f/I:channel_f/I:f/I"); 
	tree->Branch("noise", &noise, "sector_n/I:layer_n/I:channel_n/I:n/F"); 
	tree->Branch("pedestal", &pedestal, "sector_p/I:layer_p/I:channel_p/I:ped/F"); 

	int maxChar = 80; 
	char line[maxChar]; 
	char dir; //direction means X is eta and Y is phi 


//loop over the 4 files, one at a time 
	while (fgets(line, maxChar, statusFile)) {
    	int n = sscanf(line, "%d %d %c %d %d", &status.sector_s, &status.layer_s, &dir, &status.channel_s, &status.stat);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

    	if (dir=='Y') status.channel_s = -status.channel_s;

		tree->Fill();

  	}
  	
	fclose(statusFile);

	while (fgets(line, maxChar, f001File)) {
    	int n = sscanf(line, "%d %d %c %d %d", &f001.sector_f, &f001.layer_f, &dir, &f001.channel_f, &f001.f);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

    	if (dir=='Y') f001.channel_f = -f001.channel_f;

		tree->Fill();

  	}
  	
	fclose(f001File);

	while (fgets(line, maxChar, noiseFile)) {
    	int n = sscanf(line, "%d %d %c %d %f", &noise.sector_n, &noise.layer_n, &dir, &noise.channel_n, &noise.n);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

    	if (dir=='Y') noise.channel_n = -noise.channel_n;

		tree->Fill();

  	}
  	
	fclose(noiseFile);


	while (fgets(line, maxChar, pedFile)) {
    	int n = sscanf(line, "%d %d %c %d %f", &pedestal.sector_p, &pedestal.layer_p, &dir, &pedestal.channel_p, &pedestal.ped);
    	if (n!=5)   {
      		printf ("%s", line);
      		continue;
    	}

    	if (dir=='Y') pedestal.channel_p = -pedestal.channel_p;

		tree->Fill();

  	}
  	
	fclose(pedFile);

//	tree->Print(); 
	treeFile->Write(); 


}

















	

		
