clc
numRuns = 10;
load SCI/macros/scicos/lib
exec (loadpallibs,-1)
cd /hsl/homes/awkim/Projects/oooark/scicoslab/security

exec easystar-datcom_lin.sce;

//trim values
psi_t=%pi;
h_t = x0(5);
v_t = x0(1);
phi_t = x0(7);

///delay factor
n_val = 5;   //number of testing values
values1 = linspace(0,.2,n_val);  //varible values for sensitiviy study    
values2 = linspace(0,.2,n_val);
values3 = linspace(0,.2,n_val);

tf = 30;          //final integtration time for simulation
t_sample = .1;   //sampling time

load("BacksidePIDAutopilot_fuzzing2.cos")

scs_m.props.tf=tf;

for i = 1:n_val
  for j = 1:n_val
    for k = 1:n_val
      altTemp=0;
      vTemp=0;
      phiTemp = 0;
      rTemp = 0;
      psiTemp = 0;
      thetaTemp = 0;
      for m = 1:numRuns
        exec easystar-datcom_lin.sce;
        
        sig_throt = values1(i);
        sig_elev = values2(j);
        sig_rudd = values3(k);
        Info = scicos_simulate(scs_m);
        if i == 1
          savematfile('Data\fuzz\time','del_alt_ft.time');
        end
        altTemp = altTemp+del_alt_ft.values.^2;
        vTemp = vTemp+del_v_m_s.values.^2;
        phiTemp = phiTemp+del_phi_deg.values.^2;
        rTemp = rTemp +del_r_rads_s.values.^2;
        psiTemp = psiTemp +del_psi_deg.values.^2;
        thetaTemp = thetaTemp + del_theta_deg.values.^2;
        clear del_alt_ft del_v_m_s del_phi_deg del_r_rads_s del_psi_deg del_theta_deg
      end
      
      altTemp = altTemp/numRuns;
      vTemp = vTemp/numRuns;
      phiTemp = phiTemp/numRuns;
      rTemp = rTemp/numRuns;
      psiTemp = psiTemp/numRuns;
      thetaTemp = thetaTemp/numRuns;
      savematfile('Data\fuzz\del_alt_ft_'+string(i)+'_'+string(j)+'_'+string(k),'altTemp');
      savematfile('Data\fuzz\del_v_m_s_'+string(i)+'_'+string(j)+'_'+string(k),'vTemp');  
      savematfile('Data\fuzz\del_phi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'phiTemp');
      savematfile('Data\fuzz\del_r_rads_s_'+string(i)+'_'+string(j)+'_'+string(k),'rTemp');
      savematfile('Data\fuzz\del_psi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'psiTemp');
      savematfile('Data\fuzz\del_theta_deg_'+string(i)+'_'+string(j)+'_'+string(k),'thetaTemp');
      
    end
  end
end
  
savematfile('Data\fuzz\values1','values1');
savematfile('Data\fuzz\values2','values2');
savematfile('Data\fuzz\values3','values3');
  
  
  
