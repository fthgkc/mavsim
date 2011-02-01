#!/bin/bash
srcPath=../src/navigation/
maxima -p lispconfig -b ins_dynamics.wxm

cat ins_dynamics_f.f90 | sed \
	-e "s/^f(/f[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/Vn\*\*2/Vn*Vn/g" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/\]/g" \
	-e "/^.*] = 0;$/d" \
	> ${srcPath}/ins_dynamics_f.hpp

cat ins_dynamics_f_att.f90 | sed \
	-e "s/^f_att(/f[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/Vn\*\*2/Vn*Vn/g" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/\]/g" \
	-e "/^.*] = 0;$/d" \
	> ${srcPath}/ins_dynamics_f_att.hpp

cat ins_dynamics_f_vp.f90 | sed \
	-e "s/^f_vp(/f[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/Vn\*\*2/Vn*Vn/g" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/\]/g" \
	-e "/^.*] = 0;$/d" \
	> ${srcPath}/ins_dynamics_f_vp.hpp

cat ins_error_dynamics_F.f90 | sed \
	-e "s/^F(/F[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\*\*2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_F\*/g" \
	> ${srcPath}/ins_error_dynamics_F.hpp

cat ins_error_dynamics_F_att.f90 | sed \
	-e "s/^F_att(/F[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\*\*2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_F\*/g" \
	> ${srcPath}/ins_error_dynamics_F_att.hpp

cat ins_error_dynamics_F_vp.f90 | sed \
	-e "s/^F_vp(/F[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\*\*2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_F\*/g" \
	> ${srcPath}/ins_error_dynamics_F_vp.hpp

cat ins_error_dynamics_G.f90 | sed \
	-e "s/^G(/G[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_G\*/g" \
	> ${srcPath}/ins_error_dynamics_G.hpp

cat ins_error_dynamics_G_att.f90 | sed \
	-e "s/^G_att(/G[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_G\*/g" \
	> ${srcPath}/ins_error_dynamics_G_att.hpp

cat ins_error_dynamics_G_vp.f90 | sed \
	-e "s/^G_vp(/G[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_G\*/g" \
	> ${srcPath}/ins_error_dynamics_G_vp.hpp

cat ins_dynamics_H_mag.f90 | sed \
	-e "s/^H_mag(/H_mag[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/a\*\*2/aa/g" -e "s/b\*\*2/bb/g" -e "s/c\*\*2/cc/g" -e "s/d\*\*2/dd/g" \
	-e "s/a\*\*3/aaa/g" -e "s/b\*\*3/bbb/g" -e "s/c\*\*3/ccc/g" -e "s/d\*\*3/ddd/g" \
	-e "s/cos(L)/cosL/g" -e "s/sin(L)/sinL/g" -e "s/tan(L)/tanL/g" \
	-e "s/sec(L)/secL/g" \
	-e "s/secL\**2/secLsecL/g" \
	-e "s/R\*\*2/RR/g" \
	-e "s/cosL\*\*2/cosLcosL/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_H_mag\*/g" \
	> ${srcPath}/ins_dynamics_H_mag.hpp

cat ins_dynamics_R_mag_n.f90 | sed \
	-e "s/^R_mag_n(/R_mag_n[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "s/cos(dec)/cosDec/g" -e "s/sin(dec)/sinDec/g" \
	-e "s/cos(dip)/cosDip/g" -e "s/sin(dip)/sinDip/g" \
	-e "s/cosDec\*\*2/cosDec2/g" -e "s/sinDec\*\*2/cosDec2/g" \
	-e "s/cosDip\*\*2/cosDip2/g" -e "s/sinDip\*\*2/cosDip2/g" \
	-e "s/sigDip\*\*2/sigDip2/g" -e "s/sigDec\*\*2/sigDec2/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_R_mag_n\*/g" \
	> ${srcPath}/ins_dynamics_R_mag_n.hpp

cat ins_dynamics_H_gps.f90 | sed \
	-e "s/^H_gps(/H_gps[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_H_gps\*/g" \
	> ${srcPath}/ins_dynamics_H_gps.hpp

cat ins_dynamics_R_gps.f90 | sed \
	-e "s/^R_gps(/R_gps[/g" -e "s/$/;/g" -e "s/) =/] =/g" \
	-e "/^.*] = 0;$/d" \
	-e "s/\[1,/\[0,/g" \
	-e "s/\[2,/\[1,/g" \
	-e "s/\[3,/\[2,/g" \
	-e "s/\[4,/\[3,/g" \
	-e "s/\[5,/\[4,/g" \
	-e "s/\[6,/\[5,/g" \
	-e "s/\[7,/\[6,/g" \
	-e "s/\[8,/\[7,/g" \
	-e "s/\[9,/\[8,/g" \
	-e "s/\[10,/\[9,/g" \
	-e "s/,1\]/,0\]/g" \
	-e "s/,2\]/,1\]/g" \
	-e "s/,3\]/,2\]/g" \
	-e "s/,4\]/,3\]/g" \
	-e "s/,5\]/,4\]/g" \
	-e "s/,6\]/,5\]/g" \
	-e "s/,7\]/,6\]/g" \
	-e "s/,8\]/,7\]/g" \
	-e "s/,9\]/,8\]/g" \
	-e "s/,10\]/,9\]/g" \
	-e "s/,/+rows_R_gps\*/g" \
	> ${srcPath}/ins_dynamics_R_gps.hpp

rm -rf *.f90
