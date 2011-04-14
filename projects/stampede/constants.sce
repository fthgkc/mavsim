mode(-1)

// time constants
tau_motor = 1;
tau_servo = 20;

// heading 180 deg
x0(10)=90*%pi/180;

// start at purdue
x0(11)=  40.430779*%pi/180;
x0(12)= -86.916152*%pi/180;

x = createIndex(['V','roll','pitch','yaw','rollRate','pitchRate','yawRate','cog','sog','lat','lon','alt']);
u = createIndex(['STR','THR','MODE','MISC']);
y = x;
r = createIndex(['yaw','V']);

noiseStdDev = zeros(12,1);
noiseBias = zeros(12,1);

noiseStdDev(x.lat) = 1e-7;
noiseStdDev(x.lon) = 1e-7;
noiseStdDev(x.alt) = 1;

// controllers
H.yaw_STR = 0.9*%s/%s; // %s/%s lets scicoslab know this is a tranfer function
H.V_THR = 0.7  + 0.1/%s;
