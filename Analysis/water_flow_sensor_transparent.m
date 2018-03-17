%{

Authors: Emanuele Falzone, Davide Molinelli

Date: 12 December 2017

Description:
Generates a plot with the data obtained during the data acquisition phase
a use least square to get the regression line equation (green line)
It also plots the line described in the datasheet of the sensor using the
points provided
%}

% clear workspace and console
clc
clear

% read data from sensor 1 csv 
data = csvread('../data/S1.csv',1,1);

% the x is the second column
x = data(1:end, 1);

% the y is the third column divided by 1000 since the data are in
% milliliters
y = data(1:end, 2) ./ 1000;

% data provided in the datasheet
x_d = [16, 32.5, 49.3, 65.5, 82, 90.2];
y_d = [2, 4, 6, 8, 10, 12];

% line fit on the datasheet data
dlm_d = fitlm(x_d,y_d,'y~x1-1')

% line fit on data from data acquisition phase
dlm = fitlm(x,y,'y~x1-1')

% create the figure and plot the data
figure('DefaultAxesFontSize',18)
scatter(x,y)

% plot the regression line
hline = refline(dlm.Coefficients.Estimate);
hline.Color = 'g';
hline.LineWidth = 2;

% plot the line obtained from datasheet data
hline_d = refline(dlm_d.Coefficients.Estimate);
hline_d.Color = 'r';
hline_d.LineWidth = 2;

% compute the strings representing the lines' equations
line = strcat({' y = '}, num2str(dlm.Coefficients.Estimate), {' * x'})
line_d = strcat({' y = '}, num2str(dlm_d.Coefficients.Estimate), {' * x'})

% set title, legenda and axis labels 
title('Water-flow sensor S1')
legend([hline,hline_d], {cell2mat(line),cell2mat(line_d)},'Location','northwest')
grid on
box on
ylabel('L/min');
xlabel('Hz');
