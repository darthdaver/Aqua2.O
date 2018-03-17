var timers = [];

if (!Array.prototype.forEach) {
    Array.prototype.forEach = function(cb) {
        var i = 0, s = this.length;
        for (; i < s; i++) {
            cb && cb(this[i], i, this);
        }
    }
}

document.fonts && document.fonts.forEach(function(font) {
    font.loaded.then(function() {
        if (font.family.match(/Led/)) {
            document.gauges.forEach(function(gauge) {
                gauge.update();
                gauge.options.renderTo.style.visibility = 'visible';
            });
        }
    });
});

function animateGauges() {
    document.gauges.forEach(function(gauge) {
        timers.push(setInterval(function() {
            //gauge.value = Math.random() *
                //(gauge.options.maxValue - gauge.options.minValue) +
                //gauge.options.minValue;
                $.ajax({
                          url: "http://192.168.0.111:1880/realtime",
                          data: undefined,
                          method: "GET",
                          crossDomain: true,
                          contentType: "application/json; charset=UTF-8",
                          success: function(data) {},
                          error: function(xhr, status, error) {},
                          timeout: 10000
                      }).done(function (result) {
                          console.log(result);
                          gauge.value = result['QUANTITY'];
                      }).fail(function (jqXHR, textStatus, errorThrown) {
                          var error = new Error(errorThrown);
                          console.log(error);
                      });
        }, gauge.animation.duration + 50));
    });
}

function stopGaugesAnimation() {
    timers.forEach(function(timer) {
        clearInterval(timer);
    });
}

function resize() {
    var size = parseFloat(document.getElementById('gauge-size').value) || 400;

    document.gauges.forEach(function (gauge) {
        gauge.update({ width: size, height: size });
    });
}

function setText() {
    var text = document.getElementById('gauge-text').value;

    document.gauges.forEach(function (gauge) {
        gauge.update({ valueText: text });
    });
}
