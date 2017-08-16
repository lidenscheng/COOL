#include <TH2.h>

TH2F * sed;

void CoolStatus(char* filename){
  // Read moni ntuple event display

  if (!sed) sed = new TH2F("sed", "Cool Status bits",  192+48, -48, 192, 33*5, -16, 17);

  sed->GetXaxis()->SetTitle("-Y | X strip number");
  sed->GetYaxis()->SetTitle("Sector + 0.2*layer");
  FILE* infile = fopen (filename, "r");
  if (!infile) {
    printf ("Cannot open file '%s'.\n", filename);
    return;
  }
  char line[80];
  int sector, layer, channel, status;
  char dir;
  
  while (fgets(line, 80, infile)) {
    int n = sscanf(line, "%d %d %c %d %d", &sector, &layer, &dir, &channel, &status);
    if (n!=5)   {
      printf ("%s", line);
      continue;
    }
    //if (status) printf ("%s", line);
    int x = channel;
    if (dir=='Y') x = -channel;
    double y = sector + 0.2*(layer-1);
    sed->Fill(x+0.1, y+0.1, status);
  }
  fclose(infile);
  sed->SetStats(kFALSE); 
  sed->Draw("colz");
}



