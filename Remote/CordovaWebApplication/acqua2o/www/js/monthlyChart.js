var lastHour_ = moment().hour();

var monthly_config = {
  type: 'bar',
  data: {
    labels: [],
    datasets: [{
      backgroundColor: 'rgba(255,255,255,0.8)',
      borderColor: 'rgba(255,255,255,0.8)',
      strokeColor: "rgba(255,255,255,1)",
      pointColor: "rgba(255,255,255,1)",
      pointBackgroundColor: "rgba(255,255,255,1)",
      label: "L/week",
      data: []
    }]
  },
  options: {
    legend: {
      labels: {
        fontColor: "white",
      }
    },
    scales: {
      xAxes: [{
        type: "time",
        ticks: {
          fontColor: "white",
        },
        time: {
          unit: 'week',
          round: 'day',
          displayFormats: {
            day: 'MMM D'
          }
        }
      }],
      yAxes: [{
        ticks: {
          beginAtZero: true,
          fontColor: "white",
        },
      }]
    }
  }
};

$(document).ready(function() {

  monthly_setData();

  setInterval(function() {
    //console.log(monthly_config.data.labels[monthly_config.data.labels.length - 1].hour());
    //if(monthly_config.data.labels[monthly_config.data.labels.length - 1].hour() + 1 == moment().hour()){
      monthly_setData();
    //}
  }, 10000);
});

function monthly_setData() {
  //data.shift();
  //data.push(Math.floor(Math.random() * 100));

  $.ajax({
            url: "http://52.11.174.171:1880/monthly",
            data: undefined,
            method: "GET",
            crossDomain: true,
            contentType: "application/json; charset=UTF-8",
            success: function(data) {},
            error: function(xhr, status, error) {},
            timeout: 10000
        }).done(function (result) {
            monthly_config.data.datasets[0].data = [];
            monthly_config.data.labels = [];
            console.log(result);
            result.forEach(function(week){
              if(week['WEEK_OF_YEAR'] - moment().week() > -4 && week['WEEK_OF_YEAR'] - moment().week() <= 0){
                monthly_config.data.datasets[0].data.push(week['AMOUNT_SUM']);
                monthly_config.data.labels.push(moment().week(week['WEEK_OF_YEAR']).year(week['YEAR']).toDate());
              }
            });
            var ctx_ = document.getElementById("monthlyChart").getContext("2d");
            console.log(monthly_config);
            new Chart(ctx_, monthly_config);
        }).fail(function (jqXHR, textStatus, errorThrown) {
            var error = new Error(errorThrown);
            console.log(error);
        });
}
