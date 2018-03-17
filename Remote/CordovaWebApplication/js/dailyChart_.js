$(document).ready(function() {
  var ctx = document.getElementById("dailyChart").getContext("2d");

  var hours = ["3", "6", "9", "12", "15", "18", "21","24"];

  var data_ = {
    labels: hours,
    datasets: [{
      label: "L/min",
      backgroundColor: 'rgba(255,255,255,0.2)',
      borderColor: 'rgba(255,255,255,0.8)',
      strokeColor: "rgba(255,255,255,1)",
      pointColor: "rgba(255,255,255,1)",
      pointBackgroundColor: "rgba(255,255,255,1)",
      data: [65, 59, 80, 81, 56, 55, 40]
    }]
  };

  var options_ = {
    legend: {
      labels: {
        fontColor: "white",
        fontSize: 18
      }
    },

    scales: {
      yAxes: [{
        ticks: {
          fontColor: "white",
          fontSize: 18,
          stepSize: 10,
        }
      }],
      xAxes: [{
        ticks: {
          fontColor: "white",
          fontSize: 14,
          stepSize: 1,
        },
        time: {

        }
      }]
    },
  };

  setInterval(function() {
    setData(data_.datasets[0].data);
    setLabels(data_.labels);

    var chart = new Chart(ctx, {
      // The type of chart we want to create
      type: 'line',

      data: data_,

      options: options_
    });
  }, 2000);

  function setLabels(labels) {
    var nextMonthIndex = hours.indexOf(labels[labels.length - 1]) + 1;
    var nextMonthName = hours[nextMonthIndex] != undefined ? hours[nextMonthIndex] : "3";
    labels.push(nextMonthName);
    labels.shift();
  }

  function setData(data) {
    data.push(Math.floor(Math.random() * 100) + 1);
    data.shift();
  }

  function convertMonthNameToNumber(monthName) {
    var myDate = new Date(monthName + " 1, 2016");
    var monthDigit = myDate.getMonth();
    return isNaN(monthDigit) ? 0 : (monthDigit + 1);
  }

});
