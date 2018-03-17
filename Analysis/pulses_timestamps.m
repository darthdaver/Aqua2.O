%{

Authors: Emanuele Falzone, Davide Molinelli

Date: 15 December 2017

Description:
the input csv file must have only one column containing the timestamps
of the impulses. 
  - The first plot allow data brushing to compute the regression line 
    on a subset of data. The slope of the regression line represents 
    the frequency of the impulses.
  - The second plot represent how the flowrate changes at different
    timestamps.
  - The third plot represent how much water is passed throught the sensor
    according to the pulses measurement
    (from data we found that each pulse corresponds to 2.94 mL)
%}

clc
clear

% CONFIGURATION: CSV FILENAME
filename = 'time.csv';
calibration = 0.1765;

% read the timestamp values from csv file
x = csvread(filename, 1, 0);

% the values are in microseconds, so convert them to seconds
x = x ./ 1000000;

% get period array from timestamps array
period = diff(x);

% create a counter array of the timestamps
count = [1: size(x,1)];

% create new figure and plot the points
figure('Name','PULSES','NumberTitle','off','DefaultAxesFontSize',18)
scatterPlot = scatter(x,count);

% enable grid, box and set axis labels and title
grid on
box on
%title('The slope represents the frequency');

% create brush
brush_ = brush;

% set callback function to brush
brush_.ActionPostCallback = {@(src,eventdata)onBrushAction(src,eventdata,scatterPlot)};

% create array representing flowrate at timestamp
y = calibration .* (1./period);

% use median to clean data
%y = medfilt1(y,3);
y = movmean(y,100)

% get x data from the second to the end
sub_x = x(2:end);

% create new figure and plot flowrate data
figure('Name','WATER-FLOW','NumberTitle','off','DefaultAxesFontSize',18)
subplot(2,1,1);
plot(sub_x,y,'Linewidth',6)

% enable grid, box and set axis labels and title
grid on
box on
ylabel('Liters / minute');
%title('Flowrate');

% create new figure and plot consumption data
subplot(2,1,2);
plot(x, count .* calibration/60,'Linewidth',6)

% enable grid, box and set axis labels and title
grid on
box on
ylabel('Liters');
xlabel('seconds');
%title('Consumption');

%brush callback
function onBrushAction(~, ~, scatterPlot)
    
    % get the indexes of the selected points
    indexes = logical(scatterPlot.BrushData);
    
    % retrieve selected x values
    x = scatterPlot.XData(indexes);
    
    % align the x values to 0
    x = x - x(1);
    
    % create counter array
    y = [1: size(x,2)];
    
    % plot the selected points 
    scatter(x,y)
    
    % find the best regression line that fit the data
    dlm = fitlm(x,y,'y~x1-1')
    
    % create e green line on the plot representing the regression line
    line = refline(dlm.Coefficients.Estimate);
    line.Color = 'g';
    line.LineWidth = 2;

    % creates a string reprensenting the line equation
    equation = strcat({' y = '}, num2str(dlm.Coefficients.Estimate), {' * x'});

    % set the legend to see the line equation
    legend([line], {cell2mat(equation)},'Location','northwest');
    
    % set figure title
    grid on
    box on
    ylabel('Count');
    xlabel('Timestamp');
    title('The slope represents the frequency');
    
    % create brush
    b = brush;

    % set callback function [] to disable brush
    b.ActionPostCallback = [];

end