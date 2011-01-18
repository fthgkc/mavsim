clear clc
load SCI/macros/scicos/lib
exec (loadpallibs,-1)
cd /hsl/homes/awkim/Projects/oooark/scicoslab/security

exec easystar-datcom_lin.sce;

//trim values
h_t=x0(5);
v_t=x0(1);
psi_t=180;
phi_t=x0(7);


///delay factor
n_val = 5;   //number of testing values
values1 = linspace(0,.2,n_val);  //varible values for sensitiviy study    
values2 = linspace(0,.2,n_val);
values3 = linspace(0,.2,n_val); 


tf = 30;          //final integtration time for simulation
t_sample = .1;   //sampling time

load("BacksidePIDAutopilot_digital2.cos")

scs_m.props.tf=tf;



for i = 1:n_val
  for j = 1:n_val
    for k = 1:n_val
      t_delay_throt = values1(i);
      t_delay_elev = values2(j);
      t_delay_rudd = values3(k);

      Info = scicos_simulate(scs_m);
      if i == 1
        savematfile('Data\digital\time','del_alt_ft.time');
      end
      
      savematfile('Data\digital\del_alt_ft_'+string(i)+'_'+string(j)+'_'+string(k),'del_alt_ft.values');
      savematfile('Data\digital\del_v_m_s_'+string(i)+'_'+string(j)+'_'+string(k),'del_v_m_s.values');  
      savematfile('Data\digital\del_phi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_phi_deg.values');
      savematfile('Data\digital\del_r_rads_s_'+string(i)+'_'+string(j)+'_'+string(k),'del_r_rads_s.values');
      savematfile('Data\digital\del_psi_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_psi_deg.values');
      savematfile('Data\gain\del_theta_deg_'+string(i)+'_'+string(j)+'_'+string(k),'del_theta_deg.values');
      clear del_alt_ft del_v_m_s del_phi_deg del_r_rads_s del_psi_deg del_theta_deg
      mtlb_close all
    end
  end
end

savematfile('Data\digital\values1','values1');
savematfile('Data\digital\values2','values2');
savematfile('Data\digital\values3','values3');
savematfile('Data\digital\h_t','h_t');
savematfile('Data\digital\v_t','v_t');
savematfile('Data\digital\psi_t','psi_t');
savematfile('Data\digital\phi_t','phi_t');
  
  
  
