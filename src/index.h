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
 



setInterval(function() 
{
  // Call a function repetatively with 1 Second interval
  //getTimeData();
  //getNameData();
  getAllData();
  timerControl();
}, 1000); //1000mSeconds update rate

 
var switchIsFlipped = false;
var initTime = new Date().getTime();
var intervalHolder;

function timerControl()
{
	//switchIsFlipped = !switchIsFlipped;
  if(switchIsFlipped)
  {
  	initTime = new Date().getTime();
    intervalHolder = setInterval(timerDisplay, 1000, switchIsFlipped);
    timerDisplay(switchIsFlipped)
  }
  else
  {
  	clearInterval(intervalHolder);
  	//document.getElementById("TimeValue").innerHTML = "No One In BR";
  }
}

function timerDisplay(switchFlag)
{
	var sFlip = switchFlag;
  if(sFlip)
  {
  	var elapsed = (new Date().getTime()) - initTime;
    var seconds = Math.floor((elapsed / 1000) % 60);
    var minutes = Math.floor((elapsed / 60000) % 60);
    var LEDTime = ((minutes * 100) + seconds).toString();
    document.getElementById("TimeValue").innerHTML = fLEDTime(LEDTime);
  }
  else
  {
  initTime = new Date().getTime();
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
      //document.getElementById("TimeValue").innerHTML = data[1];

      if(data[1] == "true")
      {
        switchIsFlipped = true;
      }
      else if(data[1] == "false")
      {
        switchIsFlipped = false;
      }
    }
  };
}

function fLEDTime(time)
{
	var timeHolder = time;
  var lenHold = timeHolder.length;
  var t = "";
  
  switch(lenHold)
  {
    case 1:
      t = "00:0" + timeHolder;
      break;
    case 2:
      t = "00:" + timeHolder;
      break;
    case 3:
      t = "0" + timeHolder.substring(0,1) + ":" + timeHolder.substring(1);
      break;
    case 4:
      t = timeHolder.substring(0,2) + ":" + timeHolder.substring(2);
      break;
  }
  return t;
}

</script>
</body>
</html>
)=====";