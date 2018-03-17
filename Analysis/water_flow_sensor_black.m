%{

Authors: Emanuele Falzone, Davide Molinelli

Date: 12 December 2017

Description:
Generates a plot with the data obtained during the data acquisition phase
a use least square to get the regression line equation (green line)
It also plots the line described in the datasheet of the sensor using the
slope provided
%}

% clear workspace and console
clc
clear

% read data from sensor 2 csv 
data = csvread('../data/S2.csv',1,1);

% the x is the second column
x = data(1:end, 1);

% the y is the third column divided by 1000 since the data are in
% milliliters
y = data(1:end, 2) ./ 1000;

% get the regression line coefficient
dlm = fitlm(x,y,'y~x1-1')

% create the figure and plot the data
figure('DefaultAxesFontSize',18)
scatter(x,y)

% plot the regression line
hline = refline(dlm.Coefficients.Estimate);
hline.Color = 'g';
hline.LineWidth = 2;

% plot the line provided in the datasheet
hline_d = refline(1/7.5);
hline_d.Color = 'r';
hline_d.LineWidth = 2;

% compute the strings representing the lines' equations
line = strcat({' y = '}, num2str(dlm.Coefficients.Estimate), {' * x'})
line_d = strcat({' y = '}, num2str(1/7.5), {' * x'})

% set title, legenda and axis labels 
title('Water-flow sensor S2')
legend([hline,hline_d], {cell2mat(line),cell2mat(line_d)},'Location','northwest')
grid on
box on
ylabel('L/min');
xlabel('Hz');