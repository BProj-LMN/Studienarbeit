%% Trackingsystem - Unit Tests - calculation of estimated values
% Studienarbeit SS 2016 -- HAW Hamburg -- Jannik Beyerstedt
% Performanceanalyse einer optischen Trackinganlage
clc;
clear all;
close all;

%% Test 1a - CameraParams::calcObjRay(PxPos& pos, VectRay& objectRay
% sensor coordinates get transformed to a vector in camera coordinates
%   the vector is described by pos (y,z) and the focal length as X-Axis
% this vector gets transformed to global coordinates via rotation matrix
clc;

positionVector1 = [8950, 3510, 2900]'; % position of camera's focal point in global coordinates
px_pos = [100; 200]; % detected position on sensor
principalPoint1 = [319.5; 239.5]; % aka. sensor center
f1  = 850; % focal length (in pixels!)

rot_mat1 = rotate1_euler(pi); % Z: 180 deg

vect_dir = [f1;(px_pos(1)-principalPoint1(1));(px_pos(2)-principalPoint1(2))];

vect_pos = positionVector1
vect_dir = rot_mat1 * vect_dir

%% Test 1b - CameraParams::calcObjRay(PxPos& pos, VectRay& objectRay
% sensor coordinates get transformed to a vector in camera coordinates
%   the vector is described by pos (x,y) and the focal length as Z-Axis
% this vector gets transformed to global coordinates via rotation matrix
clc;

positionVector2 = [8955, 1100, 2900]'; % position of camera's focal point in global coordinates
px_pos = [100; 200]; % detected position on sensor
principalPoint2 = [639.5; 479.5]; % aka. sensor center
f2  = 1000; % focal length (in pixels!)

rot_mat2 = rotate2_euler(pi/4) * rotate1_euler(pi); % Z: 180 deg, Y: 45 deg

vect_dir = [f2;(px_pos(1)-principalPoint2(1));(px_pos(2)-principalPoint2(2))];

vect_pos = positionVector2
vect_dir = rot_mat2 * vect_dir

%% Test 3 - ImageProcessing integration test
% using the camera parameters of Test 1a, but with "real" image position
clc;

% using positionVector, principalPoint, f and rotMat of Test 1 !!!
px_pos = [101; 329]; % detected position on sensor --> see test image

vect_dir = [f1;(px_pos(1)-principalPoint1(1));(px_pos(2)-principalPoint1(2))];

vect_pos = positionVector1
vect_dir = rot_mat1 * vect_dir

