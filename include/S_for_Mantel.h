// Calculates strip index S for EndCap Mantel modules.
//
// Geometry note:
// - Voxel ids are interleaved by 4 among the quadrant copies.
// - scopy selects which local quadrant mapping is used.
// - Returned strip numbering is centered around ns=96 (half-strip count).

int calculateS_for_Mantel(int voxel,int scopy)

{
	int nw = 16;
	int ns = 96;  // Half of the strip count.
	
	int factor,strip;
	
	switch (scopy) {
	
		case 1:    // TL before rotation
		
	        	factor = (voxel - 2)/4;  // TL sequence starts at 6, then 10, 14, ...
	        	// Exact wire boundary keeps current strip, otherwise advance to next strip.
	        	if ((factor % nw) == 0) strip = ns + TMath::FloorNint(factor/nw); // upper half boundary strip 256
        			else strip = ns + TMath::FloorNint(factor/nw) + 1;
			
			break;
			
		case 2:      //TR before rotation
		
	        	factor = (voxel - 4)/4;  // TR sequence starts at 8, then 12, 16, ...
	        	if ((factor % nw) == 0) strip = ns - TMath::FloorNint(factor/nw) + 1;
        			else strip = ns - TMath::FloorNint(factor/nw);
			break;
			
		case 3:      //BL before rotation, TL after rotation
		
	        	factor = (voxel - 3)/4;  // BL sequence starts at 7, then 11, 15, ...
	        	if ((factor % nw) == 0) strip = ns  + TMath::FloorNint(factor/nw);
        			else strip = ns + TMath::FloorNint(factor/nw) + 1;
			break;
			
		case 4:      // BR before rotation, BL after rotation 
		
	        	factor = (voxel - 5)/4;  // BR sequence starts at 6, then 10, 14, ...
	        	if ((factor % nw) == 0) strip = ns  - TMath::FloorNint(factor/nw) + 1; // strip 128
        			else strip = ns - TMath::FloorNint(factor/nw);

			break;
		}
		
	return strip;
	
}	
