#include "TF1.h"
#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "TSystem.h"
#include "TRandom.h"
#include "stdio.h"
#include "stdlib.h"
#include "iostream"

//Usage: .L analysis/ReadLookupTable.C / ReadLookupTable()
// Run this from the project root directory 

void ReadLookupTable()
{
     TTree* t=new TTree("lookup_tree","lookup_tree");
     
     double posx,posy,posz,angle;
     int sumo,sec,wire,seg,strip,counter,module,xstrip,xcopy;
     UInt_t indexi,indexj;
     
     int n_strips = 96; // 1/2 of the total number of strips for Mantel
     
     t->Branch("sumo",&sumo,"sumo/I");
     t->Branch("sec",&sec,"sec/I");
     t->Branch("wire",&wire,"wire/I");
     t->Branch("seg",&seg,"seg/I");
     t->Branch("counter",&counter,"counter/I");
     t->Branch("xcopy",&xcopy,"xcopy/I");
     t->Branch("strip",&strip,"strip/I");
     t->Branch("module",&module,"module/I");
     t->Branch("indexi",&indexi,"indexi/i");
     t->Branch("indexj",&indexj,"indexj/i");
//     t->Branch("angle",&angle,"angle/D");
     t->Branch("posx",&posx,"posx/D");
     t->Branch("posy",&posy,"posy/D");
     t->Branch("posz",&posz,"posz/D");
     
 cout<<"   "<<endl;

 gSystem->Exec("rm powtex_lookup.root");
 
 cout<<"removing the old root file....done!"<<endl;
 
 TFile *rf=new TFile("powtex_lookup.root","new"); 

char hname[20];
Char_t i[7],w[7],a[7],b[7],c[7],d[10],e[10],f[10],g[5],m[7],k[5];
Int_t xseg,tseg;

     sprintf(hname,"LookupTablePowtex.txt");  
     FILE* fp;
     
     cout<<"    "<<endl;
     
     printf("Reading the data file %s\n",hname);
     
     fp=fopen(hname,"r");
     
     char line[128];

     if(fp!=NULL)
      {
      while(fgets(line,128,fp) != NULL)
       {
        sscanf(line,"%s %s %s %s %s %s %s %s %s",i,w,a,b,c,m,d,e,f);

        sumo=atoi(i);      // detector ID ==> 3-6 for SUMOs,  7 for Mantel & 8 for HR
        sec=atoi(w);       // sector number ==1 for all SUMOS and Mantel
        xseg=atoi(a);     // module number 
        xcopy=atoi(m);    // counter no (from 1 to 4)
        
//        posx=0.001*strtod(d,NULL);   //for comparison with Jen's file
//        posy=0.001*strtod(e,NULL);   
//        posz=0.001*strtod(f,NULL);   

        posx=strtod(d,NULL);   
        posy=strtod(e,NULL);   
        posz=strtod(f,NULL);   


        if (sumo<7 || sumo==8) {           // EndCap & HR
             strip=atoi(b);    
             wire=atoi(c);  
             counter = xcopy;    
             
            }
             
        else {             //  Mantel
        
             xstrip=atoi(c);    
             wire=atoi(b);       
             
            switch (xcopy) {   
            
            	case 1:     //  BR after rotation
	
			   		strip = n_strips + xstrip;
			   		counter = 2;
			   		break;
		   
                case 2:   //BL  after rotation
		   
			        strip = (n_strips + 1) - xstrip;
			        counter = 2; 
			        break; 
		   
                case 3:          //TR  after rotation
		   
				   strip = n_strips + xstrip;
				   counter = 1;
				   break;
		   
                 case 4:     //TL
		   
				   strip = (n_strips + 1) - xstrip; // TL after rotation
				   counter = 1;
				   break;
				   
				   }
                  
          }
        
//        angle=strtod(k,NULL);  // angle of the segment 



//	cout<<"posx ="<<posx<<"   posy ="<<posy<<"   posz ="<<posz<<endl;
	
//	tseg = xseg % 10000; 
		module = abs(xseg/100); 
		seg = xseg % 100;  
	
		indexi = sumo*100000+sec*10000+module*100+counter;   
		indexj = seg*100000+strip*100+wire;

		t->Fill();
	
	
	}}
	
	else printf("Couldn't open file %s!\n",hname);

		
        fclose(fp);
//	t->Print();
	t->Write();

	cout<<"  "<<endl;
	
	cout<<"Writing the output lookup table dat file....done!"<<endl;
	
	cout<<"Filling the root tree...done!"<<endl; 
	cout<<"the tree has "<<t->GetEntries()<<" entries."<<endl;
	
	rf->Close();


}
	
	
