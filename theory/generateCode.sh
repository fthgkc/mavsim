#!/bin/bash
srcPath=../src/navigation/
sciPath=.
maxima -b navigation.wxm

cat code/ins_dynamics_f.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/Vn\*\*2/Vn*Vn/g" \
	> ${srcPath}/ins_dynamics_f.hpp

cat code/ins_dynamics_f_att.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/Vn\*\*2/Vn*Vn/g" \
	> ${srcPath}/ins_dynamics_f_att.hpp

cat code/ins_dynamics_f_vp.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/Vn\*\*2/Vn*Vn/g" \
	> ${srcPath}/ins_dynamics_f_vp.hpp

cat code/ins_error_dynamics_F.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\*\*2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_error_dynamics_F.hpp

cat code/ins_error_dynamics_F_att.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\*\*2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_error_dynamics_F_att.hpp

cat code/ins_error_dynamics_F_vp.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\*\*2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_error_dynamics_F_vp.hpp

cat code/ins_error_dynamics_G.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_error_dynamics_G.hpp

cat code/ins_error_dynamics_G_att.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_error_dynamics_G_att.hpp

cat code/ins_error_dynamics_G_vp.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_error_dynamics_G_vp.hpp

cat code/ins_H_mag.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_H_mag.hpp

cat code/ins_H_mag_att.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	> ${srcPath}/ins_H_mag_att.hpp

cat code/ins_R_mag_n.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	> ${srcPath}/ins_R_mag_n.hpp

cat code/ins_H_gps.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	> ${srcPath}/ins_H_gps.hpp

cat code/ins_H_gps_vp.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	> ${srcPath}/ins_H_gps_vp.hpp

cat code/ins_R_gps.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	> ${srcPath}/ins_R_gps.hpp

cat code/ins_C_nb_quat.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	> ${srcPath}/ins_C_nb_quat.hpp

cat code/ins_C_nb_euler.f90 | sed \
	-e "s/$/;/g" \
	-e "/^.*) = 0;$/d" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	> ${srcPath}/ins_C_nb_euler.hpp

maxima -b quadDynamics.wxm

cat code/quad_forward_F.f90 | sed \
	-e "s/$/;/g" \
	> ${sciPath}/quad.sci

cat code/quad_forward_G.f90 | sed \
	-e "s/$/;/g" \
	>> ${sciPath}/quad.sci

cat code/quad_C.f90 | sed \
	-e "s/$/;/g" \
	>> ${sciPath}/quad.sci

echo "quad_forward_ss = syslin('c',quad_forward_F,quad_forward_G,quad_C);" >> ${sciPath}/quad.sci

