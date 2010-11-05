#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "math/utilities.hpp"

using namespace boost::numeric::ublas;
using namespace oooark;
int main (int argc, char const* argv[])
{
	double jFreq = 500;
	double kFreq = 100;
	double lFreq = 10;
	int integrateIndex=0;

	double dt_j, dt_k, dt_l;
	dt_j = dt_k = dt_l = 0;
	double kQuat = 1;
	double sigmaNorm, zetaNorm;
	double as, ac, bs, bc;
	vector<double> g = zero_vector<double>(3);
	//g(2) = 9.81;
	vector<double> wb = zero_vector<double>(3);
	//wb(0) = 1*M_PI/180;
	vector<double> fb = zero_vector<double>(3);
	fb(0) = 1000;
	fb(1) =1000;
	//fb(1) = 1;
	//fb(2) = 1;
	//fb(2)=-9.81;
	
	vector<double> v = zero_vector<double>(3);
	vector<double> sigma = zero_vector<double>(3);
	vector<double> alfa = zero_vector<double>(3);
	vector<double> deltaAlfa = zero_vector<double>(3);
	vector<double> zeta = zero_vector<double>(3);
	
	vector<double> q = zero_vector<double>(4);
	vector<double> vn = zero_vector<double>(3);
	//vn(0) = 10;
	//vn(1) = 10;
	vector<double> vnk = zero_vector<double>(3);
	vector<double> vnkMinus = zero_vector<double>(3);
	vector<double> r = zero_vector<double>(4);
	vector<double> p = zero_vector<double>(4);
	vector<double> un = zero_vector<double>(3);
	vector<double> unCrossIntegral = zero_vector<double>(3);
	vector<double> xn = zero_vector<double>(3);
	vector<double> xnk = zero_vector<double>(3);
	vector<double> xnkMinus = zero_vector<double>(3);
	
	q(0)=1;
	matrix<double> Sigma = zero_matrix<double>(4,4);
	boost::posix_time::ptime time, time_j, time_k, time_l, time_0;
	time = time_j = time_k = time_l= time_0 = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration diff_j, diff_k, diff_l, diff_0;
	double elapsed=0;
	matrix<double> I3 = identity_matrix<double>(3);
	vector<double> w_ie = zero_vector<double>(3);
	vector<double> w_en = zero_vector<double>(3);
	double R0 = 6.3781e6; //earth radius in meters
	double omega = 7.27e-5; //earth rotation rate in radians
	double h;
	vector<double> latLon(2);

	latLon(0) = 40*M_PI/180.0;
	latLon(1) = -86*M_PI/180.0;
	h = 200.0;

	while(1)
	{
		//calculate elapsed time for each cycle
		time = boost::posix_time::microsec_clock::universal_time();
		
		diff_j = time-time_j;
		dt_j = diff_j.total_microseconds()/1e6;
		
		if (dt_j >= 1.0/jFreq)
		{
			time_j=time;
			alfa += wb*dt_j;
			deltaAlfa+=crossProd(alfa,wb)*dt_j;
			
			v+=fb*dt_j;
			unCrossIntegral+=(crossProd(alfa,fb)-crossProd(wb,v))*dt_j;
			//std::cout<<"j-cycle Hz: "<<1.0/dt_j<<std::endl;
		}
		
		diff_k = time-time_k;
		dt_k = diff_k.total_microseconds()/1e6;
		
		if (dt_k>=1.0/kFreq)
		{
			time_k=time;
			sigma = alfa+deltaAlfa;

			sigmaNorm = norm_2(sigma);
			ac=cos(sigmaNorm/2.0);
			if (sigmaNorm<1e-100) as = 0;
			else as=sin(sigmaNorm/2.0)/sigmaNorm;
			r(0)=ac;
			r(1)=as*sigma(0);
			r(2)=as*sigma(1);
			r(3)=as*sigma(2);
			q = quatProd(q,r);
			//q+=q*kQuat*(1-(q(0)*q(0) + q(1)*q(1) + q(2)*q(2) + q(3)*q(3)));
			//std::cout<<"Euler: "<<quat2Euler(q)*180/M_PI<<std::endl;

			un = v + 0.5*crossProd(alfa,v) + 0.5*unCrossIntegral;
			un = quatRotate(q,un);
			
			xnkMinus = xnk;
			xnk = xn;
			
			vnkMinus = vnk;
			vnk = vn;
			vn += un + g*dt_k;

			if (integrateIndex == 2)
			{
				xn = xnkMinus + ((vnkMinus + 4*vnk + vn)/3.0)*dt_k;
				//std::cout<<"vnkMinus: "<<vnkMinus<<std::endl;
				//std::cout<<"vnk: "<<vnk<<std::endl;
				//std::cout<<"vn: "<<vn<<std::endl;
			}
			else if (integrateIndex == 1) xn+= ((vnk+vn)/2.0)*dt_k, integrateIndex++;
			else if (integrateIndex == 0) xn+= vnk*dt_k, integrateIndex++;

			latLon(0) += vn(0)*dt_k/(R0+h);
			latLon(1) += vn(1)*dt_k/(cos(latLon(0))*(R0+h));
			h += -vn(2)*dt_k;

			w_en(0) = vn(1)/(R0+h);
			w_en(1) = -vn(0)/(R0+h);
			w_en(2) = -vn(1)*tan(latLon(0))/(R0+h);
			zeta+=w_en*dt_k;
			//reset integrating terms
			alfa = deltaAlfa = v = unCrossIntegral = zero_vector<double>(3);

			//std::cout<<"k-cycle Hz: "<<1.0/dt_k<<std::endl;
		}

		diff_l = time-time_l;
		dt_l = diff_l.total_microseconds()/1e6;
	
		if(dt_l>=1.0/lFreq)
		{
			fb(0)=0;
			time_l=time;
			diff_0 = time - time_0;
			elapsed = diff_0.total_microseconds()/1e6;
			//std::cout<<"Elapsed: "<<elapsed<<std::endl;	
			////coriolis correction

			//w_ie(0) = omega*cos(lat);
			//w_ie(2) = -omega*sin(lat);
			//vn =  prod((I3 - 2*cross(w_ie) - cross(w_en))*dt_l,vn);
			
			//rotating navigation frame correction
			zetaNorm = norm_2(zeta);
			bc=cos(zetaNorm/2.0);
			if (zetaNorm<1e-100) bs = 0;
			else bs=sin(zetaNorm/2.0)/zetaNorm;
			p(0)=bc;
			p(1)=bs*zeta(0);
			p(2)=bs*zeta(1);
			p(3)=bs*zeta(2);
			q = quatProd(quatConj(p),q);
			vn = quatRotate(quatConj(p),vn);
			
			std::cout<<"P Euler: "<<quat2Euler(p)<<std::endl;
			std::cout<<"P Quat: "<<p<<std::endl;
			
			std::cout<<"Pos: "<<xn<<std::endl;
			std::cout<<"Vel: "<<vn<<std::endl;
			std::cout<<"Att: "<<quat2Euler(q)<<std::endl;
			std::cout<<"LAT LON H: "<<latLon*180/M_PI<<" "<<h<<std::endl;
			std::cout<<std::endl;
			
			//quaternion normilatization
			q+=q*kQuat*(1-(q(0)*q(0) + q(1)*q(1) + q(2)*q(2) + q(3)*q(3)));
			//std::cout<<"l-cycle Hz: "<<1.0/dt_l<<std::endl;
			
			//reset integration term
			zeta = zero_vector<double>(3);
		}
	}

	return 0;
}


