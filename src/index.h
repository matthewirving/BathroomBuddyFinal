const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card
{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin: 20px;
     box-shadow: 0px 2px 18px -4px rgba(0, 0, 0, 0.75);
}
</style>
<body>
 
<div class="card">
  <h1>Bathroom Buddy</h1><br>
  <h2><span id="NameValue">INIT</span>
  For: <span id="TimeValue">0</span></h2>
</div>
<script>
 
 var newTime = "0";

var switchIsFlipped = false;

function timerDisplay(switchFlag)
{
  var sFlip = switchFlag;
  if(sFlip)
  {
  	/*
    var elapsed = (new Date().getTime()) - initTime;
    var seconds = Math.floor((elapsed / 1000) % 60);
    var minutes = Math.floor((elapsed / 60000) % 60);
    var LEDTime = ((minutes * 100) + seconds).toString();
    document.getElementById("TimeValue").innerHTML = fLEDTime(LEDTime);
    */
    document.getElementById("TimeValue").innerHTML = newTime;
  }
  else
  {
  //initTime = new Date().getTime();
  document.getElementById("TimeValue").innerHTML = "00:00";
  }
}


function getAllData()
{
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "readData", true);
  xhttp.send();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200)
    {
      var holder = this.responseText;
      var data = holder.replace(/\s+/g, '').split('-');
      document.getElementById("NameValue").innerHTML = data[0];
      document.getElementById("TimeValue").innerHTML = data[1];

      /*
      if(data[2] == "true")
      {
        switchIsFlipped = true;
        newTime = data[1];
      }
      else if(data[2] == "false")
      {
        switchIsFlipped = false;
      }
      
     if(data[1] != 0)
     {
       switchIsFlipped = true;
       initTime = data[1];
     }
     else
     {
       switchIsFlipped = false;
       initTime = data[1];
     }
     */
    }
  };
}


setInterval(function() 
{
  // Call a function repetatively with 1 Second interval
  //getTimeData();
  //getNameData();
  getAllData();
  //timerDisplay(switchIsFlipped);
}, 1000); //1000mSeconds update rate

</script>
</body>
</html>
)=====";