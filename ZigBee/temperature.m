function [A,B] = temperature(plot_flag, x, sensed_temp, sensed_temp_noise, h1, scount, current_time, simulation_time, sensor_id)

%variable name: type
%possible values: 1,2
%type = 1 corresponds to a temperature model with sinusoidal variation
%type = 2 corresponds to a temperature model as a cooling function

type = 1; 

if(plot_flag==0)

save temperature_sensing.mat;
% Teamperature gradient function 1 (Sinusoidal variation)
if type==1
sensed_temp(sensor_id) = 20+20*((sin(x(sensor_id,1)*current_time)+sin(x(sensor_id,2)*current_time)));
sensed_temp_noise(sensor_id)= awgn(sensed_temp(sensor_id),80,'measured');
end


%Temperature gradient function 2 (cooling function)
if type==2 
Tmax=100;   
ct=current_time/1000000;
sensed_temp(sensor_id) = Tmax-(Tmax/simulation_time)*ct;
sensed_temp_noise(sensor_id)= awgn(sensed_temp(sensor_id),50,'measured');
end
A = sensed_temp(sensor_id);
B = sensed_temp_noise(sensor_id);

else



% 3D graph plotting starts here

figure(h1)

if(scount>3)
    
subplot(1,2,1)

tri = delaunay(x(:,1),x(:,2));
trisurf(tri,x(:,1),x(:,2),sensed_temp);
%axis vis3d
min_p = min(sensed_temp);
max_p = max(sensed_temp);

z_min = round(2*min_p - max_p);
z_max = round(2*max_p - min_p);
axis([0 50 0 50 -100 100]); 
lighting phong
shading interp
colorbar EastOutside
xlabel('Sensor x-coordinate') % x-axis label
ylabel('Sensor y-coordinate') % y-axis label
zlabel('Temperature')
title('Actual Temperature');
subplot(1,2,2)

tri = delaunay(x(:,1),x(:,2));
trisurf(tri,x(:,1),x(:,2),sensed_temp_noise);
%axis vis3d
min_p = min(sensed_temp_noise);
max_p = max(sensed_temp_noise);

z_min = round(2*min_p - max_p);
z_max = round(2*max_p - min_p);
axis([0 50 0 50 -100 100]); 
lighting phong
shading interp
colorbar EastOutside
xlabel('Sensor x-coordinate') % x-axis label
ylabel('Sensor y-coordinate') % y-axis label
zlabel('Temperature')
title('Sensed Temperature values received at Sink');
% 3D graph plotting ends here

else
subplot(1,2,1)
bar(sensed_temp);
xlabel('Sensor ID') % x-axis label
ylabel('Temperature Sensed') % y-axis label
title('Actual Temperature');
subplot(1,2,2)
bar(sensed_temp_noise);
xlabel('Sensor ID') % x-axis label
ylabel('Temperature Sensed(noise)') % y-axis label
title('Sensed Temperature values received at Sink');
pause(1);
end

end

end












