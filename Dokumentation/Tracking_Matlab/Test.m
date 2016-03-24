%% Trackingsystem - Camera Coordinate Transformation Visualisation
% *Bachelorprojekt WS 2015/16 - HAW Hamburg - Quadrocopter-Tracking und Positionsregelung*
% colors: r, g, b,  c, m, y, k

clc;
clear all;

null = [0; 0; 0];
baseXaxis = [1000; 0; 0];
baseYaxis = [0; 1000; 0];
baseZaxis = [0; 0; 1000];

% Camera Information
cam1_o = [8950 3510 2900];  % camera position
cam2_o = [8955 1100 2900];  % camera position

cam1_p = [0 810 920];       % camera central point on wall
cam1_t = [0 3920 990];      % camera second point on wall
cam2_p = [0 1780 910];      % camera central point on wall
cam2_t = [0 6430 1050];     % camera second point on wall

cam1_r = cam1_p - cam1_o;   % camera central beam (direction vector)
cam1_s = cam1_t - cam1_o;   % camera secondary beam for last degree of freedom
cam2_r = cam2_p - cam2_o;   % camera central beam (direction vector)
cam2_s = cam2_t - cam2_o;   % camera secondary beam for last degree of freedom

%% Setup
clf;
grid on;
axis equal;
xlim([0 10000]);
ylim([0 7000]);
zlim([0 4000]);
hold on;

% World Coordinate System Indicator
CoordSys = baseXaxis;
plotable = makeRay(CoordSys); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'r');
CoordSys = baseYaxis;
plotable = makeRay(CoordSys); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'g');
CoordSys = baseZaxis;
plotable = makeRay(CoordSys); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'b');

