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
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
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
  getTimeData();
  getNameData();
}, 1000); //1000mSeconds update rate

setInterval(function() 
{
  // getNameData();
}, 5000); //5000ms update rate
 
function getTimeData() 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) 
    {
      document.getElementById("TimeValue").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "readTime", true);
  xhttp.send();
}

function getNameData() 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) 
    {
      document.getElementById("NameValue").innerHTML = this.responseText;

    }
  };
  xhttp.open("GET", "readName", true);
  xhttp.send();
}


</script>
</body>
</html>
)=====";