Let colors fight over your screen canvas.		

// POPULATION UPDATE
	
	// Inspired by Conway's game of life:
	//  if there is a sufficient population, then grow in numbers:
	if(b0>=100)b=b0*1.01;
	//  if the population is small then shrink
	if(b0<100)b=b0*(0.99-fr/100);
	//  if the population is too big then collapse
	if(b0>240)b=0;
	//  repopulate collapsed populations
	if(b0<3)b=120;//zb*((1.5-f)+fr*f);
	
Updated the dot's color based on some crazy experimental whatever function.
Feel free to wildly experiment for stunning art!
	