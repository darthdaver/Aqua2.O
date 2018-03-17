var lastHour = moment().hour();

var daily_config = {
  type: 'line',
  data: {
    labels: [], //newDateInit(),
    datasets: [{
      backgroundColor: 'rgba(255,255,255,0.2)',
      borderColor: 'rgba(255,255,255,0.8)',
      strokeColor: "rgba(255,255,255,1)",
      pointColor: "rgba(255,255,255,1)",
      pointBackgroundColor: "rgba(255,255,255,1)",
      label: "L/hour",
      data: [], //newValueInit(),
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
          fontSize: 20
        },
        type: 'time',
        time: {
          unitStepSize: 1,
        }
      }],
      yAxes: [{
        ticks: {
          fontColor: "white",
          fontSize: 20
        }
      }]
    }
  }
};

$(document).ready(function() {

  daily_setData();

  setInterval(function() {

      daily_setData();
  }, 10000);
});


function newDateInit(){

	var array = [];

  for(i = 0; i <= lastHour; i++){

  	array.push(moment().hour(i).minute(0));
  }

  return array;
}

function newValueInit(){
	var array = [];

  for(i = 0; i <= lastHour; i++){

  	array.push(Math.random()*30);
  }

  return array;
}

function daily_setData() {

  $.ajax({
            url: "http://192.168.0.103:1880/daily",
            data: undefined,
            method: "GET",
            crossDomain: true,
            contentType: "application/json; charset=UTF-8",
            success: function(data) {},
            error: function(xhr, status, error) {},
            timeout: 10000
        }).done(function (result) {
            daily_config.data.datasets[0].data = [];
            daily_config.data.labels = [];
            console.log(result);
            result.forEach(function(day){
              daily_config.data.datasets[0].data.push(day['AMOUNT_SUM']);
              daily_config.data.labels.push(moment().hour(day['HOUR_OF_TIME']).minute(0));
            });
            var ctx = document.getElementById("dailyChart").getContext("2d");
            console.log(daily_config);
            new Chart(ctx, daily_config);
        }).fail(function (jqXHR, textStatus, errorThrown) {
            var error = new Error(errorThrown);
            console.log(error);
        });
}
