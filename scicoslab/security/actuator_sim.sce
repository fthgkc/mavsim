clc
load SCI/macros/scicos/lib
exec (loadpallibs,-1)
cd /hsl/homes/awkim/Projects/oooark/scicoslab/security

exec easystar-datcom_lin.sce;

//trim values

%scicos_context.h_c=x0(5);
%scicos_context.v_c=x0(1);
%scicos_context.psi_c=180;
%scicos_context.phi_c=x0(7);

///delay factor
n_val = 4;   //number of testing values

values1 = [0,-1,-.3,.3,1];
values2 = [0,-1,-.3,.3,1];
values3 = [0,-1,-.3,.3,1];

tf = 30;          //final integtration time for simulation
%scicos_context.t_sample = .1;   //sampling time

load("BacksidePIDAutopilot_actuator4.cos")

scs_m.props.tf=tf;


for i = 1:n_val+1
  for j = 1:n_val+1
    for k = 1:n_val+1

      %scicos_context.act_throttle = values1(i);
      %scicos_context.act_elevator = values2(j);
      %scicos_context.act_rudder = values3(k);
      Info = scicos_simulate(scs_m);
      if i == 1
        savematfile('Data\actuator\time','del_alt_ft.time');
      end
      
      savematfile('Data\actuator\del_alt_ft_'+string(i)+'_'+string(j)+'_'+string(k),'del_alt_ft.values');
      savematfile('Data\actuator\del_v_m_s_'+string(i)+'_'+string(j)+'_'+string(k),'del_v_m_s.values');  
      savematfile('Data\actuator\del_phi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_phi_deg.values');
      savematfile('Data\actuator\del_r_rads_s_'+string(i)+'_'+string(j)+'_'+string(k),'del_r_rads_s.values');
      savematfile('Data\actuator\del_psi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_psi_deg.values');
      clear del_alt_ft del_v_m_s del_phi_deg del_r_rads_s del_psi_deg
      mtlb_close all
    end
  end
end
  
savematfile('Data\actuator\values1','values1');
savematfile('Data\actuator\values2','values2');
savematfile('Data\actuator\values3','values3');
  
  
