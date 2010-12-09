clear clc
load SCI/macros/scicos/lib
exec (loadpallibs,-1)
cd /hsl/homes/awkim/Projects/oooark/scicoslab/security

exec easystar-datcom_lin.sce;

//trim values
psi_t=%pi;


///delay factor
n_val = 4;   //number of testing values
values1 = linspace(-.5,.5,n_val);  //varible values for sensitiviy study    
values2 = linspace(-.5,.5,n_val);
values3 = linspace(-%pi/5,%pi/5,n_val);
values1 = [0,values1];
values2 = [0,values2];
values3 = [0,values3];

tf = 30;          //final integtration time for simulation
t_sample = .1;   //sampling time

load("BacksidePIDAutopilot_gainSched2.cos")

scs_m.props.tf=tf;


for i = 1:n_val+1
  for j = 1:n_val+1
    for k = 1:n_val+1
      exec easystar-datcom_lin.sce;
      h_t = x0(5)+x0(5)*values1(i);
      v_t = x0(1)+x0(1)*values2(j);
      phi_t = x0(7)+values3(k);

      Info = scicos_simulate(scs_m);
      if i == 1
        savematfile('Data\gain\time','del_alt_ft.time');
      end
      
      savematfile('Data\gain\del_alt_ft_'+string(i)+'_'+string(j)+'_'+string(k),'del_alt_ft.values');
      savematfile('Data\gain\del_v_m_s_'+string(i)+'_'+string(j)+'_'+string(k),'del_v_m_s.values');  
      savematfile('Data\gain\del_phi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_phi_deg.values');
      savematfile('Data\gain\del_r_rads_s_'+string(i)+'_'+string(j)+'_'+string(k),'del_r_rads_s.values');
      savematfile('Data\gain\del_psi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_psi_deg.values');
      clear del_alt_ft del_v_m_s del_phi_deg del_r_rads_s del_psi_deg
      mtlb_close all
    end
  end
end
savematfile('Data\gain\values1','values1');
savematfile('Data\gain\values2','values2');
savematfile('Data\gain\values3','values3');

  
  
  
  
  
