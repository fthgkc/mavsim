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
	double lFreq = 100;
	double gFreq = 1;
	int integrateIndex=0;

	double dt_j, dt_k, dt_l, dt_g;
	dt_j = dt_k = dt_l = dt_g = 0;
	double kQuat = 1;
	double sigmaNorm, zetaNorm;
	double as, ac, bs, bc;
	double g0 = 9.81;
	vector<double> g = zero_vector<double>(3);
	//g(2) = g0;
	vector<double> wb = zero_vector<double>(3);
	//wb(0) = 1*M_PI/180;
	vector<double> fb = zero_vector<double>(3);
	fb(0) = 1000;
	fb(1) = 1000;
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

	//filter variables
	matrix<double> H = zero_matrix<double>(6,10);
	matrix<double> F = zero_matrix<double>(10,10);
	matrix<double> Frr(3,3), Frv(3,3), Fvr(3,3), Fvv(3,3), Fqr(4,3), Fqv(4,4), fnX(3,4), w_inX(4,4);
	Frr = Frv = Fvr = Fvv = zero_matrix<double>(3,3);
	Fqr = zero_matrix<double>(4,3);
       	Fqv = zero_matrix<double>(4,3);
        fnX = zero_matrix<double>(3,4);
        w_inX = zero_matrix<double>(4,4);

	boost::posix_time::ptime time, time_j, time_k, time_l, time_g, time_0;
	time = time_j = time_k = time_l= time_g = time_0 = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration diff_j, diff_k, diff_l, diff_g, diff_0;
	double elapsed=0;
	matrix<double> I3 = identity_matrix<double>(3);
	vector<double> w_ie = zero_vector<double>(3);
	vector<double> w_en = zero_vector<double>(3);
	double R0 = 6.3781e6; //earth radius in meters
	double w = 7.292115e-5; //earth rotation rate in radians
	double h;
	
	vector<double> eulerAtt(3);
	eulerAtt(0) = 0;
	eulerAtt(1) = 0;
	eulerAtt(2) = M_PI;
	q= euler2Quat(eulerAtt);
	vector<double> latLon(2);

	vector<double> qLatLon(4);

	latLon(0) = 0*M_PI/180.0;
	latLon(1) = -0*M_PI/180.0;
	h = 200.0;

	qLatLon = latLon2Quat(latLon);

	std::cout<<"Quat: "<<qLatLon<<std::endl;
	std::cout<<"Euler: "<<quat2Euler(qLatLon)*180/M_PI<<std::endl;
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
			vn += un + g*(R0/(R0+h))*(R0/(R0+h))*dt_k;

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
			fb(1)=0;
			time_l=time;
			diff_0 = time - time_0;
			elapsed = diff_0.total_microseconds()/1e6;
			std::cout<<"Elapsed: "<<elapsed<<std::endl;	
			
			//coriolis correction
			w_ie(0) = w*cos(latLon(0));
			w_ie(1) = 0;
			w_ie(2) = w*sin(latLon(0));
			
			vn =  prod((I3 - 2.0*cross(w_ie)*dt_l - cross(zeta)),vn);
		
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
			qLatLon = quatProd(p,qLatLon);
			
			std::cout<<"P Euler: "<<quat2Euler(p)<<std::endl;
			std::cout<<"P Quat: "<<p<<std::endl;
			std::cout<<"QLATLON: "<<quat2Euler(qLatLon)*180/M_PI<<std::endl;
			std::cout<<"QLATLON: "<<quat2LatLon(qLatLon)*180/M_PI<<std::endl;
			std::cout<<"LAT LON H: "<<latLon*180/M_PI<<" "<<h<<std::endl;
			
			std::cout<<"Pos: "<<xn<<std::endl;
			std::cout<<"Vel: "<<vn<<std::endl;
			std::cout<<"Att: "<<quat2Euler(q)*180/M_PI<<std::endl;
			std::cout<<std::endl;
			
			//quaternion normilatization
			q+=q*kQuat*(1-(q(0)*q(0) + q(1)*q(1) + q(2)*q(2) + q(3)*q(3)));
			qLatLon+=qLatLon*kQuat*(1-(qLatLon(0)*qLatLon(0) + qLatLon(1)*qLatLon(1) + qLatLon(2)*qLatLon(2) + qLatLon(3)*qLatLon(3)));
			//std::cout<<"l-cycle Hz: "<<1.0/dt_l<<std::endl;
			
			//reset integration term
			zeta = zero_vector<double>(3);
		}

		diff_g = time-time_g;
		dt_g = diff_g.total_microseconds()/1e6;
		if(dt_g>=gFreq)
		{
			double cosLat = cos(latLon(0));
			double tanLat = tan(latLon(0));
			double sinLat = sin(latLon(0));
			double R = R0+h;
			double vN = v(0);
			double vE = v(1);
			double vD = v(2);

			Frr(0,0)=0 	,Frr(0,1)=0 	, Frr(0,2)=vN/(R*R);
			Frr(1,0)=-vE*sinLat/(R*cosLat*cosLat) 	, Frr(1,1)=0 	, Frr(1,2)= -vE/(R*R*cosLat*cosLat);
			Frr(2,0)=0 	,Frr(2,1)=0 	, Frr(2,2)=0 	;

			Frv(0,0)=1.0/R	, Frv(0,1)=0 	, Frv(0,2)= 0	;
			Frv(1,0)=0 	, Frv(1,1)= 1.0/(R*cosLat)	, Frv(1,2)=0 	;
			Frv(2,0)= 0	, Frv(2,1)= 0	, Frv(2,2)= -1 	;
			
			Fvr(0,0)= -2*w*vE*cosLat-vE*vE/(R*cosLat*cosLat)	, Fvr(0,1)=0 	, Fvr(0,2)= -vN*vD/(R*R) + vE*vE*tanLat/(R*R);
			Fvr(1,0)= 2*w*(sinLat*vD-vN*cosLat) + vE*vN/(R*cosLat*cosLat)	, Fvr(1,1)=0 	, Fvr(1,2)= vD*vE/(R*R) + vE*vN*tanLat/(R*R)	;
			Fvr(2,0)= 2*w*vE*sinLat	, Fvr(2,1)=0 	, Fvr(2,2)= vE*vE/(R*R) + vN*vN/(R*R) - 2*g0*((R0*R0/(R*R))/(R))	;

			Fvv(0,0)=vD/R 	, Fvv(0,1)=-2*w*sinLat-2*vE*tanLat/R 	, Fvv(0,2)=vN/R	;
			Fvv(1,0)= -2*w*sinLat-vE*tanLat/R , Fvv(1,1)= -vD-vN*tanLat/R	, Fvv(1,2)= -2*w*cosLat-vE/R	;
			Fvv(2,0)= -2*vN/R	, Fvv(2,1)= -2*w*cosLat-2*vE/R	, Fvv(2,2)= 0	;

			//TODO Program Fq matrices and fnX, w_inX
			//Fqr
			//Fqv
			//fnX
			//w_inX


			subrange(F, 0,3,  0,3) = Frr, subrange(F, 0,3,  3,6) = Frv;
			subrange(F, 3,6,  0,3) = Fvr, subrange(F, 3,6,  3,6) = Fvv, subrange(F, 3,6,  6,10) = fnX;
			subrange(F, 6,10, 0,3) = Fqr, subrange(F, 6,10, 3,6) = Fqv, subrange(F, 6,10, 6,10) = w_inX;

			H(0,0) = R;
			H(1,1) = R*cosLat;
			H(2,2) = H(3,3) = H(4,4) = H(5,5) = 1.0;

		}
	}

	return 0;
}


