%{

Authors: Emanuele Falzone, Davide Molinelli

Date: 4 December 2017

Description:
Comperes the data read from three hall sensors on the same water-flow
sensor. The S1 sensor is the one embedded into the water-flow sensors while
the others are additional ones installed outside the water-flow-sensor
%}

% clear workspace and console
clc
clear

% read hall-sensors data 
data = csvread('../data/hall-sensors.csv',1);

% get sensors data from columns
s1 =  data(1:end, 1);
s2 =  data(1:end, 2);
s3 =  data(1:end, 3);

% create the figure and plot the data
figure('DefaultAxesFontSize',18)

% S1-S2
subplot(1,3,1)
scatter(s1,s2)

grid on
box on
xlabel('Hall sensor S1');
ylabel('Hall sensor S2');

% S1-S3
subplot(1,3,2)
scatter(s1,s3)

grid on
box on
xlabel('Hall sensor S1');
ylabel('Hall sensor S3');

% S2-S3
subplot(1,3,3)
scatter(s2,s3)

grid on
box on
xlabel('Hall sensor S2');
ylabel('Hall sensor S3');
