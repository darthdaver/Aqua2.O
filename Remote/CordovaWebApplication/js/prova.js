var config = {
  type: 'line',
  data: {
    labels: newDateInit(),
    datasets: [{
      backgroundColor: 'rgba(255,255,255,0.2)',
      borderColor: 'rgba(255,255,255,0.8)',
      strokeColor: "rgba(255,255,255,1)",
      pointColor: "rgba(255,255,255,1)",
      pointBackgroundColor: "rgba(255,255,255,1)",
      label: "L/hour",
      data: newValueInit(),
    }]
  },
  options: {
    legend: {
      labels: {
        fontColor: "white",
        fontSize: 30
      }
    },
    scales: {
      xAxes: [{
        ticks: {
          fontColor: "white",
          fontSize: 30,
        },
        type: 'time',
        time: {
          unitStepSize: 1,
        }
      }],
      yAxes: [{
        ticks: {
          fontColor: "white",
          fontSize: 30,
        }
      }]
    }
  }
};

$(document).ready(function() {

  var ctx = document.getElementById("dailyChart").getContext("2d");
  new Chart(ctx, config);

  setInterval(function() {
    console.log(config.data.labels[config.data.labels.length - 1].hour());
    if(config.data.labels[config.data.labels.length - 1].hour() + 1 == moment().hour()){

      setLabels(config.data.labels);
      setData(config.data.datasets[0].data);
      var ctx = document.getElementById("dailyChart").getContext("2d");
      new Chart(ctx, config);
    }
  }, 30000);
});


function newDateInit(){
	var array = [];

  for(i = 0; i <= (moment().hour()-1); i++){

  	array.push(moment().hour(i).minute(0));
  }

  return array;
}

function newValueInit(){
	var array = [];

  for(i = -24; i < 0; i++){

  	array.push(Math.random()*30);
  }

  return array;
}

function setData(data) {
  data.push(Math.floor(Math.random() * 100));
  data.shift();
}

function setLabels(labels) {
  labels.push(moment().hour(17).minute(0));
  labels.shift();
}
