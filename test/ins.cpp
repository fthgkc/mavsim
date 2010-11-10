#define NDEBUG
#include "math/GpsIns.hpp"
int main (int argc, char const* argv[])
{
	boost::numeric::ublas::bounded_vector<double,3> fb;
	boost::numeric::ublas::bounded_vector<double,3> wb;
	fb = wb = boost::numeric::ublas::zero_vector<double>(3);

	double lat = 45*M_PI/180.0;
	double lon = -86*M_PI/180.0;
	double height = 100;
	double roll = 0*M_PI/180.0;
	double pitch = 0*M_PI/180.0;
	double yaw = 0*M_PI/180.0;
	bool useGravity = false;
	oooark::GpsIns gpsIns(lat,lon,height,roll,pitch,yaw,useGravity);
	oooark::GpsIns gpsInsError(lat,lon,height,roll,pitch,yaw,useGravity);

	double fbNoiseFreq = 100;
	double fbNoiseMag = .001*9.81;
	double fbNoise;

	double wbNoiseFreq = 100;
	double wbNoiseMag = 1*M_PI/180;
	double wbNoise;
	boost::numeric::ublas::bounded_vector<double,3> ones3;
	for(int i=0;i<ones3.size();i++) ones3(i) = 1.0;

	boost::numeric::ublas::bounded_vector<double,6> z;

	while(1)
	{
		//No noise
		gpsIns.updateFast(fb,wb);
		gpsIns.updateMed();
		gpsIns.updateSlow();

		//Noisy
		fbNoise = fbNoiseMag*sin(gpsIns.elapsed*2*M_PI*fbNoiseFreq);
		wbNoise = wbNoiseMag*sin(gpsIns.elapsed*2*M_PI*wbNoiseFreq);

		gpsInsError.updateFast(fb+fbNoise*ones3,wb+wbNoise*ones3);
		gpsInsError.updateMed();
		gpsInsError.updateSlow();

		z(0) = (gpsInsError.latLonH(0) - gpsIns.latLonH(0)) * (gpsInsError.latLonH(2)+gpsInsError.R0);
		z(1) = (gpsInsError.latLonH(1) - gpsIns.latLonH(1)) * (gpsInsError.latLonH(2)+gpsInsError.R0)*cos(gpsInsError.latLonH(0));
		z(2) = (gpsInsError.latLonH(2) - gpsIns.latLonH(2));
		z(3) = gpsInsError.vn(0) - gpsIns.vn(0);
		z(4) = gpsInsError.vn(1) - gpsIns.vn(1);
		z(5) = gpsInsError.vn(2) - gpsIns.vn(2);

		gpsInsError.updateGps(z);

		usleep(1.0/1000.0*1e6);
	}
		return 0;
}



