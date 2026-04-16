// Calculates wire index W for EndCap Mantel modules.
//
// Geometry note:
// - Voxel ids are interleaved by 4 among quadrant copies.
// - Each group of 16 voxels maps to one strip index in S, while
//   W is the local wire number inside that group.

int calculateW_for_Mantel(int voxel, int scopy)

{
	int nw = 16;
	int ns = 96;  // 1/2 of the number of strips
	
	int factor,wire;
	
	switch (scopy) {
	
		case 1:    // TL before rotation, TR after rotation
		
        		factor = (voxel - 2)/4;  // TL before rotation starts at 9, next is 13
			wire = factor % nw;  // the wire number = remainder 
			if (wire == 0) {wire = nw;}
			break;
			
		case 2:      //TR before rotation, BR after rotation
		
	        	factor = (voxel - 4)/4;  // TR sequence starts at 8, then 12, 16, ...
			wire = factor % nw;
			if (wire == 0) {wire = nw;}
			break;
			
		case 3:      //BL before rotation, TL after rotation
		
	        	factor = (voxel - 3)/4;  // BL sequence starts at 7, then 11, 15, ...
			wire = factor % nw;
			if (wire == 0) {wire = nw;}
			break;
			
		case 4:      // BR before rotation, BL after rotation 
		
	        	factor = (voxel - 5)/4;  // BR sequence starts at 6, then 10, 14, ...
			wire = factor % nw;
			if (wire == 0) {wire = nw;}
			break;
			
			}

		
	return wire;
	
}	
