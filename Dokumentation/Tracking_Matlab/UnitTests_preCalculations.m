%% Trackingsystem - Unit Tests - calculations of correct values
% *Studienarbeit SS 2016 - HAW Hamburg - Performanceanalyse einer optischen Trackinganlage*
clc;
clear all;
close all;

%% Test 1a - CameraParams::calcObjRay(PxPos& pos, VectRay& objectRay)
% sensor coordinates get transformed to a vector in camera coordinates
%   the vector is described by pos (y,z) and the focal length as X-Axis
% this vector gets transformed to global coordinates via rotation matrix
clc;

positionVector = [8950, 3510, 2900]'; % position of camera focal point in global coordinates
px_pos = [100; 200]; % detected position on sensor
principalPoint = [319.5; 239.5]; % aka. sensor center
f  = 850; % focal lenght (in pixels!)

rot_mat = rotate1_euler(pi); % Z: 180 deg

vect_dir = [f;(px_pos(1)-principalPoint(1));(px_pos(2)-principalPoint(2))];

vect_pos = positionVector
vect_dir = rot_mat * vect_dir

%% Test 1b - CameraParams::calcObjRay(PxPos& pos, VectRay& objectRay)
% sensor coordinates get transformed to a vector in camera coordinates
%   the vector is described by pos (x,y) and the focal length as Z-Axis
% this vector gets transformed to global coordinates via rotation matrix
clc;

positionVector = [8955, 1100, 2900]'; % position of camera focal point in global coordinates
px_pos = [100; 200]; % detected position on sensor
principalPoint = [639.5; 479.5]; % aka. sensor center
f  = 1000; % focal lenght (in pixels!)

rot_mat = rotate2_euler(pi/4) * rotate1_euler(pi); % Z: 180 deg, Y: 45 deg

vect_dir = [f;(px_pos(1)-principalPoint(1));(px_pos(2)-principalPoint(2))];

vect_pos = positionVector
vect_dir = rot_mat * vect_dir

%% Test 3)



