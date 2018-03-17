$(document).ready(function(){
  setTimeout(function(){
    $("#div_logo").addClass("animatedUp fadeUp");
    $("#logoImg").animate({
      height: '110px',
      width: '347.36px'
    }, 2000);
    setTimeout(function(){
      $('.background').css('overflow-y','auto');
      $(".gauge").addClass("animatedIn fadeIn");
      animateGauges();
      setTimeout(function(){
        $(".daily").addClass("animatedIn fadeIn");
        setTimeout(function(){
          $(".monthly").addClass("animatedIn fadeIn");
        },1000);
      },1000);
    },1000);
  },3000);
});
