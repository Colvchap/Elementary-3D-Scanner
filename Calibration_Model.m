distancesCM =[10; 15; 20; 28; 40; 60; 80; 100];
val = [545; 544; 495; 400; 293; 196; 149; 116];
clean_val = 5/1024.*val;
hold on
plot(distancesCM,log(clean_val))
[r,m,b]=regression(distancesCM,clean_val,'one');

goodD = (log(val)-b)/m;

%% Our Model is D=(e^log(val)-2.7896)/-0.0253
