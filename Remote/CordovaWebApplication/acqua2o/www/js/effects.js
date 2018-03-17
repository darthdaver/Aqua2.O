$(document).ready(function(){
  setTimeout(function(){
    
    $("#div_logo").addClass("animatedUp fadeUp");
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
