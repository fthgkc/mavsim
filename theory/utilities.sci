function sys = unityFeedback(y,u,G,H)
	C = zeros(length(y),size(G,1));
	for (i=1:length(y)) C(i,y(i)) = 1; end;
	D = zeros(size(G,2),size(u,2));
	for (i=1:length(u)) D(u(i),i) = 1; end;
	olss = G*H;
	realSysPoles = size(olss.A,1);
	tol = 1e-15;
	sysPoles = -1;
	while(tol < 1e-2 & sysPoles ~= realSysPoles)
		tol = tol*10;
		sys=minss((eye(size(G,1),size(G,1))+G*D*H*C)\[G,G*D*H]);
		sysPoles = size(sys.A,1);
	end
	printf("\n\ttolerance converged with right number of poles at %e\n",tol);
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

    oltf = data.cltf(y,u)*H;

    pm = p_margin(oltf)+180;
    for i=1:length(pm)
        if (pm(i) > 180) pm(i) = pm(i) - 360; end
        if (pm(i) < -180) pm(i) = pm(i) + 360; end
    end

    gm = g_margin(oltf);
    if ( size(pm) == 0) pm = -%inf; end
    if ( size(gm) == 0) gm = -%inf; end
    if ( min(gm) < 0 | min(pm) < 0)
        stability = "unstable";
    else 
        stability = "stable";
    end
    printf("\t%s\tgain margin: %6.1f\tphase margin: %6.1f\t%s\n",..
        data.y.str(y),min(gm),min(pm),stability);

    printf("\t\tclosing %8s -> %8s loop\n", data.u.str(u), data.y.str(y));
    data.clss = unityFeedback(y,u,data.clss,H);
    data.cltf = clean(ss2tf(data.clss),1e-8);
    data.u = createIndex(data.y.str(y),data.u);
    dataNew = data;

endfunction

// vim:ts=4:sw=4:expandtab
