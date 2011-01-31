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

cat ins_dynamics_error_F.f90 | sed \
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
	-e "s/,/+rowsF\*/g" \
	> ${srcPath}/ins_dynamics_error_F.hpp

cat ins_dynamics_error_G.f90 | sed \
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
	-e "s/,/+rowsG\*/g" \
	> ${srcPath}/ins_dynamics_error_G.hpp

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
	-e "s/,/+rowsH\*/g" \
	> ${srcPath}/ins_dynamics_H_mag.hpp

rm -rf *.f90
