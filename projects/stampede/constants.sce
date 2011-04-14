mode(-1)

x = createIndex(['V','roll','pitch','yaw','rollRate','pitchRate','yawRate','cog','sog','lat','lon','alt']);
u = createIndex(['STR','THR','MODE','MISC']);
y = x;
r = createIndex(['yaw','V']);

// time constants
tau_motor = 1;
tau_servo = 20;

x0 = zeros(size(x.str,1),1);
x0(x.V)=2; // 2 m/s
x0(x.cog)=90*%pi/180;

// start at purdue
x0(x.lat)=  40.430779*%pi/180;
x0(x.lon)= -86.916152*%pi/180;

noiseStdDev = zeros(12,1);
noiseBias = zeros(12,1);

noiseStdDev(x.lat) = 1e-7;
noiseStdDev(x.lon) = 1e-7;
noiseStdDev(x.alt) = 1;

// controllers
H.yaw_STR = 0.9*%s/%s; // %s/%s lets scicoslab know this is a tranfer function
H.V_THR = 0.7  + 0.1/%s;

u0 = [0;.4];
