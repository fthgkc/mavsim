/*
 * GpsIns.hpp
 * Copyright (C) Brandon Wampler <brandon.wampler@gmail.com>
 *
 * GpsIns.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GpsIns.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef oooark_GpsIns_HPP
#define oooark_GpsIns_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace oooark
{
namespace ublas = boost::numeric::ublas;

class GpsIns
{
public:
	GpsIns(double lat=0, double lon=0, double height=0, double roll=0, double pitch=0, double yaw=0, bool useGravity = true);
	void updateFast(const ublas::vector<double>& fb, const ublas::vector<double> &wb);
	void updateMed();
	void updateSlow();
	void updateGps(ublas::vector<double> z);
	
	double jFreq;
	double kFreq;
	double lFreq;
	double gFreq;
	int integrateIndex;

	double R0; //earth radius in meters
	double w; //earth rotation rate in radians

	double dt_j, dt_k, dt_l, dt_g;
	double kQuat;
	double sigmaNorm, zetaNorm;
	double as, ac, bs, bc;
	double g0;
	ublas::bounded_vector<double,3>g;
	
	ublas::bounded_vector<double,3>v;
	ublas::bounded_vector<double,3>sigma;
	ublas::bounded_vector<double,3>alfa;
	ublas::bounded_vector<double,3>deltaAlfa;
	ublas::bounded_vector<double,3>zeta;
	
	ublas::bounded_vector<double,4>q;
	ublas::bounded_vector<double,3>vn;
	ublas::bounded_vector<double,3>vnk;
	ublas::bounded_vector<double,3>vnkMinus;
	ublas::bounded_vector<double,4>r;
	ublas::bounded_vector<double,4>p;
	ublas::bounded_vector<double,3>un;
	ublas::bounded_vector<double,3>unCrossIntegral;
	ublas::bounded_vector<double,3>xn;
	ublas::bounded_vector<double,3>xnk;
	ublas::bounded_vector<double,3>xnkMinus;

	double h;
	ublas::bounded_vector<double,3>eulerAtt;
	ublas::bounded_vector<double,3>latLonH;

	ublas::bounded_vector<double,4>qLatLon;

	//GPS-INS filter variables
	ublas::bounded_vector<double,4>qLatLonCorrection;
	ublas::bounded_vector<double,4>qCorrection;
	ublas::bounded_vector<double,9>xErr;
	ublas::bounded_matrix<double,9,9> inverse;
	ublas::bounded_matrix<double,9,6> kalman;
	ublas::bounded_vector<double,3>Sigma; //incremental sum of body rotation
	ublas::bounded_vector<double,3>Un; //incremental sum of change in velocity
	ublas::bounded_vector<double,3>Zeta; //incremental sume of navigation frame rotation

	ublas::bounded_matrix<double,6,9> H;
	ublas::bounded_matrix<double,9,9> Fc;
	ublas::bounded_matrix<double,9,9> Fd;
	ublas::bounded_matrix<double,3,3> Frr, Frv, Fvr, Fvv, Fqr, Fqv, fnX, w_inX;
	
	//Noise Matrices
	double sigmaDegLat;
	double sigmaAccel;
	double sigmaGyro;
	double sigmaH;
	double sigmaV;
	double sigmaDeg;
	ublas::bounded_matrix<double,9,6> G;
	ublas::bounded_matrix<double,6,6> Q;
	ublas::bounded_matrix<double,9,9> Qk;
	ublas::bounded_matrix<double,6,6> Rk;

	ublas::bounded_matrix<double,9,9> P;
	
	//Time
	boost::posix_time::ptime time, time_j, time_k, time_l, time_g, time_0;
	boost::posix_time::time_duration diff_j, diff_k, diff_l, diff_g, diff_0;
	double elapsed;
	ublas::bounded_matrix<double,3,3> I3;

	ublas::bounded_vector<double,3>w_ie;
	ublas::bounded_vector<double,3>w_en;

};

} // oooark

#endif
// vim:ts=4:sw=4