% Camera Vectors
%plotable = [null cam1_o'];    plot3(plotable(1,:),plotable(2,:),plotable(3,:),'-.c'); %position
plotable = [cam1_p' cam1_o']; plot3(plotable(1,:),plotable(2,:),plotable(3,:),'--c'); %central beam
plotable = [cam1_t' cam1_o']; plot3(plotable(1,:),plotable(2,:),plotable(3,:),':c'); %second beam

%plotable = [null cam2_o'];    plot3(plotable(1,:),plotable(2,:),plotable(3,:),'-.m'); %position
plotable = [cam2_p' cam2_o']; plot3(plotable(1,:),plotable(2,:),plotable(3,:),'--m'); %central beam
plotable = [cam2_t' cam2_o']; plot3(plotable(1,:),plotable(2,:),plotable(3,:),':m'); %second beam


% Word Coordinate System at Camera 1 Position
CoordSys = baseXaxis / 4;
plotable = translateRay(makeRay(CoordSys), cam1_o'); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'r');
CoordSys = baseYaxis;
plotable = translateRay(makeRay(CoordSys), cam1_o'); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'g');
CoordSys = baseZaxis;
plotable = translateRay(makeRay(CoordSys), cam1_o'); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'b');

% Word Coordinate System at Camera 2 Position
CoordSys = baseXaxis / 4;
plotable = translateRay(makeRay(CoordSys), cam2_o'); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'r');
CoordSys = baseYaxis;
plotable = translateRay(makeRay(CoordSys), cam2_o'); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'g');
CoordSys = baseZaxis;
plotable = translateRay(makeRay(CoordSys), cam2_o'); plot3(plotable(1,:),plotable(2,:),plotable(3,:),'b');



% translation of world coordinate system to camera coordinate system
% rotate Z, then Y', then X''
%
% correspondance Camera Coordinate System to World Coordinate System:
% u,v,w --> X,Y,Z or more precisely X'',Y'',Z''
% X --> w, Y --> u, Z --> v
% ==> X-Axis is camera viewing axis !
% ==> with this correspondance, camera is already upside down

point_cam1 = baseXaxis;
point_cam1 = [1;-0.2;0] *8000;

point_cam2 = baseXaxis;
point_cam2 = [1;-0.2;0] *8000;


% -- calculations and stuff --

% -- calculateAngles
% first rotation (around Z) from camera's central viewpoint vector
cam1_w1 = -atan2(cam1_r(2), cam1_r(1));
% second rotation (around Y') from camera's central viewpoint vector
cam1_w2 = -asin(cam1_r(3)/norm(cam1_r));
% last rotation (around X'') from the angle between:
% X'-Y'-plane and r-s-vector-plane
% ==> solution: angle between the normal vectors
Xaxis_new = rotate2_euler(cam1_w2) * rotate1_euler(cam1_w1) * baseXaxis;
Yaxis_new = rotate2_euler(cam1_w2) * rotate1_euler(cam1_w1) * baseYaxis;
rotatedSys1_Plane = cross(Xaxis_new, Yaxis_new);
cam1_ViewingPlane = cross(cam1_s', cam1_r');
cam1_w3 = -acos( dot(rotatedSys1_Plane, cam1_ViewingPlane) / ( norm(rotatedSys1_Plane)*norm(cam1_ViewingPlane) ) );
%plotable = translateRay(makeRay(cam1_ViewingPlane), cam1_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'k');

% -- transform some point to camera coordinates
point_cam1 = rotate3_euler(cam1_w3) * rotate2_euler(cam1_w2) * rotate1_euler(cam1_w1) * point_cam1;
plotable = translateRay(makeRay(point_cam1), cam1_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'k');

% -- visualization of translated coordinate system
system = rotate3_euler(cam1_w3) * rotate2_euler(cam1_w2) * rotate1_euler(cam1_w1) * baseXaxis/2;
plotable = translateRay(makeRay(system), cam1_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'r');
system = rotate3_euler(cam1_w3) * rotate2_euler(cam1_w2) * rotate1_euler(cam1_w1) * baseYaxis/2;
plotable = translateRay(makeRay(system), cam1_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'g');
system = rotate3_euler(cam1_w3) * rotate2_euler(cam1_w2) * rotate1_euler(cam1_w1) * baseZaxis/2;
plotable = translateRay(makeRay(system), cam1_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'b');



% -- calculateAngles
% first rotation (around Z) from camera's central viewpoint vector
cam2_w1 = -atan2(cam2_r(2), cam2_r(1));
% second rotation (around Y') from camera's central viewpoint vector
cam2_w2 = -asin(cam2_r(3)/norm(cam2_r));
% last rotation (around X'') from the angle between:
% X'-Y'-plane and r-s-vector-plane
% ==> solution: angle between the normal vectors
Xaxis_new = rotate2_euler(cam2_w2) * rotate1_euler(cam2_w1) * baseXaxis;
Yaxis_new = rotate2_euler(cam2_w2) * rotate1_euler(cam2_w1) * baseYaxis;
rotatedSys2_Plane = cross(Xaxis_new, Yaxis_new);
cam2_ViewingPlane = cross(cam2_s', cam2_r');
cam2_w3 = -acos( dot(rotatedSys2_Plane, cam2_ViewingPlane) / ( norm(rotatedSys2_Plane)*norm(cam2_ViewingPlane) ) );
%plotable = translateRay(makeRay(cam2_ViewingPlane), cam2_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'k');

% -- transform some point to camera coordinates
point_cam2 = rotate3_euler(cam2_w3) * rotate2_euler(cam2_w2) * rotate1_euler(cam2_w1) * point_cam2;
plotable = translateRay(makeRay(point_cam2), cam2_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'k');

% -- visualization of translated coordinate system
system = rotate3_euler(cam2_w3) * rotate2_euler(cam2_w2) * rotate1_euler(cam2_w1) * baseXaxis/2;
plotable = translateRay(makeRay(system), cam2_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'r');
system = rotate3_euler(cam2_w3) * rotate2_euler(cam2_w2) * rotate1_euler(cam2_w1) * baseYaxis/2;
plotable = translateRay(makeRay(system), cam2_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'g');
system = rotate3_euler(cam2_w3) * rotate2_euler(cam2_w2) * rotate1_euler(cam2_w1) * baseZaxis/2;
plotable = translateRay(makeRay(system), cam2_o');  plot3(plotable(1,:),plotable(2,:),plotable(3,:),'b');
