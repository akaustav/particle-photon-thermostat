function getTemperature() {
  return $.ajax({
    type: 'GET',
    url: 'https://api.particle.io/v1/devices/2f002c000647343339373536/currentTemp/',
    data: 'access_token=ad1447e50fbfb73658271b4352acd55d73589933',
    dataType: 'json',
    success: function (data) {
      // console.log(data);
      document.getElementById("curTempPlace").innerHTML = "Home Temperature set at " + data.result + " °F";
    }
  });
}

function getTime() {
  return $.ajax({
    type: 'GET',
    url: 'https://api.particle.io/v1/devices/2f002c000647343339373536/Time/',
    data: 'access_token=ad1447e50fbfb73658271b4352acd55d73589933',
    dataType: 'json',
    success: function (data) {
      // console.log(data);
      document.getElementById("curTimePlace").innerHTML = "at " + data.result + " HRS";
    }
  });
}

function getData() {
  getTemperature();
  getTime();
}

function roomTemperature() {
  return $.ajax({
    type: 'GET',
    url: 'https://api.particle.io/v1/devices/2f002c000647343339373536/Temperature/',
    data: 'access_token=ad1447e50fbfb73658271b4352acd55d73589933',
    dataType: 'json',
    success: function (data) {
      document.getElementById("roomTempPlace").innerHTML = "Room Temperature: " + data.result + " °F";
    }
  });
}

function setTemperature(inTemp) {
  return $.ajax({
    type: 'POST',
    url: 'https://api.particle.io/v1/devices/2f002c000647343339373536/TempChange/',
    data: 'access_token=ad1447e50fbfb73658271b4352acd55d73589933&args=' + inTemp,
    success: function (data) {
      // console.log(data);
      document.getElementById("changeTempPlace").innerHTML = "Changed by " + data.return_value + " °F";
      // getTemperature();
    }
  });
}
