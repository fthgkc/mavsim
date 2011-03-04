/*sci_windDynamics.cpp
 * Copyright (C) James Goppert Nicholas Metaxas 2011 
 * 
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Input: 
 *  u1: F_b_T(0,0) , F_b_T(1,0) , F_b_T(2,0) 
 *  u2: M_b_T(0,0) , M_b_T(1,0) , M_b_T(2,0)
 *  u3: F_w_A(0,0) , F_w_A(1,0) , F_w_A(2,0)
 *  u4: M_b_A(0,0) , M_b_A(1,0) , M_w_A(2,0)
 *  u5: Jx, Jy, Jz, Jxy, Jxz, Jyz, g, m 
 *  u6: (state) Vt, alpha, theta, wy, h, beta, phi, wx, psi, wz 
 * Output:
 *  y1: (state derivative)
 *
 */

#define BOOST_UBLAS_SHALLOW_ARRAY_ADAPTOR 1

#include <iostream>
#include <string>
#include <cstdlib>
#include "utilities.hpp"
#include <stdexcept>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

void sci_windDynamics(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * u3=(double*)GetInPortPtrs(block,3);
    double * u4=(double*)GetInPortPtrs(block,4);
    double * u5=(double*)GetInPortPtrs(block,5);
    double * u6=(double*)GetInPortPtrs(block,6);
    double * y1=(double*)GetOutPortPtrs(block,1);

    double * rpar=block->rpar;
    int * ipar=block->ipar;

    // aliases

    // double & Omega = rpar[0];
    //double & Re = rpar[1];
    //int & mode = ipar[0];

    // Note that l = lon, and not in the equations but left here
    // for ease of use with full state vector x
    double & Jx      = u5[0];
    double & Jy      = u5[1];
    double & Jz      = u5[2];
    double & Jxy     = u5[3];
    double & Jxz     = u5[4];
    double & Jyz     = u5[5];
    double & g       = u5[6];
    double & m       = u5[7];

    double & Vt      = u6[0];
    double & alpha   = u6[1];
    double & theta   = u6[2];
    double & wy      = u6[3];
    double & h       = u6[4];
    double & beta    = u6[5];
    double & phi     = u6[6];
    double & wx      = u6[7];
    double & psi     = u6[8];
    double & wz      = u6[9];

    // sizes
    int nY = 10;
 

    // matrices
    using namespace boost::numeric::ublas;
    matrix<double,column_major, shallow_array_adaptor<double> > F_b_T_(3,1,shallow_array_adaptor<double>(3,u1));
    matrix<double,column_major, shallow_array_adaptor<double> > M_b_T_(3,1,shallow_array_adaptor<double>(3,u2));
    matrix<double,column_major, shallow_array_adaptor<double> > F_w_A_(3,1,shallow_array_adaptor<double>(3,u3));
    matrix<double,column_major, shallow_array_adaptor<double> > M_b_A_(3,1,shallow_array_adaptor<double>(3,u4));
    matrix<double,column_major, shallow_array_adaptor<double> > d_x_wind(nY,1,shallow_array_adaptor<double>(nY,y1));

    //handle flags
    if (flag==scicos::computeOutput)
    {
        const double cosAlpha = cos(alpha);
        const double sinAlpha = sin(alpha);
        const double cosBeta = sin(beta);
        const double sinBeta = cos(beta);
        const double sinPhi = sin(phi);
        const double cosPhi = cos(phi);
        const double sinTheta = sin(theta);
        const double cosTheta = cos(theta);
        const double tanTheta = tan(theta);
        const double JxyJxy = Jxy*Jxy;
        const double JxzJxz = Jxz*Jxz;
        const double JyzJyz = Jyz*Jyz;
    
        #include "dynamics/windDynamics.hpp"
 
    }
    else if (flag==scicos::terminate)
    {
    }
    else if (flag==scicos::initialize || flag==scicos::reinitialize)
    {
    }
    else
    {
        std::cout << "unhandled block flag: " << flag << std::endl;
    }
}

} // extern c

// vim:ts=4:sw=4:expandtab
