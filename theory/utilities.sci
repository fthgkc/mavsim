function sys = unityFeedback(y,u,G,H)
	C = zeros(length(y),size(G,1));
	for (i=1:length(y)) C(i,y(i)) = 1; end;
	D = zeros(size(G,2),size(u,2));
	for (i=1:length(u)) D(u(i),i) = 1; end;
	olss = G*D*H;
	nPoles = size(olss.A,1);
    sys=minssAutoTol((eye(size(G,1),size(G,1))+G*D*H*C)\[G,G*D*H],nPoles);
endfunction

function sysMin = minssAutoTol(sys,nPoles)
    tol = 1e-30;
    while(1)
        sys = minss(sys)
		nSysPoles = size(sys.A,1);
        if (nSysPoles <= nPoles | tol > 1e-1) break; end;
		tol = tol*2;
	end
    if (nSysPoles == nPoles)
		//printf("\t\tconverged with right number of poles at %e\n",tol);
    elseif (nSysPoles < nPoles)
		//printf("\t\tpole zero cancellation occurred.\n");
    else
	    printf("\t\tWARNING: Failed to converge with correct number of poles.\n");
	    printf("\t\t\treal: %d calculated: %d\n",nRealPoles,nPoles);
    end
    sysMin = sys;
endfunction

function  infoNew = createIndex(names,info)
    // Used to define string/ indices for system

    // check if we are appending
    if (argn(2) == 2)
        iStart = max(size(info.str));
    else
        iStart = 0;
    end

    // add strings
     for i=1:max(size(names))
        execstr('info.str(i+iStart)=names(i)')
    end

    // add new indices
    for i=1:max(size(names))
        execstr('info.'+names(i)+'=i+iStart')
    end
    
    // output modified info
    infoNew = info;
endfunction

function dataNew = closeLoop(data,y,u,H)
    olss = minss(data.clss(y,u)*H);

    if (olss == 0)
        pm = -%inf;
        gm = -%inf;
    else
        pm = p_margin(olss)+180;
        for i=1:length(pm)
            if (pm(i) >= 180) pm(i) = pm(i) - 360; end
            if (pm(i) < -180) pm(i) = pm(i) + 360; end
        end
        gm = g_margin(olss);
    end

    data.clss = unityFeedback(y,u,data.clss,H);
    data.cltf = ss2tf(data.clss);
    uNew = max(size(data.u.str))+1;
    data.u = createIndex(data.y.str(y),data.u);
    
    dcGain = norm(horner(H,%eps));
    if (dcGain > 1e6) dcGain = %inf; end;

    deff("y=clMag(s)","y=norm(horner(ss2tf(minss(data.clss(y,uNew))),%i*s*2*%pi))");
    if (olss==0)
        gfc=%inf;
    else
        deff("y=clbw3dB(s)","y=clMag(s)-0.7079");
        freqGuess = 0;
        while(1)
            [gfc,v,info]=fsolve(freqGuess,clbw3dB,[],1e-30);
            if (info == 1 | freqGuess > 500) break; end;
            freqGuess = freqGuess+1;
        end
        if (gfc<0) gfc = -gfc; end
    end

    if ( size(pm) == 0) pm = -%inf; end
    if ( size(gm) == 0) gm = -%inf; end
    if ( size(gfc) == 0) gfc = %inf; end
    if ( min(gm) < 0 | min(pm) < 0)
        stability = "unstable";
    else 
        stability = "stable";
    end

    printf("%10s\t%10s\t%10s\t%7.2f\t%7.2f\t%7.2f\t%7.2f\n",..
        data.y.str(y), data.u.str(u),..
        stability, min(gm),min(pm),min(gfc),dcGain);

    dataNew = data;
endfunction

// vim:ts=4:sw=4:expandtab
