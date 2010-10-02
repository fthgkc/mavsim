mode(0)
function c=polyfit(x,y,n)
  
	for i = 1:size(x,1)	
		for j=0:n
			D(i,j+1) = x(i)^j;
	    end
    end
    c=pinv(D)*y;
endfunction

function main
	mode(0)
	S = 358.8 / 144; //reference area, Sw
	rho = 0.00250301323; //density of air at sea level in slugs/ ft^3
	
    //Drag Calibration
	// The following file holds new drag calibration data,taken to obtain an 
	// actual drag curve from the measured drag and known drag, which is not in the file. 
	fid=mopen('100730_Drag.DAT','r'); 
	[n,date_d,temp_d,press_d,vel_d,drag_d,lift_d,side_d,pitch_d,roll_d,yaw_d,aoa_d] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	dragActual=[0:5,0]'; // This is the known drag.
	Drag_Calibration = polyfit(drag_d,dragActual,1);
	
	//Lift Calibration
	// The following file holds new lift calibration data,taken to obtain an actual
	// lift curve from the measured lift and known lift, which is not in the file. 
	clear fid;
	fid=mopen('100730_Lift.DAT','r');
	[n,date_l,temp_l,press_l,vel_l,drag_l,lift_l,side_l,pitch_l,roll_l,yaw_l,aoa_l] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	liftActual = [0,1,2,2,3,4,5,6,7,8,9,10]';
    Lift_Calibration = polyfit(lift_l, liftActual,1);
    	
	//Positive Yaw Calibration
	// The following file holds positive yaw calibration data,taken to obtain an actual
	// yaw curve from the measured yaw and known yaw, which is not in the file. 
	clear fid;
	fid=mopen('100730_PositiveYaw.DAT','r');
	[n,date_py,temp_py,press_py,vel_py,drag_py,lift_py,side_py,pitch_py,roll_py,yaw_py,aoa_py] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	pyawActual = [0,1,3,3,4,5,0]';
    PYaw_Calibration = polyfit(yaw_py, pyawActual,1);
    	
	//Negative Yaw Calibration
	// The following file holds negative yaw calibration data,taken to obtain an actual
	// yaw curve from the measured yaw and known yaw, which is not in the file. 
	clear fid;
	fid=mopen('100730_NegativeYaw.DAT','r');
	[n,date_ny,temp_ny,press_ny,vel_ny,drag_ny,lift_ny,side_ny,pitch_ny,roll_ny,yaw_ny,aoa_ny] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	nyawActual = [0,1,2,3,4,5,0]';
    NYaw_Calibration = polyfit(yaw_ny, pyawActual,1);
    
    //Positive Roll Calibration
	// The following file holds positive roll calibration data,taken to obtain an actual
	// roll curve from the measured roll and known roll, which is not in the file. 
	clear fid;
	fid=mopen('100730_PositiveRoll.DAT','r');
	[n,date_pr,temp_pr,press_pr,vel_pr,drag_pr,lift_pr,side_pr,pitch_pr,roll_pr,yaw_pr,aoa_pr] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	prollActual = [0,1,3,4,5,0]';
    PRoll_Calibration = polyfit(roll_pr, prollActual,1);
    	
	//Negative Roll Calibration
	// The following file holds negative roll calibration data,taken to obtain an actual
	// roll curve from the measured roll and known roll, which is not in the file. 
	clear fid;
	fid=mopen('100730_NegativeRoll.DAT','r');
	[n,date_nr,temp_nr,press_nr,vel_nr,drag_nr,lift_nr,side_nr,pitch_nr,roll_nr,yaw_nr,aoa_nr] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	nrollActual = [0,1,3,4,5,0]';
    NRoll_Calibration = polyfit(roll_nr, nrollActual,1);
    
   //Positive Pitch Calibration
	// The following file holds positive pitch calibration data, taken to obtain an actual
	// pitch curve from the measured pitch and known pitch, which is not in the file. 
	clear fid;
	fid=mopen('100730_PositivePitch.DAT','r');
	[n,date_pp,temp_pp,press_pp,vel_pp,drag_pp,lift_pp,side_pp,pitch_pp,roll_pp,yaw_pp,aoa_pp] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	ppitchActual = [0,1,3,4,5,0]';
    PPitch_Calibration = polyfit(pitch_pp, ppitchActual,1);
    	
	//Side Force Calibration
	// The following file holds side force calibration data,taken to obtain an actual
	// side force curve from the measured side force and known side force, which is not in the file. 
	clear fid;
	fid=mopen('100730_SideForce.DAT','r');
	[n,date_np,temp_sf,press_sf,vel_sf,drag_sf,lift_sf,side_sf,pitch_sf,roll_sf,yaw_sf,aoa_sf] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);
	sideforceActual = [0,1,3,4,5,0]';
    SideForce_Calibration = polyfit(side_sf, sideforceActual,1); 
        
	// CD - Alpha
	// The following file holds data for CL and CD versus alpha; taken having 
	// run standard increment alpha tests and measured lift and drag.  Data must
	// be converted from measured to actual using the calibration curves computed
	// and then the lift/drag equations must be used to obtain CL and CD.
	fid = mopen('100730_Lift_Drag_AoA.DAT', 'r');
    [n,date2,temp1,press1,vel1,drag1,lift1,side1,pitch1,roll1,yaw1,aoa1] = ..
		mfscanf (1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
    Actual_Drag(:) = Drag_Calibration(2) * drag1(:) + Drag_Calibration(1);
  	CD = 2*Actual_Drag./((vel1*3.28).^2*rho*S);
	Actual_AOA(:) = 2.9493 * aoa1(:); // This is the computed alpha calibration equation.
	scf(2); clf(2)
    plot(Actual_AOA(2:18), CD(2:18), '.');
    title('Cd - Alpha Comparison')
	xlabel('Angle of Attack, deg')
	ylabel('Cd')
	datcom_aoa_drag  = [-16.00         .1667;
                       -8.000         .4475E-01;
                       -6.000         .3290E-01;
                       -4.000         .2639E-01;
                       -2.000         .2554E-01;
                         .000         .3065E-01;
                        2.000         .4245E-01;
                        4.000         .6156E-01;
                        8.000         .1232;    
                        9.000         .1436;    
                        10.00         .1636;    
                        12.00         .2014;    
                        14.00         .2355;    
                        16.00         .2637;    
                        18.00         .2813;    
                        19.00         .2851;    
                        20.00         .2611;    
                        21.00         .2185; 
                        22.00         .1816;  
                        24.00         .1539 ] ;
	datcom_drag = datcom_aoa_drag(:,2);
	datcom_aoa = datcom_aoa_drag(:,1);
	plot(datcom_aoa, datcom_drag, '.r')
	legend('Tunnel', 'Datcom')
	
	
    
    // CL - Alpha
    Actual_Lift(:) = Lift_Calibration(2) * lift1(:) + Lift_Calibration(1);
    CL = 2*Actual_Lift./((vel1*3.28).^2*rho*S);
    scf(4); clf(4)
    plot(Actual_AOA(2:18), CL(2:18), '.');
    title('Cl - Alpha Comparison')
	xlabel('Angle of Attack, deg')
	ylabel('Cl')
	datcom_aoa_lift = [ -16.00        -1.216;    
                       -8.000        -.4490;    
                       -6.000        -.2713;    
                       -4.000        -.9559E-01;
                       -2.000         .7950E-01;
                         .000         .2620;    
                        2.000         .4513;    
                        4.000         .6466;    
                        8.000         1.053;    
                        9.000         1.157;    
                        10.00         1.248;    
                        12.00         1.403;    
                        14.00         1.529;    
                        16.00         1.620;    
                        18.00         1.660;    
                        19.00         1.659;    
                        20.00         1.545;    
                        21.00         1.332;    
                        22.00         1.097;    
                        24.00         .8071    ];
    datcom_lift = datcom_aoa_lift(:,2);
    plot(datcom_aoa, datcom_lift, '.r');
    legend('Tunnel', 'Datcom')
    
    
    //CDDe - De - Alpha
    // The following file holds data taken for the purpose of determining the 
    // effect of elevator deflection on drag by using a known deflection which 
    // is not in the file (done by percentage of total deflection) and a measured
    // drag value, which is in the file.  This is done for 5 different alpha values. 
    fid=mopen('100713_Elevator_AoA.DAT','r');
	[n,date4,temp3,press3,vel3,drag3,lift3,side3,pitch3,roll3,yaw3,aoa3] = ..
		mfscanf(1000,fid,'%s %f %f %f %f %f %f %f %f %f %f\n');
	mclose(fid);   
	// Here I am subtracting the drag at zero elevator deflection from the drag
	// everywhere else to get drag soley due to elevator deflection and then 
	// using the drag equation to get CD values.
	CDDe=zeros(5,11);
	for (j=1:5)
	  start  = 10*(j-1);
	  k1 = start+2;
	  k2 = start+12;
	  k3 = 7+11*(j-1);
      temp = drag3(k1:k2) - drag3(k3);
      for i = 1:11
            CDDe(j,i) = 2*temp(i)/((vel3(i+1)*3.28).^2*rho*S);
      end
    end
	Deflection = linspace(-15,15,11);
	scf(3); clf(3)
	size(Deflection)
    size(aoa3([2,13,24,35,46]))
    size(CDDe)
	surf(Deflection,2.9493*aoa3([2,13,24,35,46]),CDDe(:,:),'facecol','blue')
    title('CDDe versus AoA ');
    xlabel('CDDe')
    ylabel('Alpha, deg');
    fid=mopen('DATCOM_CDDe.csv','r');
    mscanf(fid,'%f, %f, %f, %f, %f, %f, %f, %f, %f, %f')
	[n, AoA, De_neg20, De_neg15, De_neg10, De_neg5, De_0, De_5, De_10, De_13, De_16] = ..
		mfscanf(1000,fid,'%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n');
	mclose(fid);
    Deflection = [-20, -25, -20, -5, 0, 5, 10, 13, 16];
    CDDe_Datcom(1,:) = De_neg20(2:21)'
    CDDe_Datcom(2,:) = De_neg15(2:21)';
    CDDe_Datcom(3,:) = De_neg10(2:21)';
    CDDe_Datcom(4,:) = De_neg5(2:21)';
    CDDe_Datcom(5,:) = De_0(2:21)';
    CDDe_Datcom(6,:) = De_5(2:21)';
    CDDe_Datcom(7,:) = De_10(2:21)';
    CDDe_Datcom(8,:) = De_13(2:21)';
    CDDe_Datcom(9,:) = De_16(2:21)';
    surf(AoA(2:21), Deflection(:), CDDe_Datcom(:,:), 'facecol', 'red')
    // I can't figure out how to get a legend here.
    
    //CLDe - De - Alpha
    // Here I am subtracting the lift at zero elevator deflection from the lift
    // everywhere else to get drag soley due to elevator deflection and then 
    // using the drag equation to get CD values.
	CLDe=zeros(5,11);
	size(lift3)
	for (j=1:5)
	  start  = 10*(j-1);
	  k1 = start+2
	  k2 = start+12
	  k3 = 7+11*(j-1)
      temp = lift3(k1:k2) - lift3(k3);
      for i = 1:11
            CLDe(j,i) = 2*temp(i)/((vel3(i+1)*3.28).^2*rho*S);
      end
    end
//	Deflection = [0,0:10:100, 0:10:100, 0:10:100, 0:10:100, 0:10:100,0];
//	scf(5); clf(5)
//    surf(aoa3(2:56), CLDe(2:56),'facecol','blue')
//    title('CLDe versus AoA');
//    xlabel('CLDe')
//    ylabel('Alpha, deg');
//    legend('Tunnel', 'Datcom')
//    

endfunction

main

